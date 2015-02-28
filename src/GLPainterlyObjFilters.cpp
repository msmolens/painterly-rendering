#include "GLPainterlyObj.h"

GLubyte *
GLPainterlyObj::gaussian_blur (int level,
			       GLuint source_image)
{
	// Horizontal Gaussian pass.
	// Input: source_image
	// Output: rt1 (horizontal Gaussian pass)
	cgGLSetTextureParameter (cgGetNamedParameter (fp_filter_h[level], "image"), source_image);
	rt1->BeginCapture ();
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	cgGLBindProgram (vp_passthrough);
	cgGLEnableProfile (vertex_profile);
	cgGLBindProgram (fp_filter_h[level]);
	cgGLEnableProfile (fragment_profile);
	cgGLSetParameterArray1f (cgGetNamedParameter (fp_filter_h[level], "weights"), 0, 0, weights[level]);
	cgGLEnableTextureParameter (cgGetNamedParameter (fp_filter_h[level], "image"));
	draw_quad ();
	cgGLDisableTextureParameter (cgGetNamedParameter (fp_filter_h[level], "image"));
	cgGLDisableProfile (vertex_profile);
	cgGLDisableProfile (fragment_profile);
	rt1->EndCapture ();

	// Vertical Gaussian pass.
	// Input: rt1 (horizontal gaussian pass)
	// Output: rt2 (complete gaussian blur, i.e. reference image)
	cgGLSetTextureParameter (cgGetNamedParameter (fp_filter_v[level], "image"), rt1->GetTextureID ());
	rt2->BeginCapture ();
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	cgGLBindProgram (vp_passthrough);
	cgGLEnableProfile (vertex_profile);
	cgGLBindProgram (fp_filter_v[level]);
	cgGLEnableProfile (fragment_profile);
	cgGLSetParameterArray1f (cgGetNamedParameter (fp_filter_v[level], "weights"), 0, 0, weights[level]);
	cgGLEnableTextureParameter (cgGetNamedParameter (fp_filter_v[level], "image"));
	draw_quad ();
	cgGLDisableTextureParameter (cgGetNamedParameter (fp_filter_v[level], "image"));
	cgGLDisableProfile (vertex_profile);
	cgGLDisableProfile (fragment_profile);

	// Read-back reference image.
	GLubyte *reference_image = new GLubyte[tex.width () * tex.height () * 3];
	glReadPixels (0, 0, tex.width (), tex.height (), GL_RGB, GL_UNSIGNED_BYTE, reference_image);
	rt2->EndCapture ();
	return reference_image;
}

GLfloat *
GLPainterlyObj::sobel ()
{
	// Compute Sobel image.
	// Input: rt2 (reference image)
	// Output: sobel_image (sobel-filtered image)
	rt_sobel->BeginCapture ();
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	cgGLBindProgram (vp_sobel);
	cgGLEnableProfile (vertex_profile);
	cgGLBindProgram (fp_sobel);
	cgGLEnableProfile (fragment_profile);
	cgGLSetTextureParameter (cgGetNamedParameter (fp_sobel, "reference_image"), rt2->GetTextureID ());
	cgGLEnableTextureParameter (cgGetNamedParameter (fp_sobel, "reference_image"));
	draw_quad ();
	cgGLDisableTextureParameter (cgGetNamedParameter (fp_sobel, "reference_image"));
	cgGLDisableProfile (vertex_profile);
	cgGLDisableProfile (fragment_profile);

	// Read back Sobel image.
	GLfloat *sobel_image = new GLfloat[tex.width () * tex.height () * 3];
	glReadPixels (0, 0, tex.width (), tex.height (), GL_RGB, GL_FLOAT, sobel_image);
	rt_sobel->EndCapture ();	
	return sobel_image;
}

void
GLPainterlyObj::difference ()
{
	// Compute difference image.
	// Input: rt_canvas (current canvas), rt2 (reference image)
	// Output: rt1 (difference between canvas and reference image)
	rt1->BeginCapture ();
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	cgGLBindProgram (vp_passthrough);
	cgGLEnableProfile (vertex_profile);
	cgGLBindProgram (fp_difference);
	cgGLEnableProfile (fragment_profile);
	cgGLSetTextureParameter (cgGetNamedParameter (fp_difference, "canvas"), rt_canvas->GetTextureID ());
	cgGLSetTextureParameter (cgGetNamedParameter (fp_difference, "reference_image"), rt2->GetTextureID ());
	cgGLEnableTextureParameter (cgGetNamedParameter (fp_difference, "canvas"));
	cgGLEnableTextureParameter (cgGetNamedParameter (fp_difference, "reference_image"));
	draw_quad ();
	cgGLDisableTextureParameter (cgGetNamedParameter (fp_difference, "canvas"));
	cgGLDisableTextureParameter (cgGetNamedParameter (fp_difference, "reference_image"));
	cgGLDisableProfile (vertex_profile);
	cgGLDisableProfile (fragment_profile);
	rt1->EndCapture ();
}

// Apply box filter (separable).
// Returns pointer to average error image.
GLubyte *
GLPainterlyObj::average_error (int level)
{
	// Compute average error, horizontal pass.
	// Input: rt1 (difference image)
	// Output: rt2 (horizontal box filter pass)
	cgGLSetTextureParameter (cgGetNamedParameter (fp_filter_h[level], "image"), rt1->GetTextureID ());
	rt2->BeginCapture ();
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	cgGLBindProgram (vp_passthrough);
	cgGLEnableProfile (vertex_profile);
	cgGLBindProgram (fp_filter_h[level]);
	cgGLEnableProfile (fragment_profile);
	cgGLSetParameterArray1f (cgGetNamedParameter (fp_filter_h[level], "weights"), 0, 0, eweights[level]);
	cgGLEnableTextureParameter (cgGetNamedParameter (fp_filter_h[level], "image"));
	draw_quad ();
	cgGLDisableTextureParameter (cgGetNamedParameter (fp_filter_h[level], "image"));
	cgGLDisableProfile (vertex_profile);
	cgGLDisableProfile (fragment_profile);
	rt2->EndCapture ();

	// Compute average error, vertical pass.
	// Input: rt2 (horizontal box filter pass)
	// Output: rt1 (complete box filtered imagex)
	cgGLSetTextureParameter (cgGetNamedParameter (fp_filter_v[level], "image"), rt2->GetTextureID ());
	rt1->BeginCapture ();
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	cgGLBindProgram (vp_passthrough);
	cgGLEnableProfile (vertex_profile);
	cgGLBindProgram (fp_filter_v[level]);
	cgGLEnableProfile (fragment_profile);
	cgGLSetParameterArray1f (cgGetNamedParameter (fp_filter_v[level], "weights"), 0, 0, eweights[level]);
	cgGLEnableTextureParameter (cgGetNamedParameter (fp_filter_v[level], "image"));
	draw_quad ();
	cgGLDisableTextureParameter (cgGetNamedParameter (fp_filter_v[level], "image"));
	cgGLDisableProfile (vertex_profile);
	cgGLDisableProfile (fragment_profile);

	// Read-back average error image.
	GLubyte *error_image = new GLubyte[tex.width () * tex.height ()];
	glReadPixels (0, 0, tex.width (), tex.height (), GL_RED, GL_UNSIGNED_BYTE, error_image);
	rt1->EndCapture ();
	return error_image;
}
