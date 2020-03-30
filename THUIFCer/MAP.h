// MAP.h: interface for the MAP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAP_H__C542232B_367A_44D6_84F6_3228627957D4__INCLUDED_)
#define AFX_MAP_H__C542232B_367A_44D6_84F6_3228627957D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	AntNumber  20	//������Ŀ
#define	AntNumber2  20	//������Ŀ
#define	StepLimit  250	//ÿֻ���ϵ����ߵĲ�������
#define	TT  1598
#define	SS  41
#include "ANT.h"
class MAP  //�����˵�ͼ��صı����ͷ���
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
		unsigned int PointType;//������ͣ��Ƿ����ͨ�� 0����ͨ�� ���㲻����ͨ��
		double tau;//��Ϣ�غ���
	} point[350][350];
	unsigned int Target;
	double rho;		//��Ϣ�ز���ϵ��
	double TauMax;	
	double TauMin;	//��Ϣ�ؼ�ֵ ���������С��Ⱥ�㷨
	int grid_size;

};

#endif // !defined(AFX_MAP_H__C542232B_367A_44D6_84F6_3228627957D4__INCLUDED_)
