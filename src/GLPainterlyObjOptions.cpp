#include "GLPainterlyObj.h"

void
GLPainterlyObj::options_fill ()
{
	one_to_one = !one_to_one;
	update ();
}
