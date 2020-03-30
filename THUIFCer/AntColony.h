#pragma once
#include<vector>
using namespace std;

class AntColony
{
public:
	AntColony(void);
public:
	~AntColony(void);

// Attributes
public:
	CPoint StartPt,EndPt;
	int grid_size;
	int StartPoint;
	int TarGetG;

public:
	//void DataOut();
	void AnalyzePathG(int Gener, int AntNum);
	void CreatArea9_ant2(unsigned int core, unsigned int AntNum, int step2);
	void Cross();
	vector<D3DXVECTOR3> DrawPath(unsigned int *p);
	void CreatArea9(unsigned int core , unsigned int AntNum,int step2);
	int GetBestAnt();
	//void OnSetpar();
	vector<D3DXVECTOR3> OnStartsimu(int m_gener,double m_beta,double m_alpha);//开始仿真
	void MapIn();//读取障碍物
	void setStartPoint(int StartPoint);
	void setTarGetG(int TarGetG);
	void setGridSize(int grid_size);
	//void OnShowshange();
};
