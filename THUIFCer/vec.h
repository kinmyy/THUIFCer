#ifndef VEC_H
#define VEC_H

#include <math.h>

class CVec
{
	public:
		float  x;
		float  y;
		float  z;

		CVec() : x(0.0f), y(0.0f), z(0.0f) {}
		CVec(double _x, double _y, double _z) : x((float) _x), y((float) _y), z((float) _z) {}
		
		CVec(const CVec &vec) : x(vec.x), y(vec.y), z(vec.z) {}

		CVec& operator =(const CVec &vector);
		CVec operator +(const CVec &vector) const;
		CVec operator -(const CVec &vector) const;
		CVec operator -() const;
		void operator +=(const CVec &vector);
		void operator -=(const CVec &vector);
		bool operator ==(const CVec &vector) const;
		bool operator !=(const CVec &vector) const;

		double operator *(const CVec &vector) const;

		void set(double _x, double _y, double _z);
		void set(const CVec &vector);

		void negate();
		void normalize();
		void scale(double scale);
		
		double length() const;

		void crossProduct(const CVec &vector, CVec &result) const;
		CVec crossProduct(const CVec &vector) const;
		
		friend inline CVec operator *(const CVec &vector, double scalar);
};

CVec operator*(const CVec &vector, double scalar)
{
	return  CVec(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

typedef CVec CPos;

#endif