#ifndef WTEXTEDIT_H
#define WTEXTEDIT_H

#include <QMainWindow>
#include <QTextEdit>
#include <QScrollBar>
#include <QObject>
#include <QtCore>

class WTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	WTextEdit(QWidget *parent = nullptr);
	void keyPressEvent(QKeyEvent *event);
signals:
	void sKeyPressEvent(QKeyEvent *event);	
private:
	int maxHeight;
};

#endif // WTEXTEDIT_H
