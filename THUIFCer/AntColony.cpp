#include "StdAfx.h"
#include "AntColony.h"
#include "ANT.h"
#include "MAP.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include<fstream>
#include<vector>
using namespace std;

#define	AntNumber  20	//蚂蚁数目
#define	AntNumber2  20	//蚂蚁数目
#define	StepLimit  250	//每只蚂蚁的行走的步数极限
#define	TT  1598
#define	SS  41
////////////////////////////////////////////////////////////////////////////
///////////////////@@@@@@@@@数据采集程序段////////////////////////////////
struct DATA{
	unsigned int path[AntNumber][StepLimit];
	double Value[AntNumber];
}	data[500];
/////////////////////////////////////////////////////////////////////////////

double BestValue=1000,Q=1000;//Q是信息素总量
int	GenerLimit=50;	//蚁群繁殖代数极限
//unsigned int StartPoint=340,TarGetG=1300;	 
//unsigned int StartPoint=41,TarGetG=1558;	
unsigned int	BestTrail[StepLimit];//,BestAntGern=0;

MAP map;//生成并初始化地图对象和蚁群对象
ANT ant[AntNumber],ant2[AntNumber2];

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

void PathOut(void)
{ FILE *fp2;
  int i,j;
  fp2=fopen("path.dat", "w");
  for(i=0;i<AntNumber;i++)
    {for(j=0;j<StepLimit;j++)
        {
          fprintf(fp2, "%5d",ant[i].path[j]);
          fprintf(fp2, " ");
        }
      fprintf(fp2,"\n");
     }
  fclose(fp2) ;
}

AntColony::AntColony(void)
{
}

AntColony::~AntColony(void)
{
}

vector<D3DXVECTOR3> AntColony::OnStartsimu(int m_gener,double m_beta,double m_alpha)
{
	int i,j,step;
	int BestAntNum;
	int mm,nn;
	double BetValue;
	vector<D3DXVECTOR3> acPath;
	map.setGridSize(grid_size);
//	MessageBox("start sim");
	GenerLimit=m_gener;
	map.Target=TarGetG;/////////////******************??????????////////////
	for(j=0;j<AntNumber;j++)//对蚂蚁和地图初始化其变量 Target
		{
			ant[j].Target=TarGetG;////////////******************??????????////////////
			ant[j].setGridSize(grid_size);
			ant2[j].setGridSize(grid_size);
			ant2[j].Target=StartPoint;////////////******************??????????////////////
		}
	MapIn();

	for(i=0;i<GenerLimit;i++)
	{	
		for(j=0;j<AntNumber;j++)//生成新一代蚂蚁 对蚂蚁进行初始化
		{
			ant[j].init();
			ant[j].beta=m_beta;		//引力系数加强为1.0
			ant[j].alpha=m_alpha;		//XXX

			ant2[j].init();
			ant2[j].beta=m_beta;		//引力系数加强为1.0
			ant2[j].alpha=m_alpha;		//XXX
		}
		
		for(j=0;j<AntNumber;j++)//init every ant
		{
			ant[j].path[0]=StartPoint; //init ant j with start point
			ant2[j].path[0]=TarGetG; //init ant j with start point
		}

		for(step=1;step<StepLimit;step++)
		{	
			srand( (unsigned)time( NULL )*(step+1)*rand());
			for(j=0;j<AntNumber;j++)
			{
				if(ant[j].WorkDone==1||ant[j].PathValue==-1)
//					continue; //如果某蚂蚁已经达到目标点 或者作废了 则转为处理下一只
					goto pp;
				CreatArea9(ant[j].path[step-1],j,step);
				//生成邻域 9点 放到邻域数组里 以坐标点的形式存储
				ant[j].CalculateDr();
				//计算每个点的引力概率 放到邻域数组
				ant[j].CalculatePr();
				//计算每个点的转移概率 放到邻域数组
			///////////////////////////////////////////////
				BetValue=rand()/(double)RAND_MAX;
				//printf("BetValue=  %f\n",BetValue);
			///////////////////////////////////////////////
				if(ant[j].PathValue!=-1)
					ant[j].bet(j,step,BetValue);
				//根据赌盘算法选取转移点 放到path[step]中

pp:				if(ant2[j].WorkDone==1||ant2[j].PathValue==-1)
					continue; //如果某蚂蚁已经达到目标点 或者作废了 则转为处理下一只
				CreatArea9_ant2(ant2[j].path[step-1],j,step);
				//生成邻域 9点 放到邻域数组里 以坐标点的形式存储
				ant2[j].CalculateDr();
				//计算每个点的引力概率 放到邻域数组
				ant2[j].CalculatePr();
				//计算每个点的转移概率 放到邻域数组
			///////////////////////////////////////////////
				BetValue=rand()/(double)RAND_MAX;
				//printf("BetValue=  %f\n",BetValue);
			///////////////////////////////////////////////
				if(ant2[j].PathValue!=-1)
					ant2[j].bet(j,step,BetValue);
				//根据赌盘算法选取转移点 放到path[step]中

			}
			 
		}

		map.UpdateMap();	// 消散信息素;
	
		for(j=0;j<AntNumber;j++)
		{
		ant2[j].TurnPath();
		}
///**********************************************************///
//在此对所得两个种群的蚂蚁进行全面相交，把优化的结果写到第一种群中

		//	Cross();
			for(nn=0;nn<AntNumber;nn++)
			for(mm=0;mm<StepLimit;mm++)
			data[i].path[nn][mm]=ant[nn].path[mm];//把数据写入data中

///**********************************************************///
		for(j=0;j<AntNumber;j++)
		{
			ant[j].AnalyzePath();//对路径进行处理
		}
		BestAntNum=GetBestAnt();//得到各代中具有最优值的蚂蚁号码
	//	PathOut();	
	if(BestValue>ant[BestAntNum].PathValue&&ant[BestAntNum].PathValue>1)
	{
		for(j=0;j<StepLimit;j++)
		BestTrail[j]=ant[BestAntNum].path[j];
		BestValue=ant[BestAntNum].PathValue;

	}

		for(j=0;j<AntNumber;j++)
		{
			if(ant[j].PathValue>0) //只更新可行解
			map.UpdateInfo(Q/ant[j].PathValue, ant[j].path);
		}//加入新一代的信息素

	}
////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

	
	acPath = DrawPath(BestTrail);
//	MessageBox("end sim");
//	for(i=0;i<20;i++)
//	{
//		DrawPath(ant[i].path);
//		MessageBox("i");
//	}

	for(mm=0;mm<GenerLimit;mm++)
	for(nn=0;nn<AntNumber;nn++)
		AnalyzePathG(mm,nn);

	return acPath;
	//////////处理以后需要输出//////////
	//DataOut();
}

