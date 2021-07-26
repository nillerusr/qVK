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
signals:
	void sKeyPressEvent(QKeyEvent *event);	
private:
	virtual void showEvent(QShowEvent *event) override;
	virtual void keyPressEvent(QKeyEvent *event) override;

	int maxHeight;
};

#endif // WTEXTEDIT_H
