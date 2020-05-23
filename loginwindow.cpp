#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "vk_api.h"
#include <QMessageBox>
#include "messageswindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::LoginWindow)
{
	ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
	delete ui;
}

void LoginWindow::on_cancelButton_released()
{
	qApp->quit();
}

void LoginWindow::on_loginButton_released()
{
	QMessageBox Msgbox;
	QString username = ui->usernameEdit->text();
	QString passwd = ui->passwordEdit->text();
	MessagesWindow *messagewin;

	if( username.isEmpty() || passwd.isEmpty() )
	{
		Msgbox.setText("Ошибка: логин или пароль не введены !");
		Msgbox.exec();
	}
	else
	{
		if( !vkapi.login(username, passwd) )
		{
			Msgbox.setText("Ошибка: логин или пароль введены не верно!");
			Msgbox.exec();
		}
		else
		{
			messagewin = new MessagesWindow();
			Msgbox.exec();
			this->hide();
			messagewin->show();
		}
	}
}