int AntColony::GetBestAnt()
{
	int i,BestAnt=0;
	double TValue[AntNumber];
	for(i=0;i<AntNumber;i++)
		if(ant[i].PathValue>1)
		{
			TValue[i]=100000/ant[i].PathValue;
		}
		else
		TValue[i]=0;

	for(i=0;i<AntNumber;i++)
	if(TValue[i]>TValue[BestAnt])//
		BestAnt=i;
	return BestAnt;
}

void AntColony::CreatArea9(unsigned int core, unsigned int AntNum, int step2)
{
	unsigned int i,j,point;
 	ant[AntNum].area9[1][1].x=core%grid_size;                                                                                                                                                                             
 	ant[AntNum].area9[1][1].y=core/grid_size;//设置中心点－核－现在时刻所在点
	for(i=0;i<3;i++)
 	for(j=0;j<3;j++)
 	{
		ant[AntNum].area9[i][j].x=ant[AntNum].area9[1][1].x-1+j;
 		ant[AntNum].area9[i][j].y=ant[AntNum].area9[1][1].y-1+i; 
		//填充其他八点 用坐标点的方式
 		if(ant[AntNum].area9[i][j].x>=grid_size||ant[AntNum].area9[i][j].y>=grid_size
			||ant[AntNum].area9[i][j].x<0||ant[AntNum].area9[i][j].y<0)
		{	ant[AntNum].area9[i][j].state=1; 
			ant[AntNum].area9[i][j].pr=0;
			continue;  }
		//查看各点是否超过地图范围
		if(map.point[ant[AntNum].area9[i][j].y][ant[AntNum].area9[i][j].x].PointType)
		{	ant[AntNum].area9[i][j].state=1;
			ant[AntNum].area9[i][j].pr=0; 
			continue;
		}
		//查看各点的状态

		point=ant[AntNum].area9[i][j].y*grid_size+ant[AntNum].area9[i][j].x;// 对点编码
		if(ant[AntNum].FindInPath(point,step2)) //
		{ant[AntNum].area9[i][j].state=2; 
			ant[AntNum].area9[i][j].pr=0;
			continue;}	
		//还要查询禁忌表 把已经走过的点标记为不可通过  ×××
ant[AntNum].area9[i][j].state=0;
ant[AntNum].area9[i][j].pr=map.point[ant[AntNum].area9[i][j].y][ant[AntNum].area9[i][j].x].tau;
		//初始信息素 pr 单元

 	}
	
		if(ant[AntNum].area9[0][1].state==1)
		{
			ant[AntNum].area9[0][0].state=1; 
			ant[AntNum].area9[0][0].pr=0;
			ant[AntNum].area9[0][2].state=1; 
			ant[AntNum].area9[0][2].pr=0;
		}
		if(ant[AntNum].area9[1][0].state==1)
		{
			ant[AntNum].area9[0][0].state=1; 
			ant[AntNum].area9[0][0].pr=0;
			ant[AntNum].area9[2][0].state=1; 
			ant[AntNum].area9[2][0].pr=0;
		}
		if(ant[AntNum].area9[2][1].state==1)
		{
			ant[AntNum].area9[2][2].state=1; 
			ant[AntNum].area9[2][2].pr=0;
			ant[AntNum].area9[2][0].state=1; 
			ant[AntNum].area9[2][0].pr=0;
		}
		if(ant[AntNum].area9[1][2].state==1)
		{
			ant[AntNum].area9[2][2].state=1; 
			ant[AntNum].area9[2][2].pr=0;
			ant[AntNum].area9[0][2].state=1; 
			ant[AntNum].area9[0][2].pr=0;
		}
}

