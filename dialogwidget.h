#ifndef DIALOGWIDGET_H
#define DIALOGWIDGET_H

#include <QWidget>

namespace Ui {
class DialogWidget;
}

class DialogWidget : public QWidget
{
	Q_OBJECT

public:
	void mousePressEvent ( QMouseEvent * event ) override;
	explicit DialogWidget(QWidget *parent = nullptr, QString dialognm = "", QString lastmsg = "");
	~DialogWidget();

	void setDialogName(QString name);
	
	int peer_id;
	QString type;
private:
	Ui::DialogWidget *ui;
};

#endif // DIALOGWIDGET_H
