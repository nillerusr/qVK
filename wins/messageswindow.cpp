 #include "messageswindow.h"
#include "ui_messageswindow.h"
#include <QObject>
#include <QScroller>
#include <QKeyEvent>
#include <QtMath>
#include "utils/utils.h"
#include "utils/longpoll.h"
#include "utils/vk.h"
#include "customwidgets/messagewidget.h"
#include "customwidgets/dialogwidget.h"
#include "customwidgets/wscrollarea.h"
#include "wins/settingswindow.h"
#include <unistd.h>

MessagesWindow::MessagesWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MessagesWindow)
{
	ui->setupUi(this);
	longpoll.getLongPollServer();

	resizeTimer.setSingleShot( true );
	dialogs_manager = new QNetworkAccessManager();
	message_manager = new QNetworkAccessManager();
	history_manager = new QNetworkAccessManager();

	connect( dialogs_manager, SIGNAL(finished(QNetworkReply*)), SLOT(addDialogs(QNetworkReply*)));
	connect( ui->dialogsArea, SIGNAL(scrolledDown()), SLOT(loadupDialogs()));
	connect( &resizeTimer, SIGNAL(timeout()), SLOT(resizeUpdate()) );
	connect( &longpoll, SIGNAL(Message_New(const QJsonObject)), SLOT(updateMessages(const QJsonObject)));
	connect( message_manager, SIGNAL(finished(QNetworkReply*)), SLOT(messageSended(QNetworkReply*)));
	connect( ui->messageEdit, SIGNAL(sKeyPressEvent(QKeyEvent*)), SLOT(TextEditEvent(QKeyEvent*)));
	connect( history_manager, SIGNAL(finished(QNetworkReply*)), SLOT(messageHistory(QNetworkReply*)));
	connect( ui->messagesArea, SIGNAL(scrolledUp()), SLOT(loadupMessages()));
	connect( &conversation_avatar_loader, SIGNAL(downloaded(QString, download_info, int)), SLOT(conversation_avatar_downloaded(QString, download_info, int)));
	connect( &message_avatar_loader, SIGNAL(downloaded(QString, download_info, int)), SLOT(message_avatar_downloaded(QString, download_info, int)));
	connect( &profile_avatar_loader, SIGNAL(downloaded(QString, download_info, int)), SLOT(profile_avatar_downloaded(QString, download_info, int)));
	connect( &message_attachment_loader, SIGNAL(downloaded(QString, download_info, int)), SLOT(message_attachment_downloaded(QString, download_info, int)));

	
	connect( &longpoll, SIGNAL(Message_Delete(int,int)), SLOT(messageDeleted(int,int)));

	
	m_iCurDialogCount = 0;
	m_iCurMessagesCount = 0;
	m_iDialogCount = 0;
	active_dialog = nullptr;
	requestDialogs(30);
	ui->messagesArea->m_bScrollDownNeed = true;
	ui->messagesLayout->setAlignment(Qt::AlignBottom);				
	ui->messagesQueuedLayout->setAlignment(Qt::AlignBottom);

	conversation_avatar_loader.setDownloadDirectory(".image_previews");
	message_avatar_loader.setDownloadDirectory(".image_previews");
	profile_avatar_loader.setDownloadDirectory(".image_previews");
	message_attachment_loader.setDownloadDirectory(".image_previews");

	profile_avatar = utils::getHashFromPhotoUrl(vkapi.photo_url);
	profile_avatar_loader.append( vkapi.photo_url, profile_avatar, ui->profilePhoto );
	profile_avatar_loader.download();
}

MessagesWindow::~MessagesWindow()
{
	delete ui;
}

void MessagesWindow::profile_avatar_downloaded( QString filename, download_info info, int error )
{
	if( !info.data || info.bDeleted )
		return;
	
	QWidget *widget = (QWidget*)info.data;	
	
	QPixmap pix(filename);
	qobject_cast<QLabel*>(widget)->setPixmap(pix);
}

