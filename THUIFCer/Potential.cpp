#include "StdAfx.h"
#include "Potential.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include<algorithm>

Potential::Potential(void)
{
	Place zero;
	zero.x = 0;
	zero.y = 0;

	ReSet = false;
	ISVirObs = false;
	decide1 = true;
	m_flag = 0;
	m_flag1 = 0;
	K = 1;//引力系数
	M = 100;//斥力系数
	little = 0.0000001;
	shichang = 0;
	Po = 10;
	add_angle = M_PI/16;
	m_Speed = 35;
	extra.Locality = zero;
	extra1.Locality = zero;
	extra.Radius = 0;
	extra1.Radius = 0;
	virtual_obs.clear();
	Obs.clear();

	m_CurrentTIME = 0;
	m_LastTIME = 0;
	m_Destination = zero;
	m_Position = zero;
	m_PosiADD.x = m_PosiADD.y = 0;
	m_step = 0;
	POS.clear();
	my = false;
}

Potential::~Potential(void)
{
}


int Potential::Move(std::vector<Particle> Current_Obstacle)
{
	CRect rect;
	DWORD currentTIME = GetTickCount();

	if( m_LastTIME >= currentTIME )   //暂停
	{
		return NULL;
	}

	if( m_Position.x == m_Destination.x && m_Position.y == m_Destination.y )   //到达目的地
	{
		if( virtual_obs.size() != 0 )
		{
			virtual_obs.clear();
		}

		m_State = 0;
		Obs.clear();
		POSx.clear();
		POSy.clear();
		return NULL;
	}

	if( m_State == 1)  //移动状态
	{
		m_CurrentTIME = currentTIME;
		Obs = Current_Obstacle;
		Obs_1 = Current_Obstacle;

		if( m_PosiADD.x == 0 && m_PosiADD.y == 0 ) //第一次
		{
			m_PosiADD.x = m_Position.x;
			m_PosiADD.y = m_Position.y;
		}

		/*if( Obs.size() > 1 )
			Po = max(newPo(Obs),m_Radiu);
		else */
			Po = 4;

		double currentangle;//当前角度
		double angle_goal; //目标角度
		vector<double>angle_obstacle; //障碍角度
		Force Fattract; //引力
		vector<Force> Fobstacle;  //斥力
		double att,rep;
		Particle Goal;
		Goal.Locality = m_Destination;
		Goal.Radius = m_Radiu;

		///////////////////处理动态虚拟目标
		original_goal = Goal;
		Axis Current;
		Current.fx = m_PosiADD.x;
		Current.fy = m_PosiADD.y;
		double temp_potential = total_potential(Current,m_Radiu,Goal,Obs_1,K,M);
		Potent.push_back(temp_potential);
		int Potential_num = Potent.size();
		if(Potential_num>1)
		{
			if(Potent.back() - Potent[Potential_num-2] > shichang)
			{
				m_flag ++;
				if(m_flag == 1)
				{
					bool decide = IsRight(m_Position,Goal.Locality,Obs_1);//是否向右运动
					Temp_Goal = dummy_goal(Current,m_Radiu,Goal,Obs_1,decide);//求出一个虚拟目标点
					extra.Locality.x = (Current.fx + 0.5);
					extra.Locality.y = (Current.fy + 0.5);
					extra.Radius = m_Radiu;
				}
			}
			if(extra.Radius != 0 && Min_Dis(m_Position,m_Radiu,Obs)<1.5*Po)
			{
				original_goal = Temp_Goal;
				Potent.pop_back();//虚拟目标阶段求势场无意义
				double t1 = total_potential(Current,m_Radiu,original_goal,Obs,K,M);
				Potent_extra.push_back(t1);
				int Num = Potent_extra.size();
				if(Num>1)
				{
					if((Potent_extra.back() - Potent_extra[Num-2])>shichang)
					{
						ISVirObs = true;//二次目标
						m_flag1++;
						if(m_flag1 == 1)
						{
							decide1 = IsRight(m_Position,original_goal.Locality,Obs);
							Temp_Goal = dummy_goal(Current,m_Radiu,original_goal,Obs,decide1);
							extra.Locality.x = (Current.fx+0.5);
							extra.Locality.y = (Current.fy+0.5);
							extra.Radius = m_Radiu;
						}
						Potent_extra.clear();
					}
					else 
						m_flag1 = 0;
				}
			}
			else 
			{
				//虚拟障碍
				if(ISVirObs == true)
				{
					double Angle_goal;
					double dx,dy,dis;
					dx = Goal.Locality.x - m_Position.x;
					dy = Goal.Locality.y - m_Position.y;
					if(dx>0)
						Angle_goal = atan(-dy/dx);
					else 
						Angle_goal = (M_PI-atan(dy/dx));

					double temp_angle;
					if(IsRight(m_Position,Goal.Locality,Obs)==true)//向右运动
						temp_angle = Angle_goal - M_PI/4;
					else 
						temp_angle = Angle_goal + M_PI/4;

					Particle Virtual_Obs;
					Virtual_Obs.Locality.x = m_Position.x + (2*m_Radiu+Po/4)*cos(temp_angle)+0.5;
					Virtual_Obs.Locality.y = m_Position.y - (2*m_Radiu+Po/4)*sin(temp_angle)+0.5;
					Virtual_Obs.Radius = m_Radiu;
					virtual_obs.push_back(Virtual_Obs);
				}
				//否则目标正常，不设置虚拟目标original_goal
				m_flag = 0;
				m_flag1 = 0;
				extra.Radius = 0;
				original_goal = Goal;
				ISVirObs = false;
			}
		}
		
		for(int i=0;i<virtual_obs.size();i++)
		{
			if(my == true)
				Obs.push_back(virtual_obs[i]);
		}

		/////////////////////////////////////
		angle_goal = compute_angle(Current,original_goal,Obs,angle_obstacle);//计算角度
		att = compute_attract(Current,original_goal,angle_goal,Fattract,K);//计算引力
		rep = compute_obstacle(Current,m_Radiu,Obs,original_goal,angle_goal,Fobstacle,angle_obstacle,Po,M);//计算斥力
		currentangle = compute_composition(Fattract,Fobstacle,Obs,angle_goal);//计算合力
		angle_obstacle.clear();
		Fobstacle.clear();

		////////////////////////////////////////////////////////////
		//保存位置，更新方向向量
		m_PosiADD.x += (float)m_Speed*(m_CurrentTIME - m_LastTIME)/1000.0*cos(currentangle);
		m_PosiADD.y -= (float)m_Speed*(m_CurrentTIME - m_LastTIME)/1000.0*sin(currentangle);
		POSx.push_back(m_PosiADD.x);//显示轨迹
		POSy.push_back(m_PosiADD.y);
		m_Position.x = m_PosiADD.x + 0.5;
		m_Position.y = m_PosiADD.y + 0.5;

		POS.push_back(m_Position);

		//判断是否过程(或已经到达目的地)
		rect.left = m_Destination.x - m_Speed*(m_CurrentTIME - m_LastTIME)/1000.0;
		rect.right = m_Destination.x + m_Speed*(m_CurrentTIME - m_LastTIME)/1000.0;
		rect.top = m_Destination.y - m_Speed*(m_CurrentTIME - m_LastTIME)/1000.0;
		rect.bottom = m_Destination.y + m_Speed*(m_CurrentTIME - m_LastTIME)/1000.0;

		CPoint tt;
		tt.x = (int)m_Position.x;
		tt.y = (int)m_Position.y;
		if(rect.PtInRect(tt))
		{
			m_Position = m_Destination;
		}

		m_LastTIME = m_CurrentTIME;
	}
	return 0;
}

