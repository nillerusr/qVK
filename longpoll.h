#ifndef LONGPOLL_H
#define LONGPOLL_H

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QObject>

class LongPoll : public QObject
{
	Q_OBJECT

public:
	explicit LongPoll(QObject *parent = nullptr);
	void getLongPollServer();
	void LongPollRequest();

private:
	QNetworkAccessManager *_manager;
	QString lp_server, lp_key;
	int lp_ts;

private slots:
	void requestFinished(QNetworkReply* reply);
};

#endif // LONGPOLL_H
