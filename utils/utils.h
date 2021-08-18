#ifndef UTILS_H
#define UTILS_H

#include <QtCore>
#include <QLayout>
#include <QWidget>

namespace utils {
	QString TimestampToQStr( int timestamp );
	void ClearLayout(QLayout* layout, bool deleteWidgets = true);
};

#endif // UTILS_H
