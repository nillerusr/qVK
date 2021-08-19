#include "vk.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QFile>
#include <QDir>

vkApi vkapi;

vkApi::vkApi()
{
	client_secret = "lxhD8OD7dMsqtXIm5IUY";
	client_id = "2685278";
	ver = "5.101";
	access_token = "";
	page_id = 0;
	captcha_sid = "";
	networkmanager = new QNetworkAccessManager();
}

void vkApi::setAuthParams(QString token, int userid)
{
	this->access_token = token;
	this->page_id = userid;
}

QNetworkReply *vkApi::request(QUrl url)
{
	req.setUrl(url);
	req.setHeader( QNetworkRequest::UserAgentHeader, "KateMobileAndroid/60.1-60 (Android 5; SDK 21; x86; bruh moment; RU)");
	QNetworkReply *reply = networkmanager->get(req);
	QEventLoop event;
	QObject::connect(reply, SIGNAL(finished()), &event, SLOT(quit()));
	event.exec();
	return reply;
}

QNetworkRequest vkApi::method(QString api_method, QUrlQuery query)
{
	QUrl url = QUrl("https://api.vk.com/method/"+api_method);
	query.addQueryItem("v", ver);
	query.addQueryItem("access_token", access_token);
	url.setQuery(query);
	return QNetworkRequest(url);
}

const QJsonObject vkApi::login(QString username, QString password, QString captcha, QString code)
{
	QUrl url = QUrl("https://oauth.vk.com/token");
	QUrlQuery query = {
		{"grant_type", "password"},
		{"client_id", client_id},
		{"client_secret", client_secret},
		{"v", ver},
		{"scope", "notify,friends,photos,audio,video,docs,status,notes,pages,wall,groups,messages,offline,notifications,stories"},
		{"username", username},
		{"password", password},
		{"2fa_supported", "1"}
	}; 

	if( !captcha.isEmpty() && !captcha_sid.isEmpty() )
	{
		query.addQueryItem("captcha_sid", captcha_sid);
		query.addQueryItem("captcha_key", captcha);		
		captcha_sid = "";
	}
	
	if( !code.isEmpty() )
		query.addQueryItem("code", code);
	
	url.setQuery(query);
	QJsonDocument jDoc = QJsonDocument::fromJson(request(url)->readAll());
	QJsonObject jObj = jDoc.object();
	
	if (jObj.contains("access_token"))
	{
		access_token = jObj.value("access_token").toString();
		page_id = jObj.value("user_id").toInt();

		QFile file(QDir::currentPath()+"/config.json");
		file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
		file.write(jDoc.toJson());
		file.close();

		return {};
	}
	
	if( jObj.contains("captcha_sid") )
		captcha_sid = jObj["captcha_sid"].toString();

	return jObj;
}

