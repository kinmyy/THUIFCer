#ifndef CAM_H
#define CAM_H

#include "vec.h"

const   double  kCamDist = 2.5f;
const   CVec    kWorldUp(0,1,0);
	
class   CCam
{
	public:
		CCam();
		
		void    rotY(double angle, const CPos &target);
		void    pitch(double angle, const CPos &target);
						
		void    setEye(const CPos &eyePos) { eye = eyePos; }
		void    setTarget(const CPos &target);
		
		void    setFront(double x, double y, double z);
		void    setSide(double x, double y, double z);
		void    setTop(double x, double y, double z);

		CPos    getTarget() const { return eye + (z * kCamDist); } 
		
		CPos    getEye() const { return eye; }
		CVec    getCamZ() const { return z; }
		CVec    getCamY() const { return y; }
		CVec    getCamX() const { return x; }
		
	private:	
		CVec    x, y, z;
		CPos    eye;
};

extern CCam *gCam;

#endif