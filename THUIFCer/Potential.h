#pragma once
#include<vector>
using namespace std;

struct Axis//坐标
{
	double fx;
	double fy;
};

struct Force//受力
{
	double Fx;
	double Fy;
};

struct Place
{
	double x;
	double y;
};

struct Particle//粒子信息
{
	Place Locality;
	int Radius;
};

class Potential
{
public:
	Potential(void);
public:
	~Potential(void);

	Place m_Position;    //原始位置
	Place m_Destination; //目标位置
	int m_step;           //粒子移动的步数

	struct 
	{
		double x;
		double y;
	}m_PosiADD;           //当前位移，运动方向

	int m_Radiu;          //对象半径
	UINT m_State;         //状态
	int m_Speed;          //任务完成时间；在选转中表示每秒钟转过的角度
	DWORD m_LastTIME;     //开始时间
	DWORD m_CurrentTIME;  //当前时间

	int m_Num;            //目标编号

	
	vector<double>Potent;           //记录势场数值
	vector<double>Potent_extra;     //记录势场数值

	//虚拟目标相关参数
	int m_flag;     
	int m_flag1;
	Particle Temp_Goal;
	Particle extra;
	Particle extra1;
	vector<Particle>virtual_obs;
	vector<Particle>Obs;
	vector<Particle>Obs_1;

	double K;                //引力系数
	double M;                //斥力系数
	double Po;               //障碍影响距离
	double little;           //
	int shichang;            //势场灵敏度
	double add_angle;        //
	Particle original_goal;  //目标
	bool ReSet;              //用于动态避障，是否让粒子运动
	bool ISVirObs;           //是否增加虚拟障碍
	vector<double>POSx;      //
	vector<Place>POS;       //
	vector<double>POSy;      //
	bool decide1;            //

	bool my;

	int Move(vector<Particle>Current_Obstacle);
	DWORD ReadyMOVE(void);
	int Pause(int Time=0);

public:

	//计算总势能
	double total_potential(Axis current,double current_r, Particle Goal,vector<Particle>Obstacle,double k,double m);

	//设置虚拟目标
	Particle dummy_goal(Axis current,double current_r,Particle Goal,vector<Particle>Obs,bool flag);

	//更新Po
	double newPo(vector<Particle>Obs);

	//计算角度
	double compute_angle(Axis current,Particle Goal,vector<Particle>Obstacle,vector<double>& angle_obstacle);

	//计算引力
	double compute_attract(Axis current,Particle Goal,double angle_goal,Force &Fatt,double k);

	//计算斥力
	double compute_obstacle(Axis current,double current_r,vector<Particle>Obstacle,Particle Goal,double
		angle_goal,vector<Force>& Fobs,vector<double>angle_obstacle,double Po,double m);

	//计算合力方向
	double compute_composition(Force Fatt,vector<Force> Fobs,vector<Particle>Obstacle,double angle);

	//
	double Min_Dis(Place p1,int  rr,vector<Particle> p2);

	//
	double Set_Dis(Place p1,Place p2);

	//
	bool IsRight(Place Current,Place Goal,vector<Particle>OBS);


};
