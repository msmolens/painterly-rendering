// Stroke drawing code ---- (c) 2003 by Aaron Hertzmann

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Stroke.h"

#define NUM_SLICES 16

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLUquadricObj * Stroke::qobj = NULL;

Stroke::Stroke()
{
	_limit = _temp = NULL;
	_computed = false;

	_numLevels = 3;
	_cap = true;
	_extendLength = false;

	_radius = -1;
	_color = makeColor(0,0,0);
	_adjustedColor = makeColor(0,0,0);

	if (qobj == NULL)
		qobj = gluNewQuadric();
    
	_useTexture = false;
	_lumAlphaNum = 0;
	_alphaNum = 0;

}

Stroke::Stroke(float radius,GLuint color,StrokeNumT n)
{
	_limit = _temp = NULL;
	_computed = false;

	_numLevels = 3;
	_cap = true;
	_extendLength = false;

	_radius = radius;
	_color = color;
	_adjustedColor = _color;


	_num = n;

	if (qobj == NULL)
	{
		qobj = gluNewQuadric();
	}

	_useTexture = false;
	_lumAlphaNum = 0;
	_alphaNum = 0;

}

Stroke::Stroke(int cx,int cy,float radius,GLubyte r,GLubyte g,GLubyte b,
	       StrokeNumT n)
{
	_control.push_back(Point(cx,cy));

	_limit = _temp = NULL;
	_computed = false;

	_numLevels = 3;
	_cap = true;
	_extendLength = false;
  
	_radius = radius;
  
	_color = makeColor(r,g,b);
	_adjustedColor = makeColor(r,g,b);

	_num = n;

	if (qobj == NULL)
	{
		qobj = gluNewQuadric();
	}
	_useTexture = false;
	_lumAlphaNum = 0;
	_alphaNum = 0;

}

Stroke::Stroke(int cx,int cy,float radius,GLuint color,StrokeNumT n)
{
	_control.push_back(Point(cx,cy));

	_limit = _temp = NULL;
	_computed = false;

	_numLevels = 3;
	_cap = true;
	_extendLength = false;
  
	_radius = radius;
  
	_color = color;
	_adjustedColor = color;

	_num = n;
  
	if (qobj == NULL)
	{
		qobj = gluNewQuadric();
	}
	_useTexture = false;
	_lumAlphaNum = 0;
	_alphaNum = 0;

}

Stroke::Stroke(const std::vector<Point> & control, float radius,GLuint color,
	       StrokeNumT n) : _control(control)
{
	_limit = _temp = NULL;
	_computed = false;

	_numLevels = 3;
	_cap = true;
	_extendLength = false;
  
	_radius = radius;
  
	_color = color;
	_adjustedColor = color;

	_num = n;

	if (qobj == NULL)
	{
		qobj = gluNewQuadric();
	}
	_useTexture = false;
	_lumAlphaNum = 0;
	_alphaNum = 0;

}


Stroke::Stroke( const Stroke& s)
{
	_control = s._control;
	_limit = _temp = NULL;
	_computed = false;

	_numLevels = s._numLevels;
	_cap = s._cap;
	_extendLength = s._extendLength;

	_radius = s._radius;
	_color = s._color;
	_adjustedColor = s._adjustedColor;
  
	_num = s._num;
	_useTexture = s._useTexture;
	_ufreq = s._ufreq;
	// _vfreq = s._vfreq;
	_ustart = s._ustart;
//  _vstart = s._vstart;
	_depth = s._depth;
	_lumAlphaNum = s._lumAlphaNum;
	_alphaNum = s._alphaNum;

}


Stroke& Stroke::operator=( const Stroke& s)
{
	_control = s._control;
	_limit = _temp = NULL;
	_computed = false;

	_numLevels = s._numLevels;
	_cap = s._cap;
	_extendLength = s._extendLength;

	_radius = s._radius;
	_color = s._color;
	_adjustedColor = s._adjustedColor;
  
	_num = s._num;

	_useTexture = s._useTexture;
	_ufreq = s._ufreq;
	// _vfreq = s._vfreq;
	_ustart = s._ustart;
	// _vstart = s._vstart;
	_depth = s._depth;
	_lumAlphaNum = s._lumAlphaNum;
	_alphaNum = s._alphaNum;


	return *this;
}


