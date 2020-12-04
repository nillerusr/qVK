#ifndef WSCROLLAREA_H
#define WSCROLLAREA_H

#include <QMainWindow>
#include <QScrollArea>

class WScrollArea : public QScrollArea
{
	Q_OBJECT
	
public:
	WScrollArea(QWidget *parent = nullptr)
        : QScrollArea(parent)
    {}
	
	void scrollContentsBy(int dx, int dy);
};

#endif // WSCROLLAREA_H
