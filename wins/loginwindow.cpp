#include "ui_loginwindow.h"
#include "utils/vk.h"
#include <QMessageBox>
#include <QKeyEvent>
#include "messageswindow.h"
#include "loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::LoginWindow)
{
	ui->setupUi(this);
	connect( &captcha_img_loader, SIGNAL(downloaded(QString, int)), SLOT(captcha_img_downloaded(QString, int)) );	
	captcha_img_loader.setDownloadDirectory(".captcha");
	
	captchaImg = new QLabel(this);
	captchaEdit = new QLineEdit(this);
	captchaEdit->hide();
	captchaImg->hide();
}

LoginWindow::~LoginWindow()
{
	delete ui;
}


void LoginWindow::captcha_img_downloaded(QString filename, int error)
{
	Q_UNUSED(error);
	// TODO: check for errors.
	// Hmm, I should start redownload in DownloadManager class

	QPixmap pix;
	if( pix.load(filename) )
	{
		captchaImg->setPixmap(pix);
		captchaImg->setScaledContents(true);
	}
}

void LoginWindow::tryLogin()
{
	QMessageBox Msgbox;
	QString username = ui->loginEdit->text();
	QString passwd = ui->passwordEdit->text();
	MessagesWindow *messagewin;

	if( username.isEmpty() || passwd.isEmpty() )
	{
		Msgbox.setText("Ошибка: логин или пароль не введены !");
		Msgbox.exec();
	}
	else
	{
		const QJsonObject error = vkapi.login(username, passwd, captchaEdit->text());
		if( !error.isEmpty()  )
		{
			if( error["error"] == "need_validation") // TODO: 2FA Auth
			{
				
			}

			if( !error["captcha_img"].isUndefined() )
			{
				if( ui->captchaLayout->isEmpty() )
				{
					ui->captchaLayout->addWidget(captchaImg, 0, Qt::AlignVCenter);
					ui->captchaLayout->addWidget(captchaEdit);
					captchaEdit->show(); captchaImg->show();
				}
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

void LoginWindow::keyPressEvent(QKeyEvent *e)
{
	if(e->key()==Qt::Key_Return)
		tryLogin();
}

void LoginWindow::on_loginButton_released()
{
	tryLogin();
}

void LoginWindow::on_cancelButton_released()
{
	qApp->quit();
}
