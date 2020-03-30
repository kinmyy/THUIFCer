// ANT.cpp: implementation of the ANT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ANT.h"
#include "math.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define	AntNumber  20	//蚂蚁数目
#define	AntNumber2  20	//蚂蚁数目
#define	StepLimit  250	//每只蚂蚁的行走的步数极限
#define	TT  1598
#define	SS  41
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ANT::ANT()
{
	grid_size = 200;
}

ANT::~ANT()
{

}
void ANT::init()	//对蚂蚁进行初始化
{
	unsigned int i,j;
	PathValue=0;   //PathValue 非负值的时候为表示这个路径是可行解
	WorkDone=-1;	//表示任务没有完成 即还没有到达目标点
	for(i=0;i<StepLimit;i++)
	{	path[i]=0;
		TPath[i]=0;
	}
	for(i=0;i<3;i++)
	for(j=0;j<3;j++)
	{
		area9[i][j].dr=0;
		area9[i][j].pr=0;
		area9[i][j].state=0;

	}
//	printf(" one ant init ok!~\n");
}
void ANT::CalculateDr()	//计算引力概率
{
	unsigned int i,j,x,y;//x ，y 为目标解码后的坐标 sum+=area9[i][j].dr;
	double sum=0,sum2=0,temp=0;
	x=Target%grid_size;
	y=Target/grid_size;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			area9[i][j].dr=0;	//清空dr
	for(i=0;i<3;i++)
		for(j=0;j<3;j++) //写入对应位置与目标点距离的平方
		{
			area9[i][j].dr=(x-area9[i][j].x)*(x-area9[i][j].x)
						+(y-area9[i][j].y)*(y-area9[i][j].y);
		}
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
		{
			if(area9[i][j].state==1||area9[i][j].state==2) //如果此点不通 则继续下一点
				continue;
			area9[i][j].dr-=area9[1][1].dr; 
			// area9[1][1].dr 此时放置的为当前点和目标点之间的距离平方
			if(area9[i][j].dr<0)
				sum+=(-area9[i][j].dr); 
			else
				sum+=(area9[i][j].dr); //求出改变距离参考点
		}
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)//求出各个点和距离参考点的距离改变量
		{
			if(area9[i][j].state==1||area9[i][j].state==2) //如果此点不通 则继续下一点
				continue;
			temp=sum-area9[i][j].dr;
			if(temp>0.001)
				area9[i][j].dr=temp;
			sum2+=area9[i][j].dr;	//总的 距离改变量
		}
	if(sum2<0.000001)
	 PathValue=-1; //没有可以继续选择的点 使此路径作废
}

void ANT::CalculatePr()	//计算转移概率
{
	unsigned int i,j;
	double sum=0,temp[3][3];
	if(PathValue>=0)		//这只蚂蚁没有被宣判死亡
	{
	for(i=0;i<3;i++)
	for(j=0;j<3;j++)
	{
		temp[i][j]=pow(area9[i][j].pr,alpha)*pow(area9[i][j].dr,beta);
		sum=sum+temp[i][j];
	}
	for(i=0;i<3;i++)
	for(j=0;j<3;j++)
		area9[i][j].pr=temp[i][j]/sum;	
	}
//8各邻域单元处理完毕 可以用赌盘来进行路径选择
}

void ANT::bet(unsigned int AntNum,unsigned int step, double BetValue )
{
	int i,j,num,flag=1;
	double sum;
	sum=0;
	num=0;
//	srand( (unsigned)time( NULL ) );//temp,
//	printf( "  %d\n",time( NULL ) );//
//	srand( (unsigned)time( NULL )*step*rand());//加上这句话效果最好 达到尽可能的随机
//	temp=rand()/(double)RAND_MAX;
	for( i=0;i<3;i++ )
	if(flag)
	for(j=0;j<3;j++)
	{	
		num++;
		sum+=area9[i][j].pr;
		if(sum>=BetValue)
		{flag=0;break;}
	}
	num--;
	path[step]=area9[num/3][num%3].y*grid_size+area9[num/3][num%3].x;//把赌盘得到的点写入路径
	if(path[step]==Target) //选到目标点 标记WorkDone
		WorkDone=1;
}

unsigned int ANT::AnalyzePath()		//计算路径长度, 计算结果放到 PathValue 中
{
	int i,flag=0;
	if(WorkDone==-1)
		return 0;	//如果为不可行解， 则不对路径进行处理
	for(i=0;i<StepLimit;i++)
	{
	if (Target==path[i])
	{	
		flag=1;
		break;
	}
	else if(1==abs((double)(path[i+1]-path[i]))||
			grid_size==abs((double)(path[i+1]-path[i]))||grid_size==abs((double)(path[i+1]-path[i])))
		PathValue=PathValue+1.0;
	else
		PathValue=PathValue+1.414;
	}
	if(!flag)
		PathValue=0;
	return 1;
}

double ANT::GetPathValue()
{
	return PathValue;
}


unsigned int ANT::FindInPath(unsigned int point, int step) //查询禁忌表 
{
	int i;
	for(i=(step-1);i>=0;i--)
		if(point==path[i])
		{
			return 1;
			break;
		}
	return 0;

}

void ANT::TurnPath()//把路径颠倒后放置在 TPath中
{
	int i,flag;
	for(i=0;i<StepLimit;i++)
	{
		if(path[i]==0)
		break;
	}
	flag=i-1;
	for(i=0;flag>=0;i++)
	{
		TPath[i]=path[flag];
		flag--;
	}
}

double ANT::DealPath(unsigned int *p1, unsigned int *p2)//p1是起点小 p2是终点大
{
	int i;
	double Value=0;
	for(i=0;p1!=p2;i++)
	{
	if(*p2==0) break;
	if(1==abs((double)(*p1-*(p1+1)))||grid_size==abs((double)(*p1-*(p1+1)))||grid_size==abs((double)(*p1-*(p1+1))))			
		Value+=1.0;
	else
		Value+=1.414;
	p1++;
	}
	return Value;
}

//DEL void ANT::AREA9::MovePath()
//DEL {
//DEL 	int i;
//DEL 	for(i=0;i<StepLimit;i++)
//DEL 	{
//DEL 		TPath[i]=path[i];
//DEL 		path[i]=0;
//DEL 	}
//DEL }

void ANT::MovePath()
{
 	int i;
 	for(i=0;i<StepLimit;i++)
 	{
 		TPath[i]=path[i];
 		path[i]=0;
 	}
}

void ANT::setGridSize(int grid_size)
{
	this->grid_size = grid_size;
}