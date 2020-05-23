#include "messagewidget.h"
#include "ui_messagewidget.h"

messagewidget::messagewidget(QWidget *parent, QString nickname, QString msg, QString time) :
	QFrame(parent),
	ui(new Ui::messagewidget)
{
	ui->setupUi(this);

	QLabel *labelname = findChild<QLabel*> ("nickname");
	labelname->setText(nickname);
	QLabel *messagelabel = findChild<QLabel*> ("message");
	messagelabel->setText(msg);
	QLabel *datetimelabel = findChild<QLabel*> ("datetime");
	datetimelabel->setText(time);
}

messagewidget::~messagewidget()
{
	delete ui;
}
