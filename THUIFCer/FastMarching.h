#pragma once
#include "matlib.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
using namespace std;


#define GRIDNUM 100
#define eps 2.3e-016
#define doublemax 1e300
#define INF 1e305
#ifndef min
#define min(a,b)        ((a) < (b) ? (a): (b))
#endif
#ifndef max
#define max(a,b)        ((a) > (b) ? (a): (b))
#endif

class FastMarching
{
public:
	FastMarching(void);
public:
	~FastMarching(void);

public:
	int minarray(double *A, int l);
	double pow2(double val);
	void roots(double* Coeff, double* ans);
	int maxarray(double *A, int l);
	int mindex2(int x, int y, int sizx);
	bool IsFinite(double x);
	bool IsInf(double x);
	bool isntfrozen2d(int i, int j, int *dims, bool *Frozen);
	bool isfrozen2d(int i, int j, int *dims, bool *Frozen);
	int p2x(int x);
	void show_list(double **listval, int *listprop);
	void initialize_list(double ** listval, int *listprop);
	void destroy_list(double ** listval, int *listprop);
	void list_add(double ** listval, int *listprop, double val);
	int list_minimum(double ** listval, int *listprop);
	void list_remove(double ** listval, int *listprop, int index);
	void list_remove_replace(double ** listval, int *listprop, int index);
	void listupdate(double **listval, int *listprop, int index, double val);
	mxArray* msfm2d( mxArray *SpeedImage, mxArray *SourcePoint,bool a, bool b );

	int mindex3(int x, int y, int z, int sizx, int sizy);
	int checkBounds2d( double *point, int *Isize);
	int checkBounds3d( double *point, int *Isize);
	double norm2(double *a);
	double norm3(double *a);
	void interpgrad2d(double *Ireturn, double *I, int *Isize, double *point);
	void interpgrad3d(double *Ireturn, double *I, int *Isize, double *point);
	bool RK4STEP_2D(double *gradientArray, int *gradientArraySize, double *startPoint, double *nextPoint, double stepSize);
	bool RK4STEP_3D(double *gradientArray, int *gradientArraySize, double *startPoint, double *nextPoint, double stepSize);
	double* rk4(mxArray* StartPoint, Mm GradientVolume, double Stepsize);
	vector<D3DXVECTOR3> fmm(double start_x,double start_y,double end_x,double end_y,CString txt);
	int getTimeCounter();
	int getGridSize();
	void setGridSize(int grid_size);

public:
	int timeCounter;
	int grid_size;

};
