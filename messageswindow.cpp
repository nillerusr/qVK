#include "messageswindow.h"
#include "ui_messageswindow.h"
#include "dialogwidget.h"
#include "vksdk.h"
#include "messagewidget.h"
#include <QObject>

MessagesWindow::MessagesWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MessagesWindow)
{
	ui->setupUi(this);
	lp.getLongPollServer();
	
	dialogs_manager = new QNetworkAccessManager();
	QObject::connect(dialogs_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(addDialogs(QNetworkReply*)));
	QUrlQuery query
	{
		{"extended","1"}
	};
	
	dialogs_manager->get(vkapi.method("messages.getConversations", query));
}

void MessagesWindow::addDialogs(QNetworkReply *reply)
{	
	const QJsonObject jObj = QJsonDocument::fromJson(reply->readAll()).object();
	if ( !jObj["response"].isUndefined() )
	{
		const QJsonArray items = jObj["response"]["items"].toArray();
		const QJsonArray profiles = jObj["response"]["profiles"].toArray();
		for(int i = 0; i < items.count(); i++)
		{
			QJsonObject conversation = items[i]["conversation"].toObject();
			QString title = items[i]["conversation"]["chat_settings"]["title"].toString();
			QString last_msg = items[i]["last_message"]["text"].toString();
			DialogWidget *dialogwidget = new DialogWidget(nullptr, title, last_msg );
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
	}
}

MessagesWindow::~MessagesWindow()
{
	delete ui;
}
