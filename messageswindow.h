#ifndef MESSAGESWINDOW_H
#define MESSAGESWINDOW_H

#include <QMainWindow>

namespace Ui {
class MessagesWindow;
}

class MessagesWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MessagesWindow(QWidget *parent = nullptr);
	~MessagesWindow();

private:
	Ui::MessagesWindow *ui;
};

#endif // MESSAGESWINDOW_H
