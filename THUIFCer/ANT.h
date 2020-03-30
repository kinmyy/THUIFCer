// ANT.h: interface for the ANT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANT_H__3A6910BF_B3BB_47E8_BF82_14D628ACEC5A__INCLUDED_)
#define AFX_ANT_H__3A6910BF_B3BB_47E8_BF82_14D628ACEC5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include"operation.h"
#define	AntNumber  20	//������Ŀ
#define	AntNumber2  20	//������Ŀ
#define	StepLimit  250	//ÿֻ���ϵ����ߵĲ�������
#define	TT  1598
#define	SS  41

class ANT //������������صı����ͷ���
{
public:
	double GetPathValue();
	unsigned int AnalyzePath();//��·�����з�������
	void bet(unsigned int,unsigned int,double );			//�����㷨�����һ���ڵ㣬����д����ɱ�			
	void CalculatePr();//����ת�Ƹ���
	void CalculateDr();//������������
	void init();//�����Ͻ��г�ʼ��
	unsigned int FindInPath(unsigned int, int);
	void setGridSize(int grid_size);
	ANT();
	virtual ~ANT();
private:

public:
	void MovePath();
	double DealPath(unsigned int *p1,unsigned int *p2);
	void TurnPath();

	struct AREA9{
		unsigned int state;
		int x,y;
		double dr,pr;
	public:
	} area9[3][3];
	unsigned int path[StepLimit];	//�������ϲ������·�� ���ɱ�
	unsigned int TPath[StepLimit];
	unsigned int Target;			//Ŀ���
	int WorkDone;					//������ɼ�����Ŀ��� ��Ϊ1 ����Ϊ��1
	double PathValue;				//·������ֵ ͨ��·������ó� ��ʼֵ����Ϊ1�����·��������ֵ-1
	double alpha;	//��Ϣ����ǿϵ��
	double beta;	//����������ǿϵ��
	int grid_size;
};

#endif // !defined(AFX_ANT_H__3A6910BF_B3BB_47E8_BF82_14D628ACEC5A__INCLUDED_)
