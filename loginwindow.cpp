#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "vksdk.h"
#include <QMessageBox>
#include "messageswindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::LoginWindow)
{
	ui->setupUi(this);
	ui->captchaImg->setVisible(false);
	ui->captchaEdit->setVisible(false);
	connect( &captcha_img_loader, SIGNAL(downloaded(QString, int)), SLOT(captcha_img_downloaded(QString, int)) );	
	captcha_img_loader.setDownloadDirectory(".captcha");
}

LoginWindow::~LoginWindow()
{
	delete ui;
}

void LoginWindow::on_cancelButton_released()
{
	qApp->quit();
}

void LoginWindow::captcha_img_downloaded(QString filename, int error)
{
	QPixmap pix;
	if( pix.load(filename) )
	{
		ui->captchaImg->setVisible(true);
		ui->captchaEdit->setVisible(true);
		
		ui->captchaImg->setPixmap(pix);
		ui->captchaImg->setScaledContents(true);
	}
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
		const QJsonObject error = vkapi.login(username, passwd, ui->captchaEdit->text());
		if( !error.isEmpty() )
		{
			qDebug() << error;
			if( !error["captcha_img"].isUndefined() )
			{
				captcha_img_loader.append(error["captcha_img"].toString(), error["captcha_sid"].toString());
				captcha_img_loader.download();
			}
			Msgbox.setText("Ошибка: "+error["error"].toString());
			Msgbox.exec();
		}
		else
		{
			messagewin = new MessagesWindow();
			this->hide();
			messagewin->show();
		}
	}
}
