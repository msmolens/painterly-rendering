#include <qpushbutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <qframe.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qapplication.h>
#include <qkeycode.h>
#include "GLWindow.h"
#include "GLPainterlyObj.h"

GLWindow::GLWindow (QWidget* parent,
		    const char* name)
	: QWidget (parent, name)
{
	// Create frame.
	QFrame* f1 = new QFrame (this, "frame1");
//	f1->setFrameStyle (QFrame::Sunken | QFrame::Panel);
//	f1->setLineWidth (2);

	// Create painter widget.
	QGLFormat format;
	format.setDepth (false);
	GLPainterlyObj *c = new GLPainterlyObj (format, f1, "glpainterlyobj1");

	// Create menu.
	QPopupMenu *file = new QPopupMenu (this);
	file->insertItem ("Open", c, SLOT(file_open()), CTRL+Key_O);
	file->insertItem ("Save", c, SLOT(file_save()), CTRL+Key_S);
	file->insertItem ("Close", c, SLOT(file_close()), CTRL+Key_C);
	file->insertSeparator ();
	file->insertItem ("Exit", qApp, SLOT(quit()), CTRL+Key_Q);
	QPopupMenu *options = new QPopupMenu (this);
	options->insertItem ("Toggle Fill Window", c, SLOT(options_fill()), CTRL+Key_F);
	options->insertSeparator ();
	options->insertItem ("Impressionism Narrow Short", c, SLOT(set_style_impressionism_narrow_short()));
	options->insertItem ("Impressionism Narrow Long", c, SLOT(set_style_impressionism_narrow_long()));
	options->insertItem ("Impressionism Broad Short", c, SLOT(set_style_impressionism_broad_short()));
	options->insertItem ("Impressionism Broad Long", c, SLOT(set_style_impressionism_broad_long()));
	options->insertItem ("Expressionism", c, SLOT(set_style_expressionism()));
	options->insertItem ("Expressionism Opaque", c, SLOT(set_style_expressionism_opaque()));
	options->insertItem ("Expressionism Curvy", c, SLOT(set_style_expressionism_curvy()));
	options->insertItem ("Pointillism Small", c, SLOT(set_style_pointillism_small()));
	options->insertItem ("Pointillism Medium", c, SLOT(set_style_pointillism_medium()));
	options->insertItem ("Pointillism Large", c, SLOT(set_style_pointillism_large()));
	options->insertItem ("Colorist Wash", c, SLOT(set_style_wash()));
	options->insertItem ("Colorist Wash Broad", c, SLOT(set_style_wash_broad()));
    
	// Create a menu bar.
	QMenuBar *m = new QMenuBar (this);
	m->setSeparator (QMenuBar::InWindowsStyle);
	m->insertItem ("&File", file);
	m->insertItem ("&Options", options);

	// Put painter widget inside frame.
	QHBoxLayout* flayout1 = new QHBoxLayout (f1, 2, 2, "flayout1");
	flayout1->addWidget (c, 1);

	// Top level layout.
	QHBoxLayout* hlayout = new QHBoxLayout (this, 0, 0, "hlayout");
	hlayout->setMenuBar (m);
	hlayout->addWidget (f1, 1);
}
