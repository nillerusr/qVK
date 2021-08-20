#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QFrame>

namespace Ui {
	class messagewidget;
}

enum message_status_e {
	MESSAGE_STATUS_NONE = 0,
	MESSAGE_QUEUED,
	MESSAGE_SENDING,
	MESSAGE_SEND_FAILED,
	MESSAGE_SEND
};

class messagewidget : public QFrame
{
	Q_OBJECT

public:
	explicit messagewidget(QWidget *parent = nullptr, QString nickname = "", QString msg = "", QString time = "");
	~messagewidget();

	void setPhoto( QPixmap pix );
	void setName( QString name );
	void setText( QString text );
	void setDateTime( QString datetime );
	void setPhoto(QString filename);
	QString getText();
	
	int message_id;
	QString photo;
	message_status_e status;
	
private:
	Ui::messagewidget *ui;
};

#endif // MESSAGEWIDGET_H