void MessagesWindow::message_attachment_downloaded(QString filename, download_info info, int error)
{
	if( !info.data )
		return;

	image_download_queue *download_queue = (image_download_queue*)info.data;		
	messagewidget *message = download_queue->msg;		
	
	if( info.bDeleted || !message )
	{
		delete download_queue;
		return;
	}
	
	QPixmap pix(filename);
	int need_width = minimumWidth()-active_dialog->width()-ui->profilePhoto->width()-120;

	if( pix.width() >= need_width )
	{
		float flScale = (float)need_width/pix.width();
		pix = pix.scaled(pix.width()*flScale, pix.height()*flScale);
	}

	//ui->messagesArea->scrollContentsBy(0, pix.height());
	
	message->setImageAttachment(pix, download_queue->quque_id);
	delete download_queue;
}

void MessagesWindow::conversation_avatar_downloaded(QString filename, download_info info, int error)
{
	if( !info.data || info.bDeleted )
		return;
	
	QWidget *widget = (QWidget*)info.data;
	DialogWidget *d = qobject_cast<DialogWidget*>(widget);

	QPixmap pix = QPixmap(filename);
	d->setPhoto(pix);

	if( active_dialog == d )
		ui->dialogIcon->setPixmap(pix);
}

void MessagesWindow::message_avatar_downloaded(QString filename, download_info info, int error)
{
	if( !info.data || info.bDeleted )
		return;

	QWidget *widget = (QWidget*)info.data;	

	QPixmap pix(filename);
	messagewidget *m = qobject_cast<messagewidget*>(widget);
	m->setPhoto(pix);
}

void MessagesWindow::resizeEvent(QResizeEvent *event)
{
	resizeTimer.start( 500 );
	QMainWindow::resizeEvent(event);
}

void MessagesWindow::resizeUpdate()
{
	if( ui->dialogsArea->isScrolledDown() && m_iCurDialogCount < m_iDialogCount )
		requestDialogs(30, m_iCurDialogCount);
}

void MessagesWindow::requestDialogs(int count, int offset)
{
	QUrlQuery query
	{
		{"extended", "1"},
		{"fields", "photo_200"},
		{"count", QString::number(count)},
		{"offset", QString::number(offset)}
	};
	m_iCurDialogCount += count;
	dialogs_manager->get(vkapi.method("messages.getConversations", query));	
}

void MessagesWindow::loadupDialogs()
{
	if( m_iCurDialogCount < m_iDialogCount )
		requestDialogs(30, m_iCurDialogCount);
}

