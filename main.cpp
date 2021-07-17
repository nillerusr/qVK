#include "loginwindow.h"
#include <QApplication>
#include "vksdk.h"
#include "messageswindow.h"
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QCommonStyle>
#include <QStylePainter>
#include "diagnosticstyle.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	LoginWindow loginwin;
	MessagesWindow *messagewin;

	QFile file(QDir::currentPath()+"/config.json");
	file.open(QFile::ReadOnly | QFile::Text);

	QJsonDocument jDoc = QJsonDocument::fromJson(file.readAll());
	QJsonObject jObj = jDoc.object();

//	a.setStyle(new DiagnosticStyle());
	
	if (jObj.contains("access_token"))
	{
		vkapi.setAuthParams(jObj.value("access_token").toString(),  jObj.value("user_id").toInt());
		messagewin = new MessagesWindow();
		messagewin->show();
	}
	else
		loginwin.show();
	
	return a.exec();
}
