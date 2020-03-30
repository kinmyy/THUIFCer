#pragma once

#include "IFCProject.h"
#include "cam.h"
#include "vec.h"
#include <map>
#include <vector>
#include<fstream>
#include "FastMarching.h"
using namespace std;


/*
//顶点格式
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

typedef struct CUSTOMVERTEX {
	float	x;
	float	y;
	float	z;
	float	nx;
	float	ny;
	float	nz;
	float   tu;
	float   tv;
}	customvertex;
*/
/*
#include <d3d9.h>           // Directx9
#include <d3dx9.h>          //

//#import "msxml6.dll"        //msxml6.0 解析器
//using namespace MSXML2;

#include "tinyxml.h"        //tinyxml解析器

#ifdef TIXML_USE_STL
#include <iostream>
#include <sstream>
using namespace std;
#else
#include <stdio.h>
#endif
*/

class CIFCRender
{
public:
	CIFCRender(HWND RenderHwnd);
	~CIFCRender();
	//绘图基础设置
	void	InitializeDevice();
	void	InitializeDeviceBuffer();
	void	Render();
	bool SetupRender(int Lindex);//光源索引数
	int	SetupLights();
	int	SetupLight(D3DXVECTOR3 OPosit , D3DXVECTOR3 ODir , D3DXCOLOR LColor , int Lindex);
	void	UpdataLight(int Lindex);//光源索引数
	int	SetupMatrices();
	int	SetupMaterial();
	bool RenderClear();
	void RenderModelShadow();//Render() Call , draw model shadow

	void CalculatePath();
	bool setup();
	bool SetupBall();
	bool RenderMan();
	bool setup1();
	void GetAxis();

	void CalculatePath_MF();
	void GetAxis_MF();
	int GetFloor(int instanceId);

	void OnSize(int cx , int cy);
	
	bool LoadIfc(CString m_strFileName , CString m_strSchemaName);
	bool LoadIfc(CIFCProject * ifcPtr);
	bool GetDeviceState();
	CIFCProject * GetIFCProjectPtr();
	//窗口过程
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	bool IsDoorOpen(int id);
	bool IsSpacePrivate(int id);
	int GetElementRiskLevel(int id);
	void setGridSize(int grid_size);
	vector<int>getDoorX();
	vector<int>getDoorY();

	double getLength();
	double getWidth();

	double getOffsetx();
	double getOffsety();


protected:
	//Device
	HWND m_hwndRenderWindow;				//渲染窗口句柄
	LPDIRECT3D9             g_pD3D;					// Direct3D对象接口指针
	LPDIRECT3DDEVICE9       g_pd3dDevice;	// 设备对象接口指针
	LPDIRECT3DVERTEXBUFFER9 g_pVB;			// 模型顶点缓存
	LPDIRECT3DVERTEXBUFFER9 g_pCoordVB;//坐标轴顶点缓存
	LPDIRECT3DVERTEXBUFFER9 g_pSlabVB;//slab顶点缓存
	LPDIRECT3DVERTEXBUFFER9 g_pSkyVB;	// 天空盒子顶点缓存
	D3DPRESENT_PARAMETERS	d3dpp;			// 设备参数描述
	LPDIRECT3DTEXTURE9  g_pSkyTex;			// 天空盒子的纹理
	LPD3DXMESH g_pWorldCenter;				// 世界中心

	D3DMATERIAL9 mtrl[100];							// Entities Material9
	D3DLIGHT9 Light[8];									// 光源，最多8个
	D3DXVECTOR3 LightDir[8];						// 光源方向，用来在更新光源信息时使用
	D3DXCOLOR BackColor;							// 背景颜色
	std::map<CString , int> EntitytoMtrl;			// Entity Mtrl Mapping
	std::map<CString , int>::iterator mapiter;	// Entity Iteritor
	int		m_iWidth;
	int		m_iHeight;

	std::vector<D3DXVECTOR3> Path;
	D3DXVECTOR3 pre;
	D3DXVECTOR3 mid;
	int counter;

	//Primitive Data
	CIFCProject * ifcProject;
	

public:
	bool		IsVBufferInit;//判断顶点缓存和索引缓存是否初始化
	bool		initialized;//DirectX设备是否初始化
	int			DirectXStatus;//DirectX状态是否良好
	bool		IsCoordVBufferInit;//判断坐标轴缓存是否初始化
	bool		RenderStatus;//判断渲染设置的状态
	bool		IsSkyBoxVBufferInit;//天空盒子缓存状态
	bool		IsWorldCenter;//世界中心Mesh状态
	bool		IsPath;
	float		TraceTime;//路径渲染时间
	LPDIRECT3DVERTEXBUFFER9 g_pPathVB;	// 路径顶点缓存
	double arc;

	bool        IsSlabBufferInit;

	//路径相关参数
	vector<D3DXVECTOR3>wallPoints;//求凸包前障碍物顶点坐标集合
	vector<D3DXVECTOR3>mainPoints;//求凸包后障碍物顶点坐标集合

	D3DXVECTOR3 Pos;
	vector<D3DXVECTOR3>footPoints;

	vector<int>door_x;
	vector<int>door_y;

	double arcs;
	//int stepCounter;

public:
	//测试代码 取坐标中的最小值
	D3DXVECTOR3 min_x;
	D3DXVECTOR3 min_y;
	D3DXVECTOR3 max_x;
	D3DXVECTOR3 max_y;
	vector<D3DXVECTOR3>wall;

	//分为长宽各100个栅格,值0说明此处有障碍物，1为可通行
	//bool grids[GRIDNUM][GRIDNUM];

	std::vector<int>start;//墙
	std::vector<int>num;//墙
	std::vector<int>storey;//对应的楼层号

	std::vector<int>dstart;//门对应的坐标起始位置
	std::vector<int>dnum;//门对应的坐标数量
	std::vector<int>dstorey;

	std::vector<int>fstart;//家具对应的坐标起始位置
	std::vector<int>fnum;//家具对应的坐标数量
	std::vector<int>fstorey;

	std::vector<int>cstart;//柱对应的坐标起始位置
	std::vector<int>cnum;//柱对应的坐标数量
	std::vector<int>cstorey;

	std::vector<int>sstart;//空间对应的坐标起始位置
	std::vector<int>snum;//空间对应的坐标数量
	std::vector<int>sstorey;

	std::vector<int>pstart;//专用设备对应的坐标起始位置
	std::vector<int>pnum;//专用设备对应的坐标数量
	std::vector<int>pstorey;

	std::vector<int>disstart;//distribution element
	std::vector<int>disstorey;
	std::vector<int>disnum;//distribution element

	std::vector<int>ststart;//楼梯
	std::vector<int>ststorey;
	std::vector<int>stnum;//楼梯

	std::vector<int>slstart;//楼梯
	std::vector<int>slstorey;
	std::vector<int>slnum;//楼梯

	TiXmlDocument PropertyListXml;//属性列表

	int grid_size;
	FastMarching fm;

	int blength;//建筑的实际长度
	int bwidth;//建筑的实际宽度
	int offsetx;
	int offsety;
	std::vector<D3DXVECTOR3>stairPos;
	

};

//#endif