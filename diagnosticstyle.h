#ifndef DIAGNOSTICSTYLE_H
#define DIAGNOSTICSTYLE_H

#include <QCommonStyle>

class DiagnosticStyle : public QCommonStyle
{
public:
	DiagnosticStyle();
	
	typedef QStyle BaseStyle;
	void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
};

#endif // DIAGNOSTICSTYLE_H
