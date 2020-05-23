#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QFrame>

namespace Ui {
class messagewidget;
}

class messagewidget : public QFrame
{
	Q_OBJECT

public:
	explicit messagewidget(QWidget *parent = nullptr, QString nickname = "", QString msg = "", QString time = "");
	~messagewidget();

private:
	Ui::messagewidget *ui;
};

#endif // MESSAGEWIDGET_H
