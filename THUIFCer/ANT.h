// ANT.h: interface for the ANT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANT_H__3A6910BF_B3BB_47E8_BF82_14D628ACEC5A__INCLUDED_)
#define AFX_ANT_H__3A6910BF_B3BB_47E8_BF82_14D628ACEC5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include"operation.h"
#define	AntNumber  20	//蚂蚁数目
#define	AntNumber2  20	//蚂蚁数目
#define	StepLimit  250	//每只蚂蚁的行走的步数极限
#define	TT  1598
#define	SS  41

class ANT //定义了蚂蚁相关的变量和方法
{
public:
	double GetPathValue();
	unsigned int AnalyzePath();//对路径进行分析处理
	void bet(unsigned int,unsigned int,double );			//赌盘算法求出下一个节点，并且写入禁忌表			
	void CalculatePr();//计算转移概率
	void CalculateDr();//计算引力概率
	void init();//对蚂蚁进行初始化
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
	unsigned int path[StepLimit];	//放置蚂蚁测的行走路径 禁忌表
	unsigned int TPath[StepLimit];
	unsigned int Target;			//目标点
	int WorkDone;					//任务完成即到达目标点 置为1 否则为－1
	double PathValue;				//路径优劣值 通过路径处理得出 初始值设置为1，如果路径不可用值-1
	double alpha;	//信息素增强系数
	double beta;	//引力概率增强系数
	int grid_size;
};

#endif // !defined(AFX_ANT_H__3A6910BF_B3BB_47E8_BF82_14D628ACEC5A__INCLUDED_)
