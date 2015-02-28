#include <qapplication.h>
#include <qgl.h>
#include <cstdlib>
#include <ctime>
#include "GLWindow.h"

int
main (int argc,
      char *argv[])
{
	srand48 (time (NULL));
	QApplication::setColorSpec( QApplication::CustomColor );
	QApplication a(argc,argv);
	if (!QGLFormat::hasOpenGL()) {
		qWarning ("This system has no OpenGL support. Exiting.");
		return (EXIT_FAILURE);
	}
	GLWindow* w = new GLWindow;
	w->setMinimumSize (160, 120);
	w->resize (320, 240);
	w->setCaption ("GPU-Accelerated Painterly Rendering");
	a.setMainWidget (w);
	w->show ();
	int result = a.exec ();
	delete w;
	return result;
}