void AntColony::Cross()
	//第二群蚂蚁的路径放到TPath中本函数的主要工作步骤是，
//1，把第一种群中的path转移到其TPath中
//2，清空第一种群的path
//3，两个种群的TPath 全面相交，结果放到 第一种群的path中 
{
	int i,j,a,b,flag,flag2,flag3,done,test,time=0;//flag表示两条路径有没有相交
	unsigned int *I1,*I2,*i1,*i2,*TP; //分别为两个TPath中的起点和终点的坐标
	for(i=0;i<AntNumber;i++)
	{	ant[i].WorkDone=-1;
		for(j=0;j<StepLimit;j++)
		{
			if(ant[i].path[j]==TarGetG)
				ant[i].WorkDone=1;
			ant[i].TPath[j]=ant[i].path[j];
			ant[i].path[j]=0;
		}
	}
	for(i=0;i<AntNumber;i++)
	{if(ant[i].TPath[0]!=420)
			test=0;
			time=0;
		for(j=0;j<AntNumber;j++)
		{
			time++;
			TP=ant[i].path; 
			I1=ant[i].TPath; i1=I1;
			I2=ant2[j].TPath; i2=I2;
			flag=0; done=0;
			flag2=0;
			flag3=0;
////////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//////////////
//当两条路径都可行
if(ant[i].WorkDone==1&&ant2[j].WorkDone==1)
{			for(a=0;a<StepLimit;a++)
			{
				i1++;
				if((*i1==TarGetG&&flag==0)||done==1)
					break;
				for(b=0;b<StepLimit;b++)
				{
					i2++;
					if(*i1==*i2)//两条路径有相交点
					if(i1>=I1&&i2>=I2)
					{
					if(ant[i].DealPath(I1,i1)>ant2[j].DealPath(I2,i2))//2中的一段路径更优
					{
						while(I2<i2)
						{
							*TP=*I2;
							TP++;
							I2++;
						}
						I1=i1;//把可行段的起点移到相同点处
						flag=1;
					}
					else
					{
						while(I1<i1)
						{
							*TP=*I1;
							TP++;
							I1++;
						}
						I2=i2;//把可行段的起点移到相同点处
						flag=1;
					}
					}
					if(*i1==TarGetG&&*i2==TarGetG)
					{
						*TP=TarGetG;//把最后目标点写入 path中
						done=1;         //表明交叉任务已经完成
					}
					if(*i2==TarGetG) 
					{
						i2=I2;//把可行段的移动点移到起点，等待下一轮对比；
						break;
					}
				}
			}
if(!flag)//做对比的两条路径没有相交 把ant中的路径写到ant.path中
for(a=0;a<StepLimit;a++)
ant[i].path[a]=ant[i].TPath[a];
}
/////////////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/////////////
////////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//////////////
//当ant中的条路径不可行 ant2中的路径可行
if(ant[i].WorkDone==-1&&ant2[j].WorkDone==1)
{			for(a=0;a<StepLimit;a++)
			{
				i1++;
				for(b=0;b<StepLimit;b++)
				{
					i2++;
					if(*i1==*i2)//两条路径有相交点
					if(i1>=I1&&i2>=I2)
					{
					if(ant[i].DealPath(I1,i1)>ant2[j].DealPath(I2,i2))//2中的一段路径更优
					{
						while(I2<i2)
						{
							*TP=*I2;
							TP++;
							I2++;
						}
						I1=i1;//把可行段的起点移到相同点处
						flag=1;
						ant[i].WorkDone=1;
					}
					else
					{
						while(I1<i1)
						{
							*TP=*I1;
							TP++;
							I1++;
						}
						I2=i2;//把可行段的起点移到相同点处
						flag=1;
						ant[i].WorkDone=1;
					}
					}
					if(*i2==TarGetG) 
					{
						i2=I2;//把可行段的移动点移到起点，等待下一轮对比；
						break;
					}
				}
			if(*i1==0&&flag==0)
				break;
			if(*i1==0&&flag==1) 
			{
				while(*I2!=TarGetG)
				{
					*TP=*I2;
					TP++;
					I2++;
				}
				*TP=TarGetG;//把最后目标点写入 path中
				break;
			}
			}
if(!flag)//做对比的两条路径没有相交 把ant中的路径写到ant.path中
for(a=0;a<StepLimit;a++)
ant[i].path[a]=ant[i].TPath[a];
}
/////////////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/////////////
////////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//////////////
//当ant中的可行，ant2中的路径不可行
if(ant[i].WorkDone==1&&ant2[j].WorkDone==-1)
{			for(a=0;a<StepLimit;a++)
			{
				i1++;
				if(*i1==TarGetG&&flag==0)
					break;
				for(b=0;b<StepLimit;b++)
				{
					i2++;
					if(*i1==*i2&&flag==1)//两条路径有相交点
					if(i1>=I1&&i2>=I2)
					{
					if(ant[i].DealPath(I1,i1)>ant2[j].DealPath(I2,i2))//2中的一段路径更优
					{
						while(I2<i2)
						{
							*TP=*I2;
							if(*I2==0||ant[i].path[0]!=420)
								test=0;
							TP++;
							I2++;
						}
						I1=i1;//把可行段的起点移到相同点处
						flag=1;
					}
					else
					{
						while(I1<i1)
						{
							*TP=*I1;
							if(*I1==0||ant[i].path[0]!=420)
								test=0;
							TP++;
							I1++;
						}
						I2=i2;//把可行段的起点移到相同点处
						flag=1;
					}
					}
					if(*i1==*i2&&flag==0)//两条路径有相交点第一点
						//把ant中的路径从开头到这一点写入path中
					if(i1>=I1&&i2>=I2)
					{
						while(I1<i1)
						{
							*TP=*I1;
							if(*I1==0||ant[i].path[0]!=420)
								test=0;
							TP++;
							I1++;
						}
						I2=i2;//把可行段的起点移到相同点处
						flag=1;
					}
					if(*i1==TarGetG&&flag==1)
					{
						while(I1<i1)
						{
							*TP=*I1;
							if(*I1==0||ant[i].path[0]!=420)
								test=0;
							TP++;
							I1++;
						}
						*TP=TarGetG;//把最后目标点写入 path中
						flag2=1;
						break;
					}
					if(*i2==TarGetG) 
					{
						i2=I2;//把可行段的移动点移到起点，等待下一轮对比；
						break;
					}
				}
				if(flag2==1)
					break;
			}
if(!flag)//做对比的两条路径没有相交 把ant中的路径写到ant.path中
for(a=0;a<StepLimit;a++)
ant[i].path[a]=ant[i].TPath[a];
}
/////////////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/////////////
////////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//////////////
//当ant中的不可行，ant2中的路径不可行
if(ant[i].WorkDone==-1&&ant2[j].WorkDone==-1)
{			for(a=0;a<StepLimit;a++)
			{
				i1++;
				for(b=0;b<StepLimit;b++)
				{
					i2++;					
					if(*i1==*i2&&flag==1)//两条路径有相交点
					if(i1>=I1&&i2>=I2)
					{
					if(ant[i].DealPath(I1,i1)>ant2[j].DealPath(I2,i2))//***2中的一段路径更优
					{
						while(I2<i2)
						{
							*TP=*I2;
							TP++;
							I2++;
						}
						I1=i1;//把可行段的起点移到相同点处
						flag=1;
					}
					else
					{
						while(I1<i1)
						{
							*TP=*I1;
							TP++;
							I1++;
						}
						I2=i2;//把可行段的起点移到相同点处
						flag=1;
					}
					}
					if(*i2==TarGetG) 
					{
						i2=I2;//把可行段的移动点移到起点，等待下一轮对比；
						break;
					}
					if(*i1==0)
						flag3=1;
					if(*i1==0&&flag==1) 
					{
						while(*I2!=TarGetG)
						{
							*TP=*I2;
							TP++;
							I2++;
						}
						*TP=TarGetG;//把最后目标点写入 path中
						break;
					}
					if(*i1==*i2&&flag==0)//两条路径有相交点第一点
						//把ant中的路径从开头到这一点写入path中
					if(i1>=I1&&i2>=I2)
					{
						while(I1<i1)
						{
							*TP=*I1;
							TP++;
							I1++;
						}
						I2=i2;//把可行段的起点移到相同点处
						flag=1;
						ant[i].WorkDone=1;
					}

				}
				if(flag3)
					break;
			}
if(!flag)//做对比的两条路径没有相交 把ant中的路径写到ant.path中
for(a=0;a<StepLimit;a++)
ant[i].path[a]=ant[i].TPath[a];
}
/////////////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/////////////
for(a=0;a<StepLimit;a++)
ant[i].TPath[a]=ant[i].path[a]; //ant中的某只蚂蚁的路径和ant2中蚂蚁路径相交后 
//结果放到了ant.path中， 需要把结果放到ant.tpath中，
if(j<19)//如果没有到ant2中的最后一只蚂蚁，所有相交的结果放到ant.tpath中
//等带再次相交，最后一只的结果则直接放在ant.path中等待处理，不做转移
for(a=0;a<StepLimit;a++)
ant[i].path[a]=0;
		}

	}
}

