// ANT.cpp: implementation of the ANT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ANT.h"
#include "math.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define	AntNumber  20	//������Ŀ
#define	AntNumber2  20	//������Ŀ
#define	StepLimit  250	//ÿֻ���ϵ����ߵĲ�������
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
void ANT::init()	//�����Ͻ��г�ʼ��
{
	unsigned int i,j;
	PathValue=0;   //PathValue �Ǹ�ֵ��ʱ��Ϊ��ʾ���·���ǿ��н�
	WorkDone=-1;	//��ʾ����û����� ����û�е���Ŀ���
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
void ANT::CalculateDr()	//������������
{
	unsigned int i,j,x,y;//x ��y ΪĿ����������� sum+=area9[i][j].dr;
	double sum=0,sum2=0,temp=0;
	x=Target%grid_size;
	y=Target/grid_size;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			area9[i][j].dr=0;	//���dr
	for(i=0;i<3;i++)
		for(j=0;j<3;j++) //д���Ӧλ����Ŀ�������ƽ��
		{
			area9[i][j].dr=(x-area9[i][j].x)*(x-area9[i][j].x)
						+(y-area9[i][j].y)*(y-area9[i][j].y);
		}
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
		{
			if(area9[i][j].state==1||area9[i][j].state==2) //����˵㲻ͨ �������һ��
				continue;
			area9[i][j].dr-=area9[1][1].dr; 
			// area9[1][1].dr ��ʱ���õ�Ϊ��ǰ���Ŀ���֮��ľ���ƽ��
			if(area9[i][j].dr<0)
				sum+=(-area9[i][j].dr); 
			else
				sum+=(area9[i][j].dr); //����ı����ο���
		}
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)//���������;���ο���ľ���ı���
		{
			if(area9[i][j].state==1||area9[i][j].state==2) //����˵㲻ͨ �������һ��
				continue;
			temp=sum-area9[i][j].dr;
			if(temp>0.001)
				area9[i][j].dr=temp;
			sum2+=area9[i][j].dr;	//�ܵ� ����ı���
		}
	if(sum2<0.000001)
	 PathValue=-1; //û�п��Լ���ѡ��ĵ� ʹ��·������
}

void ANT::CalculatePr()	//����ת�Ƹ���
{
	unsigned int i,j;
	double sum=0,temp[3][3];
	if(PathValue>=0)		//��ֻ����û�б���������
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
//8������Ԫ������� �����ö���������·��ѡ��
}

void ANT::bet(unsigned int AntNum,unsigned int step, double BetValue )
{
	int i,j,num,flag=1;
	double sum;
	sum=0;
	num=0;
//	srand( (unsigned)time( NULL ) );//temp,
//	printf( "  %d\n",time( NULL ) );//
//	srand( (unsigned)time( NULL )*step*rand());//������仰Ч����� �ﵽ�����ܵ����
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
	path[step]=area9[num/3][num%3].y*grid_size+area9[num/3][num%3].x;//�Ѷ��̵õ��ĵ�д��·��
	if(path[step]==Target) //ѡ��Ŀ��� ���WorkDone
		WorkDone=1;
}

unsigned int ANT::AnalyzePath()		//����·������, �������ŵ� PathValue ��
{
	int i,flag=0;
	if(WorkDone==-1)
		return 0;	//���Ϊ�����н⣬ �򲻶�·�����д���
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


unsigned int ANT::FindInPath(unsigned int point, int step) //��ѯ���ɱ� 
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

void ANT::TurnPath()//��·���ߵ�������� TPath��
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

double ANT::DealPath(unsigned int *p1, unsigned int *p2)//p1�����С p2���յ��
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