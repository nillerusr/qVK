#include "dialogwidget.h"
#include "ui_dialogwidget.h"
#include <QWidget>
#include <QPainter>

DialogWidget::DialogWidget(QWidget *parent, QString dialognm, QString lastmsg, int unread, QString time) :
	QWidget(parent),
	ui(new Ui::DialogWidget)
{
	ui->setupUi(this);

	ui->dialogname->setText(dialognm);
	ui->lastmessage->setText(lastmsg);
	setUnreadCount(unread);
	ui->datetime->setText(time);
	peer_id = 0;
}

void DialogWidget::setDialogName(QString name)
{
	ui->dialogname->setText(name);
}

void DialogWidget::setUnreadCount( int unread )
{
	ui->unread_count->setText(QString::number(unread));
	if( unread == 0 )
		ui->unread_count->hide();
	else if( ui->unread_count->isHidden() )
		ui->unread_count->show();
}

void DialogWidget::setLastMessageText( QString text )
{
	ui->lastmessage->setText(text);
}

DialogWidget::~DialogWidget()
{
	delete ui;
}

void DialogWidget::mousePressEvent ( QMouseEvent * )
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

	this->setStyleSheet("background-color: rgb(165, 175, 225);");
	dialogLabel->setText(ui->dialogname->text());
	emit dialogSelected(this);
}

void DialogWidget::setPhoto(QPixmap pix)
{
	ui->photo->setPixmap( pix );
	ui->photo->setScaledContents( true );
}
