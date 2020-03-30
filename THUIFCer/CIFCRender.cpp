#include "stdafx.h"
#include "CIFCRender.h"
#include "ConvexHull.h"
#include "resource.h"//���ϲ�����ά��ʾ��Ϣ

//
//global parameter
//
float	* pVerticesDeviceBuffer, counter = 0;
const float kCamMoveAmt = 0.002f; // Amount to move camera by
const float kMaxAngle = 89.0f;
D3DXVECTOR3		m_vecOrigin;//���������ж���0 �� 0 �� 0~~~ɶ�ã�

CPos		gCubePos(0 , 0 , 0);	// Position of cube in the world
int		iZoomMouseX, iZoomMouseY;

ID3DXMesh* manMesh = 0;
std::vector<D3DMATERIAL9> manMtrl(0);
std::vector<IDirect3DTexture9*> manTexture(0);

ID3DXMesh* manMesh1 = 0;
std::vector<D3DMATERIAL9> manMtrl1(0);
std::vector<IDirect3DTexture9*> manTexture1(0);


std::vector<D3DXVECTOR3> floorCoord(0);
std::vector<int>start(0);
std::vector<int>num(0);
int cc = 0;
ID3DXMesh* ballMesh = 0;
std::vector<D3DMATERIAL9> ballMtrl(0);
std::vector<IDirect3DTexture9*> ballTexture(0);
int mark = 0;


//��պ��ӵĶ����ʽ
struct SkyVertex
{
	SkyVertex(){}
	SkyVertex(float x , float y , float z , float w ,
		float u , float v)
	{
		_x = x ; _y = y ; _z = z ; _w = w ;
		_u = u ; _v = v ;
	}
	float  _x , _y , _z , _w;
	float _u , _v;

	static const DWORD SKYFVF;
};
const DWORD SkyVertex::SKYFVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

//��Ҫ��һ���ͼ���ڵľ�����������ͼ���ڵĴ�С�����Ե���Ϣ
CIFCRender::CIFCRender(HWND RenderHwnd)
{
	initialized = false;//������ʱ�����һ��
	DirectXStatus = 0;
	IsVBufferInit = false;
	IsCoordVBufferInit = false;
	RenderStatus =false;
	IsSkyBoxVBufferInit = false;
	IsWorldCenter = false;
	IsPath = false;
	grid_size = 100;
	//stepCounter = 0;
	IsSlabBufferInit = false;


	//Device setup
	if	(!initialized)
	{
		g_pD3D       = NULL;
		g_pd3dDevice = NULL;
		g_pVB        = NULL;
		g_pCoordVB = NULL;
		g_pSkyVB = NULL;
		g_pSkyTex = NULL;
		g_pWorldCenter = NULL;
		BackColor = D3DCOLOR_XRGB(255 , 255 , 255);
		TraceTime = 0.0f;
		g_pPathVB = NULL;
		g_pSlabVB = NULL;
		counter = 0;

		//GetParentFrame()->RecalcLayout();
		//ResizeParentToFit();
		/*
		�Ӿ����ָ��           
			CWnd*   pWnd=CWnd::FromHandle(hWnd);     //a   temporary   CWnd   object   is   created and   attached.     
			pWnd->Attach(hWnd);       //Attaches   a   Windows   window   to   a   CWnd   object   
		��ָ�뵽���   
			HWND   hWnd=pWnd->GetSafeHandle();   
			hWnd=pWnd->m_hWnd;
		*/
		m_hwndRenderWindow = RenderHwnd;
		//�õ���ʾ����Ĵ�С
		//this->GetWindowRect( &rc );
		CRect rc;
		//CWnd* pGroup = CWnd::FromHandle(m_hwndRenderWindow);
		//pGroup->Attach(m_hwndRenderWindow);
		//pGroup->GetWindowRect(&rc);
		GetWindowRect(m_hwndRenderWindow , &rc);
		m_iWidth = rc.Width();
		m_iHeight = rc.Height();

		//SetWindowPos(m_hwndRenderWindow , NULL , 0 , 0 , m_iWidth , m_iHeight , SWP_NOZORDER);

		InitializeDevice();
	}
	//ifc data
	ifcProject = NULL;
}

CIFCRender::~CIFCRender()
{
	//����Ҫ���Ƕ�ifcProject�����ͷ�
	//if (ifcProject)
	//{
	//	delete ifcProject;
	//	ifcProject = NULL;
	//}

	//�Խӿڵ��ͷ�
	// Release the Buffer
	if( g_pVB != NULL ) 
	{
		if( FAILED( g_pVB->Release() ) )
		{
			DirectXStatus = -1;
			return ;
		}
	}
	if (g_pCoordVB != NULL)
	{
		if(FAILED(g_pCoordVB->Release()))
		{
			DirectXStatus = -1;
			return ;
		}
	}
	if (g_pSkyVB != NULL)
	{
		if (FAILED(g_pSkyVB->Release()))
		{
			DirectXStatus = -1;
			return ;
		}
	}
	if (g_pSkyTex != NULL)
	{
		if (FAILED(g_pSkyTex->Release()))
		{
			DirectXStatus = -1;
			return ;
		}
	}
	if (g_pWorldCenter != NULL)
	{
		if (FAILED(g_pWorldCenter->Release()))
		{
			DirectXStatus = -1;
			return;
		}
	}

	if (g_pSlabVB != NULL)
	{
		if(FAILED(g_pSlabVB->Release()))
		{
			DirectXStatus = -1;
			return ;
		}
	}
	/*if (g_pPathVB != NULL)
	{
		if (FAILED(g_pPathVB->Release()))
		{
			DirectXStatus = -1;
			return;
		}
	}*/
}

void CIFCRender::InitializeDevice()
{
	if	(!DirectXStatus)
	{
		// Reset the Buffer
		if( g_pVB != NULL ) 
		{
			if( FAILED( g_pVB->Release() ) )
			{
				DirectXStatus = -1;
				return ;
			}
		}

		if (g_pSlabVB != NULL)
		{
			if(FAILED(g_pSlabVB->Release()))
			{
				DirectXStatus = -1;
				return ;
			}
		}

		if (g_pCoordVB != NULL)
		{
			if(FAILED(g_pCoordVB->Release()))
			{
				DirectXStatus = -1;
				return ;
			}
		}
		if (g_pSkyVB != NULL)
		{
			if (FAILED(g_pSkyVB->Release()))
			{
				DirectXStatus = -1;
				return ;
			}
		}
		if (g_pSkyTex != NULL)
		{
			if (FAILED(g_pSkyTex->Release()))
			{
				DirectXStatus = -1;
				return ;
			}
		}
		if (g_pWorldCenter != NULL)
		{
			if (FAILED(g_pWorldCenter->Release()))
			{
				DirectXStatus = -1;
				return;
			}
		}
		/*if (g_pPathVB != NULL)
		{
			if (FAILED(g_pPathVB->Release()))
			{
				DirectXStatus = -1;
				return;
			}
		}*/

		// Reset the Device
		if( g_pd3dDevice != NULL ) 
		{
			if( FAILED( g_pd3dDevice->Release() ) ) 
			{
				DirectXStatus = -1;
				return;
			}
		}

		if( g_pD3D != NULL )
		{
			if( FAILED( g_pD3D->Release() ) )
			{
				DirectXStatus = -1;
				return;
			}
		}

		// Create the D3D object.
		if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) 
		{
			return;
		}

		//
		//	Specific for DirectX 9.0
		//

		D3DDISPLAYMODE d3ddm;
		if	(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		{
			DirectXStatus = -1;
			return;
		}

		ZeroMemory( &d3dpp, sizeof(d3dpp) );
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		// Create the D3DDevice
		if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwndRenderWindow,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp, &g_pd3dDevice ) ) )
		{
			if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwndRenderWindow,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pd3dDevice ) ) )
			{
				DirectXStatus = -1;
				return;
			}
		}

		// Turn off culling , ifcEngine����ȡ���ݵ�ʱ����������������⣬���������޳�����
		if( FAILED( g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE/*D3DCULL_CCW*/ ) ) ) 
		{
			DirectXStatus = -1;
			return;
		}

		// Turn on the zbuffer
		if( FAILED( g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE ) ) ) 
		{
			DirectXStatus = -1;
			return;
		}
		initialized = true;//��ʶ�豸��ʼ���ɹ�
	}
}

void CIFCRender::InitializeDeviceBuffer()
{
	//��ʼ���豸����,��������ʽ��λ�����ꡢ�������ꡢ��������
	CIFCProject *IFCProject = ifcProject;
	if (IFCProject)
	{
		if (IFCProject->GetNumVertices())
		{
			if (!DirectXStatus)
			{
				if( FAILED( g_pd3dDevice->CreateVertexBuffer( IFCProject->GetNumIndices() * sizeof(CUSTOMVERTEX),
					0, D3DFVF_CUSTOMVERTEX , D3DPOOL_DEFAULT, &g_pVB ,NULL) ) )
				{
					DirectXStatus = -1;
					return;
				}

				if( FAILED( g_pVB->Lock( 0, 0, (void **)&pVerticesDeviceBuffer, 0 ) ) )
				{
					DirectXStatus = -1;
					return;
				}

				int i = 0;
				while  (i < IFCProject->GetNumIndices()) 
				{
					ASSERT(IFCProject->GetpIndices()[i] < IFCProject->GetNumVertices());
					memcpy(&(((CUSTOMVERTEX *) pVerticesDeviceBuffer)[i]), &(((CUSTOMVERTEX *) IFCProject->GetpVertices())[IFCProject->GetpIndices()[i]]), sizeof(CUSTOMVERTEX));

					D3DXVECTOR3 floor;
					floor.x = (((CUSTOMVERTEX *) pVerticesDeviceBuffer)[i]).x;
					floor.y = (((CUSTOMVERTEX *) pVerticesDeviceBuffer)[i]).y;
					floor.z = (((CUSTOMVERTEX *) pVerticesDeviceBuffer)[i]).z;
					floorCoord.push_back(floor);
					
					i++;
				}

				if	(FAILED( g_pVB->Unlock()))
				{
					DirectXStatus = -1;
					return;
				}
			}
		}
		//ģ�����ݶ��㻺�湤���Ѿ�����
		IsVBufferInit = true;
	}
	//////////////////////////////////////////////////////////////////////////��ʼ�������ᶥ�㻺��
	if(!DirectXStatus)
	{
		if (FAILED(g_pd3dDevice->CreateVertexBuffer(6 * sizeof(CUSTOMVERTEX) , 0 , 
			D3DFVF_CUSTOMVERTEX , D3DPOOL_DEFAULT , &g_pCoordVB , NULL)))
		{
			DirectXStatus = -1;
			return;
		}
		if (FAILED(g_pCoordVB->Lock(0 , 0 , (void **)&pVerticesDeviceBuffer , 0)))
		{
			DirectXStatus = -1;
			return ;
		}
		CUSTOMVERTEX Vertex[] = 
		{
			{0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f} , 
			{1.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f} , 

			{0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f} , 
			{0.0f , 3.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f} , 

			{0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f} , 
			{0.0f , 0.0f , 3.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f} ,
		};
		memcpy(pVerticesDeviceBuffer , Vertex , sizeof(CUSTOMVERTEX) * 6);
/*
		pVerticesDeviceBuffer[0] = (0 , 0 , 0 , 0 , 0 , 0 , 0 , 0);		//ԭ��
		pVerticesDeviceBuffer[1] = (1000.0f , 0 , 0 , 0 , 0 , 0 , 0 , 0);	//X

		pVerticesDeviceBuffer[2] = (0 , 0 , 0 , 0 , 0 , 0 , 0 , 0);		//ԭ��
		pVerticesDeviceBuffer[3] = (0 , 1000.0f , 0 , 0 , 0 , 0 , 0 , 0);	//Y

		pVerticesDeviceBuffer[4] = (0 , 0 , 0 , 0 , 0 , 0 , 0 , 0);		//ԭ��
		pVerticesDeviceBuffer[5] = (0 , 0 , 1000.0f , 0 , 0 , 0 , 0 , 0);	//Z
*/
		if (FAILED(g_pCoordVB->Unlock()))
		{
			DirectXStatus = -1;
			return ;
		}
		IsCoordVBufferInit = true;
	}
	//////////////////////////////////////////////////////////////////////////��ʼ����պ��Ӷ��㻺��
	if (!DirectXStatus)
	{
		if (FAILED(g_pd3dDevice->CreateVertexBuffer(6 * sizeof(SkyVertex) , D3DUSAGE_WRITEONLY , 
			SkyVertex::SKYFVF , D3DPOOL_MANAGED , &g_pSkyVB , NULL)))
		{
			DirectXStatus = -1;
			return ;
		}

		SkyVertex * SV;
		if (FAILED(g_pSkyVB->Lock(0 , 0 , (void**)&SV , 0)))
		{
			DirectXStatus = -1;
			return ;
		}

		SV[0] = SkyVertex(0.0f , 0.0f , 1.0f , 0.0f , 0.0f , 0.0f);
		SV[1] = SkyVertex(100.0f , 0.0f , 1.0f , 0.0f , 1.0f , 0.0f);
		SV[2] = SkyVertex(0.0f , 100.0f , 1.0f , 0.0f , 0.0f , 1.0f);

		SV[3] = SkyVertex(100.0f , 0.0f , 1.0f , 0.0f , 1.0f , 0.0f);
		SV[4] = SkyVertex(100.0f , 100.0f , 1.0f , 0.0f , 1.0f , 1.0f);
		SV[5] = SkyVertex(0.0f , 100.0f , 1.0f , 0.0f , 0.0f , 1.0f);

		if (FAILED(g_pSkyVB->Unlock()))
		{
			DirectXStatus = -1;
			return ;
		}
		IsSkyBoxVBufferInit = true;
	}
	//////////////////////////////////////////////////////////////////////////��ʼ�������������ĵ�����ģ��
	if(!DirectXStatus)
	{
		if (FAILED(D3DXCreateSphere(g_pd3dDevice , 0.05f , 20 , 20 , &g_pWorldCenter , 0)))
		{
			DirectXStatus = -1;
			return ;
		}
		IsWorldCenter = true;
	}

	CUSTOMVERTEX Vertex[24];
	if(!DirectXStatus)
	{
		if (FAILED(g_pd3dDevice->CreateVertexBuffer(2*6 * sizeof(CUSTOMVERTEX) , 0 , 
			D3DFVF_CUSTOMVERTEX , D3DPOOL_DEFAULT , &g_pSlabVB , NULL)))
		{
			DirectXStatus = -1;
			return;
		}
		if (FAILED(g_pSlabVB->Lock(0 , 0 , (void **)&pVerticesDeviceBuffer , 0)))
		{
			DirectXStatus = -1;
			return ;
		}
		int cc = 0;
		//Vertex[cc].x =-0.86776;
		Vertex[cc].x =-1.4;
		Vertex[cc].y =1;
		//Vertex[cc].z =-0.514906;
		Vertex[cc].z =-0.114906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//Vertex[cc].x =0.86776;
		Vertex[cc].x =1.4;
		Vertex[cc].y =1;
		//Vertex[cc].z =-0.514906f;
		Vertex[cc].z =-0.114906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//Vertex[cc].x =0.86776;
		Vertex[cc].x =1.4;
		Vertex[cc].y =-1;
		//Vertex[cc].z =-0.514906f;
		Vertex[cc].z =-0.114906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//Vertex[cc].x =-0.86776;
		Vertex[cc].x =-1.4;
		Vertex[cc].y =1;
		//Vertex[cc].z =-0.514906f;
		Vertex[cc].z =-0.114906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//Vertex[cc].x =0.86776;
		Vertex[cc].x =1.4;
		Vertex[cc].y =-1;
		//Vertex[cc].z =-0.514906f;
		Vertex[cc].z =-0.114906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//Vertex[cc].x =-0.86776;
		Vertex[cc].x =-1.4;
		Vertex[cc].y =-1;
		//Vertex[cc].z =-0.514906f;
		Vertex[cc].z =-0.114906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//Vertex[cc].x =-0.86776;
		Vertex[cc].x =-1.4;
		Vertex[cc].y =1;
		//Vertex[cc].z =-0.525396;
		Vertex[cc].z =-0.124906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//Vertex[cc].x =0.86776;
		Vertex[cc].x =1.4;
		Vertex[cc].y =-1;
		//Vertex[cc].z =-0.525396f;
		Vertex[cc].z =-0.124906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//Vertex[cc].x =0.86776;
		Vertex[cc].x =1.4;
		Vertex[cc].y =1;
		//Vertex[cc].z =-0.525396f;
		Vertex[cc].z =-0.124906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//Vertex[cc].x =-0.86776;
		Vertex[cc].x =-1.4;
		Vertex[cc].y =1;
		//Vertex[cc].z =-0.525396f;
		Vertex[cc].z =-0.124906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//Vertex[cc].x =-0.86776;
		Vertex[cc].x =-1.4;
		Vertex[cc].y =-1;
		//Vertex[cc].z =-0.525396f;
		Vertex[cc].z =-0.124906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//Vertex[cc].x =0.86776;
		Vertex[cc].x =1.4;
		Vertex[cc].y =-1;
		//Vertex[cc].z =-0.525396f;
		Vertex[cc].z =-0.124906;
		Vertex[cc].nx =0.0f;
		Vertex[cc].ny =0.0f;
		Vertex[cc].nz =0.0f;
		Vertex[cc].tu =0.0f;
		Vertex[cc].tv =0.0f;
		cc++;

		//blackNum = cc;
		memcpy(pVerticesDeviceBuffer , Vertex , sizeof(CUSTOMVERTEX) * cc);

		if (FAILED(g_pSlabVB->Unlock()))
		{
			DirectXStatus = -1;
			return ;
		}
		IsSlabBufferInit = true;
	}
	//////////////////////////////////////////////////////////////////////////��ʼ��·������
	//CalculatePath();
	/*if (IsPath)
	{
		if(!DirectXStatus)
		{
			if(FAILED( g_pd3dDevice->CreateVertexBuffer(sizeof(CUSTOMVERTEX),
				0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pPathVB, NULL) ) )
			{
				DirectXStatus = -1;
				return;
			}
		}
		
				
	}*/
}

