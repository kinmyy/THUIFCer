// MAP.h: interface for the MAP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAP_H__C542232B_367A_44D6_84F6_3228627957D4__INCLUDED_)
#define AFX_MAP_H__C542232B_367A_44D6_84F6_3228627957D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	AntNumber  20	//蚂蚁数目
#define	AntNumber2  20	//蚂蚁数目
#define	StepLimit  250	//每只蚂蚁的行走的步数极限
#define	TT  1598
#define	SS  41
#include "ANT.h"
class MAP  //定义了地图相关的变量和方法
{
public:

	void UpdateMap();
	unsigned int PointState(unsigned int point);
	MAP();
	virtual ~MAP();
	void setGridSize(int grid_size);

private:
	void addobstacle(unsigned int);

public:
	void UpdateInfo(double ,unsigned int *);
	struct FirstTypeMap{
		unsigned int PointType;//点的类型，是否可以通过 0可以通过 非零不可以通过
		double tau;//信息素含量
	} point[350][350];
	unsigned int Target;
	double rho;		//信息素残留系数
	double TauMax;	
	double TauMin;	//信息素极值 利用最大最小蚁群算法
	int grid_size;

};

#endif // !defined(AFX_MAP_H__C542232B_367A_44D6_84F6_3228627957D4__INCLUDED_)
