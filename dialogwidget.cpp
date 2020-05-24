#include "dialogwidget.h"
#include "ui_dialogwidget.h"
#include <QWidget>

DialogWidget::DialogWidget(QWidget *parent, QString dialognm, QString lastmsg) :
	QWidget(parent),
	ui(new Ui::DialogWidget)
{
	ui->setupUi(this);

	ui->dialogname->setText(dialognm);
	ui->lastmessage->setText(lastmsg);
}

DialogWidget::~DialogWidget()
{
	delete ui;
}

void DialogWidget::mousePressEvent ( QMouseEvent * event )
{
	QWidget *win = this->window();
	QLayout *layout = win->findChild<QLayout*>("dialogsLayout");
	QLabel *dialogLabel = win->findChild<QLabel*>("dialogLabel");

	for(int i = 0; i < layout->count(); i++)
	{
		QWidget *w = layout->itemAt(i)->widget();
		if (w != NULL)
			w->setStyleSheet("");
	}

	this->setStyleSheet("background-color: rgb(200, 200, 200);");
	dialogLabel->setText(ui->dialogname->text());
}
