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


class vk_api
{
public:
	vk_api();
	QNetworkReply *request(QUrl url);
	QJsonDocument method(QString api_method, QUrlQuery query);
	bool login( QString username, QString password );
	QString token;
	int page_id;

private:
	QNetworkAccessManager *networkmanager;
	QNetworkRequest req;
	QString client_secret;
	QString client_id;
	QString ver;
};

extern vk_api vkapi;

#endif // VK_API_H
