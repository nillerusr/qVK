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

bool WScrollArea::isScrolledDown()
{
	if(this->verticalScrollBar()->value() == this->verticalScrollBar()->maximum())
		return true;
	return false;
}

bool WScrollArea::isScrolledUp()
{
	if( this->verticalScrollBar()->value() == this->verticalScrollBar()->minimum() )
		return true;
	return false;
}
