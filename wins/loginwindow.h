#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include "utils/downloadmanager.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit LoginWindow(QWidget *parent = nullptr);
	~LoginWindow();

	void tryLogin();

	
	DownloadManager captcha_img_loader;
private slots:

	void captcha_img_downloaded(QString filename, download_info info, int error);
	void on_loginButton_released();
	void on_cancelButton_released();
	

private:
	virtual void keyPressEvent(QKeyEvent *e) override;
	
	QLabel *captchaImg;
	QLineEdit *captchaEdit;
	
	Ui::LoginWindow *ui;
};

#endif // MAINWINDOW_H
