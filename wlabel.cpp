#include "wlabel.h"
#include <QPainter>
#include <QApplication>

WLabel::WLabel(QWidget *parent) : QWidget(parent)
{
	text = "";
}

void WLabel::setText(QString usertext)
{
	text = usertext;
	repaint();
}

void WLabel::paintEvent(QPaintEvent *event)
{	
	QRect rect(0,0, width(), height());
	QPainter painter(this);
	
	QFont defaultFont = QApplication::font();
	QFontMetrics fontMetrics(defaultFont); 
	QRect displayRect = fontMetrics.boundingRect(0, rect.top(), rect.width(), 0,
		Qt::AlignLeft|Qt::AlignTop|Qt::TextWrapAnywhere|Qt::TextWordWrap, text);

/*
	painter.save();
	painter.setPen( QPen(Qt::red, 2));
	painter.drawRect(rect);
	painter.setPen( QPen(Qt::yellow, 2));
	painter.drawRect(displayRect);
	painter.restore();
*/

	setFixedHeight(minimumHeight() > displayRect.height() ? minimumHeight() : displayRect.height());
	painter.setFont(defaultFont);
	painter.drawText(displayRect, Qt::AlignLeft|Qt::TextWordWrap|Qt::TextWrapAnywhere, text);	
}
