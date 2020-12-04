#include "wscrollarea.h"
#include <QScrollBar>
#include <QtCore>

void WScrollArea::scrollContentsBy(int dx, int dy)
{
    QScrollArea::scrollContentsBy(dx, dy);

	if(this->verticalScrollBar()->value() == this->verticalScrollBar()->maximum())
		emit scrolledDown();
	else if( this->verticalScrollBar()->value() == this->verticalScrollBar()->minimum() )
		emit scrolledUp();
}
