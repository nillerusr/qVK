#include "diagnosticstyle.h"
#include <QPainter>
#include <QWidget>

DiagnosticStyle::DiagnosticStyle()
{

}

void DiagnosticStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
	QCommonStyle::drawControl(element, option, painter, widget);
	if (widget && painter) {
		// draw a border around the widget
		painter->setPen(QPen(Qt::green, 2));
		painter->drawRect(widget->rect());

		/// show the classname of the widget
/*		QBrush translucentBrush(QColor(255,246,240, 100));
		painter->fillRect(widget->rect(), translucentBrush);
		painter->setPen(QColor("darkblue"));
		painter->drawText(widget->rect(), Qt::AlignLeft | Qt::AlignVCenter, widget->metaObject()->className()); */
	}
}
