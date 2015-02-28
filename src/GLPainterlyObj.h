#ifndef GLPAINTERLYOBJ_H
#define GLPAINTERLYOBJ_H

#include "RenderTexture.h"

// glew.h (in RenderTexture.h ) must be included before GL/gl.h, and therefore before qgl.h.
// Must undefine these X11 identifiers, because QT uses them.
#ifdef CursorShape
#undef CursorShape
#endif
#ifdef None
#undef None
#endif
#ifdef KeyPress
#undef KeyPress
#endif
#ifdef KeyRelease
#undef KeyRelease
#endif
#ifdef FocusIn
#undef FocusIn
#endif
#ifdef FocusOut
#undef FocusOut
#endif

#include <qgl.h>
#include <qimage.h>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>
#include <utility>
#include <algorithm>
#include <ctime>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

// Aaron Hertzmann's code.
#include "Stroke.h"
#include "Point.h"
#include "Color.h"

class GLPainterlyObj : public QGLWidget
{
	Q_OBJECT

public:
	GLPainterlyObj (QGLFormat &format, QWidget* parent, const char* name);
	~GLPainterlyObj ();

public slots:
        void file_open ();
	void file_save ();
	void file_close ();
	void options_fill ();
	void set_style_impressionism_narrow_short ();
	void set_style_impressionism_narrow_long ();
	void set_style_impressionism_broad_short ();
	void set_style_impressionism_broad_long ();
	void set_style_expressionism ();
	void set_style_expressionism_opaque ();
	void set_style_expressionism_curvy ();
	void set_style_pointillism_small ();
	void set_style_pointillism_medium ();
	void set_style_pointillism_large ();
	void set_style_wash ();
	void set_style_wash_broad ();

protected:
	void initializeGL ();
	void paintGL ();
	void resizeGL (int w, int h);
	void load_image (const char *filename);
	void save_image (const QString &filename, const QImage &image);
	void init_rendertextures ();
	void handle_cg_error ();
	void load_cg_programs ();
	void load_profiles ();
	void init_texture (const QImage &image);
	float gaussian1 (float x, float sigma);
	void make_gaussian (float weights[], int dim, float sigma);
	void make_average (float weights[], int dim);
	RenderTexture *init_rendertexture (const char *init_str);
	void draw_quad ();
	GLuint paint (GLuint source_image);
	GLubyte *gaussian_blur (int level, GLuint source_image);
	GLfloat *sobel ();
	void difference ();
	GLubyte *average_error (int level);
	void paint_canvas (const std::vector<Point> &stroke_list, GLubyte reference_image[], GLfloat gradients[], float brush_size);
	void draw_strokes (const std::vector<Point> &stroke_list, GLubyte canvas[], GLubyte reference_image[], GLfloat gradients[], float brush_size);
	void make_stroke (const Point &p, GLubyte canvas[], GLubyte reference_image[], GLfloat gradients[], float brush_size);
	bool lookup (GLubyte *dst, const GLubyte image[], int x, int y);
	float gradient_mag (const GLfloat gradients[], float x, float y);
	void gradient_dir (float *out, const GLfloat gradients[], float x, float y);
	Point rescale_vertex (const Point &p);
	float rescale (float f);
	void render_stroke (Stroke &stroke);
	int color_diff (GLubyte a[], GLubyte b[]);
	float randnum (float scale);
	bool randbool ();

private:
	QTimer* timer;

	// Source image.
	QImage tex;

	// User parameters.
	GLubyte tolerance;
	bool enable_brush[4];
	float fg;
	float fc;
	int brush_scale;
	int min_stroke_length;
	int max_stroke_length;
	GLubyte opacity;
	float jh, js, jv;	// HSV jitter
	float jr, jg, jb;	// RGB jitter

	// Cg context.
	CGcontext context;

