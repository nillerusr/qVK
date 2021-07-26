#include "wtextbrowser.h"
#include <QtDebug>


WTextBrowser::WTextBrowser(QWidget* parent) : QTextBrowser(parent)
{
	setFrameStyle(QFrame::NoFrame);
	viewport()->setAutoFillBackground(false);
	
	setLineWrapMode(QTextEdit::WidgetWidth);
	setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	document()->setDocumentMargin(0);
	
	minHeight = 0;
}

/*
void WTextBrowser::showEvent(QShowEvent *event)
{	
	QTextBrowser::showEvent(event);
}
*/

void WTextBrowser::paintEvent(QPaintEvent *event)
{
	QTextBrowser::paintEvent(event);
	if( !minHeight ) minHeight = minimumHeight();
	setFixedHeight(document()->size().height() > minHeight ? document()->size().height() : minHeight);
}
