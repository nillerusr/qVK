#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "downloadmanager.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit LoginWindow(QWidget *parent = nullptr);
	~LoginWindow();

	DownloadManager captcha_img_loader;
private slots:
	void on_cancelButton_released();
	void on_loginButton_released();
	void captcha_img_downloaded(QString filename, int error);

private:
	
	Ui::LoginWindow *ui;
};

#endif // MAINWINDOW_H
