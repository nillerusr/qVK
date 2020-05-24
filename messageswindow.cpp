#include "messageswindow.h"
#include "ui_messageswindow.h"
#include "dialogwidget.h"
#include "vksdk.h"
#include "messagewidget.h"

MessagesWindow::MessagesWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MessagesWindow)
{
	ui->setupUi(this);
	lp.getLongPollServer();

	/*
	 *
	 * example
	 *
	 */
	for( int i = 0; i < 50; i++)
	{
		DialogWidget *dialogwidget = new DialogWidget(nullptr, "dialog "+QString::number(i), "lastmessage "+QString::number(i) );
		ui->dialogsLayout->addWidget(dialogwidget);
//		messagewidget *mw = new messagewidget(nullptr, "name "+QString::number(i), "message "+QString::number(i)+"\nsasasa\nhahaha\nКИКЕР СОСАТБ", "time "+QString::number(i) );
//		ui->messagesLayout->addWidget(mw);
	}

}

MessagesWindow::~MessagesWindow()
{
	delete ui;
}