int CIFCRender::SetupMatrices()
{
	// For our world matrix, we will just leave it as the identity
	//��֤��ÿ�λ���ģ�͵�ʱ���ǳ�����������ϵ������
	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity( &matWorld );

	matWorld._22 = -1.0f;

	D3DXVec3TransformCoord((D3DXVECTOR3 *) &matWorld._41, &m_vecOrigin, &matWorld);

	if( FAILED( g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld ) ) ) 
	{
		DirectXStatus = -1;
		return	1;
	}

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to look at, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.

	//D3DXVECTOR3 vEyePt(2.0f, 3 * ((float) sin(counter)), 4 * ((float) cos(counter)));
	//D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	//D3DXMATRIX matView;
	//D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	//if( FAILED( g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView ) ) ) 
	//{
	//	DirectXStatus = -1;
	//	return	1;
	//}

	//camera�����������Ӧ���Ժ�����ι���
	// Create "D3D Vector" versions of our camera's eye, look at position, and up vector
	D3DXMATRIXA16 matrix;
	D3DXVECTOR3 eye(gCam->getEye().z, gCam->getEye().x + 0.0, gCam->getEye().y + 0.0);
	D3DXVECTOR3 lookAt(gCam->getTarget().z, gCam->getTarget().x, gCam->getTarget().y);
	D3DXVECTOR3 up(0, 0, 1); // The world's up vector

	D3DXMatrixLookAtLH(&matrix, &eye, &lookAt, &up);
	if( FAILED( g_pd3dDevice->SetTransform( D3DTS_VIEW, &matrix ) ) ) 
	{
		DirectXStatus = -1;
	}

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4,  (float) m_iWidth/(float) m_iHeight, 1.0f, 1000.0f );
	if( FAILED( g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj ) ) ) 
	{
		DirectXStatus = -1;
		return	1;
	}

	return	0;
}

