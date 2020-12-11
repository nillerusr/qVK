#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtCore>
#include <QtSql>
#include <QtNetwork>

class messagesDB
{
public:
	messagesDB();
	QSqlDatabase db;
};

struct profile_s {
	int user_id;
	QString first_name;
	QString last_name;
	QString photo_100;
};
typedef struct profile_s profile_t;

class InfoDatabase : public QObject
{
	Q_OBJECT;
	
public:
	InfoDatabase(QObject *parent);		
	void addProfile(int user_id, QString first_name, QString last_name, QString photo_100 = "");
	profile_t getProfile(int user_id);
	QSqlDatabase db;
	void updateProfiles(const QJsonObject messages);	
	
private slots:
	void slot_updateProfiles(const QJsonObject messages);
};

#endif // DATABASE_H
