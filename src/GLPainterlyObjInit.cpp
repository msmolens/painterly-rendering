#include "GLPainterlyObj.h"

void
GLPainterlyObj::init_rendertextures ()
{
	if (rt_canvas != NULL) {
		rt_canvas->Resize (tex.width (), tex.height ());
	} else {
		rt_canvas = init_rendertexture (RT_INITSTR_CANVAS);
	}
	if (rt1 != NULL) {
		rt1->Resize (tex.width (), tex.height ());
	} else {
		rt1 = init_rendertexture (RT_INITSTR);
	}
	if (rt2 != NULL) {
		rt2->Resize (tex.width (), tex.height ());
	} else {
		rt2 = init_rendertexture (RT_INITSTR);
	}
	if (rt_sobel != NULL) {
		rt_sobel->Resize (tex.width (), tex.height ()); 
	} else {
		rt_sobel = init_rendertexture (RT_INITSTR_FLOAT);
	}
}

void
GLPainterlyObj::init_texture (const QImage &image)
{
	if (source_image != 0) {
		glDeleteTextures (1, &source_image);
	}
	glGenTextures (1, &source_image);
	glBindTexture (GL_TEXTURE_RECTANGLE_NV, source_image);
	glTexParameteri (GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D (GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, image.width (), image.height (), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits ());
}

RenderTexture *
GLPainterlyObj::init_rendertexture (const char *init_str)
{
	RenderTexture *rt = new RenderTexture (init_str);
	if (!rt->Initialize (tex.width (), tex.height ())) {
		std::cerr << "Unable to initialize RenderTexture" << std::endl;
		exit (EXIT_FAILURE);
	}
	return rt;
}