int CIFCRender::SetupMaterial()
{
	//initalize mapping
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCWALL") , 0));								//ǽ
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCWALLSTANDARDCASE") , 1));	//��׼ǽ
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCWINDOW") , 62));						//��
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCDOOR") , 3));							//��
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCBEAM") , 4));								//��
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCCOLUMN") , 5));						//����
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCCURTAINWALL") , 6));				//Ļǽ
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCPILE") , 7));								//�ܵ�
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCPLATE") , 8));							//��
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCRAILING") , 9));							//����
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCRAMP") , 1));							//�µ�
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCROOF") , 7));							//�ݶ�11
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSITE") , 12));								//����
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSLAB") , 13));							//��
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTAIR") , 14));							//¥��

	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCANNOTATION") , 15));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCBUILDING") , 16));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCBUILDINGELEMENTPART") , 17));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCBUILDINGELEMENTPROXY") , 18));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCBUILDINGSTOREY") , 1));

	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCCHAMFEREDGEFEATURE") , 20));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCCOVERING") , 21));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCDISCRETEACCESSORY") , 22));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCDISTRIBUTIONCHAMBERELEMENT") , 23));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCDISTRIBUTIONCONTROLELEMENT") , 24));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCDISTRIBUTIONELEMENT"), 25));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCDISTRIBUTIONFLOWELEMENT"), 26));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCDISTRIBUTIONPORT") , 27));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCELECTRICALELEMENT") , 28));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCELECTRICDISTRIBUTIONPOINT") , 29));

	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALPLANARACTION") , 30));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALPLANARACTIONVARYING") , 31));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALPOINTACTION") , 32));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALPOINTCONNECTION") , 33));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALPOINTREACTION") , 34));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALSURFACECONNECTION") , 35));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALSURFACEMEMBER") , 36));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALSURFACEMEMBERVARYING") , 37));
	EntitytoMtrl.insert(std::pair<CString,int>(__T("IFCTENDON") , 38));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCTENDONANCHOR"), 39));

	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCELEMENTASSEMBLY") , 40));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCENERGYCONVERSIONDEVICE") , 41));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCEQUIPMENTELEMENT") , 42));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCFASTENER") , 43));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCFLOWCONTROLLER") , 44));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCFLOWFITTING") , 45));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCFLOWMOVINGDEVICE") , 46));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCFLOWSEGMENT") , 47));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCFLOWSTORAGEDEVICE") , 48));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCFLOWTERMINAL") , 49));

	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCFLOWTREATMENTDEVICE") , 50));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCFOOTING") , 61));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCFURNISHINGELEMENT") , 62));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCGRID") , 63));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCMECHANICALFASTENER") , 64));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCMEMBER") , 65));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCOPENINGELEMENT") , 66));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCPROJECTIONELEMENT") , 67));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCPROXY") , 68));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCRAMPFLIGHT") , 1));

	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCREINFORCINGBAR"), 70));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCREINFORCINGMESH") , 71));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCROUNDEDEDGEFEATURE") , 72));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSPACE") , 73));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTAIRFLIGHT") , 74));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALCURVECONNECTION") , 75));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALCURVEMEMBER") , 76));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALCURVEMEMBERVARYING") , 77));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCSTRUCTURALLINEARACTION"), 78));
	EntitytoMtrl.insert(std::pair<CString,int>(__T("IFCSTRUCTURALLINEARACTIONVARYING") , 79));

	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCTRANSPORTELEMENT") , 80));
	EntitytoMtrl.insert(std::pair<CString,int>(_T("IFCVIRTUALELEMENT") , 81));
	//set material up
	//////////////////////////////////////////////////////////////////////////ǽ
	mtrl[0].Ambient.r = 0.8790625f;
	mtrl[0].Ambient.g = 0.8790625f;
	mtrl[0].Ambient.b = 0.0790625f;
	mtrl[0].Ambient.a = 0.2f;

	mtrl[0].Diffuse.r = 0.8790625f;
	mtrl[0].Diffuse.g = 0.0790625f;
	mtrl[0].Diffuse.b = 0.0790625f;
	mtrl[0].Diffuse.a = 0.2f;

	mtrl[0].Emissive.r = 0.0790625f;
	mtrl[0].Emissive.g = 0.0790625f;
	mtrl[0].Emissive.b = 0.8790625f;
	mtrl[0].Emissive.a = 0.0790625f;

	mtrl[0].Specular.r = 0.0790625f;
	mtrl[0].Specular.g = 0.8790625f;
	mtrl[0].Specular.b = 0.0790625f;
	mtrl[0].Specular.a = 0.0f;
	//////////////////////////////////////////////////////////////////////////��׼ǽ
	mtrl[1].Ambient.r = 0.0f;
	mtrl[1].Ambient.g = 0.0f;
	mtrl[1].Ambient.b = 0.0f;
	mtrl[1].Ambient.a = 0.4f;

	mtrl[1].Diffuse.r = 0.53137f;
	mtrl[1].Diffuse.g = 0.53137f;
	mtrl[1].Diffuse.b = 0.53137f;
	mtrl[1].Diffuse.a = 0.4f;

	/*	mtrl[1].Emissive.r = 0.0f;
	mtrl[1].Emissive.g = 0.0f;
	mtrl[1].Emissive.b = 0.0f;
	mtrl[1].Emissive.a = 0.0f;
	*/
	mtrl[1].Emissive.r = 0.3843137f;
	mtrl[1].Emissive.g = 0.3843137f;
	mtrl[1].Emissive.b = 0.3843137f;
	mtrl[1].Emissive.a = 0.4f;
	//0.8790625f;

	mtrl[1].Specular.r = 0.0f;
	mtrl[1].Specular.g = 0.0f;
	mtrl[1].Specular.b = 0.0f;
	mtrl[1].Specular.a = 0.4f;
	//////////////////////////////////////////////////////////////////////////��
	mtrl[2].Ambient.r = 0.0f;
	mtrl[2].Ambient.g = 0.80f;
	mtrl[2].Ambient.b = 0.0f;
	mtrl[2].Ambient.a = 0.4f;

	mtrl[2].Diffuse.r = 0.4392157f;
	mtrl[2].Diffuse.g = 0.8627451f;
	mtrl[2].Diffuse.b = 0.8627451f;
	mtrl[2].Diffuse.a = 0.4f;

	mtrl[2].Emissive.r = 0.0392157f;
	mtrl[2].Emissive.g = 0.0627451f;
	mtrl[2].Emissive.b = 0.0627451f;
	mtrl[2].Emissive.a = 0.4f;

	mtrl[2].Specular.r = 0.4392157f;
	mtrl[2].Specular.g = 0.8627451f;
	mtrl[2].Specular.b = 0.8627451f;
	mtrl[2].Specular.a = 0.6f;
	//////////////////////////////////////////////////////////////////////////��
	mtrl[3].Ambient.r = 0.0f;
	mtrl[3].Ambient.g = 0.0f;
	mtrl[3].Ambient.b = 0.0f;
	mtrl[3].Ambient.a = 0.2f;

	mtrl[3].Diffuse.r = 0.2f;
	mtrl[3].Diffuse.g = 0.2f;
	mtrl[3].Diffuse.b = 1.0f;
	mtrl[3].Diffuse.a = 1.0f;

	mtrl[3].Emissive.r = 0.2f;
	mtrl[3].Emissive.g = 0.2f;
	mtrl[3].Emissive.b = 1.0f;
	mtrl[3].Emissive.a = 1.0f;

	mtrl[3].Specular.r = 0.584314f;
	mtrl[3].Specular.g = 0.084314f;
	mtrl[3].Specular.b = 0.305882f;
	mtrl[3].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////��
	mtrl[4].Ambient.r = 0.831373f;
	mtrl[4].Ambient.g = 0.858824f;
	mtrl[4].Ambient.b = 0.8f;
	mtrl[4].Ambient.a = 0.2f;

	mtrl[4].Diffuse.r = 0.831373f;
	mtrl[4].Diffuse.g = 0.858824f;
	mtrl[4].Diffuse.b = 0.8f;
	mtrl[4].Diffuse.a = 0.2f;

	mtrl[4].Emissive.r = 0.75f;
	mtrl[4].Emissive.g = 0.45f;
	mtrl[4].Emissive.b = 0.23f;
	mtrl[4].Emissive.a = 1.0f;

	mtrl[4].Specular.r = 0.898039f;
	mtrl[4].Specular.g = 0.780392f;
	mtrl[4].Specular.b = 0.52549f;
	mtrl[4].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////����
	mtrl[5].Ambient.r = 0.780392f;
	mtrl[5].Ambient.g = 0.27451f;
	mtrl[5].Ambient.b = 0.176471f;
	mtrl[5].Ambient.a = 0.2f;

	mtrl[5].Diffuse.r = 0.780392f;
	mtrl[5].Diffuse.g = 0.27451f;
	mtrl[5].Diffuse.b = 0.176471f;
	mtrl[5].Diffuse.a = 0.2f;

	mtrl[5].Emissive.r = 0.0f;
	mtrl[5].Emissive.g = 0.6313725f;
	mtrl[5].Emissive.b = 0.0784314f;
	mtrl[5].Emissive.a = 1.0f;

	mtrl[5].Specular.r = 0.941176f;
	mtrl[5].Specular.g = 0.615686f;
	mtrl[5].Specular.b = 0.207843f;
	mtrl[5].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////Ļǽ
	mtrl[6].Ambient.r = 0.505882f;
	mtrl[6].Ambient.g = 0.721569f;
	mtrl[6].Ambient.b = 0.176471f;
	mtrl[6].Ambient.a = 0.2f;

	mtrl[6].Diffuse.r = 0.805882f;
	mtrl[6].Diffuse.g = 0.121569f;
	mtrl[6].Diffuse.b = 0.476471f;
	mtrl[6].Diffuse.a = 0.2f;

	mtrl[6].Emissive.r = 0.54f;
	mtrl[6].Emissive.g = 0.1f;
	mtrl[6].Emissive.b = 0.9f;
	mtrl[6].Emissive.a = 0.5f;

	mtrl[6].Specular.r = 0.40f;
	mtrl[6].Specular.g = 0.05f;
	mtrl[6].Specular.b = 0.60f;
	mtrl[6].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////�ܵ�
	mtrl[7].Ambient.r = 0.835294f;
	mtrl[7].Ambient.g = 0.772549f;
	mtrl[7].Ambient.b = 0.12549f;
	mtrl[7].Ambient.a = 0.2f;

	mtrl[7].Diffuse.r = 0.835294f;
	mtrl[7].Diffuse.g = 0.772549f;
	mtrl[7].Diffuse.b = 0.12549f;
	mtrl[7].Diffuse.a = 0.2f;

	mtrl[7].Emissive.r = 0.66f;
	mtrl[7].Emissive.g = 0.9f;
	mtrl[7].Emissive.b = 0.2f;
	mtrl[7].Emissive.a = 1.0f;

	mtrl[7].Specular.r = 0.894118f;
	mtrl[7].Specular.g = 0.372549f;
	mtrl[7].Specular.b = 0.231373f;
	mtrl[7].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////��
	mtrl[8].Ambient.r = 0.0f;
	mtrl[8].Ambient.g = 0.0f;
	mtrl[8].Ambient.b = 0.0f;
	mtrl[8].Ambient.a = 0.6f;

	mtrl[8].Diffuse.r = 0.4392157f;
	mtrl[8].Diffuse.g = 0.8627451f;
	mtrl[8].Diffuse.b = 0.8627451f;
	mtrl[8].Diffuse.a = 0.6f;

	mtrl[8].Emissive.r = 0.0392157f;
	mtrl[8].Emissive.g = 0.0627451f;
	mtrl[8].Emissive.b = 0.0627451f;
	mtrl[8].Emissive.a = 0.6f;

	mtrl[8].Specular.r = 0.4392157f;
	mtrl[8].Specular.g = 0.8627451f;
	mtrl[8].Specular.b = 0.8627451f;
	mtrl[8].Specular.a = 0.6f;
	//////////////////////////////////////////////////////////////////////////����
	mtrl[9].Diffuse.r = mtrl[9].Ambient.r = 0.1f;
	mtrl[9].Diffuse.g = mtrl[9].Ambient.g = 0.1f;
	mtrl[9].Diffuse.b = mtrl[9].Ambient.b = 0.1f;
	mtrl[9].Diffuse.a = mtrl[9].Ambient.a = 0.07f;

	mtrl[9].Emissive.r = 0.0f;
	mtrl[9].Emissive.g = 0.7882353f;
	mtrl[9].Emissive.b = 0.0f;
	mtrl[9].Emissive.a = 0.07f;
	//////////////////////////////////////////////////////////////////////////�µ�
	mtrl[10].Diffuse.r = mtrl[10].Ambient.r = 0.1f;
	mtrl[10].Diffuse.g = mtrl[10].Ambient.g = 0.1f;
	mtrl[10].Diffuse.b = mtrl[10].Ambient.b = 0.1f;
	mtrl[10].Diffuse.a = mtrl[10].Ambient.a = 0.07f;

	mtrl[10].Emissive.r = 0.85f;
	mtrl[10].Emissive.g = 0.3f;
	mtrl[10].Emissive.b = 0.3f;
	mtrl[10].Emissive.a = 1.0f;

	mtrl[10].Specular.r = 0.1976471f;
	mtrl[10].Specular.g = 0.780392f;
	mtrl[10].Specular.b = 0.270588f;
	mtrl[10].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////�ݶ�
	mtrl[11].Ambient.r = 0.988235f;
	mtrl[11].Ambient.g = 0.068627f;
	mtrl[11].Ambient.b = 0.02549f;
	mtrl[11].Ambient.a = 0.2f;

	mtrl[11].Diffuse.r = 0.988235f;
	mtrl[11].Diffuse.g = 0.068627f;
	mtrl[11].Diffuse.b = 0.92549f;
	mtrl[11].Diffuse.a = 0.2f;

	mtrl[11].Emissive.r = 0.08f;
	mtrl[11].Emissive.g = 0.56f;
	mtrl[11].Emissive.b = 0.6f;
	mtrl[11].Emissive.a = 1.0f;

	mtrl[11].Specular.r = 0.284314f;
	mtrl[11].Specular.g = 0.484314f;
	mtrl[11].Specular.b = 0.305882f;
	mtrl[11].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////����
	mtrl[12].Diffuse.r = mtrl[12].Ambient.r = 0.1f;
	mtrl[12].Diffuse.g = mtrl[12].Ambient.g = 0.1f;
	mtrl[12].Diffuse.b = mtrl[12].Ambient.b = 0.1f;
	mtrl[12].Diffuse.a = mtrl[12].Ambient.a = 0.07f;

	mtrl[12].Emissive.r = 0.85f;
	mtrl[12].Emissive.g = 0.3f;
	mtrl[12].Emissive.b = 0.3f;
	mtrl[12].Emissive.a = 1.0f;

	mtrl[12].Specular.r = 0.1976471f;
	mtrl[12].Specular.g = 0.780392f;
	mtrl[12].Specular.b = 0.270588f;
	mtrl[12].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////��(floor ,roof)
	mtrl[13].Ambient.r = 0.0f;
	mtrl[13].Ambient.g = 0.0f;
	mtrl[13].Ambient.b = 0.0f;
	mtrl[13].Ambient.a = 0.9f;

	mtrl[13].Diffuse.r = 0.235294f;
	mtrl[13].Diffuse.g = 0.472549f;
	mtrl[13].Diffuse.b = 0.12549f;
	mtrl[13].Diffuse.a = 0.9f;

	mtrl[13].Emissive.r = 0.1568627f;
	mtrl[13].Emissive.g = 0.2705882f;
	mtrl[13].Emissive.b = 0.1176471f;
	mtrl[13].Emissive.a = 0.9f;

	mtrl[13].Specular.r = 0.694118f;
	mtrl[13].Specular.g = 0.472549f;
	mtrl[13].Specular.b = 0.231373f;
	mtrl[13].Specular.a = 0.9f;
	//////////////////////////////////////////////////////////////////////////¥��
	mtrl[14].Diffuse.r = mtrl[14].Ambient.r = 0.1f;
	mtrl[14].Diffuse.g = mtrl[14].Ambient.g = 0.1f;
	mtrl[14].Diffuse.b = mtrl[14].Ambient.b = 0.1f;
	mtrl[14].Diffuse.a = mtrl[14].Ambient.a = 0.07f;

	mtrl[14].Emissive.r = 0.85f;
	mtrl[14].Emissive.g = 0.3f;
	mtrl[14].Emissive.b = 0.3f;
	mtrl[14].Emissive.a = 1.0f;

	mtrl[14].Specular.r = 0.1976471f;
	mtrl[14].Specular.g = 0.780392f;
	mtrl[14].Specular.b = 0.270588f;
	mtrl[14].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////���������磺ϴ�ֳص�
	mtrl[18].Ambient.r = 0.0f;
	mtrl[18].Ambient.g = 0.0f;
	mtrl[18].Ambient.b = 0.0f;
	mtrl[18].Ambient.a = 0.2f;

	mtrl[18].Diffuse.r = 0.9f;
	mtrl[18].Diffuse.g = 0.9f;
	mtrl[18].Diffuse.b = 0.9f;
	mtrl[18].Diffuse.a = 1.0f;

	mtrl[18].Emissive.r = 0.1f;
	mtrl[18].Emissive.g = 0.1f;
	mtrl[18].Emissive.b = 0.1f;
	mtrl[18].Emissive.a = 1.0f;

	mtrl[18].Specular.r = 0.694118f;
	mtrl[18].Specular.g = 0.472549f;
	mtrl[18].Specular.b = 0.231373f;
	mtrl[18].Specular.a = 1.0f;
	/////////////////////////////////////////////////////////////////////////22
	mtrl[22].Ambient.r = 0.0f;
	mtrl[22].Ambient.g = 0.0f;
	mtrl[22].Ambient.b = 0.0f;
	mtrl[22].Ambient.a = 0.4f;

	mtrl[22].Diffuse.r = 0.53137f;
	mtrl[22].Diffuse.g = 0.53137f;
	mtrl[22].Diffuse.b = 0.53137f;
	mtrl[22].Diffuse.a = 0.4f;

	/*	mtrl[1].Emissive.r = 0.0f;
	mtrl[1].Emissive.g = 0.0f;
	mtrl[1].Emissive.b = 0.0f;
	mtrl[1].Emissive.a = 0.0f;
	*/
	mtrl[22].Emissive.r = 0.3843137f;
	mtrl[2].Emissive.g = 0.3843137f;
	mtrl[22].Emissive.b = 0.3843137f;
	mtrl[22].Emissive.a = 0.4f;
	//0.8790625f;

	mtrl[22].Specular.r = 0.0f;
	mtrl[22].Specular.g = 0.0f;
	mtrl[22].Specular.b = 0.0f;
	mtrl[22].Specular.a = 0.4f;

	//////////////////////////////////////////////////////////////////////////�ռ�
	mtrl[73].Diffuse.r = mtrl[73].Ambient.r = 0.1f;
	mtrl[73].Diffuse.g = mtrl[73].Ambient.g = 0.1f;
	mtrl[73].Diffuse.b = mtrl[73].Ambient.b = 0.1f;
	mtrl[73].Diffuse.a = mtrl[73].Ambient.a = 0.07f;

	mtrl[73].Emissive.r = 0.9f;
	mtrl[73].Emissive.g = 0.0f;
	mtrl[73].Emissive.b = 0.0f;
	mtrl[73].Emissive.a = 1.0f;

	mtrl[73].Specular.r = 0.1976471f;
	mtrl[73].Specular.g = 0.780392f;
	mtrl[73].Specular.b = 0.270588f;
	mtrl[73].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////�Ҿ�
	mtrl[62].Ambient.r = 0.0f;
	mtrl[62].Ambient.g = 0.0f;
	mtrl[62].Ambient.b = 0.0f;
	mtrl[62].Ambient.a = 0.2f;

	mtrl[62].Diffuse.r = 0.9764706f;
	mtrl[62].Diffuse.g = 0.6313725f;
	mtrl[62].Diffuse.b = 0.0f;
	mtrl[62].Diffuse.a = 1.0f;

	mtrl[62].Emissive.r = 0.0568627f;
	mtrl[62].Emissive.g = 0.0705882f;
	mtrl[62].Emissive.b = 0.0176471f;
	mtrl[62].Emissive.a = 1.0f;

	mtrl[62].Specular.r = 0.694118f;
	mtrl[62].Specular.g = 0.472549f;
	mtrl[62].Specular.b = 0.231373f;
	mtrl[62].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////̨��
	mtrl[74].Ambient.r = 0.0f;
	mtrl[74].Ambient.g = 0.0f;
	mtrl[74].Ambient.b = 0.0f;
	mtrl[74].Ambient.a = 0.2f;

	mtrl[74].Diffuse.r = 0.9764706f;
	mtrl[74].Diffuse.g = 0.6313725f;
	mtrl[74].Diffuse.b = 0.0f;
	mtrl[74].Diffuse.a = 1.0f;

	mtrl[74].Emissive.r = 0.0568627f;
	mtrl[74].Emissive.g = 0.0705882f;
	mtrl[74].Emissive.b = 0.0176471f;
	mtrl[74].Emissive.a = 1.0f;

	mtrl[74].Specular.r = 0.694118f;
	mtrl[74].Specular.g = 0.472549f;
	mtrl[74].Specular.b = 0.231373f;
	mtrl[74].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////�ܹ�
	mtrl[47].Ambient.r = 0.0f;
	mtrl[47].Ambient.g = 0.0f;
	mtrl[47].Ambient.b = 0.0f;
	mtrl[47].Ambient.a = 0.2f;

	mtrl[47].Diffuse.r = 0.9764706f;
	mtrl[47].Diffuse.g = 0.6313725f;
	mtrl[47].Diffuse.b = 0.0f;
	mtrl[47].Diffuse.a = 1.0f;

	mtrl[47].Emissive.r = 0.0568627f;
	mtrl[47].Emissive.g = 0.0705882f;
	mtrl[47].Emissive.b = 0.0176471f;
	mtrl[47].Emissive.a = 1.0f;

	mtrl[47].Specular.r = 0.694118f;
	mtrl[47].Specular.g = 0.472549f;
	mtrl[47].Specular.b = 0.231373f;
	mtrl[47].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////Ĭ�ϲ���
	mtrl[99].Ambient.r = 1.0f;
	mtrl[99].Ambient.g = 1.0f;
	mtrl[99].Ambient.b = 1.0f;
	mtrl[99].Ambient.a = 1.2f;

	mtrl[99].Diffuse.r = 1.0f;
	mtrl[99].Diffuse.g = 1.0f;
	mtrl[99].Diffuse.b = 1.0f;
	mtrl[99].Diffuse.a = 1.0f;

	mtrl[99].Emissive.r = 0.0f;
	mtrl[99].Emissive.g = 0.0f;
	mtrl[99].Emissive.b = 0.0f;
	mtrl[99].Emissive.a = 0.0f;

	mtrl[99].Specular.r = 1.0f;
	mtrl[99].Specular.g = 1.0f;
	mtrl[99].Specular.b = 1.0f;
	mtrl[99].Specular.a = 1.0f;
	//////////////////////////////////////////////////////////////////////////
	return 1;
}

int CIFCRender::SetupLight(D3DXVECTOR3 OPosit , D3DXVECTOR3 ODir , D3DXCOLOR LColor , int Lindex)
{
	ZeroMemory(&Light[Lindex], sizeof(D3DLIGHT9));
	Light[Lindex].Type       = D3DLIGHT_DIRECTIONAL;

	Light[Lindex].Diffuse.r  = LColor.r;
	Light[Lindex].Diffuse.g  = LColor.g;
	Light[Lindex].Diffuse.b  = LColor.b;
	Light[Lindex].Diffuse.a  = LColor.a;
	Light[Lindex].Specular.r = LColor.r;
	Light[Lindex].Specular.g = LColor.g;
	Light[Lindex].Specular.b = LColor.b;
	Light[Lindex].Specular.a = LColor.a;
	Light[Lindex].Ambient.r  = LColor.r;
	Light[Lindex].Ambient.g  = LColor.g;
	Light[Lindex].Ambient.b  = LColor.b;
	Light[Lindex].Ambient.a  = LColor.a;

	Light[Lindex].Position.x = OPosit.x;
	Light[Lindex].Position.y = OPosit.y;
	Light[Lindex].Position.z = OPosit.z;
	Light[Lindex].Range       = 5.0f;
	
	LightDir[Lindex] = ODir;
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[Lindex].Direction, &ODir);//��һ�������ù��ճ���
	
	if	(FAILED(g_pd3dDevice->SetLight(Lindex, &Light[Lindex]))) 
	{
		DirectXStatus = -1;
		return	1;
	}
	if	(FAILED(g_pd3dDevice->LightEnable(Lindex, TRUE)))
	{
		DirectXStatus = -1;
		return	1;
	}
	if	(FAILED(g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		DirectXStatus = -1;
		return	1;
	}

	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS , true);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE , false);

	return 0;
}

