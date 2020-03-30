#include "stdafx.h"
#include "vec.h"

CVec&   CVec::operator =(const CVec &vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	return  *this;
}

CVec    CVec::operator +(const CVec &vector) const
{
	return  CVec(x + vector.x, y + vector.y, z + vector.z);
}

CVec    CVec::operator -(const CVec &vector) const
{
	return  CVec(x - vector.x, y - vector.y, z - vector.z);
}

CVec    CVec::operator -() const
{
	return  CVec(-x, -y, -z);
}

void    CVec::operator +=(const CVec &vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
}

void    CVec::operator -=(const CVec &vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
}

bool    CVec::operator ==(const CVec &vector) const
{
	return  (x == vector.x  &&  y == vector.y  &&  z == vector.z);
}

bool    CVec::operator !=(const CVec &vector) const
{
	return  !(*this == vector);
}

double  CVec::operator *(const CVec &vector) const
{
	return  (x * vector.x + y * vector.y + z * vector.z);
}

void    CVec::set(double _x, double _y, double _z)
{
	x = (float) _x;
	y = (float) _y;
	z = (float) _z;
}

void    CVec::set(const CVec &vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
}

void    CVec::negate()
{
	x = -x;
	y = -y;
	z = -z;
}

void    CVec::normalize()
{
	double dist = length();
    if  (dist) {
	    dist = 1.0f / dist;
    }
	x *= (float) dist;
	y *= (float) dist;
	z *= (float) dist;
}

void    CVec::scale(double scale)
{
	x *= (float) scale;
	y *= (float) scale;
	z *= (float) scale;
}

double  CVec::length() const
{
	return  sqrt((x * x) + (y * y) + (z * z));
}

void   CVec::crossProduct(const CVec &vector, CVec &result) const
{
	result.x = (y * vector.z) - (vector.y * z);
	result.y = (z * vector.x) - (vector.z * x);
	result.z = (x * vector.y) - (vector.x * y);
}

CVec    CVec::crossProduct(const CVec &vector) const
{
	return  CVec((y * vector.z) - (vector.y * z), 
				 (z * vector.x) - (vector.z * x),
				 (x * vector.y) - (vector.x * y));
}