void MessagesWindow::addDialogs(QNetworkReply *reply)
{
	QString msg_time, img_url;

	const QJsonObject jObj = QJsonDocument::fromJson(reply->readAll()).object();	
	
	if ( !jObj["response"].isUndefined() )
	{
		m_iDialogCount = jObj["response"]["count"].toInt();
		const QJsonArray items = jObj["response"]["items"].toArray();
		const QJsonArray profiles = jObj["response"]["profiles"].toArray();
		const QJsonArray groups = jObj["response"]["groups"].toArray();

		//m_iCurDialogCount += items.count();
		for(int i = 0; i < items.count(); i++)
		{
			const QJsonObject conversation = items[i]["conversation"].toObject();

			QString title = items[i]["conversation"]["chat_settings"]["title"].toString();
			QString last_msg = items[i]["last_message"]["text"].toString();

			int unread = items[i]["conversation"]["unread_count"].toInt();

			msg_time = utils::TimestampToQStr(items[i]["last_message"]["date"].toInt());

			QJsonObject message = items[i]["last_message"].toObject();

			DialogWidget *dialogwidget = new DialogWidget(nullptr, title, last_msg, unread, msg_time );
			dialogwidget->peer_id = conversation["peer"]["id"].toInt();
			dialogwidget->type = items[i]["conversation"]["peer"]["type"].toString();

			connect( dialogwidget, SIGNAL(dialogSelected(DialogWidget*)), SLOT(dialogSelected(DialogWidget*)) );
			ui->dialogsLayout->addWidget(dialogwidget);

			if( dialogwidget->type == "user")
			{
				for( int j = 0; j < profiles.count(); j++)
				{
					const QJsonObject profile = profiles[j].toObject();
					if( profile["id"].toInt() == dialogwidget->peer_id )
					{
						if( !profile["photo_200"].isUndefined() )
						{
							QString img_url = profile["photo_200"].toString();
							dialogwidget->photo = utils::getHashFromPhotoUrl( img_url );
							conversation_avatar_loader.append(img_url, dialogwidget->photo, dialogwidget);	
						}
						dialogwidget->setDialogName(profile["first_name"].toString()+" "+profile["last_name"].toString());
						break;
					}
				}
			}
			else if( dialogwidget->type == "group")
			{
				for( int j = 0; j < groups.size(); j++)
				{
					const QJsonObject group = groups[j].toObject();

					if( group["id"].toInt() == -dialogwidget->peer_id )
					{
						if( !group["photo_200"].isUndefined() )
						{
							img_url = group["photo_200"].toString();
							dialogwidget->photo = utils::getHashFromPhotoUrl( img_url );
							conversation_avatar_loader.append(img_url, dialogwidget->photo, dialogwidget);
						}
						dialogwidget->setDialogName(group["name"].toString());
						break;
					}
				}
			}
			else if( dialogwidget->type == "chat" )
			{
				if( !conversation["chat_settings"]["photo"]["photo_200"].isUndefined() )
				{
					img_url = conversation["chat_settings"]["photo"]["photo_200"].toString();
					dialogwidget->photo = utils::getHashFromPhotoUrl( img_url );
					conversation_avatar_loader.append(img_url, dialogwidget->photo, dialogwidget);
				}
			}
		}

		if( ui->dialogsArea->isScrolledDown() && m_iCurDialogCount < m_iDialogCount )
			requestDialogs(30, m_iCurDialogCount);
	}
	conversation_avatar_loader.download();
}

