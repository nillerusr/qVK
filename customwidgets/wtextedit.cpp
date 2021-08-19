#include "wtextedit.h"
#include <QKeyEvent>
#include <QGuiApplication>

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
	if( event->type() == QKeyEvent::KeyPress && (event->key() == Qt::Key_Return
		&& !(QGuiApplication::queryKeyboardModifiers() & Qt::ShiftModifier)) )
	{
		emit sKeyPressEvent(event);
		return;
	}

	QTextEdit::keyPressEvent(event);

	if( document()->size().height() <= maxHeight)
		setFixedHeight(document()->size().height()+4);
}
