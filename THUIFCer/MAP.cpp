// MAP.cpp: implementation of the MAP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MAP.h"

#define	AntNumber  20	//蚂蚁数目
#define	AntNumber2  20	//蚂蚁数目
#define	StepLimit  250	//每只蚂蚁的行走的步数极限
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

void MAP::UpdateMap() //挥发信息素
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

void MAP::UpdateInfo(double info,unsigned int *path)//写入新代信息素
{
	unsigned int i,x,y;
	//限制只写入可行解的信息素到地图中
	for(i=0;i<StepLimit;i++)
	{
		x=*path%grid_size;
		y=*path/grid_size;  //得到坐标点的位置

		point[y][x].tau+=info;
		if(point[y][x].tau>=200)
			point[y][x].tau=200;

		path++;

		if(*(path-1)==Target)//目标点 
		break;

	}
}

void MAP::setGridSize(int grid_size)
{
	this->grid_size = grid_size;
}