void MessagesWindow::updateMessages(const QJsonObject messages, bool bottom)
{
	QString img_url;
	const QJsonArray msgs = messages["response"]["items"].toArray();
	const QJsonArray profiles = messages["response"]["profiles"].toArray();
	const QJsonArray groups = messages["response"]["groups"].toArray();

	qDebug() << messages;	
	
	for(int i = 0; i < msgs.count();i++)
	{
		DialogWidget *widget = getDialogById(messages["response"]["items"][i]["peer_id"].toInt());
		if( widget )
		{
			const QJsonObject msg = msgs[i].toObject();

			if(bottom)
			{
				ui->messagesArea->m_bFuckedUp = true;				
				ui->dialogsLayout->removeWidget(widget);
				ui->dialogsLayout->insertWidget(0, widget);
				widget->setLastMessageText(msg["text"].toString());
				widget->messages.append(msg);
			}
			else
				widget->messages.insert(0, msg);

			m_iCurMessagesCount++;

			if( active_dialog && active_dialog == widget )
			{
				QString msg_time = utils::TimestampToQStr(msg["date"].toInt());

				int from_id = msg["from_id"].toInt();

				QString name;

				messagewidget *message = nullptr;

				for( int i = 0; i < ui->messagesLayout->count(); i++ )
				{
					message = qobject_cast<messagewidget *>(ui->messagesLayout->itemAt(i)->widget());
					if( message->message_id == msg["id"].toInt() )
						break;
					message = nullptr;
				}

				if( !message )
				{
					message = new messagewidget(this, "", msg["text"].toString() , msg_time );
					message->message_id = msg["id"].toInt();
				}

				int queue_id = 0;
				
				const QJsonArray attachments = msg["attachments"].toArray();
				for( int i = 0; i < attachments.size(); i++ )
				{
					if( !attachments[i]["photo"].isUndefined() )
					{						
						QString filename = QString::number(attachments[i]["photo"]["owner_id"].toInt())+
								"_"+QString::number(attachments[i]["photo"]["id"].toInt());
						
						const QJsonArray sizes = attachments[i]["photo"]["sizes"].toArray();
						for( int j = 0; j < sizes.size(); j++ )
						{
							if( sizes[j]["type"].toString() == "x" )
							{
								QString url = sizes[j]["url"].toString();
								int height = sizes[j]["height"].toInt();
								int width = sizes[j]["width"].toInt();
								
								int need_width = minimumWidth()-active_dialog->width()-ui->profilePhoto->width()-120;

								if( width >= need_width )
								{
									float flScale = (float)need_width/width;
									width *= flScale;
									height *= flScale;
								}
								
								image_download_queue *download_queue = new image_download_queue;
								download_queue->quque_id = queue_id;
								download_queue->msg = message;
								
								message->addImageAttachment(QSize(width, height), queue_id);
								message_attachment_loader.append(url, filename, download_queue);
								
								queue_id++;
							}
						}
						
					}
				}
				
				if( !msg["update_time"].isUndefined() )
				{
					message->setStatusText("edited");
					message->setText(msg["text"].toString());
					msg_time = utils::TimestampToQStr(msg["update_time"].toInt());
					message->setDateTime(msg_time);
				}
				
				if(bottom)
					ui->messagesLayout->addWidget(message);
				else
					ui->messagesLayout->insertWidget(0, message);
				
				if( from_id > 0 )
				{
					for( int j = 0; j < profiles.count(); j++)
					{
						const QJsonObject profile = profiles[j].toObject();
						if( profile["id"].toInt() == from_id )
						{
							name = profile["first_name"].toString()+" "+profile["last_name"].toString();

							if( !profile["photo_200"].isUndefined() )
							{
								img_url = profile["photo_200"].toString();
								message_avatar_loader.append(img_url, utils::getHashFromPhotoUrl( img_url ), message);
							}
							break;
						}
					}
				}
				else
				{
					for( int j = 0; j < groups.count(); j++)
					{
						const QJsonObject group = groups[j].toObject();

						if( group["id"].toInt() == -from_id )
						{
							name = group["name"].toString();

							if( !group["photo_200"].isUndefined() )
							{
								img_url = group["photo_200"].toString();
								message_avatar_loader.append(img_url, utils::getHashFromPhotoUrl( img_url ), message);	
							}
							break;
						}
					}
				}
				message->setName(name);
			}
		}
	}

	message_avatar_loader.download();
	message_attachment_loader.download();
}

DialogWidget *MessagesWindow::getDialogById(int peer_id)
{
	for( int i = 0; i < ui->dialogsLayout->count(); i++ )
	{
		DialogWidget *widget = qobject_cast<DialogWidget *>(ui->dialogsLayout->itemAt(i)->widget());

		if( widget && widget->peer_id == peer_id )
			return widget;
	}
	return nullptr;
}

void MessagesWindow::dialogSelected(DialogWidget *dialog)
{
	if( !dialog ) return;

	if( active_dialog )
	{
		message_attachment_loader.freeDataInQueues();
		profile_avatar_loader.freeDataInQueues();
	}
		
	active_dialog = dialog;

	utils::ClearLayout(ui->messagesLayout); // TODO: should I store this layout in memory ?
	utils::ClearLayout(ui->messagesQueuedLayout);
	ui->dialogIcon->setPixmap(QPixmap(".image_previews/"+dialog->photo));

	m_iCurMessagesCount = 0;
	loadupMessages();
}