	// Handles to vertex profiles and programs.
	CGprofile vertex_profile;
	CGprofile fragment_profile;
	CGprogram vp_passthrough;
	CGprogram fp_passthrough;
	CGprogram fp_filter_h[4];
	CGprogram fp_filter_v[4];
	CGprogram fp_difference;
	CGprogram vp_sobel, fp_sobel;

	// Tex id.
	GLuint source_image;

	// Gaussian weights.
	// Sizes: 5, 9, 15, 19
	float weights[4][19];

	// Error weights.
	// Sizes: 5, 9, 15, 19
	float eweights[4][19];

	// Brush sizes.
	//float brushes[4] = {2.5, 4.5, 7.5, 9.5};
	static const float brushes[4];

	// When true, repaint the canvas.
	bool repaint_canvas;

	// Offscreen buffers.
	RenderTexture *rt_canvas;
	RenderTexture *rt1;
	RenderTexture *rt2;
	RenderTexture *rt_sobel;
	static const char *RT_INITSTR_CANVAS;
	static const char *RT_INITSTR;
	static const char *RT_INITSTR_FLOAT;

	// Various state.
	bool initialized;
	bool one_to_one;
	bool points;
};

inline bool
GLPainterlyObj::randbool ()
{
	return drand48()<0.5;
}

inline float
GLPainterlyObj::randnum (float f)
{
	static const float JITTER_FACTOR = 0.33;
	return JITTER_FACTOR*f*drand48();
}

inline float
GLPainterlyObj::gaussian1 (float x,
			   float sigma)
{
	return exp(-0.5 * (x*x) / (sigma*sigma)) / (2*M_PI * sigma);
}

inline bool
GLPainterlyObj::lookup (GLubyte *dst,
			const GLubyte image[],
			int x,
			int y)
{
	if (x >= 0 && x < tex.width() && y >= 0 && y < tex.height ()) {
		*(dst+0) = image[tex.width ()*y*3+x*3+0];
		*(dst+1) = image[tex.width ()*y*3+x*3+1];
		*(dst+2) = image[tex.width ()*y*3+x*3+2];
		return true;
	}
	return false;
}

inline float
GLPainterlyObj::gradient_mag (const GLfloat gradients[],
			      float x,
			      float y)
{
	return gradients[tex.width ()*static_cast<int>(y)*3+static_cast<int>(x)*3];
}

inline void
GLPainterlyObj::gradient_dir (float *out,
			      const GLfloat gradients[],
			      float x,
			      float y)
{
	*(out+0) = gradients[tex.width ()*static_cast<int>(y)*3+static_cast<int>(x)*3+2];
	*(out+1) = gradients[tex.width ()*static_cast<int>(y)*3+static_cast<int>(x)*3+1];
}

inline float
GLPainterlyObj::rescale (float f)
{
	return f/tex.width ();
}

inline Point
GLPainterlyObj::rescale_vertex (const Point &p)
{
	Point q;
	q.x = 2.0f * p.x/tex.width () - 1.0f;
	q.y = 2.0f * p.y/tex.height () - 1.0f;
	return q;
}

inline void
GLPainterlyObj::render_stroke (Stroke &stroke)
{
	stroke.render (points);
//	stroke.print ();
//	stroke.drawLineCurve ();
//	stroke.drawControlPolygon ();
}

inline int
GLPainterlyObj::color_diff (GLubyte a[],
			    GLubyte b[])
{
	return abs(a[0]-b[0])+abs(a[1]-b[1])+abs(a[2]-b[2]);
}

inline void
GLPainterlyObj::draw_quad ()
{
	glBegin (GL_QUADS);
	glTexCoord2i (0, 0);                        glVertex2i (-1, -1);
	glTexCoord2i (tex.width (), 0);             glVertex2i (1, -1);
	glTexCoord2i (tex.width (), tex.height ()); glVertex2i (1, 1);
	glTexCoord2i (0, tex.height ());            glVertex2i (-1, 1);
	glEnd();
}

#endif // GLPAINTERLYOBJ_H
