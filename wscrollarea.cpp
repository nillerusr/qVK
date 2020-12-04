#include "wscrollarea.h"
#include <QScrollBar>
#include <QtCore>

void WScrollArea::scrollContentsBy(int dx, int dy)
{
    QScrollArea::scrollContentsBy(dx, dy);

    bool scrolled_top = false;
	
    if(this->verticalScrollBar()->value() == this->verticalScrollBar()->minimum())
        scrolled_top = true;

    bool scrolled_bottom = false;
    if(this->verticalScrollBar()->value() == this->verticalScrollBar()->maximum())
        scrolled_bottom = true;

    if(scrolled_top)
        qDebug() << "TOP";

    if(scrolled_bottom)
        qDebug() << "BOTTOM";
}
