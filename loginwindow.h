#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit LoginWindow(QWidget *parent = nullptr);
	~LoginWindow();

private slots:
	void on_cancelButton_released();
	void on_loginButton_released();

private:
	Ui::LoginWindow *ui;
};

#endif // MAINWINDOW_H
