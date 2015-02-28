// Stroke drawing code ---- (c) 2003 by Aaron Hertzmann

#ifndef __STROKE_HH__
#define __STROKE_HH__

#include <cmath>
#include <vector>
#include <cstdio>
#include "Point.h"
#include "Color.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

typedef long int StrokeNumT;
#define NOSTROKE (StrokeNumT(-1))
inline StrokeNumT makeStrokeNum(long int num,int) { return num; }

class Stroke
{
private:
	// control polygon
	std::vector<Point> _control;

	// limit curve
	std::vector<Point> * _limit;  
	// temporary data for subdivision
	std::vector<Point> * _temp;

	// z value
	float _depth;

	// has the limit curve been computed from the current CP?
	bool _computed;  

	// number of subdivision levels to use
	int _numLevels; 

	// brush radius
	// if this is changed, then the bounding box will need to be updated
	float _radius;

	// draw round caps at the ends of the stroke?
	bool _cap;

	bool _extendLength;

	// stroke color
	GLuint _color;
	GLuint _adjustedColor;

	// stroke number
	StrokeNumT _num;

	// texture mapping parameters
	bool _useTexture;
	float _ustart;
	float _ufreq;
//  float _vfreq;
//  float _vstart;

	GLuint _lumAlphaNum;
	GLuint _alphaNum;


	// private member functions

	//  void discPoint(float x,float y,void (*drawPoint)(int x,int y));
	//  void drawCap(const Point & p0, float dx, float dy,
	//	       void (*drawPoint)(int x,int y));
	void subdivideCubicBSpline(std::vector<Point> * inputCurve, 
				   std::vector<Point> * outputCurve);
	void subdivideFourPoint(std::vector<Point> * inputCurve, 
				std::vector<Point> * outputCurve);
	void subdivide(std::vector<Point> * inputCurve, 
		       std::vector<Point> * outputCurve);
	void computeLimitCurve();
	void scanConvert(const std::vector<Point> * curve, bool points);

	static GLUquadricObj * qobj;

public:
	Stroke();
	Stroke(float radius,GLuint color,StrokeNumT n);
	Stroke(int cx,int cy,float radius,GLubyte r,GLubyte g,GLubyte b,
	       StrokeNumT n);
	Stroke(int cx,int cy,float radius,GLuint color,StrokeNumT n);
	Stroke(const std::vector<Point> & control,float radius,GLuint color,StrokeNumT n);

	Stroke( const Stroke& s);
	Stroke& operator=( const Stroke& s);

	~Stroke();

	float radius() const { return _radius; }
	float & radius() { return _radius; }
	float depth() const { return _depth; }
	float & depth() { return _depth; }
	bool cap() const { return _cap; }
	bool & cap() { return _cap; }
	bool extendLength() const { return _extendLength; }
	bool & extendLength() { return _extendLength; }

	std::vector<Point> & control() { return _control; }
	const std::vector<Point> & control() const { return _control; }  
	StrokeNumT num() const { return _num; }

	GLubyte red() const { return getRed(_color); }
	GLubyte green() const { return getGreen(_color); }
	GLubyte blue() const { return getBlue(_color); }
	GLubyte alpha() const { return getAlpha(_color); }
	GLuint color() const { return _color; }
	GLuint & color() { return _color; }
	GLuint adjustedColor() const { return _adjustedColor; }
	GLuint & adjustedColor() { return _adjustedColor; }

	void clear();
	static void drawLines(std::vector<Point> * curve);
	void drawControlPolygon();
	void drawLineCurve();
	void render(bool points);
	void print(FILE * fp = stdout);

	void addControlPoint(float x, float y);
	void addControlPoint(const Point &p);

	void removeDuplicateControlPoints();

	static void drawDisc(float x,float y, float z, float radius);
	void drawCap(const Point & p0, float dx, float dy, 
		     float textureU = 0, bool orientation=false);

	// texture-mapping parameters
	bool useTexture() const { return _useTexture; }
	bool & useTexture() { return _useTexture; }
	float ufreq() const { return _ufreq; }
	float & ufreq() { return _ufreq; }
	float ustart() const { return _ustart; }
	float & ustart()  { return _ustart; }

	GLuint & lumAlphaNum() { return _lumAlphaNum; }
	GLuint lumAlphaNum() const { return _lumAlphaNum; }
	GLuint & alphaNum() { return _alphaNum; }
	GLuint alphaNum() const { return _alphaNum; }

	float arcLength();
};

#endif
