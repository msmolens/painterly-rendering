#include "GLPainterlyObj.h"
#include <qapplication.h>
#include <qtimer.h>
#include <qdatetime.h>

const char *GLPainterlyObj::RT_INITSTR_CANVAS = "rgba depth texRECT";
const char *GLPainterlyObj::RT_INITSTR        = "rgb texRECT";
const char *GLPainterlyObj::RT_INITSTR_FLOAT  = "rgb=16f";
const float GLPainterlyObj::brushes[4] = {2, 4, 8, 16};
const int redraw_wait = 50;

static void
handle_cg_error ()
{
	CGerror error = cgGetError ();
	if (error != CG_NO_ERROR) {
		std::cerr << cgGetErrorString (error) << std::endl;
		exit (EXIT_FAILURE);
	}
}

void
GLPainterlyObj::initializeGL ()
{
	// Initialize GLEW.
	int err = glewInit ();
	if (GLEW_OK != err) {
		std::cerr << "GLEW ERROR: " << glewGetErrorString (err) << std::endl;
		exit (EXIT_FAILURE);
	}
	std::cerr << "Using GLEW " << glewGetString (GLEW_VERSION) << std::endl;

	// Load image specified on command-line.
	if (qApp->argc() > 1) {
		load_image (qApp->argv()[1]);
		init_rendertextures ();
	}
	
	// Initialize Cg.
	cgSetErrorCallback (::handle_cg_error);
	context = cgCreateContext ();
	load_profiles ();
	load_cg_programs ();
	glClearColor (0.0, 0.0, 0.0, 1.0);
	glPixelStorei (GL_PACK_ALIGNMENT, 1);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glDisable (GL_LIGHTING);
	make_gaussian (weights[0], 5, 2.0);
	make_gaussian (weights[1], 9, 3.0);
	make_gaussian (weights[2], 15, 4.0);
	make_gaussian (weights[3], 19, 6.0);
	make_average (eweights[0], 5);
	make_average (eweights[1], 9);
	make_average (eweights[2], 15);
	make_average (eweights[3], 19);
	initialized = true;
}

GLPainterlyObj::GLPainterlyObj (QGLFormat &format,
				QWidget* parent,
				const char* name)
	: QGLWidget (format, parent, name),
	  repaint_canvas (true),
	  rt_canvas (NULL),
	  rt1 (NULL),
	  rt2 (NULL),
	  rt_sobel (NULL),
	  initialized (false),
	  one_to_one (false)
{
	set_style_impressionism_narrow_short ();
	setAutoBufferSwap (false);
	timer = new QTimer (this);
	connect (timer, SIGNAL(timeout()), SLOT(update()));
	timer->start (redraw_wait, TRUE);
}

GLPainterlyObj::~GLPainterlyObj ()
{
	delete timer;
}

void
GLPainterlyObj::paintGL ()
{
	// Paint canvas.
	if (repaint_canvas && source_image && rt_canvas) {
		glClearColor (0.3, 0.3, 0.3, 1.0);
		glClear (GL_COLOR_BUFFER_BIT);
		glColor3f (1.0, 1.0, 1.0);
		QFont font ("SansSerif", 18, QFont::Normal);
		font.setStyleStrategy (QFont::PreferAntialias);
		renderText (8, 25, "Please wait...", font);
		swapBuffers ();

		QTime timer;
		timer.start ();
		std::cerr << "Painting..." << std::endl;
		paint (source_image);
		repaint_canvas = false;
		std::cerr << "done" << std::endl;
		std::cerr << "Paint: " << timer.elapsed () << " ms"<< std::endl;
	}

	// Display canvas.
	if (one_to_one) {
		glViewport (0, 0, tex.width(), tex.height ());
	} else {
		glViewport (0, 0, width(), height());
	}
	if (source_image && rt_canvas) {
		glClearColor (0.0, 0.0, 0.0, 1.0);
		glClear (GL_COLOR_BUFFER_BIT);
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();
		cgGLBindProgram (vp_passthrough);
		cgGLEnableProfile (vertex_profile);
		cgGLBindProgram (fp_passthrough);
		cgGLEnableProfile (fragment_profile);
		cgGLSetTextureParameter (cgGetNamedParameter (fp_passthrough, "image"), rt_canvas->GetTextureID ());
		cgGLEnableTextureParameter (cgGetNamedParameter (fp_passthrough, "image"));
		draw_quad ();
		cgGLDisableTextureParameter (cgGetNamedParameter (fp_passthrough, "image"));
		cgGLDisableProfile (vertex_profile);
		cgGLDisableProfile (fragment_profile);
	} else {
		glClearColor (0.3, 0.3, 0.3, 1.0);
		glClear (GL_COLOR_BUFFER_BIT);
	}
	swapBuffers ();
	timer->start (redraw_wait, TRUE);
}

void
GLPainterlyObj::resizeGL (int w,
			  int h)
{
	updateGL ();
}

