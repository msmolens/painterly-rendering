#include "GLPainterlyObj.h"

void
GLPainterlyObj::set_style_impressionism_narrow_short ()
{
	std::cerr << "Impressionism Narrow Short" << std::endl;
	points = true;
	tolerance = 25;
	enable_brush[0] = true;
	enable_brush[1] = true;
	enable_brush[2] = true;
	enable_brush[3] = true;
	fg = 0.5;
	fc = 1.0;
	brush_scale = 2;
	min_stroke_length = 2;
	max_stroke_length = 5;
	opacity = 255;
	jh = 0.0;
	js = 0.0;
	jv = 0.0;
	jr = 0.0;
	jg = 0.0;
	jb = 0.0;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}

void
GLPainterlyObj::set_style_impressionism_narrow_long ()
{
	std::cerr << "Impressionism Narrow Long" << std::endl;
	points = true;
	tolerance = 25;
	enable_brush[0] = true;
	enable_brush[1] = true;
	enable_brush[2] = true;
	enable_brush[3] = true;
	fg = 0.5;
	fc = 1.0;
	brush_scale = 2;
	min_stroke_length = 5;
	max_stroke_length = 16;
	opacity = 255;
	jh = 0.0;
	js = 0.0;
	jv = 0.0;
	jr = 0.0;
	jg = 0.0;
	jb = 0.0;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}

void
GLPainterlyObj::set_style_impressionism_broad_short ()
{
	std::cerr << "Impressionism Broad Short" << std::endl;
	points = true;
//	tolerance = 100;
//	tolerance = 50;
	tolerance = 75;
	enable_brush[0] = true;
	enable_brush[1] = true;
	enable_brush[2] = true;
	enable_brush[3] = true;
	fg = 1.0;
//	fg = 0.5;
	fc = 1.0;
	brush_scale = 3;
	min_stroke_length = 2;
	max_stroke_length = 5;
	opacity = 255;
	jh = 0.0;
	js = 0.0;
	jv = 0.0;
	jr = 0.0;
	jg = 0.0;
	jb = 0.0;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}

void
GLPainterlyObj::set_style_impressionism_broad_long ()
{
	std::cerr << "Impressionism Broad Long" << std::endl;
	points = true;
//	tolerance = 100;
//	tolerance = 50;
	tolerance = 75;
	enable_brush[0] = true;
	enable_brush[1] = true;
	enable_brush[2] = true;
	enable_brush[3] = true;
	fg = 1.5;
//	fg = 0.5;
	fc = 1.0;
	brush_scale = 3;
	min_stroke_length = 5;
	max_stroke_length = 12;
	opacity = 255;
	jh = 0.0;
	js = 0.0;
	jv = 0.0;
	jr = 0.0;
	jg = 0.0;
	jb = 0.0;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}

void
GLPainterlyObj::set_style_expressionism ()
{
	std::cerr << "Expressionism" << std::endl;
	points = false;
	tolerance = 50;
	enable_brush[0] = false;
	enable_brush[1] = true;
	enable_brush[2] = true;
	enable_brush[3] = true;
	fg = 1.0;
	fc = 0.02;
	brush_scale = 2;
	min_stroke_length = 5;
	max_stroke_length = 10;
	opacity = 180;
	jh = 0.0;
	js = 0.0;
	jv = 0.3;
	jr = 0.0;
	jg = 0.0;
	jb = 0.0;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}

void
GLPainterlyObj::set_style_expressionism_opaque ()
{
	std::cerr << "Expressionism Opaque" << std::endl;
	points = true;
	tolerance = 50;
	enable_brush[0] = true;
	enable_brush[1] = true;
	enable_brush[2] = true;
	enable_brush[3] = true;
	fg = 1.0;
	fc = 0.02;
	brush_scale = 2;
	min_stroke_length = 5;
	max_stroke_length = 10;
	opacity = 180;
	jh = 0.0;
	js = 0.0;
	jv = 0.3;
	jr = 0.0;
	jg = 0.0;
	jb = 0.0;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}

