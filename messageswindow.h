#ifndef MESSAGESWINDOW_H
#define MESSAGESWINDOW_H

#include <QMainWindow>
#include "longpoll.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QListWidget>
#include <QTimer>

namespace Ui {
class MessagesWindow;
}

class MessagesWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MessagesWindow(QWidget *parent = nullptr);
	~MessagesWindow();
	void resizeEvent(QResizeEvent *event);
	
private:
	Ui::MessagesWindow *ui;
	LongPoll lp;
	QNetworkAccessManager *dialogs_manager;
	
	void requestDialogs(int count, int offset = 0);
	int m_iCurDialogCount;
	int m_iDialogCount;
	QTimer resizeTimer;
	
private slots:
	void addDialogs(QNetworkReply *reply);
	void loadupDialogs();
	void resizeUpdate();
};

#endif // MESSAGESWINDOW_H
