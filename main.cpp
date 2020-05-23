#include "loginwindow.h"
#include <QApplication>
#include "vk_api.h"
#include "messageswindow.h"
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QJsonObject>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	LoginWindow loginwin;
	MessagesWindow *messagewin;

	QFile file(QDir::currentPath()+"/config.json");
	file.open(QFile::ReadOnly | QFile::Text);

	QJsonDocument jDoc = QJsonDocument::fromJson(file.readAll());
	QJsonObject jObj = jDoc.object();

	if (jObj.contains("access_token"))
	{
		vkapi.token = jObj.value("access_token").toString();
		vkapi.page_id = jObj.value("user_id").toInt();
		messagewin = new MessagesWindow();
		messagewin->show();
	}
	else
	{
		loginwin.show();
	}

	return a.exec();
}
