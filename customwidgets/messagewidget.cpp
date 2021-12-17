#include "messagewidget.h"
#include "ui_messagewidget.h"
#include <QPainterPath>
#include <QPainter>
#include <QScrollBar>

messagewidget::messagewidget(QWidget *parent, QString nickname, QString msg, QString time) :
	QFrame(parent),
	ui(new Ui::messagewidget)
{
	ui->setupUi(this);

	ui->nickname->setText(nickname);
	ui->message->setText(msg);
	ui->datetime->setText(time);

	message_id = 0;
	status = MESSAGE_STATUS_NONE;
}

messagewidget::~messagewidget()
{
	delete ui;
}

void messagewidget::setPhoto(QPixmap pix)
{
	ui->photo->setPixmap( pix );
	ui->photo->setScaledContents( true );
}

void messagewidget::setPhoto(QString filename)
{
	QPixmap pix;
	if( pix.load(filename) )
	{
		ui->photo->setPixmap( pix );
		ui->photo->setScaledContents( true );
	}
}

void messagewidget::setName(QString name)
{
	ui->nickname->setText(name);
}

void messagewidget::setDateTime( QString datetime )
{
	ui->datetime->setText( datetime );
}

void messagewidget::setText( QString text )
{
	ui->message->setText(text);
}

QString messagewidget::getText()
{
	return ui->message->toPlainText();
}

void messagewidget::setStatusText( QString status )
{
	ui->status->setText( status );
}

void messagewidget::addImageAttachment(QSize size, int queue_id)
{
	QLabel *lab = new QLabel;
	
	QPixmap pixmap(size.width(), size.height());
    {
        QPainter p(&pixmap);
		QPainterPath path;
		path.addRect(0, 0, size.width(), size.height());
		QPen pen(QColor(127, 127, 127));
		p.setPen(pen);
		p.fillPath(path, QColor(127, 127, 127));
		p.drawPath(path);
    }
	
	images_map[queue_id] = lab;
	
	lab->setPixmap(pixmap);
	ui->attachments_layout->addWidget( lab );
}

void messagewidget::setImageAttachment( QPixmap pix, int queue_id )
{
	QLabel *lab = images_map[queue_id];
	if( lab )
	{
		images_map[queue_id] = nullptr;
		lab->setPixmap(pix);
	}
}
