#include "GLPainterlyObj.h"

void
GLPainterlyObj::paint_canvas (const std::vector<Point> &stroke_list,
			      GLubyte reference_image[],
			      GLfloat gradients[],
			      float brush_size)
{
	// Draw strokes on canvas.
	rt_canvas->BeginCapture ();

	// Read-back canvas.
	GLubyte *canvas = new GLubyte[tex.width () * tex.height () * 3];
	glReadPixels (0, 0, tex.width (), tex.height (), GL_RGB, GL_UNSIGNED_BYTE, canvas);
	glPointSize (brush_size);
//	glHint (GL_POINT_SMOOTH_HINT, GL_FASTEST);
	glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable (GL_POINT_SMOOTH);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);
	glClear (GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	draw_strokes (stroke_list, canvas, reference_image, gradients, brush_size);
	glDisable (GL_BLEND);
	rt_canvas->EndCapture ();
	delete [] canvas;
}

// Draws strokes in stroke list 
void
GLPainterlyObj::draw_strokes (const std::vector<Point> &stroke_list,
			      GLubyte canvas[],
			      GLubyte reference_image[],
			      GLfloat gradients[],
			      float brush_size)
{
	std::vector<Point>::const_iterator it;
	for (it = stroke_list.begin();
	     it != stroke_list.end();
	     it++) {
		make_stroke (*it, canvas, reference_image, gradients, brush_size);
	}
}

void
GLPainterlyObj::make_stroke (const Point &start,
			     GLubyte canvas[],
			     GLubyte reference_image[],
			     GLfloat gradients[],
			     float brush_size)
{
#if 0
	// Draw circle.
	float x = 2.0f * static_cast<float>(start.x)/tex.width () - 1.0f;
	float y = 2.0f * static_cast<float>(start.y)/tex.height () - 1.0f;
	glTexCoord2i (start.x, start.y);
	glVertex2f (x, y);
#else
	// Jitter HSV, RGB
	GLubyte r = reference_image[tex.width ()*static_cast<int>(start.y)*3+static_cast<int>(start.x)*3+0];
	GLubyte g = reference_image[tex.width ()*static_cast<int>(start.y)*3+static_cast<int>(start.x)*3+1];
	GLubyte b = reference_image[tex.width ()*static_cast<int>(start.y)*3+static_cast<int>(start.x)*3+2];
	float h, s, v;
	RGB_To_HSV (r, g, b, &h, &s, &v);
	float jitter;
	jitter = randnum (jh);
	h += randbool () ? jitter : -jitter;
	Clamp (h);
	jitter = randnum (js);
	s += randbool () ? jitter : -jitter;
	Clamp (s);
	jitter = randnum (jv);
	v += randbool () ? jitter : -jitter;
	Clamp (v);
	float fr,fg,fb;
	HSV_To_RGB (&fr, &fg, &fb, h, s, v);
	fr += randbool () ? jitter : -jitter;
	Clamp (fr);
	fg += randbool () ? jitter : -jitter;
	Clamp (fg);
	fb += randbool () ? jitter : -jitter;
	Clamp (fb);
	r = static_cast<GLubyte>(fr * 255.0f);
	g = static_cast<GLubyte>(fg * 255.0f);
	b = static_cast<GLubyte>(fb * 255.0f);
	
	GLuint stroke_color = makeColor (r, g, b, opacity);
	GLubyte stroke_color_ub[3];
	stroke_color_ub[0] = r;
	stroke_color_ub[1] = g;
	stroke_color_ub[2] = b;
	brush_size *= 0.5;
	Stroke stroke (rescale (brush_size), stroke_color, 0);
	// Unnecessary to set depth since stroke order is randomized.
	// TEMP what about with opacity
	stroke.depth () = start.z;
	Point p(start.x, start.y);
	stroke.addControlPoint (rescale_vertex (p));
	Point last_dir (0.0, 0.0);
	for (int i = 1; i <= max_stroke_length; i++) {
		// Get gradient unit vector.
		Point dir;
		gradient_dir (&dir.x, gradients, p.x, p.y);
		// Detect vanishing gradient.
		float gmag = 255.0f * gradient_mag (gradients, p.x, p.y);
		if (gmag * brush_size >= 1) {
			//dir.normalize (); // performed in sobel fragment program
			// Compute a normal direction.
			//dir.x = -dir.x; // performed in sobel fragment program
			// Reverse direction if necessary.
			if (i > 1 && last_dir.x * dir.x + last_dir.y * dir.y < 0.0f) {
				dir = -dir;
			}
			// Filter the stroke direction.
			dir = dir * fc + last_dir * (1.0f-fc);
			dir.normalize ();
		} else {
			if (i == 1) {
//				render_stroke (stroke);
				return;
			}
			dir = last_dir;
		}
		p = p + dir * brush_size;
		last_dir = dir;
		GLubyte ri_color[3];
		GLubyte canvas_color[3];
		if (!lookup (ri_color, reference_image, static_cast<int>(p.x), static_cast<int>(p.y)) ||
		    !lookup (canvas_color, canvas, static_cast<int>(p.x), static_cast<int>(p.y))) {
			stroke.addControlPoint (rescale_vertex (p));
			render_stroke (stroke);
			return;
		}
		int diff_canvas = color_diff (&ri_color[0], &canvas_color[0]);
		int diff_stroke = color_diff (&ri_color[0], &stroke_color_ub[0]);
		if (i >= min_stroke_length && (diff_canvas < diff_stroke)) {
			render_stroke (stroke);
			return;
		}
		stroke.addControlPoint (rescale_vertex (p));
	}
	render_stroke (stroke);
#endif
}

