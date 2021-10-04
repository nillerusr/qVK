#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QMainWindow>
#include <QDebug>

settingswindow::settingswindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::settingswindow)
{	
	ui->setupUi(this);
}

settingswindow::~settingswindow()
{
	delete ui;
}

void settingswindow::applySettings()
{
	
}

void settingswindow::closeEvent(QCloseEvent *event)
{
	QMainWindow *window;
	if( parent() )
	{
		window = qobject_cast<QMainWindow*>(parent());
		window->show();
	}
}

void settingswindow::on_cancel_released()
{
    close();
}

void settingswindow::on_apply_released()
{
    applySettings();
}


void settingswindow::on_ok_released()
{
	applySettings();
	close();    
}

