#ifndef WTEXTBROWSER_H
#define WTEXTBROWSER_H

#include <QTextBrowser>
#include <QWidget>

class WTextBrowser : public QTextBrowser
{
	Q_OBJECT
	
public:
	explicit WTextBrowser(QWidget* parent = nullptr);

private:
	//virtual void resizeEvent (QResizeEvent *event) override;
	//virtual void showEvent(QShowEvent *event) override;
	virtual void paintEvent(QPaintEvent *event) override;
	
	int minHeight;
	
};

#endif // WTEXTBROWSER_H
