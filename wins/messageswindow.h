#ifndef MESSAGESWINDOW_H
#define MESSAGESWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QListWidget>
#include <QTimer>
#include "customwidgets/dialogwidget.h"
//#include "database.h"
#include "utils/downloadmanager.h"
#include "utils/longpoll.h"

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
	
//	InfoDatabase *db;
	
	QNetworkAccessManager *dialogs_manager;
	QNetworkAccessManager *message_manager;
	QNetworkAccessManager *history_manager;

	DownloadManager conversation_avatar_loader, message_avatar_loader;
	
	void requestDialogs(int count, int offset = 0);
	DialogWidget *getDialogById(int peer_id);
	
	int m_iCurDialogCount, m_iDialogCount, m_iCurMessagesCount;
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
	void conversation_avatar_downloaded(QString filename, int error);
	void message_avatar_downloaded(QString filename, int error);
	
	void on_sendButton_released();
	void TextEditEvent(QKeyEvent *event);
	void sendMessage();
};

#endif // MESSAGESWINDOW_H
