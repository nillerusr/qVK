#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>

namespace Ui {
class settingswindow;
}

class settingswindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit settingswindow(QWidget *parent = nullptr);
	~settingswindow();
private slots:	
	void on_cancel_released();
	
	void on_apply_released();
	
	void on_ok_released();
	
private:
	void applySettings();
	void closeEvent(QCloseEvent *event);	
	Ui::settingswindow *ui;
};

#endif // SETTINGSWINDOW_H