void AntColony::CreatArea9_ant2(unsigned int core, unsigned int AntNum, int step2)
{
	unsigned int i,j,point;
 	ant2[AntNum].area9[1][1].x=core%grid_size;                                                                                                                                                                             
 	ant2[AntNum].area9[1][1].y=core/grid_size;//设置中心点－核－现在时刻所在点
	for(i=0;i<3;i++)
 	for(j=0;j<3;j++)
 	{
		ant2[AntNum].area9[i][j].x=ant2[AntNum].area9[1][1].x-1+j;
 		ant2[AntNum].area9[i][j].y=ant2[AntNum].area9[1][1].y-1+i; 
		//填充其他八点 用坐标点的方式
 		if(ant2[AntNum].area9[i][j].x>=grid_size||ant2[AntNum].area9[i][j].y>=grid_size
			||ant2[AntNum].area9[i][j].x<0||ant2[AntNum].area9[i][j].y<0)
		{	ant2[AntNum].area9[i][j].state=1; 
			ant2[AntNum].area9[i][j].pr=0;
			continue;  }
		//查看各点是否超过地图范围
		if(map.point[ant2[AntNum].area9[i][j].y][ant2[AntNum].area9[i][j].x].PointType)
		{	ant2[AntNum].area9[i][j].state=1;
			ant2[AntNum].area9[i][j].pr=0; 
			continue;
		}
		//查看各点的状态

		point=ant2[AntNum].area9[i][j].y*grid_size+ant2[AntNum].area9[i][j].x;// 对点编码
		if(ant2[AntNum].FindInPath(point,step2)) //
		{ant2[AntNum].area9[i][j].state=2; 
			ant2[AntNum].area9[i][j].pr=0;
			continue;}	
		//还要查询禁忌表 把已经走过的点标记为不可通过  ×××
ant2[AntNum].area9[i][j].state=0;
ant2[AntNum].area9[i][j].pr=map.point[ant2[AntNum].area9[i][j].y][ant2[AntNum].area9[i][j].x].tau;
		//初始信息素 pr 单元

 	}
	
		if(ant2[AntNum].area9[0][1].state==1)
		{
			ant2[AntNum].area9[0][0].state=1; 
			ant2[AntNum].area9[0][0].pr=0;
			ant2[AntNum].area9[0][2].state=1; 
			ant2[AntNum].area9[0][2].pr=0;
		}
		if(ant2[AntNum].area9[1][0].state==1)
		{
			ant2[AntNum].area9[0][0].state=1; 
			ant2[AntNum].area9[0][0].pr=0;
			ant2[AntNum].area9[2][0].state=1; 
			ant2[AntNum].area9[2][0].pr=0;
		}
		if(ant2[AntNum].area9[2][1].state==1)
		{
			ant2[AntNum].area9[2][2].state=1; 
			ant2[AntNum].area9[2][2].pr=0;
			ant2[AntNum].area9[2][0].state=1; 
			ant2[AntNum].area9[2][0].pr=0;
		}
		if(ant2[AntNum].area9[1][2].state==1)
		{
			ant2[AntNum].area9[2][2].state=1; 
			ant2[AntNum].area9[2][2].pr=0;
			ant2[AntNum].area9[0][2].state=1; 
			ant2[AntNum].area9[0][2].pr=0;
		}
}