void Stroke::clear()
{
	_computed = false;
	_control.erase(_control.begin(),_control.end());
	if (_limit != NULL)
		_limit->erase(_limit->begin(),_limit->end());
}

Stroke::~Stroke()
{
	if (_limit != NULL)
		delete _limit;

	if (_temp != NULL)
		delete _temp;
}

void Stroke::addControlPoint(float x,float y)
{
	if (_control.empty() || _control.back().x != x || _control.back().y != y)
	{
		_control.push_back(Point(x,y));
		_computed = false;
	}
}

void Stroke::addControlPoint(const Point &p)
{
	if (_control.empty() || _control.back().x != p.x || _control.back().y != p.y)
	{
		_control.push_back(p);
		_computed = false;
	}
}

void Stroke::drawLines(std::vector<Point> *curve)
{
	glBegin(GL_LINE_STRIP);
	for(int i=0;i<curve->size();i++)
	{
		Point & p = (*curve)[i];
		glVertex2f(p.x,p.y);
	}
	glEnd();
}

void Stroke::drawDisc(float x,float y,float z, float radius)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	gluDisk(qobj,0,radius,NUM_SLICES,1);
	glPopMatrix();
}

void Stroke::drawCap(const Point & p0, float dx, float dy, float textureU, bool orientation)
{
	float theta = atan2(-dy,-dx);
	
//	glMatrixMode (GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity ();
//	glTranslatef(p0.x,p0.y,_depth);

	glBegin(GL_TRIANGLE_FAN);
	//        glVertex3f(p0->x, p0->y,z);
//	if (_useTexture)
//		glTexCoord2f(textureU, .5);

//	glVertex3i(0,0,0);
	glVertex3f (p0.x, p0.y, _depth);

//	if (_useTexture)
//		glTexCoord2f(textureU , 0);
//	glVertex3f(-_radius*dx,-_radius*dy,0);
	glVertex3f(p0.x-_radius*dx,p0.y-_radius*dy,_depth);

	int sign = orientation ? -1 : 1;

	for(int i=1;i<=NUM_SLICES-1;i++)
	{
		float dx1 = cos(theta + i * M_PI / NUM_SLICES);
		float dy1 = sin(theta + i * M_PI / NUM_SLICES);
      
//		if (_useTexture)
//		{
//			if (!orientation)
//				glTexCoord2f(textureU + _ufreq * _radius * (-sin(i*M_PI/NUM_SLICES)+1)/2, (-cos(i*M_PI/NUM_SLICES)+1)/2 );
//			else
//				glTexCoord2f(textureU + _ufreq * _radius * (-sin(i*M_PI/NUM_SLICES+M_PI)+1)/2, (-cos(i*M_PI/NUM_SLICES+M_PI)+1)/2 );
//		}
//		glVertex3i((int)(_radius * dx1), (int)(_radius * dy1), 0);
//		glVertex3f(_radius * dx1, _radius * dy1, 0);
		glVertex3f(p0.x+_radius * dx1, p0.y+_radius * dy1, _depth);
		//glVertex3f(p0->x + radius * dx1, p0->y + radius * dy1, z);
	}

	//  glVertex3f(-radius*dx+radius*dy,-radius*dy-radius*dx,0);
	  //  glVertex3f(radius*dx+radius*dy,radius*dy-radius*dx,0);

//	if (_useTexture)
//		glTexCoord2f(textureU , 1 );
//	glVertex3f(_radius*dx,_radius*dy,0);
	glVertex3f(p0.x+_radius*dx,p0.y+_radius*dy,_depth);
  
	glEnd();
  
	glPopMatrix();
}


