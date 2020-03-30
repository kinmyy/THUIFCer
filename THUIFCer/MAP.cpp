// MAP.cpp: implementation of the MAP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MAP.h"

#define	AntNumber  20	//������Ŀ
#define	AntNumber2  20	//������Ŀ
#define	StepLimit  250	//ÿֻ���ϵ����ߵĲ�������
#define	TT  1598
#define	SS  41

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MAP::MAP()
{
	int i,j;
	rho=0.7;// XXX
	for(j=0;j<grid_size;j++)
		for(i=0;i<grid_size;i++)
		{
			point[i][j].PointType=0;
			point[i][j].tau=20;
		}

	grid_size = 100;
}

MAP::~MAP()
{

}

void MAP::addobstacle(unsigned int point)
{

}

unsigned int MAP::PointState(unsigned int p)
{
	unsigned int i,j,state;
	i=p%grid_size;
	j=p/grid_size;
	state=point[j][i].PointType;
	return state;
}

void MAP::UpdateMap() //�ӷ���Ϣ��
{
	unsigned int i,j;
	for(i=0;i<grid_size;i++)
		for(j=0;j<grid_size;j++)
		{
			point[i][j].tau = point[i][j].tau * rho;
			if(point[i][j].tau<20)
				point[i][j].tau=20;
		}
}

void MAP::UpdateInfo(double info,unsigned int *path)//д���´���Ϣ��
{
	unsigned int i,x,y;
	//����ֻд����н����Ϣ�ص���ͼ��
	for(i=0;i<StepLimit;i++)
	{
		x=*path%grid_size;
		y=*path/grid_size;  //�õ�������λ��

		point[y][x].tau+=info;
		if(point[y][x].tau>=200)
			point[y][x].tau=200;

		path++;

		if(*(path-1)==Target)//Ŀ��� 
		break;

	}
}

void MAP::setGridSize(int grid_size)
{
	this->grid_size = grid_size;
}