//���¹�Դ����
void	CIFCRender::UpdataLight(int Lindex)
{
	//////////////////////////////////////////////////////////////////////////
	D3DXMATRIXA16 matrix;
	for (int i = 0 ; i <= Lindex ; ++i)
	{
		D3DXVECTOR3 eye(gCam->getEye().z, gCam->getEye().x + 0.0, gCam->getEye().y + 0.0);
		D3DXVECTOR3 lookAt(gCam->getTarget().z, gCam->getTarget().x, gCam->getTarget().y);
		D3DXVECTOR3 up(0, 0, 1); 
		D3DXMatrixLookAtLH(&matrix, &eye, &lookAt, &up);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&Light[i].Direction , &LightDir[i] , &matrix);
		//����Դ���������仯ʱ����Ҫ�µĲ�����ô�����������������
		if	(FAILED(g_pd3dDevice->SetLight(i , &Light[i]))) 
		{
			DirectXStatus = -1;
			return;
		}

		if	(FAILED(g_pd3dDevice->LightEnable(i , TRUE)))
		{
			DirectXStatus = -1;
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

//ԭʼ��Դ����
int CIFCRender::SetupLights()
{
	D3DXVECTOR3 vecDir;

	D3DLIGHT9 light0;
	ZeroMemory(&light0, sizeof(D3DLIGHT9));
	light0.Type       = D3DLIGHT_DIRECTIONAL;
	light0.Diffuse.r  = 1.0f;
	light0.Diffuse.g  = 1.0f;
	light0.Diffuse.b  = 1.0f;
	light0.Diffuse.a  = 0.4f;
	light0.Specular.r = 0.1f;
	light0.Specular.g = 0.1f;
	light0.Specular.b = 0.1f;
	light0.Specular.a = 0.5f;
	light0.Ambient.r  = 1.0f;
	light0.Ambient.g  = 1.0f;
	light0.Ambient.b  = 1.0f;
	light0.Ambient.a  = 1.0f;
	light0.Position.x = (float) 2.0f;
	light0.Position.y = (float) -2.0f;
	light0.Position.z = (float) -2.0f;
	light0.Range       = 5.0f;

	vecDir.x = 2.0f;
	vecDir.y = -6.0f;
	vecDir.z = -1.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&light0.Direction, &vecDir);//��һ�������ù��ճ���
	//////////////////////////////////////////////////////////////////////////
	D3DXMATRIXA16 matrix;

	D3DXVECTOR3 eye(gCam->getEye().z, gCam->getEye().x + 0.0, gCam->getEye().y + 0.0);
	D3DXVECTOR3 lookAt(gCam->getTarget().z, gCam->getTarget().x, gCam->getTarget().y);
	D3DXVECTOR3 up(0, 0, 1); 
	D3DXMatrixLookAtLH(&matrix, &eye, &lookAt, &up);
	D3DXVec3TransformCoord((D3DXVECTOR3*)&light0.Direction , &vecDir , &matrix);
	//////////////////////////////////////////////////////////////////////////

	if	(FAILED(g_pd3dDevice->SetLight(0, &light0))) 
	{
		DirectXStatus = -1;
		return	1;
	}

	if	(FAILED(g_pd3dDevice->LightEnable(0, TRUE)))
	{
		DirectXStatus = -1;
		return	1;
	}

	D3DLIGHT9 light1;
	ZeroMemory(&light1, sizeof(D3DLIGHT9));
	light1.Type       = D3DLIGHT_DIRECTIONAL;
	light1.Diffuse.r  = 1.0f;
	light1.Diffuse.g  = 1.0f;
	light1.Diffuse.b  = 1.0f;
	light1.Diffuse.a  = 1.0f;
	light1.Specular.r = 0.1f;
	light1.Specular.g = 0.1f;
	light1.Specular.b = 0.1f;
	light1.Specular.a = 0.5f;
	light1.Ambient.r  = 1.0f;
	light1.Ambient.g  = 1.0f;
	light1.Ambient.b  = 1.0f;
	light1.Ambient.a  = 1.0f;
	light1.Position.x = (float) -2.0f;
	light1.Position.y = (float) 2.0f;
	light1.Position.z = (float) 2.0f;
	light1.Range       = 5.0f;
	vecDir.x = -2.0f;
	vecDir.y = 6.0f;
	vecDir.z = 1.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&light1.Direction, &vecDir);
	//////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 eye1(gCam->getEye().z, gCam->getEye().x + 0.0, gCam->getEye().y + 0.0);
	D3DXVECTOR3 lookAt1(gCam->getTarget().z, gCam->getTarget().x, gCam->getTarget().y);
	D3DXVECTOR3 up1(0, 0, 1); 
	D3DXMatrixLookAtLH(&matrix, &eye1, &lookAt1, &up1);
	D3DXVec3TransformCoord((D3DXVECTOR3*)&light1.Direction , &vecDir , &matrix);
	//////////////////////////////////////////////////////////////////////////
	if	(FAILED(g_pd3dDevice->SetLight(1, &light1))) 
	{
		DirectXStatus = -1;
		return	1;
	}

	if	(FAILED(g_pd3dDevice->LightEnable(1, TRUE)))
	{
		DirectXStatus = -1;
		return	1;
	}

	if	(FAILED(g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		DirectXStatus = -1;
		return	1;
	}

	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS , true);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE , false);

	// Finally, turn on some ambient light.
	//g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00707070);
	//g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00000000);
	return	0;
}

//������Ⱦǰ����Ҫ����һ���״ֵ̬
bool CIFCRender::SetupRender(int Lindex)
{
	if (!DirectXStatus)
	{
		SetupMaterial();

		D3DXVECTOR3 Dir0(2.0f , 6.0f , 1.0f);
		D3DXVECTOR3 Dir1(-2.0f , -6.0f , -1.0f);
		D3DXVECTOR3 Pos0(2.0f , 2.0f , 2.0f);
		D3DXVECTOR3 Pos1(-2.0f , -2.0f , -2.0f);
		D3DXCOLOR	 LColor0(1.0f , 1.0f , 1.0f , 0.4f);
		D3DXCOLOR	 LColor1(1.0f , 1.0f , 1.0f , 0.4f);
		SetupLight(Pos0 , Dir0 , LColor0 , 0);
		SetupLight(Pos1 , Dir1 , LColor1 , 1);

		//���Ʊ���
		//if( FAILED( g_pd3dDevice->Clear(0 , NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ,
		//	BackColor, 1.0f, 0L ) ) ) 
		//{
		//	DirectXStatus = -1;
		//	return false;
		//}

		//������պ�������
		if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice , _T("Sky.jpg") , &g_pSkyTex)))
		{
			//::MessageBox(0 , _T("Create Texture Error!") , 0 , 0);
		}
		g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		RenderStatus = true;
	}
	return true;
}

void CIFCRender::Render()
{
	ofstream aa("E:\\doorName.txt");
	if	(!DirectXStatus)
	{
		// Clear the backbuffer and the zbuffer
		if( FAILED( g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER/* | D3DCLEAR_STENCIL */,
			BackColor , 1.0f , 0 ) ) ) 
		{
			DirectXStatus = -1;
			return;
		}

		// Begin the scene
		if	(SUCCEEDED(g_pd3dDevice->BeginScene()))
		{
			/*/ Setup the lights and materials
			if	(SetupLights())
			{
				DirectXStatus = -1;
				return;
			}
			*/
			//���¹��յ�λ����Ϣ
			UpdataLight(1);

			// Setup the world, view, and projection matrices
			if	(SetupMatrices())
			{
				DirectXStatus = -1;
				return;
			}

			//Setup Material
			//SetupMaterial();
			//////////////////////////////////////////////////////////////////////////SkyBox����
			if (!IsSkyBoxVBufferInit)
			{
				if (g_pd3dDevice->SetStreamSource(0 , g_pSkyVB , 0 , sizeof(SkyVertex)))
				{
					DirectXStatus = -1;
					return ;
				}
				if (g_pd3dDevice->SetFVF(SkyVertex::SKYFVF))
				{
					DirectXStatus = -1;
					return ;
				}
				//g_pd3dDevice->SetTexture(0 , g_pSkyTex);
				g_pd3dDevice->SetMaterial(&mtrl[99]);

				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST , 0 , 2);
			}

			if (IsSlabBufferInit)
			{
				if	(g_pd3dDevice->SetStreamSource(0 , g_pSlabVB , 0 , sizeof(CUSTOMVERTEX))) 
				{
					DirectXStatus = -1;
					return;
				}
				if	(g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX)) 
				{
					DirectXStatus = -1;
					return;
				}
				
				g_pd3dDevice->SetMaterial(&mtrl[1]);//13/7
				g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , true);
				g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA);
				g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
				//g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST , 0 , 4);
				g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , false);
			}

			

			//////////////////////////////////////////////////////////////////////////ģ�ͻ���
			if	(g_pd3dDevice->SetStreamSource(0 , g_pVB , 0 , sizeof(CUSTOMVERTEX))) 
			{
				DirectXStatus = -1;
				return;
			}
			if	(g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX)) 
			{
				DirectXStatus = -1;
				return;
			}
			//Render
			CIFCProject *IFCProject = ifcProject;
			if (IFCProject && IsVBufferInit)//ifcProject�൱�����������IsVBufferInit��ʶ�˶��㻺���Ƿ�������
			{
				const EntityGroup* pGroups;
				const InstanceGroup *pInstances;
				Entities_constIter pGroup;
				Instance_constIter pInstance;

				pGroups=IFCProject->GetEntityGroup();
				pGroup=pGroups->begin();
				//����
				for (;pGroup!=pGroups->end();++pGroup)
				{
					CIFCEntity ifcEntity=pGroup->second;
					bool check = false;
					mapiter = EntitytoMtrl.begin();
					for (; mapiter != EntitytoMtrl.end() ; ++mapiter)
					{
						if (ifcEntity.GetNameIFC() == mapiter->first)
						{
							g_pd3dDevice->SetMaterial(&mtrl[mapiter->second]);
							check = true;
							break;
						}
					}
					
					//�����ƿռ�Ԫ�ؼ�IFCOPENINGELEMENT
					if (ifcEntity.GetNameIFC() == _T("IFCSPACE") || ifcEntity.GetNameIFC() == _T("IFCOPENINGELEMENT")
						|| ifcEntity.GetNameIFC() == _T("IFCBEAM") || ifcEntity.GetNameIFC() == _T("IFCSTAIRFLIGHT")
						)
						continue;
					g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , false);
					//���ƴ���ʱ��Ҫ�򿪻��
					if(ifcEntity.GetNameIFC() == _T("IFCWINDOW") || ifcEntity.GetNameIFC() == _T("IFCCURTAINWALL")
						|| ifcEntity.GetNameIFC() ==_T("IFCPLATE") || ifcEntity.GetNameIFC() == _T("IFCROOF"))
					{
						g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , true);
						g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA);
						g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
					}

					/*if(ifcEntity.GetNameIFC() == _T("IFCWALLSTANDARDCASE"))
					{
						g_pd3dDevice->SetMaterial(&mtrl[1]);
						g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , true);
						g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA);
						g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
					}*/
			
					//û�ж�Ӧ��IFCEntity������һ��Ĭ�ϵĲ���
					if (false == check)//���û�й����Ͳ��ʱ��Ӧ
						g_pd3dDevice->SetMaterial(&mtrl[99]);

					pInstances=ifcEntity.GetInstanceGroup();
					pInstance=pInstances->begin();
					for (;pInstance!=pInstances->end();++pInstance)
					{
						CIFCInstance ifcInstance=pInstance->second;
						if (ifcEntity.GetNameIFC()==_T("IFCDOOR") && (!IsDoorOpen(ifcInstance.GetID())))
						{
							g_pd3dDevice->SetMaterial(&mtrl[3]);
							g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , false);
						}
						if (ifcEntity.GetNameIFC()==_T("IFCDOOR") && (IsDoorOpen(ifcInstance.GetID())))
						{
							g_pd3dDevice->SetMaterial(&mtrl[13]);
							/*g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , true);
							g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA);
							g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);*/
						}
						//g_pd3dDevice->SetRenderState(D3DRS_FILLMODE , D3DFILL_WIREFRAME);
						if (ifcInstance.GetSelected())
							g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST , ifcInstance.GetStartIndex(), ifcInstance.GetPrimitiveCount());

						if (ifcEntity.GetNameIFC()==_T("IFCDOOR") && IsDoorOpen(ifcInstance.GetID()))
						{
							
							CT2A kk(ifcInstance.GetName());
							string bb = kk;
							//aa.write((char*)(LPCTSTR)(ifcInstance.GetName().GetBuffer()),ifcInstance.GetName().GetLength()*2);

							aa.write(bb.c_str(),bb.length());
							aa<<endl;
							
						}
						
						//g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST , 0 , 0 , ifcInstance.GetPrimitiveCount() * 3 , ifcInstance.GetStartIndex(), ifcInstance.GetPrimitiveCount());
					}
					//��������֮��ر�Alpha Blend
					if(ifcEntity.GetNameIFC() == _T("IFCWINDOW") || ifcEntity.GetNameIFC() == _T("IFCCURTAINWALL")
						|| ifcEntity.GetNameIFC() ==_T("IFCPLATE") )
						g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , false);
					//if(ifcEntity.GetNameIFC() == _T("IFCWALLSTANDARDCASE"))
					//{
						//g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , false);
					//}

				}

				//��ǽ��Ӧ���㼯���е�λ�ú���Ŀ��¼����
				
			}
			//////////////////////////////////////////////////////////////////////////���������
			/*if (IsCoordVBufferInit)
			{
				if	(g_pd3dDevice->SetStreamSource(0 , g_pCoordVB , 0 , sizeof(CUSTOMVERTEX))) 
				{
					DirectXStatus = -1;
					return;
				}
				if	(g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX)) 
				{
					DirectXStatus = -1;
					return;
				}
				g_pd3dDevice->SetMaterial(&mtrl[99]);

				g_pd3dDevice->DrawPrimitive(D3DPT_LINELIST , 0 , 3);
			}*/
			//////////////////////////////////////////////////////////////////////////�����������ĵĻ���
			if (IsWorldCenter)
			{
				D3DXMATRIX Tran;
				D3DXMatrixTranslation(&Tran , gCubePos.x , gCubePos.y , gCubePos.z);
				g_pd3dDevice->SetTransform(D3DTS_WORLD , &Tran);
				g_pd3dDevice->SetMaterial(&mtrl[10]);
				//g_pWorldCenter->DrawSubset(0);
			}
			//////////////////////////////////////////////////////////////////////////������Ӱ

			//////////////////////////////////////////////////////////////////////////����·��
			if (IsPath )
			{
				//setup();
				//RenderPath();
				RenderMan();
			}
			
	
			//////////////////////////////////////////////////////////////////////////End the scene
			
			if( FAILED( g_pd3dDevice->EndScene() ) ) 
			{
				DirectXStatus = -1;
				return;
			}
		}

		// Present the backbuffer contents to the display
		if( FAILED( g_pd3dDevice->Present( NULL, NULL, NULL, NULL ) ) ) 
		{
			DirectXStatus = -1;
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////������������㻺���е�����
bool CIFCRender::RenderClear()
{
	if	(!DirectXStatus)
	{
		// Clear the backbuffer and the zbuffer
		if( FAILED( g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER/* |D3DCLEAR_STENCIL*/ ,
			BackColor, 1.0f, 0 ) ) ) 
		{
			DirectXStatus = -1;
			return false;
		}
		if (ifcProject)
		{
			//������㻺������
			if( FAILED( g_pVB->Lock( 0, 0, (void **)&pVerticesDeviceBuffer, 0 ) ) )
			{
				DirectXStatus = -1;
				return false;
			}

			//������㻺�����ά��ʾ���ݣ����������������ݣ�
			int i = 0;
			while  (i < ifcProject->GetNumIndices()) 
			{
				ASSERT(ifcProject->GetpIndices()[i] < ifcProject->GetNumVertices());
				memset(&(((CUSTOMVERTEX *) pVerticesDeviceBuffer)[i]),  0 , sizeof(CUSTOMVERTEX));
				i++;
			}

			if	(FAILED( g_pVB->Unlock()))
			{
				DirectXStatus = -1;
				return false;
			}
			ifcProject = NULL;
		}
		
		if( FAILED( g_pd3dDevice->Present( NULL, NULL, NULL, NULL ) ) ) 
		{
			DirectXStatus = -1;
			return false;
		}
		IsVBufferInit = false;
	}
	
	return true;
}