void Stroke::scanConvert(const std::vector<Point> *curve,
			 bool points)
{
	assert(curve != NULL);

	glColor4ub(red(),green(),blue(),alpha());
	if (points) {
		// max:
		// Point size, POINT_SMOOTH should already be set.
		// Draw using points.
		glBegin (GL_POINTS);
		std::vector<Point>::const_iterator it;
		for (it = curve->begin(); it != curve->end(); it++) {
			glVertex3f ((*it).x, (*it).y, _depth);
		}
		glEnd ();
		return;
	}

	// Hertzmann's original TRIANGLE_STRIP code
	float dx,dy,mag;
	Point p0;
	Point p1;
	Point p2;
	if (curve->empty())
		return;
	float textureU = _ustart;
	p0 = (*curve)[0];
	if (curve->size() == 1)	{
		if (_cap) {
			drawDisc(p0.x,p0.y,depth(),radius());
		}
		return;
	}
	p1 = (*curve)[1];
	dx = p1.y - p0.y;
	dy = p0.x - p1.x;
	mag = sqrtf(dx*dx + dy*dy);
	if (mag != 0) {
		dx /= mag;
		dy /= mag;
	}
	if (_cap) {
		textureU += _radius;
		drawCap(p0, dx, dy, textureU, false);
	}

	glBegin(GL_TRIANGLE_STRIP);
  	const float z = depth();
	glVertex3f(p0.x + _radius * dx, p0.y + _radius * dy,z);
	glVertex3f(p0.x - _radius * dx, p0.y - _radius * dy,z);
	for (int i=1; i<curve->size()-1 ;i++) {
		p0 = (*curve)[i-1];
		p2 = (*curve)[i+1];

		dx = p2.y - p0.y;
		dy = p0.x - p2.x;
		mag = sqrtf(dx*dx + dy*dy);
		if (mag != 0) {
			dx /= mag;
			dy /= mag;
		}
		glVertex3f(p0.x + _radius * dx, p0.y + _radius * dy,z);
		glVertex3f(p0.x - _radius * dx, p0.y - _radius * dy,z);
	}
	p0 = (*curve)[curve->size()-2];
	p1 = (*curve)[curve->size()-1];
	dx = p1.y - p0.y;
	dy = p0.x - p1.x;
	mag = sqrtf(dx*dx + dy*dy);
	if (mag != 0) {
		dx /= mag;
		dy /= mag;
	}
	glVertex3f(p1.x + _radius * dx, p1.y + _radius * dy,z);
	glVertex3f(p1.x - _radius * dx, p1.y - _radius * dy,z);
	glEnd();
	
	if (_cap)
		drawCap(p1, -dx, -dy, textureU, true);
}

void Stroke::drawControlPolygon()
{
	glColor3ub(255,0,0);
	drawLines(&_control);
}

void Stroke::drawLineCurve()
{
	if (!_computed)
		computeLimitCurve();

	drawLines(_limit);
}

void Stroke::render(bool points)
{
	if (!_computed)
		computeLimitCurve();

	scanConvert(_limit, points);
}

float Stroke::arcLength()
{
	if (!_computed)
		computeLimitCurve();

	float length = 0;
	for(int i=0;i<_limit->size()-1;i++)
	{
		Point & p0 = (*_limit)[i];
		Point & p1 = (*_limit)[i+1];
		float dist = sqrt((p1.x-p0.x)*(p1.x-p0.x)+(p1.y-p0.y)*(p1.y-p0.y));
		length += dist;
	}

	if (_cap)
		length += 2*_radius;

	return length;
}

void Stroke::subdivideCubicBSpline(std::vector<Point> * inputCurve, 
				   std::vector<Point> * outputCurve)
{
	outputCurve->erase(outputCurve->begin(),outputCurve->end());

	if (inputCurve->size() < 1)
		return;

	Point pi0;
	Point pi1;
	Point pi2;

	pi0 = (*inputCurve)[0];

	outputCurve->push_back(Point(pi0.x,pi0.y));

	if (inputCurve->size() == 1)
		return;

	if (inputCurve->size() == 2)
	{
		pi1 = (*inputCurve)[1];

		outputCurve->push_back(Point(pi1.x,pi1.y));

		return;
	}

	pi1 = (*inputCurve)[1];

	outputCurve->push_back(Point((pi0.x + pi1.x)/2,(pi0.y + pi1.y)/2));

	for(int i=1;i<inputCurve->size()-1;i++)
	{
		pi0 = (*inputCurve)[i-1];
		pi1 = (*inputCurve)[i];
		pi2 = (*inputCurve)[i+1];

		outputCurve->push_back(Point( (pi0.x + 6*pi1.x + pi2.x)/8,
					      (pi0.y + 6*pi1.y + pi2.y)/8));

		outputCurve->push_back(Point( (pi1.x + pi2.x)/2,(pi1.y + pi2.y)/2));
	}
	
	outputCurve->push_back(Point(pi2.x,pi2.y));
}

