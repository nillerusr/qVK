#ifndef LONGPOLL_H
#define LONGPOLL_H

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QObject>
#include <QJsonObject>

enum LONGPOLL_EVENTS
{
	MESSAGE_SET_FLAG = 2,
	MESSAGE_NEW = 4,
	MESSAGE_EDIT = 5,
	INPUT_MESSAGE_READ = 6,
	OUTPUT_MESSAGE_READ = 7,
	UNREAD_DIALOGS_CHANGED = 80
};

enum MESSAGE_FLAGS
{
	MESSAGE_UNREAD = 1,
	MESSAGE_OUTBOX = 2,
	MESSAGE_REPLIED = 4,
	MESSAGE_IMPORTANT = 8,
	MESSAGE_CHAT = 16,
	MESSAGE_FRIENDS = 32,
	MESSAGE_SPAM = 64,
	MESSAGE_DELETED = 128,
	MESSAGE_FIXED = 256,
	MESSAGE_MEDIA = 512,
	MESSAGE_HIDDEN = 65536,
	MESSAGE_DELETE_FOR_ALL = 131072,
	MESSAGE_NOT_DELIVERED = 262144
};

class LongPoll : public QObject
{
	Q_OBJECT

public:
	explicit LongPoll(QObject *parent = nullptr);
	void getLongPollServer();
	void LongPollRequest();
	void ParseLongPollEvents(const QJsonArray &updates);
	
private:
	QNetworkAccessManager *_manager;
	QNetworkAccessManager *msg_manager;
	
	QString lp_server, lp_key;
	int lp_ts;
	
	void getMsg(int message_id);	

private slots:
	void longpollReply(QNetworkReply* reply);
	void getMsgReply(QNetworkReply *reply);
	
signals:
	void Message_New( const QJsonObject reply );
	void Message_Delete( int peer_id, int message_id );
	void Message_Edit();
};

static LongPoll longpoll;

#endif // LONGPOLL_H
