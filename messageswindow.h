#ifndef MESSAGESWINDOW_H
#define MESSAGESWINDOW_H

#include <QMainWindow>
#include "longpoll.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QListWidget>

namespace Ui {
class MessagesWindow;
}

class MessagesWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MessagesWindow(QWidget *parent = nullptr);
	~MessagesWindow();

private:
	Ui::MessagesWindow *ui;
	LongPoll lp;
	QNetworkAccessManager *dialogs_manager;
	
private slots:
	void addDialogs(QNetworkReply *reply);	
};

#endif // MESSAGESWINDOW_H
