#ifndef VK_API_H
#define VK_API_H

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include "downloadmanager.h"


// Documentation
// https://vk.com/dev/methods

class vkApi : public QObject
{
    Q_OBJECT
public:
	vkApi(QObject *parent = 0);
	QNetworkReply *request(QUrl url);
	QNetworkRequest method(QString api_method, QUrlQuery query = QUrlQuery());

	const QJsonObject login( QString username, QString password, QString captcha = "", QString code = "");
	void init( QString token, int userid);
	QString getUserName();

	int page_id;
	QString photo_url;
private:
	QNetworkAccessManager *networkmanager;
	QNetworkRequest req;
	QString client_secret, client_id, ver, access_token, captcha_sid;
	QString first_name, last_name;


private slots:
	void pageinfoReply(QNetworkReply *reply);	
};

extern vkApi vkapi;

#endif // VK_API_H