void Stroke::subdivideFourPoint(std::vector<Point> * inputCurve, 
				std::vector<Point> * outputCurve)
{
	outputCurve->erase(outputCurve->begin(),outputCurve->end());

	if (inputCurve->size() < 1)
		return;

	Point pi0;
	Point pi1;
	Point pi2;
	Point pi3;

	if (inputCurve->size() == 1)
	{
		pi0 = (*inputCurve)[0];
		outputCurve->push_back(Point(pi0.x,pi0.y));

		return;
	}

	if (inputCurve->size() == 2)
	{
		pi0 = (*inputCurve)[0];
		pi1 = (*inputCurve)[1];
	
		outputCurve->push_back(Point(pi0.x,pi0.y));
		outputCurve->push_back(Point((pi0.x+pi1.x)/2,(pi0.y+pi1.y)/2));
		outputCurve->push_back(Point(pi1.x,pi1.y));
	
		return;
	}

	pi0 = (*inputCurve)[0];
	pi1 = (*inputCurve)[1];

	Point piminus1(2*pi0.x - pi1.x,2*pi0.y - pi1.y);

	pi0 = (*inputCurve)[inputCurve->size()-1];
	pi1 = (*inputCurve)[inputCurve->size()-2];

	Point piplus1(2*pi0.x - pi1.x,2*pi0.y - pi1.y);
    
	for(int i=0;i<inputCurve->size()-1;i++)
	{
		pi0 = (i==0 ? piminus1 : (*inputCurve)[i-1]);
		pi1 = (*inputCurve)[i];
		pi2 = (*inputCurve)[i+1];
		pi3 = (i==inputCurve->size()-2? piplus1:(*inputCurve)[i+2]);

		outputCurve->push_back(Point( pi1.x, pi1.y));

		outputCurve->push_back(Point( (-pi0.x + 9*pi1.x + 9*pi2.x - pi3.x)/16,
					      (-pi0.y + 9*pi1.y + 9*pi2.y - pi3.y)/16));
	}

	pi0 = inputCurve->back();

	outputCurve->push_back(Point(pi0.x,pi0.y));
}

void Stroke::subdivide(std::vector<Point> * inputCurve, std::vector<Point> * outputCurve)
{
	subdivideCubicBSpline(inputCurve,outputCurve);
}

void Stroke::computeLimitCurve()
{
	if (_limit == NULL)
		_limit = new std::vector<Point>(50);

	if (_temp == NULL)
		_temp = new std::vector<Point>(50);

	subdivide(&_control,_limit);

	if (_extendLength && _control.size() > 1)
	{
		Point & p0 = _control[0];
		Point & p1 = _control[1];

		Point v = p0 - p1;
		v.normalize();
		v = v*_radius;
		_limit->insert(_limit->begin(),p0 + v);


		Point &pn_1 = _control[_control.size()-2];
		Point &pn =   _control[_control.size()-1];

		v = pn - pn_1;
		v.normalize();
		v = v*_radius;
		_limit->push_back(pn + v);
	}

	for(int i=0;i<_numLevels/2;i++)
	{
		subdivide(_limit,_temp);
		subdivide(_temp,_limit);
	}

	_computed = true;
}
  
  

void Stroke::print(FILE * fp)
{
	fprintf(fp,"Stroke %d: Color = %X. Radius %f. ",
		(long int)_num,color(),radius());


	fprintf(fp,"Points = ");
	for(std::vector<Point>::iterator it = _control.begin(); it!=_control.end(); ++it)
		fprintf(fp,"[%f,%f] ",(*it).x,(*it).y);

	fprintf(fp,"\n\n");
}



void Stroke::removeDuplicateControlPoints()
{
	int startsize = _control.size();

	std::vector<Point>::iterator it = _control.begin();
  
	while (it != _control.end())
	{
		std::vector<Point>::iterator next = it;   ++ next;
      
		if (next == _control.end())
			return;
      
		if ((*it) == (*next))
			_control.erase(it);
      
		it = next;
	}
}
