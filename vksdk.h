#ifndef VK_API_H
#define VK_API_H

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

// Documentation
// https://vk.com/dev/methods

class vkSdk
{
public:
	vkSdk();
	QNetworkReply *request(QUrl url);
	QNetworkRequest method(QString api_method, QUrlQuery query = QUrlQuery());

	const QJsonObject login( QString username, QString password, QString captcha = "");
	void setAuthParams( QString token, int userid);

private:
	QNetworkAccessManager *networkmanager;
	QNetworkRequest req;
	QString client_secret;
	QString client_id;
	QString ver;
	QString access_token;
	QString captcha_sid;
	int page_id;
};

extern vkSdk vkapi;

#endif // VK_API_H
