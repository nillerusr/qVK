#include "messagewidget.h"
#include "ui_messagewidget.h"

messagewidget::messagewidget(QWidget *parent, QString nickname, QString msg, QString time) :
	QFrame(parent),
	ui(new Ui::messagewidget)
{
	ui->setupUi(this);

	ui->nickname->setText(nickname);
	ui->message->setText(msg);
	ui->datetime->setText(time);
}

messagewidget::~messagewidget()
{
	delete ui;
}