DWORD Potential::ReadyMOVE()
{
	m_LastTIME = GetTickCount();
	m_State = 1;
	return m_LastTIME;
}

int Potential::Pause(int Time)
{
	if(Time == 0)
	{
		m_State = 0;//暂停运行
		m_LastTIME = GetTickCount();
	}
	else 
		m_LastTIME = GetTickCount() + Time;

	return 0;
}

//返回所有障碍物中半径最小的值
double Potential::newPo(std::vector<Particle> Obs)
{
	vector<double>rr;
	for(int j=0;j<Obs.size();j++)
	{
		rr.push_back(Obs[j].Radius);
	}

	double min_r = *min_element(rr.begin(),rr.end());
	rr.clear();
	return min_r;
}

//计算角度
double Potential::compute_angle(Axis current, Particle Goal, std::vector<Particle> Obstacle, std::vector<double> &angle_obstacle)
{
	double angle_goal;//当前位置与目标夹角
	double dx,dy;
	dx = Goal.Locality.x - current.fx;
	dy = Goal.Locality.y - current.fy;
	if(dx>0)
		angle_goal = atan(-dy/dx);
	else 
		angle_goal = M_PI - atan(dy/dx);

	int num = Obstacle.size();
	if( num == 0 )
	    return angle_goal;
	
	for(int i=0;i<num;i++)
	{
		dx = Obstacle[i].Locality.x - current.fx;
		dy = Obstacle[i].Locality.y - current.fy;
		if(dx>0)
			angle_obstacle.push_back(atan(-dy/dx));
		else 
			angle_obstacle.push_back(M_PI-atan(dy/dx));
	}
	return angle_goal;
}

