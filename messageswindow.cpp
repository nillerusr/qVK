#include "messageswindow.h"
#include "ui_messageswindow.h"
#include "dialogwidget.h"
#include "vksdk.h"
#include "messagewidget.h"
#include <QObject>
#include <QScroller>
#include <wscrollarea.h>

MessagesWindow::MessagesWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MessagesWindow)
{
	ui->setupUi(this);
	lp.getLongPollServer();
	
	resizeTimer.setSingleShot( true );
	dialogs_manager = new QNetworkAccessManager();
	QObject::connect(dialogs_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(addDialogs(QNetworkReply*)));
	QObject::connect(ui->dialogsArea, SIGNAL(scrolledDown()), this, SLOT(loadupDialogs()));
	QObject::connect( &resizeTimer, SIGNAL(timeout()), SLOT(resizeUpdate()) );

	m_iCurDialogCount = 0;
	m_iDialogCount = 0;
	
	requestDialogs(10);
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
				msg_time = timestamp.toString("h:m");
			
			DialogWidget *dialogwidget = new DialogWidget(nullptr, title, last_msg, unread, msg_time );
			dialogwidget->peer_id = items[i]["conversation"]["peer"]["id"].toInt();
			dialogwidget->type = items[i]["conversation"]["peer"]["type"].toString();
			
			if( dialogwidget->type == "user" )
			{
				for( int j = 0; j < profiles.count(); j++)
				{
					if( profiles[j]["id"].toInt() == dialogwidget->peer_id )
					{
						dialogwidget->setDialogName(profiles[j]["first_name"].toString()+" "+profiles[j]["last_name"].toString());
					}
				}
			}
			ui->dialogsLayout->addWidget(dialogwidget);
		}
		
		if( ui->dialogsArea->isScrolledDown() && m_iCurDialogCount < m_iDialogCount )
		{
			requestDialogs(10, m_iCurDialogCount);
		}
	}
}

MessagesWindow::~MessagesWindow()
{
	delete ui;
}