void AntColony::AnalyzePathG(int Gener, int AntNum)
{
	unsigned int *p;
	int i,flag=0;
	p=data[Gener].path[AntNum];
	for(i=0;i<StepLimit;i++)
	{
	if(1==abs((double)(*p-*(p+1)))||40==abs((double)(*p-*(p+1))))
		data[Gener].Value[AntNum]+=1;
	else if(41==abs((double)(*p-*(p+1)))||39==abs((double)(*p-*(p+1))))
		data[Gener].Value[AntNum]+=1.414;
	else
	{
		flag=2;
		data[Gener].Value[AntNum]=0;
		break;
	}
	p++;
	if (TarGetG==*p)
	{
		flag=1;
		break;
	}
	}
}

void AntColony::MapIn()
//{ FILE *fp1; 
//  int i,j;
//  fp1=fopen("map.dat", "r");
//  for(i=0;i<mapH;i++)
//		for(j=0;j<mapV;j++)
//         fscanf(fp1,"%5d",&map.point[i][j].PointType);
//  fclose(fp1) ;
//	map.point[0][0].PointType=1;//第一点一定要为不可通过的点
{
	int x=0;
	ifstream readMap("E:\\grids100.txt");
	for(int i=0;i<grid_size;i++)
	{
		for(int j=0;j<grid_size;j++)
		{
			readMap>>x;
			if(x==0)
				map.point[i][j].PointType=1;
			else if(x==1)
				map.point[i][j].PointType=0;
				
		}
	}


}

