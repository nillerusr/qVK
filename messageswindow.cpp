#include "messageswindow.h"
#include "ui_messageswindow.h"
#include "dialogwidget.h"
#include "vksdk.h"
#include "messagewidget.h"
#include <QObject>
#include <QScroller>
#include <wscrollarea.h>
#include "dialogwidget.h"
#include <QKeyEvent>
#include <QtMath>

MessagesWindow::MessagesWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MessagesWindow)
{
	ui->setupUi(this);
	lp.getLongPollServer();
	
	resizeTimer.setSingleShot( true );
	dialogs_manager = new QNetworkAccessManager();
	message_manager = new QNetworkAccessManager();
	db = new InfoDatabase(parent);

	QObject::connect(dialogs_manager, SIGNAL(finished(QNetworkReply*)), SLOT(addDialogs(QNetworkReply*)));
	QObject::connect(ui->dialogsArea, SIGNAL(scrolledDown()), SLOT(loadupDialogs()));
	QObject::connect( &resizeTimer, SIGNAL(timeout()), SLOT(resizeUpdate()) );
	QObject::connect( &lp, SIGNAL(Message_New(const QJsonObject)), db, SLOT(slot_update(const QJsonObject)) );	
	QObject::connect( &lp, SIGNAL(Message_New(const QJsonObject)), SLOT(updateMessages(const QJsonObject)) );
	QObject::connect( message_manager, SIGNAL(finished(QNetworkReply*)), SLOT(messageSended(QNetworkReply*)));
	QObject::connect( ui->messageEdit, SIGNAL(sKeyPressEvent(QKeyEvent*)), SLOT(TextEditEvent(QKeyEvent*)));
	

	m_iCurDialogCount = 0;
	m_iDialogCount = 0;
	active_dialog = nullptr;
	requestDialogs(10);
	ui->messagesArea->m_bScrollDownNeed = true;
}

void MessagesWindow::resizeEvent(QResizeEvent *event)
{
	resizeTimer.start( 500 );
	QMainWindow::resizeEvent(event);
}

void MessagesWindow::resizeUpdate()
{
	if( ui->dialogsArea->isScrolledDown() && m_iCurDialogCount < m_iDialogCount )
		requestDialogs(10, m_iCurDialogCount);
}

void MessagesWindow::requestDialogs(int count, int offset)
{
	QUrlQuery query
	{
		{"extended", "1"},
		{"count", QString::number(count)},
		{"offset", QString::number(offset)}
	};
	dialogs_manager->get(vkapi.method("messages.getConversations", query));	
}

void MessagesWindow::loadupDialogs()
{
	if( m_iCurDialogCount < m_iDialogCount )
		requestDialogs(10, m_iCurDialogCount);
}

void MessagesWindow::addDialogs(QNetworkReply *reply)
{
	QString msg_time;
	const QJsonObject jObj = QJsonDocument::fromJson(reply->readAll()).object();
	db->update(jObj);
		
	if ( !jObj["response"].isUndefined() )
	{
		m_iDialogCount = jObj["response"]["count"].toInt();
		const QJsonArray items = jObj["response"]["items"].toArray();
		const QJsonArray profiles = jObj["response"]["profiles"].toArray();
		m_iCurDialogCount += items.count();
		
		for(int i = 0; i < items.count(); i++)
		{
			QJsonObject conversation = items[i]["conversation"].toObject();
			QString title = items[i]["conversation"]["chat_settings"]["title"].toString();
			QString last_msg = items[i]["last_message"]["text"].toString();
			
			int unread = items[i]["conversation"]["unread_count"].toInt();
			
			QDateTime timestamp;
			timestamp.setTime_t(items[i]["last_message"]["date"].toInt());
			QDateTime dateTime = QDateTime::currentDateTime();
			if( dateTime.date().day() != timestamp.date().day())
				msg_time = timestamp.toString("dd.MM.yyyy");
			else
				msg_time = timestamp.toString("hh:mm");

			QJsonObject message = items[i]["last_message"].toObject();
			
			DialogWidget *dialogwidget = new DialogWidget(nullptr, title, last_msg, unread, msg_time );
			dialogwidget->peer_id = items[i]["conversation"]["peer"]["id"].toInt();
			dialogwidget->type = items[i]["conversation"]["peer"]["type"].toString();		
			dialogwidget->messages.append(message);
			
			QObject::connect( dialogwidget, SIGNAL(dialogSelected(DialogWidget *)), SLOT(dialogSelected(DialogWidget *)) );
			
			if( dialogwidget->type == "user" )
			{
				profile_t profile = db->getProfile( dialogwidget->peer_id );
				dialogwidget->setDialogName(profile.first_name+" "+profile.last_name);
			}
			else if( dialogwidget->peer_id < 0 )
				dialogwidget->setDialogName(db->getGroup(qAbs(dialogwidget->peer_id)).name);

			ui->dialogsLayout->addWidget(dialogwidget);
		}
		
		if( ui->dialogsArea->isScrolledDown() && m_iCurDialogCount < m_iDialogCount )
		{
			requestDialogs(10, m_iCurDialogCount);
		}
	}
}