void
GLPainterlyObj::set_style_expressionism_curvy ()
{
	std::cerr << "Expressionism Curvy" << std::endl;
	points = true;
	tolerance = 50;
	enable_brush[0] = true;
	enable_brush[1] = true;
	enable_brush[2] = true;
	enable_brush[3] = true;
	fg = 1.0;
	fc = 0.25;
	brush_scale = 2;
	min_stroke_length = 10;
	max_stroke_length = 20;
	opacity = 180;
	jh = 0.0;
	js = 0.0;
	jv = 0.3;
	jr = 0.0;
	jg = 0.0;
	jb = 0.0;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}

void
GLPainterlyObj::set_style_pointillism_small ()
{
	std::cerr << "Pointillism Small" << std::endl;
	points = true;
	tolerance = 25;
	enable_brush[0] = true;
	enable_brush[1] = true;
	enable_brush[2] = false;
	enable_brush[3] = false;
	fg = 0.5;
	fc = 1.0;
	brush_scale = 1;
	min_stroke_length = 0;
	max_stroke_length = 0;
	opacity = 255;
	jh = 0.3;
	js = 0.0;
	jv = 1.0;
	jr = 0.0;
	jg = 0.0;
	jb = 0.0;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}

void
GLPainterlyObj::set_style_pointillism_medium ()
{
	std::cerr << "Pointillism Medium" << std::endl;
	points = true;
	tolerance = 25;
	enable_brush[0] = true;
	enable_brush[1] = true;
	enable_brush[2] = false;
	enable_brush[3] = false;
	fg = 0.5;
	fc = 1.0;
	brush_scale = 2;
	min_stroke_length = 0;
	max_stroke_length = 0;
	opacity = 255;
	jh = 0.3;
	js = 0.0;
	jv = 1.0;
	jr = 0.0;
	jg = 0.0;
	jb = 0.0;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}

void
GLPainterlyObj::set_style_pointillism_large ()
{
	std::cerr << "Pointillism Large" << std::endl;
	points = true;
	tolerance = 25;
	enable_brush[0] = true;
	enable_brush[1] = true;
	enable_brush[2] = false;
	enable_brush[3] = false;
	fg = 0.5;
	fc = 1.0;
	brush_scale = 3;
	min_stroke_length = 0;
	max_stroke_length = 0;
	opacity = 255;
	jh = 0.3;
	js = 0.0;
	jv = 1.0;
	jr = 0.0;
	jg = 0.0;
	jb = 0.0;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}

void
GLPainterlyObj::set_style_wash ()
{
	std::cerr << "Colorist Wash" << std::endl;
	points = false;
	tolerance = 25;
	enable_brush[0] = true;
	enable_brush[1] = true;
	enable_brush[2] = true;
	enable_brush[3] = true;
	fg = 1.0;
	fc = 1.0;
	brush_scale = 2;
	min_stroke_length = 4;
	max_stroke_length = 16;
	opacity = 127;
	jh = 0.0;
	js = 0.0;
	jv = 0.0;
	jr = 0.3;
	jg = 0.3;
	jb = 0.3;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}

void
GLPainterlyObj::set_style_wash_broad ()
{
	std::cerr << "Colorist Wash Broad" << std::endl;
	points = false;
	tolerance = 25;
	enable_brush[0] = true;
	enable_brush[1] = true;
	enable_brush[2] = true;
	enable_brush[3] = true;
	fg = 1.0;
	fc = 1.0;
	brush_scale = 4;
	min_stroke_length = 4;
	max_stroke_length = 16;
	opacity = 127;
	jh = 0.0;
	js = 0.0;
	jv = 0.0;
	jr = 0.3;
	jg = 0.3;
	jb = 0.3;
	if (initialized) {
		repaint_canvas = true;
		update ();
	}
}
