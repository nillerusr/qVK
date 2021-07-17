#ifndef DIALOGWIDGET_H
#define DIALOGWIDGET_H

#include <QWidget>
#include <QtCore>

namespace Ui {
class DialogWidget;
}

class DialogWidget : public QWidget
{
	Q_OBJECT

public:
	void mousePressEvent ( QMouseEvent * event ) override;
	explicit DialogWidget(QWidget *parent = nullptr, QString dialognm = "", QString lastmsg = "", int unread = 0, QString times = "");
	~DialogWidget();

	void setDialogName(QString name);
	void setUnreadCount( int unread );
	void setLastMessageText( QString text );
	void setPhoto( QPixmap pix );
	
	int peer_id;
	QString type, photo;
	QList<QJsonObject> messages;
	QList<QJsonObject> profiles;
	
private:
	Ui::DialogWidget *ui;
signals:
	void dialogSelected(DialogWidget *dialog);
};

#endif // DIALOGWIDGET_H