void MessagesWindow::updateMessages(const QJsonObject messages)
{
	DialogWidget *widget = getDialogById(messages["response"]["items"][0]["peer_id"].toInt());
	if( widget )
	{
		if( !messages["response"]["items"][0].isUndefined() )
		{
			ui->dialogsLayout->removeWidget(widget);
			ui->dialogsLayout->insertWidget(0, widget);
			const QJsonObject msg = messages["response"]["items"][0].toObject();
			widget->setLastMessageText(msg["text"].toString());			
			widget->messages.append(msg);
			qDebug() << QJsonDocument(msg).toJson();
			if( active_dialog && active_dialog == widget )
			{
				QString msg_time;
				QDateTime timestamp;
				timestamp.setTime_t(msg["date"].toInt());
				QDateTime dateTime = QDateTime::currentDateTime();
				if( dateTime.date().day() != timestamp.date().day())
					msg_time = timestamp.toString("dd.MM.yyyy");
				else
					msg_time = timestamp.toString("hh:mm");
				
				int id = msg["from_id"].toInt();
				QString name;
				if( id < 0 )
					name = db->getGroup(qAbs(id)).name;
				else
				{
					profile_t profile = db->getProfile(id);
					name = profile.first_name+" "+profile.last_name;
				}
				QWidget *message = new messagewidget(this, name, msg["text"].toString() , msg_time );
				ui->messagesLayout->addWidget(message);
			}
		}
	}
}

DialogWidget *MessagesWindow::getDialogById(int peer_id)
{
	for( int i = 0; i < ui->dialogsLayout->count(); i++ )
	{
		DialogWidget *widget = qobject_cast<DialogWidget *>(ui->dialogsLayout->itemAt(i)->widget());
		
		if( widget )
		{
			if( widget->peer_id == peer_id )
				return widget;
		}
	}
	return nullptr;
}

void clearLayout(QLayout* layout, bool deleteWidgets = true)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        QWidget* widget;
        if (  (deleteWidgets)
              && (widget = item->widget())  ) {
            delete widget;
        }
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout, deleteWidgets);
        }
        delete item;
    }
}

void MessagesWindow::dialogSelected(DialogWidget *dialog)
{
	active_dialog = dialog;
	QList<QJsonObject>::iterator it; 
	
	clearLayout(ui->messagesLayout);
	
	for( it = dialog->messages.begin(); it != dialog->messages.end(); it++)
	{
		QString name;
		int id = (*it)["from_id"].toInt();
		
		if( id < 0 )
			name = db->getGroup(qAbs(id)).name;
		else
		{
			profile_t profile = db->getProfile(id);
			name = profile.first_name+" "+profile.last_name;
		}
					
		QString msg_time;
		QDateTime timestamp;
		timestamp.setTime_t((*it)["date"].toInt());
		QDateTime dateTime = QDateTime::currentDateTime();
		if( dateTime.date().day() != timestamp.date().day())
			msg_time = timestamp.toString("dd.MM.yyyy");
		else
			msg_time = timestamp.toString("hh:mm");

		QWidget *message = new messagewidget(this, name, (*it)["text"].toString(), msg_time );
		ui->messagesLayout->addWidget(message);
	}
}

MessagesWindow::~MessagesWindow()
{
	delete ui;
}

void MessagesWindow::messageSended(QNetworkReply* reply)
{

}

void MessagesWindow::on_sendButton_released()
{
	if( !active_dialog )
		return;
	
	QUrlQuery query
	{
		{"message", ui->messageEdit->toPlainText()},
		{"random_id", "0"},
		{"peer_id", QString::number(active_dialog->peer_id) }
	};
	ui->messageEdit->clear();
    message_manager->get(vkapi.method("messages.send", query));
}

void MessagesWindow::TextEditEvent(QKeyEvent *event)
{
	if( event->type() == QKeyEvent::KeyPress )
	{		
		if( event->key() == Qt::Key_Return && !(QGuiApplication::queryKeyboardModifiers() & Qt::ShiftModifier))
		{
			if( !active_dialog )
				return;
			
			QUrlQuery query
			{
				{"message", ui->messageEdit->toPlainText()},
				{"random_id", "0"},
				{"peer_id", QString::number(active_dialog->peer_id) }
			};
			ui->messageEdit->clear();
			message_manager->get(vkapi.method("messages.send", query));
		}
	}
}
