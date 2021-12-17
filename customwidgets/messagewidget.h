#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QFrame>
#include <QMap>
#include <QLabel>

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
	void setStatusText( QString status );
	void addImageAttachment( QSize size, int queue_id );
	void setImageAttachment( QPixmap pix, int queue_id );
	
	QString getText();
	
	int message_id;
	message_status_e status;
	
private:
	
	QMap<int, QLabel*> images_map;
//	void resizeEvent( QResizeEvent *event );
	
	Ui::messagewidget *ui;
};

struct image_download_queue
{
	messagewidget *msg;
	int quque_id;
};


#endif // MESSAGEWIDGET_H