void AntColony::setGridSize(int grid_size)
{
	this->grid_size = grid_size;
}

void AntColony::setStartPoint(int StartPoint)
{
	this->StartPoint = StartPoint;
}

void AntColony::setTarGetG(int TarGetG)
{
	this->TarGetG = TarGetG;
}

vector<D3DXVECTOR3> AntColony::DrawPath(unsigned int *p)
{
	int i;
	unsigned int x,y,gys,gys2;
	unsigned int a,b;
	vector<D3DXVECTOR3> temp;
	D3DXVECTOR3 tt;
	ofstream writeFile(_T("E:\\pp.txt"));
	for(i=0;i<StepLimit;i++)
	{
		if(*p==ant[0].Target)
			break;
		/*x=(*p%mapH)*gys+gys2;
		y=(*p/mapV)*gys+gys2;*/
		a=(*p%grid_size);
		b=(*p/grid_size);
		tt.x = b;
		tt.y = a;
		temp.push_back(tt);
		writeFile<<a<<" "<<b<<endl;
		if(*(p+1)==0)
		if(*(p+2)==0||i==StepLimit-1)
			break;
		p++;
		/*x=(*p%mapH)*gys+gys2;
		y=(*p/mapV)*gys+gys2;*/
		a=(*p%grid_size);
		b=(*p/grid_size);
		tt.x = b;
		tt.y = a;
		temp.push_back(tt);
		writeFile<<a<<" "<<b<<endl;

	}

	return temp;
}