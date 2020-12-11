#include "wtextedit.h"

WTextEdit::WTextEdit( QWidget *parent ) : QTextEdit(parent)
{
	maxHeight = 0; 
}

void WTextEdit::keyPressEvent(QKeyEvent *event)
{
	//TODO: resize widget here
	QTextEdit::keyPressEvent(event);	
	emit sKeyPressEvent(event); 
	
	if( !maxHeight ) maxHeight = maximumSize().height();
	
	if( document()->size().height() <= maxHeight)
		setFixedHeight(document()->size().height()+8);
}