void CIFCRender::RenderModelShadow()
{
	//Enable stencil and set the state
	g_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE , true);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC , D3DCMP_ALWAYS);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILREF , 0x0);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILMASK , 0xffffffff);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK , 0xffffffff);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILZFAIL , D3DSTENCILOP_KEEP);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILFAIL , D3DSTENCILOP_KEEP);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILPASS , D3DSTENCILOP_INCR);

	//shadow position
	D3DXVECTOR4 lightDirection(0.707f , -0.707f , 0.707f , 0.0f);
	D3DXPLANE groundPlane(0.0f , -2.0f , 0.0f , 0.0f);

	D3DXMATRIX S;
	D3DXMatrixShadow(&S , &lightDirection , & groundPlane);

	D3DXMATRIX T;
	D3DXMatrixTranslation(&T , 0 , -0.5 , 0);

	D3DXMATRIX W;
	W = S * T;
	g_pd3dDevice->SetTransform(D3DTS_WORLD , &W);

	//ALPHA blend the shadow
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , true);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 mtrl;
	mtrl.Diffuse.a = 0.5f;

	//Disable the depth buffer so that z-fighting doesn't occur when we render the shadow
	//on top of the floor
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE , false);
	//g_pd3dDevice->SetMaterial(&mtrl);
	g_pd3dDevice->SetTexture(0 , 0);

	//Draw the shadow 
	//////////////////////////////////////////////////////////////////////////
	//Render
	CIFCProject *IFCProject = ifcProject;
	if (IFCProject && IsVBufferInit)//ifcProject�൱�����������IsVBufferInit��ʶ�˶��㻺���Ƿ�������
	{
		const EntityGroup* pGroups;
		const InstanceGroup* pInstances;
		Entities_constIter pGroup;
		Instance_constIter pInstance;

		pGroups=IFCProject->GetEntityGroup();
		pGroup=pGroups->begin();
		//����
		for (;pGroup!=pGroups->end();++pGroup)
		{
			CIFCEntity ifcEntity=pGroup->second;
			bool check = false;
			mapiter = EntitytoMtrl.begin();

			pInstances=ifcEntity.GetInstanceGroup();
			pInstance=pInstances->begin();
			for (;pInstance!=pInstances->end();++pInstance)
			{
				CIFCInstance ifcInstance=pInstance->second;
				//g_pd3dDevice->SetRenderState(D3DRS_FILLMODE , D3DFILL_WIREFRAME);
				if (ifcInstance.GetSelected())
					g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST , ifcInstance.GetStartIndex(), ifcInstance.GetPrimitiveCount());
				//g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST , 0 , 0 , ifcInstance.GetPrimitiveCount() * 3 , ifcInstance.GetStartIndex(), ifcInstance.GetPrimitiveCount());
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	g_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE , false);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , false);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE , true);

}
//////////////////////////////////////////////////////////////////////////Calculat Path
void CIFCRender::CalculatePath()
{
	Path.clear();
	footPoints.clear();
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	double time;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);
	GetAxis();
	QueryPerformanceCounter(&nEndTime);
	time = (double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart;
	setup();
	//setup1();
	SetupBall();

	IsPath = true;
}



