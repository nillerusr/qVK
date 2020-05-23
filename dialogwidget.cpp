#include "dialogwidget.h"
#include "ui_dialogwidget.h"

DialogWidget::DialogWidget(QWidget *parent, QString dialognm, QString lastmsg) :
	QWidget(parent),
	ui(new Ui::DialogWidget)
{
	ui->setupUi(this);
	QLabel *dialogname = findChild<QLabel*> ("dialogname");
	dialogname->setText(dialognm);
	QLabel *lastmessage = findChild<QLabel*> ("lastmessage");
	lastmessage->setText(lastmsg);

}

DialogWidget::~DialogWidget()
{
	delete ui;
}
