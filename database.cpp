#include "database.h"
#include <QJsonDocument>

InfoDatabase::InfoDatabase(QObject *parent) : QObject(parent)
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(QDir::currentPath()+"/profiles.db");
	db.open();
	QSqlQuery query( db );
	query.prepare("CREATE TABLE IF NOT EXISTS profiles(user_id INT, first_name TINYTEXT, last_name TINYTEXT, photo_100 TEXT)");
	query.exec();
	query.prepare("CREATE TABLE IF NOT EXISTS groups(group_id INT, name TINYTEXT, photo_100 TEXT)");
	query.exec();
	db.commit();
}

void InfoDatabase::addProfile(int user_id, QString first_name, QString last_name, QString photo_100)
{
	QSqlQuery query( db );
	query.prepare("SELECT count(*) FROM profiles WHERE user_id=:user_id");
	query.bindValue(":user_id", user_id);
	if( query.exec() && query.next() && !query.value(0).toInt() )
	{
		query.prepare("INSERT INTO profiles VALUES(:user_id, :first_name, :last_name, :photo_100)");
		query.bindValue(":user_id", user_id);
		query.bindValue(":first_name", first_name);
		query.bindValue(":last_name", last_name);
		query.bindValue(":photo_100", photo_100);
		query.exec();
		db.commit();
	}
}

profile_t InfoDatabase::getProfile(int user_id)
{
	static profile_t profile;
	profile.photo_100 = "";
	profile.first_name = "";
	profile.last_name = "";
	profile.user_id = 0;
	
	QSqlQuery query(db);
	query.prepare("SELECT * FROM profiles WHERE user_id=:user_id");
	query.bindValue(":user_id", user_id);
	query.exec();
	while( query.next() )
	{
		profile.user_id = query.value(0).toInt();
		profile.first_name = query.value(1).toString();
		profile.last_name = query.value(2).toString();
		profile.photo_100 = query.value(3).toString();
	}
	
	return profile;
}

void InfoDatabase::slot_update(const QJsonObject messages)
{
	update(messages);
}

void InfoDatabase::addGroup(int group_id, QString name, QString photo_100)
{
	QSqlQuery query( db );
	query.prepare("SELECT count(*) FROM groups WHERE group_id=:group_id");
	query.bindValue(":group_id", group_id);
	if( query.exec() && query.next() && !query.value(0).toInt() )
	{
		query.prepare("INSERT INTO groups VALUES(:group_id, :name, :photo_100)");
		query.bindValue(":group_id", group_id);
		query.bindValue(":name", name);
		query.bindValue(":photo_100", photo_100);
		query.exec();
		db.commit();
	}
}

group_t InfoDatabase::getGroup(int group_id)
{
	static group_t group;
	group.photo_100 = "";
	group.name = "";
	group.group_id = 0;
	
	QSqlQuery query(db);
	query.prepare("SELECT * FROM groups WHERE group_id=:group_id");
	query.bindValue(":group_id", group_id);
	query.exec();
	if( query.next() )
	{
		group.group_id = query.value(0).toInt();
		group.name = query.value(1).toString();
		group.photo_100 = query.value(2).toString();
	}
	
	return group;
}

void InfoDatabase::update(const QJsonObject messages)
{
	const QJsonArray arr = messages["response"]["profiles"].toArray();	
	for( int i = 0; i < arr.size(); i++)
	{
		const QJsonObject profile = arr[i].toObject();
		addProfile(profile["id"].toInt(), profile["first_name"].toString(), profile["last_name"].toString(), profile["photo_100"].toString());
	}

	const QJsonArray groups = messages["response"]["groups"].toArray();	
	for( int i = 0; i < groups.size(); i++)
	{
		const QJsonObject group = groups[i].toObject();
		addGroup(group["id"].toInt(), group["name"].toString(), group["photo_100"].toString());
	}
}
