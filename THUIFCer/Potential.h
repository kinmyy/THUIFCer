#pragma once
#include<vector>
using namespace std;

struct Axis//����
{
	double fx;
	double fy;
};

struct Force//����
{
	double Fx;
	double Fy;
};

struct Place
{
	double x;
	double y;
};

struct Particle//������Ϣ
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

	Place m_Position;    //ԭʼλ��
	Place m_Destination; //Ŀ��λ��
	int m_step;           //�����ƶ��Ĳ���

	struct 
	{
		double x;
		double y;
	}m_PosiADD;           //��ǰλ�ƣ��˶�����

	int m_Radiu;          //����뾶
	UINT m_State;         //״̬
	int m_Speed;          //�������ʱ�䣻��ѡת�б�ʾÿ����ת���ĽǶ�
	DWORD m_LastTIME;     //��ʼʱ��
	DWORD m_CurrentTIME;  //��ǰʱ��

	int m_Num;            //Ŀ����

	
	vector<double>Potent;           //��¼�Ƴ���ֵ
	vector<double>Potent_extra;     //��¼�Ƴ���ֵ

	//����Ŀ����ز���
	int m_flag;     
	int m_flag1;
	Particle Temp_Goal;
	Particle extra;
	Particle extra1;
	vector<Particle>virtual_obs;
	vector<Particle>Obs;
	vector<Particle>Obs_1;

	double K;                //����ϵ��
	double M;                //����ϵ��
	double Po;               //�ϰ�Ӱ�����
	double little;           //
	int shichang;            //�Ƴ�������
	double add_angle;        //
	Particle original_goal;  //Ŀ��
	bool ReSet;              //���ڶ�̬���ϣ��Ƿ��������˶�
	bool ISVirObs;           //�Ƿ����������ϰ�
	vector<double>POSx;      //
	vector<Place>POS;       //
	vector<double>POSy;      //
	bool decide1;            //

	bool my;

	int Move(vector<Particle>Current_Obstacle);
	DWORD ReadyMOVE(void);
	int Pause(int Time=0);

public:

	//����������
	double total_potential(Axis current,double current_r, Particle Goal,vector<Particle>Obstacle,double k,double m);

	//��������Ŀ��
	Particle dummy_goal(Axis current,double current_r,Particle Goal,vector<Particle>Obs,bool flag);

	//����Po
	double newPo(vector<Particle>Obs);

	//����Ƕ�
	double compute_angle(Axis current,Particle Goal,vector<Particle>Obstacle,vector<double>& angle_obstacle);

	//��������
	double compute_attract(Axis current,Particle Goal,double angle_goal,Force &Fatt,double k);

	//�������
	double compute_obstacle(Axis current,double current_r,vector<Particle>Obstacle,Particle Goal,double
		angle_goal,vector<Force>& Fobs,vector<double>angle_obstacle,double Po,double m);

	//�����������
	double compute_composition(Force Fatt,vector<Force> Fobs,vector<Particle>Obstacle,double angle);

	//
	double Min_Dis(Place p1,int  rr,vector<Particle> p2);

	//
	double Set_Dis(Place p1,Place p2);

	//
	bool IsRight(Place Current,Place Goal,vector<Particle>OBS);


};
