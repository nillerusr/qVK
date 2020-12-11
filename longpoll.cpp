#include "longpoll.h"
#include "vksdk.h"
#include <QObject>

LongPoll::LongPoll(QObject *parent) : QObject(parent)
{
	_manager = new QNetworkAccessManager();
	msg_manager = new QNetworkAccessManager();
	QObject::connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(longpollReply(QNetworkReply*)));
	QObject::connect(msg_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getMsgReply(QNetworkReply*)));
	
}

void LongPoll::getLongPollServer()
{
	_manager->get(vkapi.method("messages.getLongPollServer"));
}

void LongPoll::longpollReply(QNetworkReply* reply)
{
	const QJsonObject jObj = QJsonDocument::fromJson(reply->readAll()).object();
	if (lp_server.isNull() || lp_server.isEmpty())
	{
		lp_server = jObj["response"]["server"].toString();
		lp_key = jObj["response"]["key"].toString();
		lp_ts = jObj["response"]["ts"].toInt();
		qDebug() << lp_server;
		LongPollRequest();
	}
	else
	{
		if (jObj.contains("failed"))
		{
			if (jObj["failed"].toInt() == 1)
			{
				lp_ts = jObj["ts"].toInt();
				LongPollRequest();
			}
			else
			{
				lp_server.clear();
				lp_key.clear();
				lp_ts = 0;
				getLongPollServer();
			}
		}
		else
		{
			lp_ts = jObj["ts"].toInt();

			// TODO: Parse longpoll reply
			ParseLongPollEvents(jObj["updates"].toArray());
			LongPollRequest();
		}
	}
	reply->deleteLater();
}

void LongPoll::LongPollRequest()
{
	QUrl url("https://" + lp_server);
	QUrlQuery query
	{
		{"act", "a_check"},
		{"key", lp_key},
		{"ts", QString::number(lp_ts)},
		{"wait", "25"},
		{"mode", "10"}
	};
	
	url.setQuery(query);
	_manager->get(QNetworkRequest(url));
}

void LongPoll::getMsgReply(QNetworkReply *reply)
{
	const QJsonObject object = QJsonDocument::fromJson(reply->readAll()).object();	
	emit Message_New(object);
}

void LongPoll::getMsg(int message_id)
{
	QUrlQuery query
	{
		{"message_ids", QString::number(message_id)},
		{"extended", "1"}
	};
	
	msg_manager->get(vkapi.method("messages.getById", query));
}

void LongPoll::ParseLongPollEvents(const QJsonArray &updates)
{
	for(int i = 0; i < updates.count(); i++)
	{
		QJsonArray update = updates.at(i).toArray();
		switch(update.at(0).toInt())
		{
		case MESSAGE_NEW:
			getMsg(update[1].toInt());
			break;
		case MESSAGE_EDIT:
			break;
		case OUTPUT_MESSAGE_READ:
			break;
		case INPUT_MESSAGE_READ:
			break;
		case UNREAD_DIALOGS_CHANGED:
			break;
		default: break;
		}
	}
}
