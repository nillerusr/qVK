#ifndef MESSAGESWINDOW_H
#define MESSAGESWINDOW_H

#include <QMainWindow>
#include "longpoll.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QListWidget>
#include <QTimer>
#include "dialogwidget.h"

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
	QNetworkAccessManager *message_manager;
	
	
	void requestDialogs(int count, int offset = 0);
	DialogWidget *getDialogById(int peer_id);
	
	int m_iCurDialogCount;
	int m_iDialogCount;
	QTimer resizeTimer;
	DialogWidget *active_dialog;
	
private slots:
	void addDialogs(QNetworkReply *reply);
	void loadupDialogs();
	void resizeUpdate();
	void dialogSelected(DialogWidget *dialog);
	void updateMessages(const QJsonObject messages);
	void messageSended(QNetworkReply *reply);
	void on_sendButton_released();
};

#endif // MESSAGESWINDOW_H
