#ifndef WSCROLLAREA_H
#define WSCROLLAREA_H

#include <QMainWindow>
#include <QScrollArea>
#include <QObject>

class WScrollArea : public QScrollArea
{
	Q_OBJECT
	
public:
	WScrollArea(QWidget *parent = nullptr);
	
	void scrollContentsBy(int dx, int dy);
	
	bool isScrolledUp();	
	bool isScrolledDown();

	bool m_bScrolledDown;
	bool m_bScrollDownNeed;
	
private slots: 
	void moveScrollBarToBottom(int min, int max);

signals:
	void scrolledDown();
	void scrolledUp();	
};

#endif // WSCROLLAREA_H
