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
//�����ʽ
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

//#import "msxml6.dll"        //msxml6.0 ������
//using namespace MSXML2;

#include "tinyxml.h"        //tinyxml������

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
	//��ͼ��������
	void	InitializeDevice();
	void	InitializeDeviceBuffer();
	void	Render();
	bool SetupRender(int Lindex);//��Դ������
	int	SetupLights();
	int	SetupLight(D3DXVECTOR3 OPosit , D3DXVECTOR3 ODir , D3DXCOLOR LColor , int Lindex);
	void	UpdataLight(int Lindex);//��Դ������
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
	//���ڹ���
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
	HWND m_hwndRenderWindow;				//��Ⱦ���ھ��
	LPDIRECT3D9             g_pD3D;					// Direct3D����ӿ�ָ��
	LPDIRECT3DDEVICE9       g_pd3dDevice;	// �豸����ӿ�ָ��
	LPDIRECT3DVERTEXBUFFER9 g_pVB;			// ģ�Ͷ��㻺��
	LPDIRECT3DVERTEXBUFFER9 g_pCoordVB;//�����ᶥ�㻺��
	LPDIRECT3DVERTEXBUFFER9 g_pSlabVB;//slab���㻺��
	LPDIRECT3DVERTEXBUFFER9 g_pSkyVB;	// ��պ��Ӷ��㻺��
	D3DPRESENT_PARAMETERS	d3dpp;			// �豸��������
	LPDIRECT3DTEXTURE9  g_pSkyTex;			// ��պ��ӵ�����
	LPD3DXMESH g_pWorldCenter;				// ��������

	D3DMATERIAL9 mtrl[100];							// Entities Material9
	D3DLIGHT9 Light[8];									// ��Դ�����8��
	D3DXVECTOR3 LightDir[8];						// ��Դ���������ڸ��¹�Դ��Ϣʱʹ��
	D3DXCOLOR BackColor;							// ������ɫ
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
	bool		IsVBufferInit;//�ж϶��㻺������������Ƿ��ʼ��
	bool		initialized;//DirectX�豸�Ƿ��ʼ��
	int			DirectXStatus;//DirectX״̬�Ƿ�����
	bool		IsCoordVBufferInit;//�ж������Ỻ���Ƿ��ʼ��
	bool		RenderStatus;//�ж���Ⱦ���õ�״̬
	bool		IsSkyBoxVBufferInit;//��պ��ӻ���״̬
	bool		IsWorldCenter;//��������Mesh״̬
	bool		IsPath;
	float		TraceTime;//·����Ⱦʱ��
	LPDIRECT3DVERTEXBUFFER9 g_pPathVB;	// ·�����㻺��
	double arc;

	bool        IsSlabBufferInit;

	//·����ز���
	vector<D3DXVECTOR3>wallPoints;//��͹��ǰ�ϰ��ﶥ�����꼯��
	vector<D3DXVECTOR3>mainPoints;//��͹�����ϰ��ﶥ�����꼯��

	D3DXVECTOR3 Pos;
	vector<D3DXVECTOR3>footPoints;

	vector<int>door_x;
	vector<int>door_y;

	double arcs;
	//int stepCounter;

public:
	//���Դ��� ȡ�����е���Сֵ
	D3DXVECTOR3 min_x;
	D3DXVECTOR3 min_y;
	D3DXVECTOR3 max_x;
	D3DXVECTOR3 max_y;
	vector<D3DXVECTOR3>wall;

	//��Ϊ�����100��դ��,ֵ0˵���˴����ϰ��1Ϊ��ͨ��
	//bool grids[GRIDNUM][GRIDNUM];

	std::vector<int>start;//ǽ
	std::vector<int>num;//ǽ
	std::vector<int>storey;//��Ӧ��¥���

	std::vector<int>dstart;//�Ŷ�Ӧ��������ʼλ��
	std::vector<int>dnum;//�Ŷ�Ӧ����������
	std::vector<int>dstorey;

	std::vector<int>fstart;//�Ҿ߶�Ӧ��������ʼλ��
	std::vector<int>fnum;//�Ҿ߶�Ӧ����������
	std::vector<int>fstorey;

	std::vector<int>cstart;//����Ӧ��������ʼλ��
	std::vector<int>cnum;//����Ӧ����������
	std::vector<int>cstorey;

	std::vector<int>sstart;//�ռ��Ӧ��������ʼλ��
	std::vector<int>snum;//�ռ��Ӧ����������
	std::vector<int>sstorey;

	std::vector<int>pstart;//ר���豸��Ӧ��������ʼλ��
	std::vector<int>pnum;//ר���豸��Ӧ����������
	std::vector<int>pstorey;

	std::vector<int>disstart;//distribution element
	std::vector<int>disstorey;
	std::vector<int>disnum;//distribution element

	std::vector<int>ststart;//¥��
	std::vector<int>ststorey;
	std::vector<int>stnum;//¥��

	std::vector<int>slstart;//¥��
	std::vector<int>slstorey;
	std::vector<int>slnum;//¥��

	TiXmlDocument PropertyListXml;//�����б�

	int grid_size;
	FastMarching fm;

	int blength;//������ʵ�ʳ���
	int bwidth;//������ʵ�ʿ��
	int offsetx;
	int offsety;
	std::vector<D3DXVECTOR3>stairPos;
	

};

//#endif