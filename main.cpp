#include <QApplication>
#include "utils/vk.h"
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QCommonStyle>
#include <QStylePainter>
#include "wins/messageswindow.h"
#include "wins/loginwindow.h"
#include "wins/settingswindow.h"
#include "utils/diagnosticstyle.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QCoreApplication::setOrganizationName("nillerusr");
    //QCoreApplication::setOrganizationDomain("");
    QCoreApplication::setApplicationName("qVK");
	
	LoginWindow loginwin;
	MessagesWindow *messagewin;

	QFile file(QDir::currentPath()+"/config.json");
	file.open(QFile::ReadOnly | QFile::Text);

	QJsonDocument jDoc = QJsonDocument::fromJson(file.readAll());
	QJsonObject jObj = jDoc.object();

	//a.setStyle(new DiagnosticStyle());
	QFile style(":/styles/white.css");
	style.open(QFile::ReadOnly);
	a.setStyleSheet(style.readAll());
	
	if (jObj.contains("access_token"))
	{
		vkapi.init(jObj.value("access_token").toString(),  jObj.value("user_id").toInt());
		messagewin = new MessagesWindow();
		messagewin->show();
	}
	else
		loginwin.show();
	
	return a.exec();
}
