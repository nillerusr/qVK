#ifndef LONGPOLL_H
#define LONGPOLL_H

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QObject>

enum LONGPOLL_EVENTS
{
	MESSAGE_NEW = 4,
	MESSAGE_EDIT = 5,
	INPUT_MESSAGE_READ = 6,
	OUTPUT_MESSAGE_READ = 7,
	UNREAD_DIALOGS_CHANGED = 80
};

class LongPoll : public QObject
{
	Q_OBJECT

public:
	explicit LongPoll(QObject *parent = nullptr);
	void getLongPollServer();
	void LongPollRequest();
	void ParseLongPollEvents(const QJsonArray &updates);

private:
	QNetworkAccessManager *_manager;
	QString lp_server, lp_key;
	int lp_ts;

private slots:
	void requestFinished(QNetworkReply* reply);
};

#endif // LONGPOLL_H
