#include "StdAfx.h"
#include "FMToolBox.h"

FMToolBox::FMToolBox(void)
{
}

FMToolBox::~FMToolBox(void)
{
}

void FMToolBox::PathPlanning()
{
	mxArray *SpeedImage = NULL;
	//mxArray *distanceMap = NULL;
	SpeedImage = mxCreateDoubleMatrix(GRIDNUM,GRIDNUM,mxREAL);
	//distanceMap = mxCreateDoubleMatrix(GRIDNUM,GRIDNUM,mxREAL);

	ifstream rr("D:\grids100.txt");
	double *M= (double*)mxGetPr(SpeedImage);
	for (int i=0;i<GRIDNUM;i++)
	{
		for (int j=0;j<GRIDNUM;j++)
		{
			rr>>M[i*GRIDNUM+j];
		}
	}

	rr.close();

	Mm aa = zeros(GRIDNUM,GRIDNUM);
	memcpy(aa.addr(),M,GRIDNUM*GRIDNUM*sizeof(double));
	rescale(aa);
	Mm bb = zeros(GRIDNUM,GRIDNUM);
	aa = 1-rescale(aa);
	int n = GRIDNUM;
	int ntheta = 21;


}