// Given texture id of source image, returns texture id of painted image.
GLuint
GLPainterlyObj::paint (GLuint source_image)
{
	// Start with blank canvas.
	rt_canvas->BeginCapture ();
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	rt_canvas->EndCapture ();
	
	// Paint a layer for each brush size.
	for (int i = 3; i >= 0; i--) {
		if (!enable_brush[i]) {
			continue;
		}
		float brush_size = brush_scale * brushes[i];
		
		// Stroke list.
		std::vector<Point> stroke_list;

		// Create reference image with Gaussian blur.
		GLubyte *reference_image = gaussian_blur (i, source_image);

		// Compute gradients of reference image by Sobel filtering.
		GLfloat *gradients = sobel ();
		
		// Find difference between canvas and reference image.
		difference ();

		// Find average error image.
		GLubyte *error_image = average_error (i);
		
		int step = static_cast<int>(brushes[i]);
		step = static_cast<int>(static_cast<float>(step) * fg);
		if (step < 1) {
			std::cerr << "WARNING: fg too low" << std::endl;
			step = 1;
		}
		for (int y = 0; y < tex.height (); y += step) {
			for (int x = 0; x < tex.width (); x += step) {
				if (error_image[tex.width ()*y+x] > tolerance) {
					// Find maximum difference in neighborhood.
					int index = tex.width ()*y+x;
					GLubyte max = error_image[index];
					GLubyte cur = max;
					int saved_ci = x;
					int saved_cj = y;
					for (int j = -step/2; j < step/2; j++) {
						for (int i = -step/2; i < step/2; i++) {
							int ci = x+i;
							int cj = y+j;
							if (ci > 0 && ci < tex.width () && cj > 0 && cj < tex.height ()) {
								int cur_index = tex.width ()*y+x + tex.width ()*j + i;
								cur = error_image[cur_index];
								if (cur > max) {
									index = cur_index;
									saved_ci = ci;
									saved_cj = cj;
								}
							}
						}
					}
					stroke_list.push_back (Point(saved_ci, saved_cj));
				}
			}
		}
		// Randomize stroke list.
		random_shuffle (stroke_list.begin (), stroke_list.end ());
		std::vector<Point>::iterator it;
		int i = 1;
		for (it = stroke_list.begin(); it!=stroke_list.end(); it++) {
			(*it).z = static_cast<float>(i)/stroke_list.size ();
		}
		paint_canvas (stroke_list, reference_image, gradients, brush_size);
		delete [] reference_image;
		delete [] gradients;
		delete [] error_image;
	}
	return rt_canvas->GetTextureID ();
}
