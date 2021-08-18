#include "utils.h"

QString utils::TimestampToQStr( int timestamp )
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

void utils::ClearLayout(QLayout* layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        QWidget* widget;
        if (  (deleteWidgets)
              && (widget = item->widget())  ) {
            delete widget;
        }
        if (QLayout* childLayout = item->layout()) {
            utils::ClearLayout(childLayout, deleteWidgets);
        }
        delete item;
    }
}