void MessagesWindow::messageSended(QNetworkReply *reply)
{
	const QJsonObject json_obj = QJsonDocument::fromJson(reply->readAll()).object();
	if( json_obj["response"].isUndefined() )
	{
		for( int i = 0; i < ui->messagesQueuedLayout->count(); i++ )
		{
			messagewidget *message = qobject_cast<messagewidget *>(ui->messagesQueuedLayout->itemAt(i)->widget());
			if( message->status == MESSAGE_SENDING || message->status == MESSAGE_QUEUED )
			{
				message->status = MESSAGE_SEND_FAILED;
				message->setDateTime("Error"); // setDateTime? Why not? 
			}
		}
	}
	else
	{
		for( int i = 0; i < ui->messagesQueuedLayout->count(); i++ )
		{
			messagewidget *message = qobject_cast<messagewidget *>(ui->messagesQueuedLayout->itemAt(i)->widget());
			if( message->status == MESSAGE_SENDING )
			{
				message->setDateTime(QDateTime().currentDateTime().toString("hh:mm"));
				ui->messagesQueuedLayout->removeWidget(message);
				ui->messagesLayout->addWidget(message);
				message->message_id = json_obj["response"].toInt();
				message->status = MESSAGE_SEND;
				m_iCurMessagesCount++;
			}
			else if( message->status == MESSAGE_QUEUED )
			{
				message->status = MESSAGE_SENDING;
				QUrlQuery query
				{
					{"message", message->getText()},
					{"random_id", "0"},
					{"peer_id", QString::number(active_dialog->peer_id) }
				};
				message_manager->get(vkapi.method("messages.send", query));
				break;
			}
		}
	}
}

void MessagesWindow::sendMessage()
{
	if( ui->messageEdit->toPlainText().isEmpty() )
		return;
		
	QUrlQuery query
	{
		{"message", ui->messageEdit->toPlainText()},
		{"random_id", "0"},
		{"peer_id", QString::number(active_dialog->peer_id) }
	};

	messagewidget *message = new messagewidget(this, vkapi.getUserName(), ui->messageEdit->toPlainText() , "In queue" );
	ui->messagesArea->m_bFuckedUp = true;
	ui->messagesQueuedLayout->addWidget(message);
	message->setPhoto(".image_previews/"+profile_avatar);

	bool bSending = false;
	for( int i = 0; i < ui->messagesQueuedLayout->count(); i++ )
	{
		messagewidget *widget = qobject_cast<messagewidget *>(ui->messagesQueuedLayout->itemAt(i)->widget());
		if( widget->status == MESSAGE_SENDING )
		{
			bSending = true;
			break;
		}
	}

	if( bSending )
	{
		message->setDateTime("In queue");
		message->status = MESSAGE_QUEUED;
	}
	else
	{
		message->setDateTime("Sending");
		message->status = MESSAGE_SENDING;
		message_manager->get(vkapi.method("messages.send", query));
	}

	ui->messageEdit->clear();
}

void MessagesWindow::on_sendButton_released()
{
	if( active_dialog )
		sendMessage();
}

void MessagesWindow::TextEditEvent(QKeyEvent *)
{
	if( !ui->messageEdit->toPlainText().isEmpty() &&
		active_dialog )
	{
		sendMessage();
	}
}

void MessagesWindow::messageHistory(QNetworkReply *reply)
{
	const QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
	updateMessages(obj, false);
}

void MessagesWindow::loadupMessages()
{
	if( !active_dialog )
		return;

	QUrlQuery query
	{
		{"count", "30"},
		{"offset", QString::number(m_iCurMessagesCount) },
		{"peer_id", QString::number(active_dialog->peer_id) },
		{"extended", "1"},
		{"fields", "photo_200"}
	};

	history_manager->get(vkapi.method("messages.getHistory", query));
}

void MessagesWindow::messageDeleted(int peer_id, int message_id)
{
	if( active_dialog && active_dialog->peer_id == peer_id )
	{
		for( int i = 0; i < ui->messagesLayout->count(); i++ )
		{
			messagewidget *widget = qobject_cast<messagewidget *>(ui->messagesLayout->itemAt(i)->widget());
			if( widget->message_id == message_id )
			{
				delete widget;
				break;
			}
		}
	}
}


void MessagesWindow::on_settingsButton_released()
{
	settingswindow *settings_win = new settingswindow(this);
    this->hide();
	settings_win->show();
}
