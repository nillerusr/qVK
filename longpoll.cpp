#include "longpoll.h"
#include "vksdk.h"
#include <QObject>

LongPoll::LongPoll(QObject *parent) : QObject(parent)
{
	_manager = new QNetworkAccessManager();
	QObject::connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
}

void LongPoll::getLongPollServer()
{
	_manager->get(vkapi.method("messages.getLongPollServer"));
}

void LongPoll::requestFinished(QNetworkReply* reply)
{
	QJsonDocument jDoc = QJsonDocument::fromJson(reply->readAll());
	if (lp_server.isNull() || lp_server.isEmpty())
	{
		QJsonObject jObj = jDoc.object().value("response").toObject();
		lp_server = jObj.value("server").toString();
		lp_key = jObj.value("key").toString();
		lp_ts = jObj.value("ts").toInt();
		qDebug() << lp_server;
		LongPollRequest();
	}
	else
	{
		QJsonObject jObj = jDoc.object();
		if (jObj.contains("failed"))
		{
			if (jObj.value("failed").toInt() == 1)
			{
				lp_ts = jObj.value("ts").toInt();
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
			lp_ts = jObj.value("ts").toInt();

			// TODO: Parse longpoll reply
			ParseLongPollEvents(jObj.value("updates").toArray());
			LongPollRequest();
		}
	}
	reply->deleteLater();
}

void LongPoll::LongPollRequest()
{
	QUrl url("https://" + lp_server);
	QUrlQuery query;
	query.addQueryItem("act", "a_check");
	query.addQueryItem("key", lp_key);
	query.addQueryItem("ts", QString::number(lp_ts));
	query.addQueryItem("wait", "25");
	query.addQueryItem("mode", "10");
	url.setQuery(query);
	_manager->get(QNetworkRequest(url));
}

void LongPoll::ParseLongPollEvents(const QJsonArray &updates)
{
	for(int i = 0; i < updates.count(); i++)
	{
		QJsonArray update = updates.at(i).toArray();
		switch(update.at(0).toInt())
		{
		case MESSAGE_NEW:
			qDebug() << "longpoll event: MESSAGE_NEW";
			break;
		case MESSAGE_EDIT:
			qDebug() << "longpoll event: MESSAGE_EDIT";
			break;
		case OUTPUT_MESSAGE_READ:
			qDebug() << "longpoll event: OUTPUT_MESSAGE_READ";
			break;
		case INPUT_MESSAGE_READ:
			qDebug() << "longpoll event: INPUT_MESSAGE_READ";
			break;
		case UNREAD_DIALOGS_CHANGED:
			qDebug() << "longpoll event: UNREAD_DIALOGS_CHANGED";
			break;
		default: break;
		}
	}
}
