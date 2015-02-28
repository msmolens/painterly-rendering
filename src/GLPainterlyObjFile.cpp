#include "GLPainterlyObj.h"
#ifdef Unsorted
#undef Unsorted
#endif
#include <qfiledialog.h>
#include <qmessagebox.h>

void
GLPainterlyObj::load_image (const char *filename)
{
	// Load image.
	QImage buf;
	if (!buf.load (filename)) {
		qWarning ("ERROR: Could not read image file '%s'.", filename);
		exit (EXIT_FAILURE);
	}
	tex = QGLWidget::convertToGLFormat (buf);
	init_texture (tex);
	resize (tex.width (), tex.height ());
	parentWidget()->parentWidget()->resize (tex.width(), tex.height());
	parentWidget()->parentWidget()->update ();
	parentWidget()->update ();
	update ();
}

void
GLPainterlyObj::file_open ()
{
        QString filename = QFileDialog::getOpenFileName(QString::null, "Images (*.png *.jpg *.ppm *.bmp *.gif *.tiff)", this, "file open", "Open Image" );
        if (!filename.isEmpty()) {
		load_image (filename);
		init_rendertextures ();
		if (initialized) {
			repaint_canvas = true;
			update ();
		}
	}
}

void
GLPainterlyObj::file_save ()
{
	GLubyte *buf = new GLubyte[rt_canvas->GetWidth() * rt_canvas->GetHeight() * 4];
//	rt_canvas->BeginCapture ();
	glGetTexImage (rt_canvas->GetTextureTarget (), 0, GL_BGRA, GL_UNSIGNED_BYTE, buf);
//	rt_canvas->EndCapture ();
	QImage image (buf, rt_canvas->GetWidth(), rt_canvas->GetHeight(), 32, 0, 0, QImage::LittleEndian);
        QString filename = QFileDialog::getSaveFileName(QString::null, "Images (*.png)", this, "file save", "Save Image" );
        if (!filename.isEmpty()) {
		int answer = 0;
		if (QFile::exists (filename))
			answer = QMessageBox::warning (this, "Overwrite File", QString ("Overwrite\n\'%1\'?").arg (filename), "&Yes", "&No", QString::null, 1, 1);
		if (answer == 0) {
			save_image (filename, image.mirror ());
			return;
		}
	}
	delete [] buf;
}

void
GLPainterlyObj::save_image (const QString &filename,
			    const QImage &image)
{
	image.save (filename, "PNG");
	std::cerr << "Saved '" << filename << "'" << std::endl;
}

void
GLPainterlyObj::file_close ()
{
	glDeleteTextures (1, &source_image);
	source_image = 0;
	repaint_canvas = true;
	update ();
}