//计算引力
double Potential::compute_attract(Axis current, Particle Goal, double angle_goal, Force &Fatt, double k)
{
	double dx,dy;
	dx = Goal.Locality.x - current.fx;
	dy = Goal.Locality.y - current.fy;
	double R;
	R = sqrt(dx*dx+dy*dy);
	if(R>300)
	{
		Fatt.Fx = 300*k*cos(angle_goal);
		Fatt.Fy = 300*k*sin(angle_goal);
	}
	else 
	{
		Fatt.Fx = k*R*cos(angle_goal);
		Fatt.Fy = k*R*sin(angle_goal);
	}
	
	return 1.0/2*k*(dx*dx+dy*dy);
}

//计算斥力
double Potential::compute_obstacle(Axis current, double current_r, std::vector<Particle> Obstacle, Particle Goal, double angle_goal, std::vector<Force> &Fobs, std::vector<double> angle_obstacle, double Po, double m)
{
	double urep = 0;
	double dx,dy;
	double R;
	double ss;
	int num = Obstacle.size();
	if(num == 0)
		return 0;

	double sum_r;
	
	for(int i=0;i<num;i++)
	{
		dx = Obstacle[i].Locality.x - current.fx;
		dy = Obstacle[i].Locality.y - current.fy;
		ss = (dx*dx+dy*dy);
		R = sqrt(ss);
		sum_r = current_r + Obstacle[i].Radius;
		if(R - current_r>=Po)
		{
			Force temp;
			temp.Fx = 0;
			temp.Fy = 0;
			Fobs.push_back(temp);
		}
		else 
		{
			if(R - current_r<Po && R - current_r>0)
			{
				double gg;
				gg = (current.fx-Goal.Locality.x)*(current.fx-Goal.Locality.x)+(current.fy-Goal.Locality.y)*(current.fy-Goal.Locality.y);//与目标点的距离
				double Fre1,Fre2;
				double sum_r;
				sum_r = current_r + Obstacle[i].Radius;
				Fre1 = m*(1.0/(R - current_r+little)-1.0/Po)*gg/((R - current_r+little)*(R - current_r+little));//斥力
				Fre2 = m*(1.0/(R - current_r+little)-1.0/Po)*(1.0/(R - current_r+little)-1.0/Po)*sqrt(gg);//引力
				Force temp;
				temp.Fx = Fre1*cos(angle_obstacle[i])-Fre2*cos(angle_goal);
				temp.Fy = Fre1*sin(angle_obstacle[i])-Fre2*sin(angle_goal);
				Fobs.push_back(temp);
				urep += 1.0/2*m*(1.0/(R - current_r+little)-1.0/Po)*(1.0/(R - current_r+little)-1.0/Po)*gg;
			}
		}
	}
	return urep;
}

//计算合力方向
double Potential::compute_composition(Force Fatt, std::vector<Force> Fobs, std::vector<Particle> Obstacle, double angle)
{
	double Fxx;
	double Fyy;
	int num = Obstacle.size();
	double sx;
	double sy;
	sx = 0;
	sy = 0;

	if(num == 0)
	{
		sx = 0;
		sy = 0;
	}
	else 
	{
		for(int i=0;i<num;i++)
		{
			sx += -(Fobs[i].Fx);
			sy += -(Fobs[i].Fy);
		}
	}

	Fxx = Fatt.Fx + sx;
	Fyy = Fatt.Fy + sy;

	if( Fxx > 0 )
		return atan(Fyy/Fxx);
	else 
		return M_PI + atan(Fyy/Fxx);
}

//计算总势能
double Potential::total_potential(Axis current, double current_r, Particle Goal, std::vector<Particle> Obstacle, double k, double m)
{
	double uatt;
	double dx1,dy1;
	dx1 = Goal.Locality.x - current.fx;
	dy1 = Goal.Locality.y - current.fy;
	double gg;
	gg = (dx1*dx1 + dy1*dy1);
	if( sqrt(gg)>300 )
	{
		uatt = 1.0/2*k*(2*300*sqrt(gg)-300*300);
	}
	else 
		uatt = 1.0/2*k*gg;//引力势场

	double urep = 0;
	double dx,dy;
	double R;
	int num = Obstacle.size();
	double sum_r;
	for(int i=0;i<num;i++)
	{
		dx = Obstacle[i].Locality.x - current.fx;
		dy = Obstacle[i].Locality.y - current.fy;
		R = sqrt(dx*dx+dy*dy);
		sum_r = current_r + Obstacle[i].Radius;//计算某点的势能，并非粒子势能
		if( Obstacle[i].Radius - current_r < Po && Obstacle[i].Radius - current_r>=0 )
			urep += 1/2.0*m*(1.0/(Obstacle[i].Radius - current_r+little)-1.0/Po)*(1.0/(Obstacle[i].Radius - current_r+little)-1.0/Po)*gg;
	}

	return uatt+urep;
}

