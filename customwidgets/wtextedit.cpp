#include "wtextedit.h"

WTextEdit::WTextEdit( QWidget *parent ) : QTextEdit(parent)
{
	maxHeight = 0;
}

void WTextEdit::showEvent(QShowEvent *event)
{
	QTextEdit::showEvent(event);
	maxHeight = maximumSize().height();	
	setFixedHeight(document()->size().height()+4);
}

void WTextEdit::keyPressEvent(QKeyEvent *event)
{
	QTextEdit::keyPressEvent(event);	
	emit sKeyPressEvent(event);
	
	if( document()->size().height() <= maxHeight)
		setFixedHeight(document()->size().height()+4);
}
