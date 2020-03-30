#include "stdafx.h"
#include "cam.h"

CCam::CCam()
{
	x.set(1., 0., 0.);
	y.set(0., 1., 0.);
	z.set(0., 0., 1.);

	eye.set(0., 0., -kCamDist);
}

void    CCam::rotY(double angle, const CPos &target)
{
	double  _x, _z,
	        sinAngle, cosAngle;
	
	sinAngle = sin(angle);
	cosAngle = cos(angle);
	
	eye -= target;
	
	_x = eye.x;
	_z = eye.z;
	
	eye.x = (float) (_x * cosAngle + _z * sinAngle);
	eye.z = (float) (_x * -sinAngle + _z * cosAngle);
	
	eye += target;
	
	setTarget(target);
}

void    CCam::pitch(double angle, const CPos &target)
{
	double  sinAng = sin(angle),
	        cosAng = cos(angle);

	eye -= target;

	double  factor = sqrt(eye.x*eye.x + eye.y*eye.y + eye.z*eye.z);

	double  _x = (x.x * x.x * (1.0f - cosAng) + cosAng) * eye.x +
			 	 (x.x * x.y * (1.0f - cosAng) + x.z * sinAng) * eye.y +
				 (x.x * x.z * (1.0f - cosAng) - x.y * sinAng) * eye.z;

	double  _y = (x.x * x.y * (1.0f - cosAng) - x.z * sinAng) * eye.x +
				 (x.y * x.y * (1.0f - cosAng) + cosAng) * eye.y +
				 (x.y * x.z * (1.0f - cosAng) + x.x * sinAng) * eye.z;

	double  _z = (x.x * x.z * (1.0f - cosAng) + x.y * sinAng) * eye.x +
				 (x.y * x.z * (1.0f - cosAng) - x.x * sinAng) * eye.y +
				 (x.z * x.z * (1.0f - cosAng) + cosAng) * eye.z;

	eye.set(_x, _y, _z);
	eye += target;
	
	CVec vector = eye - target;
	vector.normalize();
	vector = vector * (factor/kCamDist);

	eye = target + (vector * kCamDist);
	
	setTarget(target);
}

void    CCam::setFront(double x, double y, double z)
{
	eye.x = (float) (kCamDist * x);
	eye.y = 0;
	eye.z = 0;
}

void    CCam::setSide(double x, double y, double z)
{
	eye.x = 0;
	eye.y = 0;
	eye.z = (float) (kCamDist * x);
}

//something wrong
void    CCam::setTop(double x, double y, double z)
{
	eye.x = 0;
	eye.y = (float) (kCamDist * x);
	eye.z = 0;
}

void    CCam::setTarget(const CPos &target)
{
	z = target - eye;
	z.normalize();
	
	x = kWorldUp.crossProduct(z);
	x.normalize();
	
	y = z.crossProduct(x);
}

CCam    theCam;
CCam    * gCam = &theCam;