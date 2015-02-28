#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <qwidget.h>

class GLWindow : public QWidget
{
	Q_OBJECT
public:
	GLWindow (QWidget* parent = 0, const char* name = 0);
};

#endif // GLWINDOW_H