Particle Potential::dummy_goal(Axis current, double current_r, Particle Goal, std::vector<Particle> Obs, bool flag)
{
	//得到当前位置与目标夹角
	double Angle_goal;
	double virtual_angle;
	Particle Dummy_Goal;
	double dx,dy,dis;
	dx = Goal.Locality.x - current.fx;
	dy = Goal.Locality.y - current.fy;
	dis = sqrt(dx*dx+dy*dy);
	if(dx>0)
		Angle_goal = atan(-dy/dx);
	else 
		Angle_goal = (M_PI-atan(dy/dx));
	
	//得到当前粒子最近的障碍信息
	vector<double>Obs_Dis;
	int record_i = -1; //用来存储最近的障碍物是第几个
	for(int i=0;i<Obs.size();i++)
	{
		dx = Obs[i].Locality.x - current.fx;
		dy = Obs[i].Locality.y - current.fy;
		//double Dis = sqrt(dx*dx+dy*dy) - (current_r + Obs[i].Radius);
		double Dis = Obs[i].Radius - current_r;
		Obs_Dis.push_back(Dis);
	}

	double Nearest = *min_element(Obs_Dis.begin(),Obs_Dis.end());
	for(int i=0;i<Obs.size();i++)
	{
		if(Obs_Dis[i] == Nearest)
		{
			record_i = i;
			break;
		}
	}

	double angle_Obs;//与最近的障碍物的角度
	dx = Obs[record_i].Locality.x - current.fx;
	dy = Obs[record_i].Locality.y - current.fy;
	if(dx>0)
		angle_Obs = atan(-dy/dx);
	else
		angle_Obs = (M_PI-atan(dy/dx));

	double CornerCut;


	//CornerCut = asin(Obs[record_i].Radius/sqrt(dx*dx+dy*dy))+add_angle;
	CornerCut = asin(1.5/sqrt(dx*dx+dy*dy))+add_angle;



	if(flag == true)//虚拟目标始终在右边
		virtual_angle = angle_Obs - Angle_goal + 2*CornerCut;
	else //虚拟目标始终在左边
		virtual_angle = angle_Obs - Angle_goal - 2*CornerCut;

	Dummy_Goal.Locality.x = int(current.fx + 2*dis*cos(Angle_goal+virtual_angle));
	Dummy_Goal.Locality.y = int(current.fy - 2*dis*sin(Angle_goal+virtual_angle));
	Dummy_Goal.Radius = current_r;
	Obs_Dis.clear();
	return Dummy_Goal;
}

double Potential::Min_Dis(Place p1,int  rr,vector<Particle> p2)
{
	vector<double>dis;
	for(int i=0;i<p2.size();i++)
	{
		//double s1 = (p1.x-p2[i].Locality.x)*(p1.x-p2[i].Locality.x)+(p1.y-p2[i].Locality.y)*(p1.y-p2[i].Locality.y);
		//dis.push_back(sqrt(s1)-rr-p2[i].Radius);
		dis.push_back(p2[i].Radius-rr);
	}
	double MinDis = *min_element(dis.begin(),dis.end());
	return MinDis;
}

double Potential::Set_Dis(Place p1, Place p2)
{
	return sqrt(double((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)));
}

//由目标及各障碍物位置判断接下来运动方向是否向右，若是则返回TRUE，否则返回FALSE
bool Potential::IsRight(Place Current, Place Goal, std::vector<Particle> OBS)
{
	double Angle_goal;//目标角度
	double dx,dy,dis;
	dx = Goal.x - Current.x;
	dy = Goal.y - Current.y;
	if(dx>0)
		Angle_goal = atan(-dy/dx);
	else 
		Angle_goal = (M_PI - atan(dy/dx));  //范围（-90,270）

	vector<double>AngleObs;//障碍角度
	for(int i=0;i<OBS.size();i++)
	{
		dx = OBS[i].Locality.x - Current.x;
		dy = OBS[i].Locality.y - Current.y;
		//double dis = sqrt(dx*dx+dy*dy);  //离障碍物的距离
		double dis = OBS[i].Radius;  //离障碍物的距离
		if( dis < 20*Po )
		{
			if( dx > 0 )
				AngleObs.push_back(atan(-dy/dx));
			else
				AngleObs.push_back(M_PI-atan(dy/dx));
		}
	}

	int left_num,right_num;
	left_num = 0;
	right_num = 0;
	for(int i=0;i<AngleObs.size();i++)
	{
		if(sin(AngleObs[i]-Angle_goal)>0)
			left_num ++;
		else 
			right_num ++;
	}

	if(left_num < right_num)
		return true;
	else 
		return false;
}

