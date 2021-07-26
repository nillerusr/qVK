#include "utils.h"

QString UTIL_TimestampToQStr( int timestamp )
{
	QDateTime t;
	t.setTime_t(timestamp);
	QDateTime dateTime = QDateTime::currentDateTime();
	if( dateTime.date().year() != t.date().year() )
		return t.toString("dd.MM.yyyy");
	else if( dateTime.date().day() != t.date().day() )
		return t.toString("dd.MM");
	else
		return t.toString("hh:mm");
}
