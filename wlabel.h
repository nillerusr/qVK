#ifndef WLABEL_H
#define WLABEL_H

#include <QWidget>
#include <QtCore>

class WLabel : public QWidget
{
	Q_OBJECT
public:
	explicit WLabel(QWidget *parent = nullptr);
	virtual void paintEvent(QPaintEvent *event);
	void setText(QString text);
private:
	QString text;
signals:
	
};

#endif // WLABEL_H
