#include "vk_api.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QFile>
#include <QDir>

vk_api vkapi;

vk_api::vk_api()
{
	client_secret = "lxhD8OD7dMsqtXIm5IUY";
	client_id = "2685278";
	ver = "5.101";
	token = "";
	page_id = 0;
	networkmanager = new QNetworkAccessManager();
}

QNetworkReply *vk_api::request(QUrl url)
{
	req.setUrl(url);
	req.setHeader( QNetworkRequest::UserAgentHeader, "KateMobileAndroid/60.1-60 (Android 5; SDK 21; x86; bruh moment; RU)");
	QNetworkReply *reply = networkmanager->get(req);
	QEventLoop event;
	QObject::connect(reply, SIGNAL(finished()), &event, SLOT(quit()));
	event.exec();
	return reply;
}

QJsonDocument vk_api::method(QString api_method, QUrlQuery query)
{
	QUrl url = QUrl("https://api.vk.com/method/"+api_method);
	query.addQueryItem("v", ver);
	query.addQueryItem("access_token", token);
	url.setQuery(query);
	//qDebug() << token;
	QByteArray reply = request(url)->readAll();
	qDebug() << reply;
	return QJsonDocument::fromJson(reply);
}

bool vk_api::login(QString username, QString password)
{
	QUrl url = QUrl("https://oauth.vk.com/token");
	QUrlQuery query;
	query.addQueryItem("grant_type", "password");
	query.addQueryItem("client_id", client_id);
	query.addQueryItem("client_secret", client_secret);
	query.addQueryItem("v", ver);
	query.addQueryItem("scope", "notify,friends,photos,audio,video,docs,status,notes,pages,wall,groups,messages,offline,notifications,stories");
	query.addQueryItem("username", username);
	query.addQueryItem("password", password);
	url.setQuery(query);
	QJsonDocument jDoc = QJsonDocument::fromJson(request(url)->readAll());
	QJsonObject jObj = jDoc.object();


	if (jObj.contains("access_token"))
	{
		token = jObj.value("access_token").toString();
		page_id = jObj.value("user_id").toInt();

		QFile file(QDir::currentPath()+"/config.json");
		file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
		file.write(jDoc.toJson());
		file.close();

		return true;
	}

	return false;
}

