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
#include "database.h"

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
	
	InfoDatabase *db;
	
	QNetworkAccessManager *dialogs_manager;
	QNetworkAccessManager *message_manager;
	QNetworkAccessManager *history_manager;
	
	
	void requestDialogs(int count, int offset = 0);
	DialogWidget *getDialogById(int peer_id);
	
	int m_iCurDialogCount;
	int m_iDialogCount;
	QTimer resizeTimer;
	DialogWidget *active_dialog;

private slots:
	void addDialogs(QNetworkReply *reply);
	void loadupDialogs();
	void loadupMessages();
	
	void resizeUpdate();
	void dialogSelected(DialogWidget *dialog);
	void updateMessages(const QJsonObject messages, bool bottom = true);
	void messageSended(QNetworkReply *reply);
	void messageHistory(QNetworkReply *reply);
	
	void on_sendButton_released();
	void TextEditEvent(QKeyEvent *event);
};

#endif // MESSAGESWINDOW_H
