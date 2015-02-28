// Stroke drawing code ---- (c) 2003 by Aaron Hertzmann

#ifndef __POINT_HH__
#define __POINT_HH__

#include <math.h>
#include <assert.h>

struct Point
{
	float x,y,z;

	Point() { };
	Point(float x,float y) { this->x = x; this->y = y; }
	Point(const Point & v) { x = v.x;y=v.y; }
	Point & operator=(const Point & v) { x = v.x;y=v.y; return *this; }

	Point operator+(const Point & v) const { return Point(x+v.x,y+v.y); }
	Point operator-(const Point & v) const { return Point(x-v.x,y-v.y); }
	Point operator*(float v) const { return Point(x*v,y*v); }
	Point operator/(float v) const { return Point(x/v,y/v); }
	Point operator+(float v) const { return Point(x+v,y+v); }
	Point operator-(float v) const { return Point(x-v,y-v); }
     
	bool operator==(const Point & v) const { return x == v.x && y == v.y; }
	Point operator-() const { return Point(-x,-y); }

	Point & operator+=(const Point & v) { x += v.x;y+=v.y; return *this; }
	float L2sqr() const { return x*x+y*y; }
	float L2() const { return sqrtf(x*x+y*y); }
	Point round() const { return Point(floor(x+.5),floor(y+.5)); }
	float dot(const Point & p) const { return x*p.x+y*p.y; }

	void normalize() { float mag=L2(); if (mag != 0) {x/=mag;y/=mag;} }
};



#endif