/////////////////////////////////////////////////////////////////////////��ȡǽ���ϰ�������
void CIFCRender::GetAxis()
{
	start.clear();
	num.clear();
	dstart.clear();
	dnum.clear();
	fstart.clear();
	fnum.clear();
	cstart.clear();
	cnum.clear();
	sstart.clear();
	snum.clear();
	pstart.clear();
	pnum.clear();
	disstart.clear();
	disnum.clear();
	door_x.clear();
	door_y.clear();

	slstart.clear();
	slnum.clear();


	CIFCProject *IFCProject = ifcProject;
	const EntityGroup* pGroups;
	const InstanceGroup *pInstances;
	Entities_constIter pGroup;
	Instance_constIter pInstance;

	double *bb;
	vector<int>proxyId;
	int ww = 0;//ǽ
	int  dd = 0;//��
	int ff = 0;//�Ҿ�
	int cc = 0;//��
	int ss = 0;//�ռ�
	int pp = 0;//ר���豸
	int dis = 0;//distribution element
	int sl = 0;

	//grid_size = fm.getGridSize();

	min_x.x = 0;
	min_y.y = 0;
	max_x.x = -1;
	max_y.y = -1;


	//��Ϊ�����100��դ��,ֵ0˵���˴����ϰ��1Ϊ��ͨ��
	vector<vector<bool> > grids(grid_size, vector<bool>(grid_size));

	//��ʼ��
	for (int i = 0;i<grid_size;i++)
	{
		for(int j = 0;j<grid_size;j++)
		{
			grids[i][j] = 1;
		}
	}

	pGroups=IFCProject->GetEntityGroup();
	pGroup=pGroups->begin();
	//����
	for(;pGroup!=pGroups->end();++pGroup)
	{
		CIFCEntity ifcEntity = pGroup->second;
		pInstances = ifcEntity.GetInstanceGroup();
		pInstance = pInstances->begin();
		for(;pInstance!=pInstances->end();++pInstance)
		{
			CIFCInstance ifcInstance = pInstance->second;
			int kk = 0;
			/*if(ifcEntity.GetNameIFC() == _T("IFCWALLSTANDARDCASE") || ifcEntity.GetNameIFC() == _T("IFCWALL")
			|| ifcEntity.GetNameIFC()== _T("IFCBUILDINGELEMENTPROXY") || ifcEntity.GetNameIFC() == _T("IFCFURNISHINGELEMENT"))*/
			//��ȡǽ��������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCWALLSTANDARDCASE") || ifcEntity.GetNameIFC() == _T("IFCWALL"))
			{
				start.push_back(ifcInstance.GetStartIndex());
				num.push_back(ifcInstance.GetPrimitiveCount());
				ww++;
				if(start[kk] == ifcInstance.GetStartIndex() && ww!=1)
				{
					start.pop_back();
					num.pop_back();
					kk++;
					break;
				}
			}

			//kk=0;
			//��ȡ�ŵ�������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCDOOR") && IsDoorOpen(ifcInstance.GetID()) )
			//if(ifcEntity.GetNameIFC() == _T("IFCDOOR") )
			{
				dstart.push_back(ifcInstance.GetStartIndex());
				dnum.push_back(ifcInstance.GetPrimitiveCount());
				dd++;
				if(dstart[kk] == ifcInstance.GetStartIndex() && dd!=1)
				{
					dstart.pop_back();
					dnum.pop_back();
					kk++;
					break;
				}
			}

			kk =0;
			//��ȡ�Ҿߵ�������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCFURNISHINGELEMENT") )
			{
				fstart.push_back(ifcInstance.GetStartIndex());
				fnum.push_back(ifcInstance.GetPrimitiveCount());
				ff++;
				if(fstart[kk] == ifcInstance.GetStartIndex() && ff!=1)
				{
					fstart.pop_back();
					fnum.pop_back();
					kk++;
					break;
				}
			}

			//¥�����
			if(ifcEntity.GetNameIFC() == _T("IFCSLAB")|| ifcEntity.GetNameIFC()==_T("IFCCOVERING") || ifcEntity.GetNameIFC()==_T("IFCROOF")|| ifcEntity.GetNameIFC()==_T("IFCCHAMFEREDGEFEATURE"))
			{
				slstart.push_back(ifcInstance.GetStartIndex());
				slnum.push_back(ifcInstance.GetPrimitiveCount());
				sl++;
				if(slstart[kk] == ifcInstance.GetStartIndex() && sl!=1)
				{
					slstart.pop_back();
					slnum.pop_back();
					kk++;
					break;
				}
			}

			kk =0;
			//��ȡ����������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCCOLUMN") )
			{
				cstart.push_back(ifcInstance.GetStartIndex());
				cnum.push_back(ifcInstance.GetPrimitiveCount());
				cc++;
				if(cstart[kk] == ifcInstance.GetStartIndex() && cc!=1)
				{
					cstart.pop_back();
					cnum.pop_back();
					kk++;
					break;
				}
			}

			kk =0;
			//��ȡ�ռ��������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCSPACE")&& IsSpacePrivate(ifcInstance.GetID()) )
			{
				sstart.push_back(ifcInstance.GetStartIndex());
				snum.push_back(ifcInstance.GetPrimitiveCount());
				ss++;
				if(sstart[kk] == ifcInstance.GetStartIndex() && ss!=1)
				{
					sstart.pop_back();
					snum.pop_back();
					kk++;
					break;
				}
			}

			kk =0;
			//��ȡר���豸��������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCBUILDINGELEMENTPROXY"))
			{
				proxyId.push_back(ifcInstance.GetID());
				pstart.push_back(ifcInstance.GetStartIndex());
				pnum.push_back(ifcInstance.GetPrimitiveCount());
				pp++;
				if(pstart[kk] == ifcInstance.GetStartIndex() && pp!=1)
				{
					pstart.pop_back();
					pnum.pop_back();
					kk++;
					break;
				}
			}

			kk =0;
			//��ȡdistribution element��������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCDISTRIBUTIONELEMENT") )
			{
				disstart.push_back(ifcInstance.GetStartIndex());
				disnum.push_back(ifcInstance.GetPrimitiveCount());
				dis++;
				if(disstart[kk] == ifcInstance.GetStartIndex() && dis!=1)
				{
					disstart.pop_back();
					disnum.pop_back();
					kk++;
					break;
				}
			}

		}

	}

	D3DXVECTOR3 tmpPoint;
	ConvexHull convex;
	//ofstream kk("e:\\axis.txt");
	for(int i=0;i<start.size();i++)
	{
		for(int j=0;j<num[i];j++)
		{
			tmpPoint.x = floorCoord[start[i]+j].x;
			tmpPoint.y = floorCoord[start[i]+j].y;
			tmpPoint.z = i;
			wallPoints.push_back(tmpPoint);
			convex.add_point(tmpPoint.x*100,tmpPoint.y*100);
			//kk<<tmpPoint.x*100<<","<<tmpPoint.y*100<<endl;

			//20110225��ȡ���������ı߽���Ϣ
			if (tmpPoint.x < min_x.x)
			{
				min_x.x = tmpPoint.x;
				min_x.y = tmpPoint.y;
				min_x.z = tmpPoint.z;
			}
			if (tmpPoint.y < min_y.y)
			{
				min_y.x = tmpPoint.x;
				min_y.y = tmpPoint.y;
				min_y.z = tmpPoint.z;
			}
			if (tmpPoint.x > max_x.x)
			{
				max_x.x = tmpPoint.x;
				max_x.y = tmpPoint.y;
				max_x.z = tmpPoint.z;
			}
			if (tmpPoint.y > max_y.y)
			{
				max_y.x = tmpPoint.x;
				max_y.y = tmpPoint.y;
				max_y.z = tmpPoint.z;
			}
		}
	}
	//kk<<endl<<endl<<endl<<endl<<endl;


	double length_x = (max_x.x-min_x.x) * 100;//����������Χ�ĳ�
	double width_y = (int)(((abs)(max_y.y-min_y.y)) * 100+1);//����������Χ�Ŀ�

	double offset_x = abs(min_x.x)*100;
	double offset_y = abs(min_y.y)*100;
	blength = length_x;
	bwidth = width_y;
	offsetx = offset_x;
	offsety = offset_y;
	double left,right;
	double up,down;

	for (int i=0;i<start.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;
		for (int j=0;j<num[i];j++)
		{
			tmpPoint.x = floorCoord[start[i]+j].x*100;
			tmpPoint.y = floorCoord[start[i]+j].y*100;
			if(tmpPoint.x + offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		//��ǽ�ϵĵ���Ϊ����ͨ��
		for(int p = (int)(left*grid_size/length_x);p<=(int)(right*grid_size/length_x);p++)
		{
			for (int q = (int)(down*grid_size/width_y);q<=(int)(up*grid_size/width_y);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
					grids[q][p] = 0;
			}
		}
	}

	for (int i=0;i<dstart.size();i++)
	{
		D3DXVECTOR3 middle;//���ڱ�ʾ�ŵ��е�λ��

		left = length_x;
		right = 0;
		up = 0;
		down = width_y;
		for (int j=0;j<dnum[i];j++)
		{
			tmpPoint.x = floorCoord[dstart[i]+j].x*100;
			tmpPoint.y = floorCoord[dstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

			middle.x = (left+right)/2;
			middle.y = (up+down)/2;

		}

		//�����ŵ����ϵĵ���Ϊ��ͨ��
		for(int p = (int)(left*grid_size/length_x)-1;p<=(int)(right*grid_size/length_x)+1;p++)
		{
			for (int q = (int)(down*grid_size/width_y)-1;q<=(int)(up*grid_size/width_y)+1;q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
					grids[q][p] = 1;
			}
		}
		int x = middle.x*grid_size/length_x;
		int y = middle.y*grid_size/width_y;
		door_x.push_back(x);
		door_y.push_back(y);
	}

	for (int i=0;i<fstart.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;
		for (int j=0;j<fnum[i];j++)
		{
			tmpPoint.x = floorCoord[fstart[i]+j].x*100;
			tmpPoint.y = floorCoord[fstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		//���Ҿ��ϵĵ���Ϊ����ͨ��
		for(int p = (int)(left*grid_size/length_x);p<=(int)(right*grid_size/length_x);p++)
		{
			for (int q = (int)(down*grid_size/width_y);q<=(int)(up*grid_size/width_y);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
					grids[q][p] = 0;
			}
		}
	}

	for (int i=0;i<cstart.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;
		for (int j=0;j<cnum[i];j++)
		{
			tmpPoint.x = floorCoord[cstart[i]+j].x*100;
			tmpPoint.y = floorCoord[cstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		//�����ϵĵ���Ϊ����ͨ��
		for(int p = (int)(left*grid_size/length_x);p<=(int)(right*grid_size/length_x);p++)
		{
			for (int q = (int)(down*grid_size/width_y);q<=(int)(up*grid_size/width_y);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
					grids[q][p] = 0;
			}
		}
	}

	for (int i=0;i<sstart.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;
		for (int j=0;j<snum[i];j++)
		{
			tmpPoint.x = floorCoord[sstart[i]+j].x*100;
			tmpPoint.y = floorCoord[sstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		//��˽�˷����ϵĵ���Ϊ����ͨ��
		for(int p = (int)(left*grid_size/length_x);p<=(int)(right*grid_size/length_x);p++)
		{
			for (int q = (int)(down*grid_size/width_y);q<=(int)(up*grid_size/width_y);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
					grids[q][p] = 0;
			}
		}
	}

	int level = 0;
	//ר���豸building proxy
	for (int i=0;i<pstart.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;
		for (int j=0;j<pnum[i];j++)
		{
			tmpPoint.x = floorCoord[pstart[i]+j].x*100;
			tmpPoint.y = floorCoord[pstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		level = GetElementRiskLevel(proxyId[i]);
		//level = 0;
		//��ר���豸�ϵĵ���Ϊ����ͨ��
		for(int p = (int)((left*grid_size/length_x)-level);p<=(int)((right*grid_size/length_x)+level);p++)
		{
			for (int q = (int)((down*grid_size/width_y)-level);q<=(int)((up*grid_size/width_y)+level);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
					grids[q][p] = 0;
			}
		}
	}

	//distribution element
	for (int i=0;i<disstart.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;
		for (int j=0;j<disnum[i];j++)
		{
			tmpPoint.x = floorCoord[disstart[i]+j].x*100;
			tmpPoint.y = floorCoord[disstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		//��ǽ�ϵĵ���Ϊ����ͨ��
		for(int p = (int)(left*grid_size/length_x);p<=(int)(right*grid_size/length_x);p++)
		{
			for (int q = (int)(down*grid_size/width_y);q<=(int)(up*grid_size/width_y);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
					grids[q][p] = 0;
			}
		}
	}


	ofstream kk("e:\\grids100.txt");
	for (int i=0;i<grid_size;i++)
	{
		for (int j = 0;j<grid_size;j++)
		{
			kk<<grids[i][j]<<" ";
		}
		kk<<endl;
	}
	kk.close();
}


//////////////////////////////////////////////////////////////////////////Calculat Path
void CIFCRender::CalculatePath_MF()
{
	Path.clear();
	footPoints.clear();
	GetAxis_MF();
	setup();
	//setup1();
	SetupBall();

	IsPath = true;
}



/////////////////////////////////////////////////////////////////////////��ȡǽ���ϰ�������
void CIFCRender::GetAxis_MF()
{
	/*start.clear();
	num.clear();
	storey.clear();
	dstart.clear();
	dnum.clear();
	dstorey.clear();
	fstart.clear();
	fnum.clear();
	fstorey.clear();
	cstart.clear();
	cnum.clear();
	cstorey.clear();
	sstart.clear();
	snum.clear();
	sstorey.clear();
	pstart.clear();
	pnum.clear();
	pstorey.clear();
	disstart.clear();
	disnum.clear();
	disstorey.clear();
	door_x.clear();
	door_y.clear();

	ststart.clear();
	stnum.clear();
	ststorey.clear();*///¥��


	CIFCProject *IFCProject = ifcProject;
	const EntityGroup* pGroups;
	const InstanceGroup *pInstances;
	Entities_constIter pGroup;
	Instance_constIter pInstance;

	double *bb;
	vector<int>proxyId;
	int ww = 0;//ǽ
	int  dd = 0;//��
	int ff = 0;//�Ҿ�
	int cc = 0;//��
	int ss = 0;//�ռ�
	int pp = 0;//ר���豸
	int dis = 0;//distribution element
	int st = 0;//¥��

	//grid_size = fm.getGridSize();

	min_x.x = 0;
	min_y.y = 0;
	max_x.x = -1;
	max_y.y = -1;


	//��Ϊ�����100��դ��,ֵ0˵���˴����ϰ��1Ϊ��ͨ��
	vector<vector<bool> > grids1(grid_size, vector<bool>(grid_size));
	vector<vector<bool> > grids2(grid_size, vector<bool>(grid_size));

	//��ʼ��
	for (int i = 0;i<grid_size;i++)
	{
		for(int j = 0;j<grid_size;j++)
		{
			grids1[i][j] = 1;
		}
	}

	for (int i = 0;i<grid_size;i++)
	{
		for(int j = 0;j<grid_size;j++)
		{
			grids2[i][j] = 1;
		}
	}

	pGroups=IFCProject->GetEntityGroup();
	pGroup=pGroups->begin();
	//����
	for(;pGroup!=pGroups->end();++pGroup)
	{
		CIFCEntity ifcEntity = pGroup->second;
		pInstances = ifcEntity.GetInstanceGroup();
		pInstance = pInstances->begin();
		for(;pInstance!=pInstances->end();++pInstance)
		{
			CIFCInstance ifcInstance = pInstance->second;
			int kk = 0;
			/*if(ifcEntity.GetNameIFC() == _T("IFCWALLSTANDARDCASE") || ifcEntity.GetNameIFC() == _T("IFCWALL")
			|| ifcEntity.GetNameIFC()== _T("IFCBUILDINGELEMENTPROXY") || ifcEntity.GetNameIFC() == _T("IFCFURNISHINGELEMENT"))*/
			//��ȡǽ��������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCWALLSTANDARDCASE") || ifcEntity.GetNameIFC() == _T("IFCWALL"))
			{
				start.push_back(ifcInstance.GetStartIndex());
				num.push_back(ifcInstance.GetPrimitiveCount());
				storey.push_back(GetFloor(ifcInstance.GetID()));
				ww++;
				if(start[kk] == ifcInstance.GetStartIndex() && ww!=1)
				{
					start.pop_back();
					num.pop_back();
					storey.pop_back();
					kk++;
					break;
				}
			}

			//kk=0;
			//��ȡ�ŵ�������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCDOOR") && IsDoorOpen(ifcInstance.GetID()) )
			{
				dstart.push_back(ifcInstance.GetStartIndex());
				dnum.push_back(ifcInstance.GetPrimitiveCount());
				dstorey.push_back(GetFloor(ifcInstance.GetID()));
				dd++;
				if(dstart[kk] == ifcInstance.GetStartIndex() && dd!=1)
				{
					dstart.pop_back();
					dnum.pop_back();
					dstorey.pop_back();
					kk++;
					break;
				}
			}

			kk =0;
			//��ȡ�Ҿߵ�������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCFURNISHINGELEMENT") )
			{
				fstart.push_back(ifcInstance.GetStartIndex());
				fnum.push_back(ifcInstance.GetPrimitiveCount());
				fstorey.push_back(GetFloor(ifcInstance.GetID()));
				ff++;
				if(fstart[kk] == ifcInstance.GetStartIndex() && ff!=1)
				{
					fstart.pop_back();
					fnum.pop_back();
					fstorey.pop_back();
					kk++;
					break;
				}
			}

			kk =0;
			//��ȡ����������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCCOLUMN") )
			{
				cstart.push_back(ifcInstance.GetStartIndex());
				cnum.push_back(ifcInstance.GetPrimitiveCount());
				cstorey.push_back(GetFloor(ifcInstance.GetID()));
				cc++;
				if(cstart[kk] == ifcInstance.GetStartIndex() && cc!=1)
				{
					cstart.pop_back();
					cnum.pop_back();
					cstorey.pop_back();
					kk++;
					break;
				}
			}

			kk =0;
			//��ȡ�ռ��������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCSPACE")&& IsSpacePrivate(ifcInstance.GetID()) )
			{
				sstart.push_back(ifcInstance.GetStartIndex());
				snum.push_back(ifcInstance.GetPrimitiveCount());
				sstorey.push_back(GetFloor(ifcInstance.GetID()));
				ss++;
				if(sstart[kk] == ifcInstance.GetStartIndex() && ss!=1)
				{
					sstart.pop_back();
					snum.pop_back();
					sstorey.pop_back();
					kk++;
					break;
				}
			}

			kk =0;
			//��ȡר���豸��������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCBUILDINGELEMENTPROXY"))
			{
				proxyId.push_back(ifcInstance.GetID());
				pstart.push_back(ifcInstance.GetStartIndex());
				pnum.push_back(ifcInstance.GetPrimitiveCount());
				pstorey.push_back(GetFloor(ifcInstance.GetID()));
				pp++;
				if(pstart[kk] == ifcInstance.GetStartIndex() && pp!=1)
				{
					pstart.pop_back();
					pnum.pop_back();
					pstorey.pop_back();
					kk++;
					break;
				}
			}

			kk =0;
			//��ȡdistribution element��������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCDISTRIBUTIONELEMENT") )
			{
				disstart.push_back(ifcInstance.GetStartIndex());
				disnum.push_back(ifcInstance.GetPrimitiveCount());
				disstorey.push_back(GetFloor(ifcInstance.GetID()));
				dis++;
				if(disstart[kk] == ifcInstance.GetStartIndex() && dis!=1)
				{
					disstart.pop_back();
					disnum.pop_back();
					disstorey.pop_back();
					kk++;
					break;
				}
			}

			kk =0;
			//��ȡdistribution element��������Ϣ
			if(ifcEntity.GetNameIFC() == _T("IFCSTAIRFLIGHT") )
			{
				ststart.push_back(ifcInstance.GetStartIndex());
				stnum.push_back(ifcInstance.GetPrimitiveCount());
				//ststorey.push_back(GetFloor(ifcInstance.GetID()));
				st++;
				if(ststart[kk] == ifcInstance.GetStartIndex() && st!=1)
				{
					ststart.pop_back();
					stnum.pop_back();
					//ststorey.pop_back();
					kk++;
					break;
				}
			}

			if (ifcEntity.GetNameIFC() == _T("IFCSTAIR"))
			{
				ststorey.push_back(GetFloor(ifcInstance.GetID()));
			}

		}

	}

	D3DXVECTOR3 tmpPoint;
	ConvexHull convex;
	for(int i=0;i<start.size();i++)
	{
		for(int j=0;j<num[i];j++)
		{
			tmpPoint.x = floorCoord[start[i]+j].x;
			tmpPoint.y = floorCoord[start[i]+j].y;
			tmpPoint.z = i;
			wallPoints.push_back(tmpPoint);
			convex.add_point(tmpPoint.x*100,tmpPoint.y*100);
			//kk<<tmpPoint.x*100<<","<<tmpPoint.y*100<<endl;

			//20110225��ȡ���������ı߽���Ϣ
			if (tmpPoint.x < min_x.x)
			{
				min_x.x = tmpPoint.x;
				min_x.y = tmpPoint.y;
				min_x.z = tmpPoint.z;
			}
			if (tmpPoint.y < min_y.y)
			{
				min_y.x = tmpPoint.x;
				min_y.y = tmpPoint.y;
				min_y.z = tmpPoint.z;
			}
			if (tmpPoint.x > max_x.x)
			{
				max_x.x = tmpPoint.x;
				max_x.y = tmpPoint.y;
				max_x.z = tmpPoint.z;
			}
			if (tmpPoint.y > max_y.y)
			{
				max_y.x = tmpPoint.x;
				max_y.y = tmpPoint.y;
				max_y.z = tmpPoint.z;
			}
		}
	}
	//kk<<endl<<endl<<endl<<endl<<endl;


	double length_x = max_x.x * 200;//����������Χ�ĳ�
	double width_y = (int)(max_y.y * 200);//����������Χ�Ŀ�

	double offset_x = abs(min_x.x)*100;
	double offset_y = abs(min_y.y)*100;
	blength = length_x;
	bwidth = width_y;
	double left,right;
	double up,down;
	int storeyNum;

	for (int i=0;i<start.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;
		storeyNum = storey[i];

		for (int j=0;j<num[i];j++)
		{
			tmpPoint.x = floorCoord[start[i]+j].x*100;
			tmpPoint.y = floorCoord[start[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		//��ǽ�ϵĵ���Ϊ����ͨ��
		for(int p = (int)(left*grid_size/length_x);p<=(int)(right*grid_size/length_x);p++)
		{
			for (int q = (int)(down*grid_size/width_y);q<=(int)(up*grid_size/width_y);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
				{
					if(storeyNum == 1)
						grids1[q][p] = 0;
					else if(storeyNum == 3)//2
						grids2[q][p] = 0;
				}
			}
		}
	}

	for (int i=0;i<dstart.size();i++)
	{
		D3DXVECTOR3 middle;//���ڱ�ʾ�ŵ��е�λ��

		left = length_x;
		right = 0;
		up = 0;
		down = width_y;

		storeyNum = dstorey[i];

		for (int j=0;j<dnum[i];j++)
		{
			tmpPoint.x = floorCoord[dstart[i]+j].x*100;
			tmpPoint.y = floorCoord[dstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

			//regular ifc model
			/*middle.x = (left+right)/2;
			middle.y = (up+down)/2;*/

			//library ifc model
			middle.x = abs(left-right);
			middle.y = abs(up-down);

		}

		//�����ŵ����ϵĵ���Ϊ��ͨ��
		for(int p = (int)(left*grid_size/length_x);p<=(int)(right*grid_size/length_x);p++)
		{
			for (int q = (int)(down*grid_size/width_y);q<=(int)(up*grid_size/width_y);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
				{
					if(storeyNum == 1)
						grids1[q][p] = 1;
					else if(storeyNum == 3)//2
						grids2[q][p] = 1;
				}
			}
		}
		int x = middle.x*grid_size/length_x;
		int y = middle.y*grid_size/width_y;
		door_x.push_back(x);
		door_y.push_back(y);
	}

	for (int i=0;i<fstart.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;

		storeyNum = fstorey[i];

		for (int j=0;j<fnum[i];j++)
		{
			tmpPoint.x = floorCoord[fstart[i]+j].x*100;
			tmpPoint.y = floorCoord[fstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		//���Ҿ��ϵĵ���Ϊ����ͨ��
		for(int p = (int)(left*grid_size/length_x);p<=(int)(right*grid_size/length_x);p++)
		{
			for (int q = (int)(down*grid_size/width_y);q<=(int)(up*grid_size/width_y);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
				{
					if(storeyNum == 1)
						grids1[q][p] = 0;
					else if(storeyNum == 3)//2
						grids2[q][p] = 0;
				}
			}
		}
	}

	for (int i=0;i<cstart.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;

		storeyNum = cstorey[i];

		for (int j=0;j<cnum[i];j++)
		{
			tmpPoint.x = floorCoord[cstart[i]+j].x*100;
			tmpPoint.y = floorCoord[cstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		//�����ϵĵ���Ϊ����ͨ��
		for(int p = (int)(left*grid_size/length_x);p<=(int)(right*grid_size/length_x);p++)
		{
			for (int q = (int)(down*grid_size/width_y);q<=(int)(up*grid_size/width_y);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
				{
					if(storeyNum == 1)
						grids1[q][p] = 0;
					else if(storeyNum == 3)//2
						grids2[q][p] = 0;
				}
			}
		}
	}

	for (int i=0;i<sstart.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;

		storeyNum = sstorey[i];

		for (int j=0;j<snum[i];j++)
		{
			tmpPoint.x = floorCoord[sstart[i]+j].x*100;
			tmpPoint.y = floorCoord[sstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		//��˽�˷����ϵĵ���Ϊ����ͨ��
		for(int p = (int)(left*grid_size/length_x);p<=(int)(right*grid_size/length_x);p++)
		{
			for (int q = (int)(down*grid_size/width_y);q<=(int)(up*grid_size/width_y);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
				{
					if(storeyNum == 1)
						grids1[q][p] = 0;
					else if(storeyNum == 3)//2
						grids2[q][p] = 0;
				}
			}
		}
	}

	int level = 0;
	//ר���豸building proxy
	for (int i=0;i<pstart.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;

		storeyNum = pstorey[i];

		for (int j=0;j<pnum[i];j++)
		{
			tmpPoint.x = floorCoord[pstart[i]+j].x*100;
			tmpPoint.y = floorCoord[pstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		level = GetElementRiskLevel(proxyId[i]);
		//��ר���豸�ϵĵ���Ϊ����ͨ��
		for(int p = (int)((left*grid_size/length_x)-level);p<=(int)((right*grid_size/length_x)+level);p++)
		{
			for (int q = (int)((down*grid_size/width_y)-level);q<=(int)((up*grid_size/width_y)+level);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
				{
					if(storeyNum == 1)
						grids1[q][p] = 0;
					else if(storeyNum == 3)//2
						grids2[q][p] = 0;
				}
			}
		}
	}

	//distribution element
	for (int i=0;i<disstart.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;

		storeyNum = disstorey[i];

		for (int j=0;j<disnum[i];j++)
		{
			tmpPoint.x = floorCoord[disstart[i]+j].x*100;
			tmpPoint.y = floorCoord[disstart[i]+j].y*100;
			if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;

		}

		//��ǽ�ϵĵ���Ϊ����ͨ��
		for(int p = (int)(left*grid_size/length_x);p<=(int)(right*grid_size/length_x);p++)
		{
			for (int q = (int)(down*grid_size/width_y);q<=(int)(up*grid_size/width_y);q++)
			{
				if (p>=0 && p<grid_size && q>=0 && q<grid_size)
				{
					if(storeyNum == 1)
						grids1[q][p] = 0;
					else if(storeyNum == 3)//2
						grids2[q][p] = 0;
				}
			}
		}
	}

	D3DXVECTOR3 zmax;
	D3DXVECTOR3 zmin;
	for (int i=0;i<ststart.size();i++)
	{
		left = length_x;
		right = 0;
		up = 0;
		down = width_y;
		
		zmax.z = -200;
		zmin.z = 200;

		storeyNum = ststorey[i];

		for (int j=0;j<stnum[i];j++)
		{
			tmpPoint.x = floorCoord[ststart[i]+j].x*100;
			tmpPoint.y = floorCoord[ststart[i]+j].y*100;
			tmpPoint.z = floorCoord[ststart[i]+j].z*100;
		/*	if(tmpPoint.x +offset_x < left)
				left = tmpPoint.x + offset_x;
			if(tmpPoint.x +offset_x > right)
				right = tmpPoint.x + offset_x;
			if(tmpPoint.y + offset_y > up)
				up = tmpPoint.y + offset_y;
			if(tmpPoint.y + offset_y < down)
				down = tmpPoint.y + offset_y;*/
			if (tmpPoint.z < zmin.z)
			{
				zmin.z = tmpPoint.z;
				zmin.x = tmpPoint.x+offset_x;
				zmin.y = tmpPoint.y+ offset_y;
			}
			if (tmpPoint.z > zmax.z)
			{
				zmax.z = tmpPoint.z;
				zmax.x = tmpPoint.x+offset_x;
				zmax.y = tmpPoint.y+ offset_y;
			}

		}

		D3DXVECTOR3 temp;
		
		temp.x = zmin.x*grid_size/length_x;
		temp.y = zmin.y*grid_size/width_y;
		temp.z = storeyNum;
		stairPos.push_back(temp);

		temp.x = zmax.x*grid_size/length_x;
		temp.y = zmax.y*grid_size/width_y;
		temp.z = storeyNum+1;
		stairPos.push_back(temp);

	}


	ofstream kk("e:\\grids100.txt");
	for (int i=0;i<grid_size;i++)
	{
		for (int j = 0;j<grid_size;j++)
		{
			kk<<grids1[i][j]<<" ";
		}
		kk<<endl;
	}
	kk.close();

	ofstream tt("e:\\grids200.txt");
	for (int i=0;i<grid_size;i++)
	{
		for (int j = 0;j<grid_size;j++)
		{
			tt<<grids2[i][j]<<" ";
		}
		tt<<endl;
	}
	tt.close();
}

/////////////////////////////////////////////////////////////////////////��ȡ����3Dģ��
bool CIFCRender::setup()
{
	HRESULT hr = 0;
	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;

	hr = D3DXLoadMeshFromX(
		_T("man4.x"),
		D3DXMESH_MANAGED,
		g_pd3dDevice,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&manMesh);

	if(FAILED(hr))
	{
		::MessageBox(0,_T("D3DXLoadMeshFromX() - FAILED"),0,0);
		return false;
	}

	if(mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* m = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		for(int i=0;i<numMtrls;i++)
		{
			m[i].MatD3D.Ambient = m[i].MatD3D.Diffuse;
			manMtrl.push_back(m[i].MatD3D);
			if(m[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					g_pd3dDevice,
					(LPWSTR)(LPCWSTR)m[i].pTextureFilename,
					&tex);

				manTexture.push_back(tex);
			}
			else
			{
				manTexture.push_back(0);
			}
		}
	}

	return true;
}


bool CIFCRender::setup1()
{
	HRESULT hr1 = 0;
	ID3DXBuffer* adjBuffer1 = 0;
	ID3DXBuffer* mtrlBuffer1 = 0;
	DWORD numMtrls1 = 0;

	hr1 = D3DXLoadMeshFromX(
		_T("m1.x"),
		D3DXMESH_MANAGED,
		g_pd3dDevice,
		&adjBuffer1,
		&mtrlBuffer1,
		0,
		&numMtrls1,
		&manMesh1);

	if(FAILED(hr1))
	{
		::MessageBox(0,_T("D3DXLoadMeshFromX() - FAILED"),0,0);
		return false;
	}

	if(mtrlBuffer1 != 0 && numMtrls1 != 0)
	{
		D3DXMATERIAL* m = (D3DXMATERIAL*)mtrlBuffer1->GetBufferPointer();
		for(int i=0;i<numMtrls1;i++)
		{
			m[i].MatD3D.Ambient = m[i].MatD3D.Diffuse;
			manMtrl1.push_back(m[i].MatD3D);
			if(m[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					g_pd3dDevice,
					(LPWSTR)(LPCWSTR)m[i].pTextureFilename,
					&tex);

				manTexture1.push_back(tex);
			}
			else
			{
				manTexture1.push_back(0);
			}
		}
	}

	return true;
}


//////////////////////////��ȡ��ģ��
bool CIFCRender::SetupBall()
{
	HRESULT hr = 0;
	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;

	//yy1.x
	hr = D3DXLoadMeshFromX(
		_T("bb2.X"),
		D3DXMESH_MANAGED,
		g_pd3dDevice,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&ballMesh);

	if(FAILED(hr))
	{
		::MessageBox(0,_T("D3DXLoadMeshFromX() - FAILED"),0,0);
		return false;
	}

	if(mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* m = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		for(int i=0;i<numMtrls;i++)
		{
			m[i].MatD3D.Ambient = m[i].MatD3D.Diffuse;
			ballMtrl.push_back(m[i].MatD3D);
			if(m[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					g_pd3dDevice,
					(LPWSTR)(LPCWSTR)m[i].pTextureFilename,
					&tex);

				ballTexture.push_back(tex);
			}
			else
			{
				ballTexture.push_back(0);
			}
		}
	}

	return true;
}


bool CIFCRender::RenderMan()
{
	if(g_pd3dDevice)
	{

		D3DXMATRIX Tran;
		D3DXMATRIX Scale;
		D3DXMATRIX Angle;

		//б�ʵ��ж�
		//ģ�����ݶ��㻺�湤���Ѿ�����
		IsVBufferInit = true;
		//if (counter % 5 == 0)
		//{
		//	
		//	D3DXMatrixTranslation(&Tran, footPoints[counter].x, footPoints[counter].y, 0.015f);
		//	
		//	
		//	//D3DXMatrixScaling(&Scale, 0.15f, 0.15f, 0.15f);regular ifc models
		//	D3DXMatrixScaling(&Scale, 0.15f, 0.15f, 0.15f);//for the xuzhou library
		//	
		//	arcs = (Pos.y - footPoints[0].y)/(Pos.x - footPoints[0].x);
		//	if (arcs<0)
		//	{
		//		arcs = D3DX_PI/2+arcs;
		//	}
		//	D3DXMatrixRotationZ(&Angle,arcs);
		//	Tran = Scale * Tran;
		//	Tran = Angle * Tran;
		//	g_pd3dDevice->SetTransform(D3DTS_WORLD , &Tran);
		//	for(int i=0;i<manMtrl.size();i++)
		//	{
		//		g_pd3dDevice->SetMaterial( &manMtrl[i] );
		//		g_pd3dDevice->SetTexture( 0, manTexture[i] );
		//		manMesh->DrawSubset(i);
		//	}
		//}
		//counter++;
		
		/*if (stepCounter == 0)
		{
			arcs = (Pos.y - footPoints[0].y)/(Pos.x - footPoints[0].x);
		}
		else
		{
			k1 = (Pos.y*100 - footPoints[stepCounter-1].y*100)/(Pos.x*100 - footPoints[stepCounter-1].x*100);
			k2 = (footPoints[stepCounter+1].y*100 - Pos.y*100)/( footPoints[stepCounter+1].x*100 - Pos.x*100);
			if (k1*k2 == -1)
				arcs = D3DX_PI/4;
			else 
				arcs = abs((k2*100-k1*100)/(100+k1*100*k2));
		}*/


		
		/*mark ++;
		if (mark < 15)
		{
			for(int i=0;i<manMtrl.size();i++)
			{
				g_pd3dDevice->SetMaterial( &manMtrl[i] );
				g_pd3dDevice->SetTexture( 0, manTexture[i] );
				manMesh->DrawSubset(i);
			}
		}
		else if(mark >=15 && mark <30){
			for(int i=0;i<manMtrl1.size();i++)
			{
				g_pd3dDevice->SetMaterial( &manMtrl1[i] );
				g_pd3dDevice->SetTexture( 0, manTexture1[i] );
				manMesh1->DrawSubset(i);
			}
		}
		if (mark == 30)
		{
			mark =0;
		}*/
		

		/*for(int i=0;i<manMtrl1.size();i++)
		{
			g_pd3dDevice->SetMaterial( &manMtrl1[i] );
			g_pd3dDevice->SetTexture( 0, manTexture1[i] );
			manMesh1->DrawSubset(i);
		}*/

		
		for(int i = 0;i<footPoints.size();i++)
		{
			int stepp = 0;
			if (grid_size<100)
			{
				stepp = 3;
			}
			else 
				stepp = (int)(grid_size/100*1);
				//stepp = (int)(grid_size/100*16);
			if( i>1 && footPoints[i].x - footPoints[i-1].x!=0)
				arc = (footPoints[i].y - footPoints[i-1].y)/(footPoints[i].x - footPoints[i-1].x);
			if (arc<0)
			{
				arc = D3DX_PI/2+arc;
			}
			if (i % stepp == 0)
			{
				D3DXMatrixTranslation(&Tran, footPoints[i].x, footPoints[i].y, footPoints[i].z);
				D3DXMatrixScaling(&Scale, 0.0002f, 0.0002f, 0.0002f);//library
				//D3DXMatrixScaling(&Scale, 0.0025f, 0.0025f, 0.0025f);for regular ifc model
				D3DXMatrixRotationZ(&Angle,arc);
				Tran = Scale * Tran;
				Tran = Angle * Tran;
				g_pd3dDevice->SetTransform(D3DTS_WORLD , &Tran);
				for(int i=0;i<ballMtrl.size();i++)
				{
					g_pd3dDevice->SetMaterial( &ballMtrl[i] );
					g_pd3dDevice->SetTexture( 0, ballTexture[i] );
					ballMesh->DrawSubset(i);
				}
			}
			
		}

		D3DXMatrixTranslation(&Tran, 0.1, 0.1, 0.1);
		D3DXMatrixScaling(&Scale, 0.19f, 0.19f, 0.19f);//library
		arc = D3DX_PI/2;
		D3DXMatrixRotationZ(&Angle,arc);
		Tran = Scale * Tran;
		Tran = Angle * Tran;
		g_pd3dDevice->SetTransform(D3DTS_WORLD , &Tran);
		for(int i=0;i<manMtrl.size();i++)
		{
			g_pd3dDevice->SetMaterial( &manMtrl[i] );
			g_pd3dDevice->SetTexture( 0, manTexture[i] );
			manMesh->DrawSubset(i);
		}

		/*D3DXVECTOR3 tempPoint;
		vector<D3DXVECTOR3>midPoints;
		double a=0.036,b=0.007,c=0.037;
		tempPoint.x = -0.758;
		tempPoint.y = -0.414;
		tempPoint.z = 0.023;
		midPoints.push_back(tempPoint);

		tempPoint.x = tempPoint.x + 0.036;
		tempPoint.z = tempPoint.z - 0.037;
		midPoints.push_back(tempPoint);

		tempPoint.x = tempPoint.x + 0.036;
		tempPoint.z = tempPoint.z - 0.037;
		midPoints.push_back(tempPoint);
		tempPoint.x = tempPoint.x + 0.036;
		tempPoint.z = tempPoint.z - 0.037;
		midPoints.push_back(tempPoint);
		tempPoint.x = tempPoint.x + 0.036;
		tempPoint.z = tempPoint.z - 0.037;
		midPoints.push_back(tempPoint);
		tempPoint.x = tempPoint.x + 0.036;
		tempPoint.z = tempPoint.z - 0.037;
		midPoints.push_back(tempPoint);
		tempPoint.x = tempPoint.x + 0.036;
		tempPoint.z = tempPoint.z - 0.037;
		midPoints.push_back(tempPoint);
		tempPoint.x = tempPoint.x + 0.036;
		tempPoint.z = tempPoint.z - 0.037;
		midPoints.push_back(tempPoint);
		tempPoint.x = tempPoint.x + 0.036;
		tempPoint.z = tempPoint.z - 0.037;
		midPoints.push_back(tempPoint);
		tempPoint.x = tempPoint.x + 0.036;
		tempPoint.z = tempPoint.z - 0.037;
		midPoints.push_back(tempPoint);*/
		/*for (int i=1;i<midPoints.size();i++)
		{
			if(footPoints[i].x - footPoints[i-1].x!=0)
				arc = (footPoints[i].y - footPoints[i-1].y)/(footPoints[i].x - footPoints[i-1].x);
			if (arc<0)
			{
				arc = D3DX_PI/2+arc;
			}
			D3DXMatrixTranslation(&Tran, midPoints[i].x, midPoints[i].y, midPoints[i].z);
			D3DXMatrixScaling(&Scale, 0.0025f, 0.0025f, 0.0025f);
			D3DXMatrixRotationZ(&Angle,arc);
			Tran = Scale * Tran;
			Tran = Angle * Tran;
			g_pd3dDevice->SetTransform(D3DTS_WORLD , &Tran);
			for(int i=0;i<ballMtrl.size();i++)
			{
				g_pd3dDevice->SetMaterial( &ballMtrl[i] );
				g_pd3dDevice->SetTexture( 0, ballTexture[i] );
				ballMesh->DrawSubset(i);
			}
		}*/

		pre = mid;
		mid = Pos;
		/*stepCounter++;*/

		//g_pd3dDevice->SetMaterial(&mtrl[10]);
		//g_pWorldCenter->DrawSubset(0);
		//g_pd3dDevice->Present(0,0,0,0);
	}
	
	return true;
}

//����ʾ���ڵ�onsize������ã��ı���ʾ���ڵĸ߿�
void CIFCRender::OnSize(int cx , int cy)
{
	if	(initialized) 
	{
		CRect rc;

		m_iWidth = cx;
		m_iHeight = cy;

		// Save static reference to the render window
		CWnd* pGroup = CWnd::FromHandle(m_hwndRenderWindow);
		pGroup->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
		pGroup->GetClientRect(&rc);
		pGroup->MapWindowPoints(CWnd::FromHandle(m_hwndRenderWindow), &rc);

		Render();
	}
}

//////////////////////////////////////////////////////////////////////////ifc���ش���
bool CIFCRender::LoadIfc(CString m_strFileName , CString m_strSchemaName)
{
	if (ifcProject)
	{
		delete ifcProject;
		ifcProject=NULL;
		manMtrl.clear();
		ballMtrl.clear();
		manMesh = NULL;
		ballMesh = NULL;
	}
	/*
	TCHAR AppPath[MAX_PATH]; //�260
	GetModuleFileName(NULL, AppPath, MAX_PATH);
	CString path(AppPath);
	int position=path.ReverseFind('\\');
	path=path.Left(position);
	path+=_T("\\IFC2X3_TC1.exp");
	*/
	ifcProject = new CIFCProject(m_strFileName , m_strSchemaName);
	if (!ifcProject->OpenIFCProject())
	{
		CString Err=_T("���ܼ���IFCģ��,����������ȱ��IFC Schema.\n�������Ƿ����:");
		Err += m_strSchemaName;
		MessageBox(GetActiveWindow(), Err, _T("ERROR loading IFC file"), 0);
		ifcProject=NULL;
		return false;
	}

	return true;
}

bool CIFCRender::LoadIfc(CIFCProject * ifcPtr)
{
	if (ifcPtr != NULL)
	{
		ifcProject = ifcPtr;
		manMtrl.clear();
		ballMtrl.clear();
		manMesh = NULL;
		ballMesh = NULL;
		return true;
	}
	else
	{
		MessageBox(0 , _T("���ȼ���IFCģ��") , 0 , 0);
		manMtrl.clear();
		ballMtrl.clear();
		manMesh = NULL;
		ballMesh = NULL;
		return false;
	}
}

//��ȡ�豸״̬
bool CIFCRender::GetDeviceState()
{
	if (initialized)
		return true;
	else
		return false;
}

CIFCProject * CIFCRender::GetIFCProjectPtr()
{
	if (ifcProject != NULL)
		return ifcProject;
	else
		return NULL;
}

//////////////////////////////////////////////////////////////////////////��Ϣѭ��
LRESULT CIFCRender::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CVec	vec;	// Used to hold camera's forward vector
	CPos	eye;	// Used to hold camera's eye

	double	amt;
	int	iMouseX = LOWORD(lParam);
	int    iMouseY = HIWORD(lParam);
	static double pitchAmt = 0.0f;

	switch  (message)
	{
	case WM_LBUTTONDOWN:
		iZoomMouseX = iMouseX;
		iZoomMouseY = iMouseY;
		break;
	case WM_MOUSEWHEEL:

		if (MK_SHIFT&wParam )
		{
			if   ( (short)HIWORD(wParam) > 0 )
			{
				vec = gCam->getCamZ();
				eye = gCam->getEye();

				eye += vec * (float) 0.1;

				gCam->setEye(eye);
				gCam->setTarget(gCubePos); // Set the camera to look at the cube
				if  (initialized && RenderStatus) 
				{
					Render();
				}
				break;
			}
			if   ( (short)HIWORD(wParam) < 0 )
			{
				vec = gCam->getCamZ();
				eye = gCam->getEye();

				eye -= vec * (float) 0.1;

				gCam->setEye(eye);
				gCam->setTarget(gCubePos); // Set the camera to look at the cube
				if  (initialized && RenderStatus) 
				{
					Render();
				}
				break;
			}
		}
		else
		{
			if   ( (short)HIWORD(wParam) > 0 )
			{
				vec = gCam->getCamZ();
				eye = gCam->getEye();

				eye += vec * (float) 0.1;

				gCam->setEye(eye);
				gCam->setTarget(gCubePos); // Set the camera to look at the cube
				if  (initialized && RenderStatus) 
				{
					Render();
				}
				break;
			}
			if   ( (short)HIWORD(wParam) < 0 )
			{
				vec = gCam->getCamZ();
				eye = gCam->getEye();

				eye -= vec * (float) 0.1;

				gCam->setEye(eye);
				gCam->setTarget(gCubePos); // Set the camera to look at the cube
				if  (initialized && RenderStatus) 
				{
					Render();
				}
				break;
			}
		}
		break;
	case WM_MOUSEMOVE:
		//
		//	Mouse moved
		//
		//rotate
		if	(MK_LBUTTON&wParam && !(MK_CONTROL&wParam)) 
		{
			//////////////////////////////////////////////////////////////////////////rotate Y axis
			amt = (iMouseX - iZoomMouseX) * kCamMoveAmt * 300;
			gCam->rotY(((float) amt * 3.14159265f / 180.0f), gCubePos);
			iZoomMouseX = iMouseX;

			//////////////////////////////////////////////////////////////////////////rotate XZ axis or named pitch
			amt = -(iMouseY - iZoomMouseY) * kCamMoveAmt * 300;
			// Cap pitch
			if(pitchAmt + amt < -kMaxAngle)
			{
				amt = -kMaxAngle - pitchAmt;
				pitchAmt = -kMaxAngle;
			}
			else if(pitchAmt + amt > kMaxAngle)
			{
				amt = kMaxAngle - pitchAmt;
				pitchAmt = kMaxAngle;
			}
			else
			{
				pitchAmt += amt;
			}
			// Pitch the camera up/down
			gCam->pitch(((float) amt * 3.14159265f / 180.0f), gCubePos);
			iZoomMouseY = iMouseY;
			if  (initialized && RenderStatus) 
			{
				Render();
			}
			break;
		}
		//pan something to improve
		if (MK_LBUTTON&wParam && (MK_CONTROL&wParam))
		{
			//////////////////////////////////////////////////////////////////////////X Vector
			vec = gCam->getCamX();
			eye = gCam->getEye();

			eye -= vec * (((float) (iMouseX-iZoomMouseX))/200);
			gCubePos -=  vec * (((float) (iMouseX-iZoomMouseX))/200);

			iZoomMouseX = iMouseX;

			gCam->setEye(eye);
			gCam->setTarget(gCubePos); // Set the camera to look at the cube
			
			//////////////////////////////////////////////////////////////////////////Y Vector
			vec = gCam->getCamY();
			eye = gCam->getEye();

			eye += vec * (((float) (iMouseY-iZoomMouseY))/200);
			gCubePos +=  vec * (((float) (iMouseY-iZoomMouseY))/200);

			iZoomMouseY = iMouseY;

			gCam->setEye(eye);
			gCam->setTarget(gCubePos); // Set the camera to look at the cube

			if  (initialized && RenderStatus) 
			{
				Render();
			}
			break;
		}
		break;
		//////////////////////////////////////////////////////////////////////////
	case ID_RESET_TOP:
		m_vecOrigin.x = 0;
		m_vecOrigin.y = 0;
		m_vecOrigin.z = 0;
		gCubePos.x = 0;
		gCubePos.y = 0;
		gCubePos.z = 0;
		gCam->setTop(1,gCubePos.y,gCubePos.z);
		gCam->setTarget(gCubePos); // Set the camera to look at the cube

		if  (initialized && RenderStatus)
		{
			Render();
		}
		break;
	case ID_RESET_FRONT:
		m_vecOrigin.x = 0;
		m_vecOrigin.y = 0;
		m_vecOrigin.z = 0;
		gCubePos.x = 0;
		gCubePos.y = 0;
		gCubePos.z = 0;
		gCam->setFront(1,gCubePos.y,gCubePos.z);
		gCam->setTarget(gCubePos); // Set the camera to look at the cube

		if  (initialized && RenderStatus) 
		{
			Render();
		}
		break;
	case ID_RESET_SIDE:
		m_vecOrigin.x = 0;
		m_vecOrigin.y = 0;
		m_vecOrigin.z = 0;
		gCubePos.x = 0;
		gCubePos.y = 0;
		gCubePos.z = 0;
		gCam->setSide(1,gCubePos.y,gCubePos.z);
		gCam->setTarget(gCubePos); // Set the camera to look at the cube

		if  (initialized && RenderStatus) 
		{
			Render();
		}
		break;
		//////////////////////////////////////////////////////////////////////////
	default:
		break;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////���ض�ӦIDֵ���ŵĿ����״̬,���ŷ����棬���򷵻ؼ�
bool CIFCRender::IsDoorOpen(int instanceId)
{
	PropertyListXml=*(ifcProject->GetPropertySet(instanceId));
	TiXmlHandle hXmlRoot(&PropertyListXml);
	TiXmlElement *pElem;

	//PSet���Լ��ĸ��ڵ�
	pElem=hXmlRoot.FirstChild("InstancePropertySet").FirstChild("PropertySet").ToElement();
	if (!pElem)
		return true;


	CString numString;
	bool status = true;
	for (const TiXmlElement *sub_tag =pElem; sub_tag; sub_tag = sub_tag->NextSiblingElement())
	{
		CString PropertySetName(sub_tag->Attribute("PropertySetName"));

		for (const TiXmlElement *sub_record =sub_tag->FirstChildElement(); sub_record; sub_record = sub_record->NextSiblingElement())
		{
			CString Name(sub_record->Attribute("Name"));
			CString Value(sub_record->Attribute("Value"));
			CString Description(sub_record->Attribute("Description"));	
			if (Name == _T("IsLocked"))
			{
				if(Value == _T(".T."))
					return false;
				else if(Value == _T(".F."))
					return true;

			}

		}
	}
	return status;
}

//////////////////////////////////////////////////////////////////////////���ض�ӦIDֵ�ķ����Ƿ��ͨ��,��ͨ�з��ؼ٣����򷵻���
bool CIFCRender::IsSpacePrivate(int instanceId)
{
	PropertyListXml=*(ifcProject->GetPropertySet(instanceId));
	TiXmlHandle hXmlRoot(&PropertyListXml);
	TiXmlElement *pElem;

	//PSet���Լ��ĸ��ڵ�
	pElem=hXmlRoot.FirstChild("InstancePropertySet").FirstChild("PropertySet").ToElement();
	if (!pElem)
		return true;


	CString numString;
	bool status = false;
	for (const TiXmlElement *sub_tag =pElem; sub_tag; sub_tag = sub_tag->NextSiblingElement())
	{
		CString PropertySetName(sub_tag->Attribute("PropertySetName"));

		for (const TiXmlElement *sub_record =sub_tag->FirstChildElement(); sub_record; sub_record = sub_record->NextSiblingElement())
		{
			CString Name(sub_record->Attribute("Name"));
			CString Value(sub_record->Attribute("Value"));
			CString Description(sub_record->Attribute("Description"));	
			if (Name == _T("Comments"))
			{
				if(Value == _T("Public"))
					return false;
				else if(Value == _T("Private"))
					return true;

			}

		}
	}
	return status;
}

/////////////////////////////////////////////////////////////////////��Σ���豸��Σ�յȼ�
int CIFCRender::GetElementRiskLevel(int instanceId)
{
	PropertyListXml=*(ifcProject->GetPropertySet(instanceId));
	TiXmlHandle hXmlRoot(&PropertyListXml);
	TiXmlElement *pElem;

	//PSet���Լ��ĸ��ڵ�
	pElem=hXmlRoot.FirstChild("InstancePropertySet").FirstChild("PropertySet").ToElement();
	if (!pElem)
		return true;


	CString numString;
	int level = 0;
	for (const TiXmlElement *sub_tag =pElem; sub_tag; sub_tag = sub_tag->NextSiblingElement())
	{
		CString PropertySetName(sub_tag->Attribute("PropertySetName"));

		for (const TiXmlElement *sub_record =sub_tag->FirstChildElement(); sub_record; sub_record = sub_record->NextSiblingElement())
		{
			CString Name(sub_record->Attribute("Name"));
			CString Value(sub_record->Attribute("Value"));
			CString Description(sub_record->Attribute("Description"));	
			if (Name == _T("RiskLevel"))
			{
				level = atoi((char*)(LPCTSTR)Value);
				int k;
				return level;

			}

		}
	}
	return level;
}

int CIFCRender::GetFloor(int instanceId)
{
	PropertyListXml=*(ifcProject->GetPropertySet(instanceId));
	TiXmlHandle hXmlRoot(&PropertyListXml);
	TiXmlElement *pElem;

	//PSet���Լ��ĸ��ڵ�
	pElem=hXmlRoot.FirstChild("InstancePropertySet").FirstChild("PropertySet").ToElement();
	if (!pElem)
		return true;


	CString numString;
	int level = 0;
	for (const TiXmlElement *sub_tag =pElem; sub_tag; sub_tag = sub_tag->NextSiblingElement())
	{
		CString PropertySetName(sub_tag->Attribute("PropertySetName"));

		for (const TiXmlElement *sub_record =sub_tag->FirstChildElement(); sub_record; sub_record = sub_record->NextSiblingElement())
		{
			CString Name(sub_record->Attribute("Name"));
			CString Value(sub_record->Attribute("Value"));
			CString Description(sub_record->Attribute("Description"));	
			if (Name == _T("Level")|| Name == _T("Base Constraint") || Name == _T("Base Level"))
			{
				char a = Value.GetAt(Value.GetLength()-1);
				
				level = (int)(a-'0');
				//level = atoi((char*)(LPCTSTR)Value);
				//int k;
				return level;

			}

		}
	}
	return level;
}
void CIFCRender::setGridSize(int grid_size)
{
	this->grid_size = grid_size;
}

vector<int> CIFCRender::getDoorX()
{
	return door_x;
}

vector<int> CIFCRender::getDoorY()
{
	return door_y;
}

double CIFCRender::getLength()
{
	return blength;
}

double CIFCRender::getWidth()
{
	return bwidth;
}

double CIFCRender::getOffsetx()
{
	return offsetx;
}

double CIFCRender::getOffsety()
{
	return offsety;
}