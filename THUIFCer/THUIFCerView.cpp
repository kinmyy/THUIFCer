// THUIFCerView.cpp : CTHUIFCerView 类的实现
//

#include "stdafx.h"
#include<fstream>
#include "THUIFCer.h"
#include "THUIFCerView.h"
#include "cam.h"
#include "MainFrm.h"
#include "OptionDlg.h"
#include "CarbonAnalysisDlg.h"
#include "StatisticsPlateDlg.h"
#include "StatisticsWallDlg.h"
#include "SetValueDialog.h"
#include"FindPath.h"
#include"AntDialog.h"
#include"AntColony.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool isRenderReady = false;
#define WM_ONOPENIFC	 (WM_USER + 100)//doc打开ifc文件后给view发送的消息

IMPLEMENT_DYNCREATE(CTHUIFCerView, CFormView)

BEGIN_MESSAGE_MAP(CTHUIFCerView, CFormView)
	ON_WM_SIZE()
	ON_COMMAND(ID_3D_RESETSIDE, &CTHUIFCerView::On3dResetside)
	ON_COMMAND(ID_3D_RESETFRONT, &CTHUIFCerView::On3dResetfront)
	ON_COMMAND(ID_3D_RESETTOP, &CTHUIFCerView::On3dResettop)
	ON_COMMAND(ID_EDIT_OPTION, &CTHUIFCerView::OnEditOption)
	ON_COMMAND(ID_FILE_SAVE_ENTITYXML, &CTHUIFCerView::OnFileSaveEntityxml)
	ON_COMMAND(ID_FILE_SAVE_PROPERTYXML, &CTHUIFCerView::OnFileSavePropertyxml)

	ON_MESSAGE(WM_ONOPENIFC, OnOpenIfc)
	ON_COMMAND(ID_ANALYSIS_CARBON, &CTHUIFCerView::OnAnalysisCarbon)
	ON_COMMAND(ID_FILE_SAVE_ASIFC, &CTHUIFCerView::OnFileSaveAsifc)
	ON_COMMAND(ID_FILE_NEW, &CTHUIFCerView::OnFileNew)
	ON_COMMAND(ID_FILE_SAVE, &CTHUIFCerView::OnFileSave)
	ON_COMMAND(ID_SUMMARY_PLATE, &CTHUIFCerView::OnSummaryPlate)
	ON_COMMAND(ID_SUMMARY_WALL, &CTHUIFCerView::OnSummaryWall)
	ON_COMMAND(ID_SUMMARY_DOOR, &CTHUIFCerView::OnSummaryDoor)
	ON_COMMAND(ID_SUMMARY_WINDOW, &CTHUIFCerView::OnSummaryWindow)
	ON_COMMAND(ID_ANALYSIS_SEARCH, &CTHUIFCerView::OnAnalysisSearch)
	ON_WM_TIMER()
	//ON_COMMAND(ID_PATHSIMULATE, &CTHUIFCerView::OnPathsimulate)
	ON_COMMAND(ID_ENERGY, &CTHUIFCerView::OnEnergy)
	ON_COMMAND(ID_FMM, &CTHUIFCerView::OnFmm)
	ON_COMMAND(ID_PATHSIMULATE_POTENTIAL, &CTHUIFCerView::OnPathsimulatePotential)
	ON_COMMAND(ID_PATHSIMULATE_FASTMARCHING, &CTHUIFCerView::OnPathsimulateFastmarching)
	ON_COMMAND(ID_PATHSIMULATE_FMTOOLBOX, &CTHUIFCerView::OnPathsimulateFmtoolbox)
	ON_COMMAND(ID_PATHSIMULATE_MF, &CTHUIFCerView::OnPathsimulateMf)
	ON_COMMAND(ID_PATHSIMULATE_ASTARALGORITHM, &CTHUIFCerView::OnPathsimulateAstaralgorithm)
	ON_COMMAND(ID_PATHSIMULATE_ANTCOLONYALGORITHM, &CTHUIFCerView::OnPathsimulateAntcolonyalgorithm)
	ON_COMMAND(ID_PATHSIMULATE_MULTI32841, &CTHUIFCerView::OnPathsimulateMulti32841)
END_MESSAGE_MAP()

// CTHUIFCerView 构造/析构

CTHUIFCerView::CTHUIFCerView()
	: CFormView(CTHUIFCerView::IDD)
	,start_x(41.0)
	,start_y(27.0)
	,end_x(75.0)
	,end_y(94.0)
	,grid_size(100)
{
	ifcRender = NULL;
	method = 1;
	mm = 0;
	r = 15;
	reset = false;
	Isgo = false;
	IsPause = false;
	clock = false;
	timer = 0;
}

CTHUIFCerView::~CTHUIFCerView()
{
	if (ifcRender)
	{
		delete ifcRender;
		ifcRender = NULL;
	}
}

void CTHUIFCerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CTHUIFCerView::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	//cs.style &= ~WS_BORDER; //无边框
	cs.style=WS_CHILD | WS_VISIBLE | WS_THICKFRAME;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return CFormView::PreCreateWindow(cs);
}
//初始化
void CTHUIFCerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	if (!isRenderReady)
	{	
		m_hwndRenderWindow = this->GetSafeHwnd();
		ifcRender = new CIFCRender(m_hwndRenderWindow);
	}
	if (GetDocument()->ifcProject != NULL)
		SendMessage(WM_ONOPENIFC , 0 , 0);
	((CMainFrame*)GetParentFrame())->GetEntityTree()->SendMessage(ID_UPDATE_ENTITYTREE, 0, 0);
}

LRESULT CTHUIFCerView::OnOpenIfc(WPARAM wParam , LPARAM lParam)
{
	if (ifcRender->LoadIfc(GetDocument()->ifcProject))
	{
		ifcRender->InitializeDeviceBuffer();//初始化顶点缓存
		ifcRender->SetupRender(1);//设置绘制参数（光照，材质~）
		isRenderReady = true;
	}
	return 0;
}
// CTHUIFCerView 诊断

#ifdef _DEBUG
void CTHUIFCerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTHUIFCerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTHUIFCerDoc* CTHUIFCerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTHUIFCerDoc)));
	return (CTHUIFCerDoc*)m_pDocument;
}
#endif //_DEBUG


// CTHUIFCerView 消息处理程序

LRESULT CTHUIFCerView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (isRenderReady && ifcRender->RenderStatus)
		ifcRender->WindowProc(message , wParam , lParam);//窗口过程处理三维模型基本变换

	switch(message)
	{
	case  ID_UPDATE_IFCVIEW://更新视图窗口
		if  (ifcRender->initialized && ifcRender->RenderStatus) 
			ifcRender->Render();
		break;
	}
	return	CFormView::WindowProc(message, wParam, lParam);
}

void CTHUIFCerView::OnSize(UINT nType, int cx, int cy)
{
	SetScrollSizes( MM_TEXT, CSize(cx, cy) );

	CFormView::OnSize(nType, cx, cy);
}

void CTHUIFCerView::OnDraw(CDC* /*pDC*/)
{
	if (false == ifcRender->RenderStatus)
		ifcRender->SetupRender(1);//设置绘制参数（光照，材质~）
	if (ifcRender->initialized && ifcRender->RenderStatus)
		ifcRender->Render();
}

void CTHUIFCerView::On3dResetside()
{
	this->SendMessage(ID_RESET_SIDE, 0, 0);
}

void CTHUIFCerView::On3dResetfront()
{
	this->SendMessage(ID_RESET_FRONT, 0, 0);
}

void CTHUIFCerView::On3dResettop()
{
	this->SendMessage(ID_RESET_TOP, 0, 0);
}

void CTHUIFCerView::OnEditOption()
{
	COptionPageIFC pageIFC;
	COptionSheet sheet(this);
	sheet.AddPage(&pageIFC);
	sheet.DoModal();
}

void CTHUIFCerView::OnFileSaveEntityxml()
{
	CIFCProject *project = GetDocument()->ifcProject;
	if (!project)
		return;
	CFileDialog saveFile(FALSE,_T("*.xml"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST,_T("xml文件(*.xml)|*.xml||"));
	if (saveFile.DoModal() == IDOK)
	{
		project->SaveEnityTree(saveFile.GetPathName());
	}
}

void CTHUIFCerView::OnFileSavePropertyxml()
{
	CIFCProject *project = GetDocument()->ifcProject;
	if (!project)
		return;
	CFileDialog saveFile(FALSE,_T("*.xml"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST,_T("xml文件(*.xml)|*.xml||"));
	if (saveFile.DoModal() == IDOK)
	{
		project->SavePropertySet(saveFile.GetPathName());
	}
}

void CTHUIFCerView::OnAnalysisCarbon()
{
	CCarbonAnalysisDlg CarbonDlg;
	CarbonDlg.DoModal();
}

void CTHUIFCerView::OnFileSaveAsifc()
{
	CIFCProject *project = GetDocument()->ifcProject;
	if (!project)
		return;
	CFileDialog saveFile(FALSE,_T("*.ifc"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST,_T("ifc文件(*.ifc)|*.ifc||"));
	if (saveFile.DoModal() == IDOK)
	{
		project->SaveIFCProject(saveFile.GetPathName());
	}
}

void CTHUIFCerView::OnFileNew()
{
	CIFCProject *project = GetDocument()->ifcProject;
	if (project)
	{
		//清空左边的树
		CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
		pMainFrame->GetEntityTree()->SendMessage(ID_UPDATE_CLEAR, 0, 0);
		//清空右边的属性列表
		pMainFrame->GetPropertyList()->SendMessage(ID_UPDATE_CLEAR, 0, 0);
		//重置3维视图
		ifcRender->RenderClear();
		//关闭模型
		project->CloseIFCProject();
	}
	//清除和旧文件的关联
	GetDocument()->OnNewDocument();
	
	//创建新工程
	if (project)
	{
		project->CreateIFCProject(GetDocument()->GetPathName(),GetDocument()->SchemaPath);
		//Hello Wall！
		//TODO
	}
}

void CTHUIFCerView::OnFileSave()
{
	CIFCProject *project = GetDocument()->ifcProject;
	if (!project)
		return;
	project->SaveIFCProject();
}

//统计楼板
void CTHUIFCerView::OnSummaryPlate()
{
	CStatisticsPlateDlg PlateDlg;
	PlateDlg.DoModal();
}

//统计墙
void CTHUIFCerView::OnSummaryWall()
{
	CStatisticsWallDlg WallDlg;
	WallDlg.DoModal();
}

//统计门
void CTHUIFCerView::OnSummaryDoor()
{
	// TODO: 在此添加命令处理程序代码
}

//统计窗
void CTHUIFCerView::OnSummaryWindow()
{
	// TODO: 在此添加命令处理程序代码
}

//检索工程中的构件
void CTHUIFCerView::OnAnalysisSearch()
{
	// TODO: 在此添加命令处理程序代码
}

void CTHUIFCerView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	


	///////////////////////////////////////////////人工势场相关操作

	//ofstream writeFile(_T("aa.txt"),ios::app);
	if(method == 1)
	{
		int flag=0;
		if (IsPause==false)
		{
			for(int i=0;i<Controllor.size();i++)
			{
				vector<Particle>temp_obs;
				Particle temp1;
				temp1.Locality=Controllor.at(i).m_Position;
				temp1.Radius=Controllor.at(i).m_Radiu;

				for(int j=0;j<Controllor.size();j++)
				{
					int count=0;
					Particle temp2;
					temp2.Locality=Controllor.at(j).m_Position;
					temp2.Radius=Controllor.at(j).m_Radiu;
					if (j!=i)
					{
						temp_obs.push_back(temp2);//增加动态障碍,即运动对象之间互相构成障碍
					}

					if (j!=i&& ( Controllor.at(j).m_Position.x !=Controllor.at(j).m_Destination.x ||  Controllor.at(j).m_Position.y !=Controllor.at(j).m_Destination.y ) )//粒子未到目标时优先级才有效
					{
				
						if (Dis(temp2,temp1)>4*(temp1.Radius+temp2.Radius))
						{
							Controllor.at(i).m_Speed=35;//距离较远，全速前进
					
						}
						else if (Dis(temp2,temp1)<4*(temp1.Radius+temp2.Radius)&&Dis(temp2,temp1)>2*(temp1.Radius+temp2.Radius))
						{
					
							if (i<j)//后面序号粒子优先级高
								Controllor.at(i).m_Speed=15;//距离中等，半速前进
					
						}
						else if (Dis(temp2,temp1)<2*(temp1.Radius+temp2.Radius))
						{
								if (i<j)//后面序号粒子优先级高
								{
									count++;
									Controllor.at(i).Pause();//距离太近，暂停前进

								}
						}
				
					}
				else//在所有粒子到达目标后，该粒子全速前进
					if (j!=i)
					{
						Controllor.at(i).m_Speed=35;

					}
				if (count!=0)
					Controllor.at(i).ReSet=true;
				else
					Controllor.at(i).ReSet=false;
			}

			if (Controllor.at(i).ReSet==false)
			{
				Controllor.at(i).m_State = 1;
			}

			Potential mesh;
			mesh = Controllor.at(i);
			vector<D3DXVECTOR3>disToWall;
			disToWall = GetDistance(mesh.m_Position);
			for (int k=0;k<disToWall.size();k++)
			{
				obstacle[k].Locality.x = disToWall[k].x;
				obstacle[k].Locality.y = disToWall[k].y;
				obstacle[k].Radius = disToWall[k].z;
				temp_obs.push_back(obstacle[k]);//增加静态障碍
			}

			Controllor.at(i).Move(temp_obs);
			temp_obs.clear();

			Place posi;
			posi.x=mesh.m_Position.x;
			posi.y = mesh.m_Position.y;
			int offset=mesh.m_Radiu;
			D3DXVECTOR3 pp;
			pp.x = posi.x/100;
			pp.y = posi.y/100;
			pp.z = -0.3f;
			ifcRender->footPoints.push_back(pp);
			if (i==0)
			{
			//draw(posi,offset,138,43,226);
			//writeFile<<posi.x<<"   "<<posi.y<<endl;
			//writeFile.close();
			//GetDistance(posi);
				if (TRUE == ifcRender->RenderStatus)
				{
					ifcRender->Pos.x = posi.x/100;
					ifcRender->Pos.y = posi.y/100;
					ifcRender->Pos.z = 0;
				}
				Invalidate(FALSE);
			}
			else if (i==1)
			{
				//draw(posi,offset,218,112,214);
			}
			else if (i==2)
			{
				//draw(posi,offset,25,25,112);
			}
			else			
			{
				//draw(posi,offset,135,38,87);
			}
		
			if (posi.x == mesh.m_Destination.x && posi.y == mesh.m_Destination.y)
			{
				flag++;
			}

		}
	}


		if (flag==Controllor.size())
		{

			start.clear();
			goal.clear();
			obstacle.clear();
			r=15;
			Controllor.clear();
			reset = false;
			Isgo = false;
			IsPause = false;
			clock = false;
			KillTimer(1);
			MessageBox(_T("Finish!"),0,0);

		}
		for (int i=0;i<Controllor.size();i++)
		{
			if (Controllor[i].my==true)
			for (int j=0;j<Controllor.at(i).virtual_obs.size();j++)
			{
			
			//show(Controllor.at(i).virtual_obs[j].Locality,Controllor.at(i).virtual_obs[j].Radius);
			}
		}
	}

	else if(method == 2)
	{
		if (TRUE == ifcRender->RenderStatus)
		{
			ifcRender->Pos.x =(fmmPath[timer].x*(ifcRender->getLength()/grid_size)-(ifcRender->getLength()/2))/100;
			ifcRender->Pos.y = -(fmmPath[timer].y*(ifcRender->getWidth()/grid_size)-(ifcRender->getWidth()/2))/100;
			ifcRender->Pos.z = fmmPath[timer].z;
		}
		Invalidate(FALSE);
		
		timer++;
		if ( timer == fmmPath.size())
		{
			KillTimer(1);
			fmmPath.clear();
			//ifcRender->footPoints.clear();
			clock = false;
			timer = 0;
			MessageBox(_T("Finish!"),0,0);
		}
	}

	else if(method == 3)
	{
		timer = 0;
		ifcRender->Pos.x = (fmmPath[timer].x*(ifcRender->getLength()/grid_size)-(98))/100;
		ifcRender->Pos.y = -(fmmPath[timer].y*(ifcRender->getWidth()/grid_size)-(ifcRender->getWidth()/2))/100;
		ifcRender->Pos.z = fmmPath[timer].z+0.01;
		Invalidate(FALSE);

		timer++;
		if ( timer == fmmPath.size())
		{
			KillTimer(1);
			fmmPath.clear();
			//ifcRender->footPoints.clear();
			clock = false;
			timer = 0;
			MessageBox(_T("Finish!"),0,0);
		}
	}

	//UpdateWindow();
	CFormView::OnTimer(nIDEvent);
}

//void CTHUIFCerView::OnPathsimulate()
//{
//	// TODO: 在此添加命令处理程序代码
//	
//}

double CTHUIFCerView::Dis(Particle p1, Particle p2)
{
	return sqrt(double((p1.Locality.x-p2.Locality.x)*(p1.Locality.x-p2.Locality.x)+(p1.Locality.y-p2.Locality.y)*(p1.Locality.y-p2.Locality.y)))-(p1.Radius+p2.Radius);
}

//需要多增加一个判断垂足是否在线段上的操作
vector<D3DXVECTOR3> CTHUIFCerView::GetDistance(Place posi)
{
	vector<D3DXVECTOR3>v;
	D3DXVECTOR3 v1;
	D3DXVECTOR3 v2;
	vector<double>dd;
	vector<D3DXVECTOR3>d;
	double k,b;//直线方程斜率和截距
	double sum_x,sum_y;
	//int a = ifcRender->mainPoints[ifcRender->mainPoints.size()-1].z;
	for (int i=0;i<=ifcRender->mainPoints[ifcRender->mainPoints.size()-1].z;i++)
	{
		v = GetGroup(i);
		sum_x = 0;
		sum_y = 0;
		for (int j=0;j<v.size();j++)
		{
			sum_x+=v[j].x;
			sum_y+=v[j].y;
		}
		for (int j=0;j<v.size()-1;j++)
		{
			v1 = v[j];
			v2 = v[j+1];
			if(v1.x == v2.x)
			{
				dd.push_back(abs(posi.x - v1.x));
			}
			else
			{
				k = (v2.y - v1.y)/(v2.x - v1.x);
				b = v1.y - ( k*v1.x );
				dd.push_back((abs(k*posi.x-posi.y+b))/sqrt(k*k+1));
			}
		}

		//判断第一个顶点和最后一个顶点的情况
		v1 = v[0];
		v2 = v[v.size()-1];
		if(v1.x == v2.x)
		{
			dd.push_back(abs(posi.x - v1.x));
		}
		else
		{
			k = (v2.y - v1.y)/(v2.x - v1.x);
			b = v1.y - ( k*v1.x );
			dd.push_back(abs(posi.x - v1.x));
		}

		double min;
		min = dd[0];
		for (int q=0;q<dd.size();q++)
		{
			if (min > dd[q] )
			{
				min = dd[q];
			}
		}
		D3DXVECTOR3 temp;
		temp.x = sum_x/v.size();
		temp.y = sum_y/v.size();
		//temp.z = min;
		double kk;
		kk = (temp.x-posi.x)*(temp.x-posi.x)+(temp.y-posi.y)*(temp.y-posi.y);
		temp.z = sqrt(kk);
		d.push_back(temp);
		v.clear();
		dd.clear();
	}

	return d;
	
}

vector<D3DXVECTOR3>CTHUIFCerView::GetGroup(int i)
{
	vector<D3DXVECTOR3>tp;
	tp = ifcRender->mainPoints;
	vector<D3DXVECTOR3>v;
	for (int j=0;j<tp.size();j++)
	{
		if(tp[j].z == i)
		{
			v.push_back(tp[j]);
		}
	}
	return v;

}
void CTHUIFCerView::OnEnergy()
{
	// TODO: 在此添加命令处理程序代码
	ifcRender->CalculatePath();


	Particle t;
	t.Locality.x = 0.8*100;
	t.Locality.y = 0.3*100;
	t.Radius = 10;//参数待修改
	start.push_back(t);

	t.Locality.x = 0*100;
	t.Locality.y =0*100;
	t.Radius = 10;
	goal.push_back(t);

	int m = start.size();
	int n = goal.size();

	if( m == 0 || n == 0)
		return;

	CString strNumber1,strNumber2;
	strNumber1.Format(_T("%d"),m);
	strNumber2.Format(_T("%d"),n);
	if( m != n )
	{
		MessageBox(_T("运动粒子数与目标位置数不同！\n运动粒子数：") + strNumber1 + _T("目标位置数：") + strNumber2, _T("提示") );
		return;
	}

	if( Controllor.size() != 0 )
		Controllor.clear();

	for(int i=0;i<start.size();i++)
	{
		Potential p;
		p.m_Position = start[i].Locality;
		p.m_Destination = goal[i].Locality;
		p.m_Radiu = start[i].Radius;
		Controllor.push_back(p);
	}

	//读取模型中的墙数据
	vector<D3DXVECTOR3>wPoints;
	wPoints = ifcRender->mainPoints;
	Particle obs;
	for(int i=0;i<=wPoints[wPoints.size()-1].z;i++)
	{

		obs.Locality.x = wPoints[i].x;
		obs.Locality.y = wPoints[i].y;
		obs.Radius = 0;
		obstacle.push_back(obs);
	}



	

	Isgo = true;
	clock = false;
	//人工势场初始化完毕



	//OnReady
	IsPause=false;
	for (int i=0;i<Controllor.size();i++)
	{
		Controllor.at(i).ReadyMOVE();
	}
	if (clock==false)
	{
		SetTimer(1,100,NULL);
		clock=true;

	}
}

void CTHUIFCerView::OnFmm()
{
	// TODO: 在此添加命令处理程序代码
	method = 2;
	/*fm = new FastMarching();
	Mm fmmPath = fm.fmm();
	int n = fm.getTimeCounter();
	D3DXVECTOR3 pp;
	for (int i=1;i<=n;i++)
	{
		pp.x = fmmPath.r(i,1);
		pp.y = fmmPath.r(i,2);
		pp.z = -0.3f;
		ifcRender->footPoints.push_back(pp);
	}*/

	SetTimer(1,100,NULL);
	
}

void CTHUIFCerView::OnPathsimulatePotential()
{
	// TODO: 在此添加命令处理程序代码
	ifcRender->CalculatePath();


	Particle t;
	t.Locality.x = 0.8*100;
	t.Locality.y = 0.3*100;
	t.Radius = 10;//参数待修改
	start.push_back(t);

	t.Locality.x = 0*100;
	t.Locality.y =0*100;
	t.Radius = 10;
	goal.push_back(t);

	int m = start.size();
	int n = goal.size();

	if( m == 0 || n == 0)
		return;

	CString strNumber1,strNumber2;
	strNumber1.Format(_T("%d"),m);
	strNumber2.Format(_T("%d"),n);
	if( m != n )
	{
		MessageBox(_T("运动粒子数与目标位置数不同！\n运动粒子数：") + strNumber1 + _T("目标位置数：") + strNumber2, _T("提示") );
		return;
	}

	if( Controllor.size() != 0 )
		Controllor.clear();

	for(int i=0;i<start.size();i++)
	{
		Potential p;
		p.m_Position = start[i].Locality;
		p.m_Destination = goal[i].Locality;
		p.m_Radiu = start[i].Radius;
		Controllor.push_back(p);
	}

	//读取模型中的墙数据
	vector<D3DXVECTOR3>wPoints;
	wPoints = ifcRender->mainPoints;
	Particle obs;
	for(int i=0;i<=wPoints[wPoints.size()-1].z;i++)
	{

		obs.Locality.x = wPoints[i].x;
		obs.Locality.y = wPoints[i].y;
		obs.Radius = 0;
		obstacle.push_back(obs);
	}

	Isgo = true;
	clock = false;
	//人工势场初始化完毕



	//OnReady
	IsPause=false;
	for (int i=0;i<Controllor.size();i++)
	{
		Controllor.at(i).ReadyMOVE();
	}
	if (clock==false)
	{
		SetTimer(1,100,NULL);
		clock=true;

	}
}

void CTHUIFCerView::OnPathsimulateFastmarching()
{

	
	SetValueDialog dlg;
	dlg.grid_size = grid_size;
	ofstream readPath("D:\\emptygrid.txt");
	int k=1;
	for (int i=0;i<100;i++)
	{
		for (int j=0;j<100;j++)
		{
			readPath<<k<<" ";
		}
		readPath<<endl;

	}
	//ifcRender->stepCounter = 0;

	if (parDialog.DoModal() == IDOK)
	{
		//ifcRender->footPoints.clear();
		grid_size = parDialog.grid_size;
		int startPos = parDialog.getStartPos();
		int endPos = parDialog.getEndPos();
		fm = new FastMarching();
		fm->setGridSize(grid_size);
		ifcRender->setGridSize(grid_size);

		ifcRender->CalculatePath();
		method = 2;

		vector<int>x;
		vector<int>y;
		x = ifcRender->getDoorX();
		y = ifcRender->getDoorY();

		//fmmPath = fm->fmm(start_x,start_y,end_x,end_y);
		LARGE_INTEGER nFreq;
		LARGE_INTEGER nBeginTime;
		LARGE_INTEGER nEndTime;
		double time;
		QueryPerformanceFrequency(&nFreq);
		QueryPerformanceCounter(&nBeginTime);
		fmmPath = fm->fmm(x[startPos],y[startPos],x[endPos],y[endPos],_T("E:\\grids100.txt"));
		//fmmPath = fm->fmm(86,59,32,44,_T("E:\\grids100.txt"));
		QueryPerformanceCounter(&nEndTime);
		time = (double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart;
		
		D3DXVECTOR3 pp;

		for (int i=0;i<fmmPath.size();i++)
		{
			/*pp.x =(fmmPath[i].x*2-100)/100;
			pp.y = -(fmmPath[i].y*1.28-64)/100;*/

			pp.x =(fmmPath[i].x*(ifcRender->getLength()/grid_size)-(ifcRender->getOffsetx()))/100;
			pp.y = -(fmmPath[i].y*(ifcRender->getWidth()/grid_size)-(ifcRender->getOffsety()))/100;

			pp.z = 0.025f;
			ifcRender->footPoints.push_back(pp);
		}

		double pathDistance;
		pathDistance = GetPathDistance(ifcRender->footPoints);
		if (clock==false)
		{
			if(grid_size == 50)
				SetTimer(1,100,NULL);
			else if(grid_size == 100)
				SetTimer(1,50,NULL);
			else if(grid_size == 200)
				SetTimer(1,25,NULL);
			clock=true;

		}
	}
	

	
	// TODO: 在此添加命令处理程序代码
}

void CTHUIFCerView::OnPathsimulateFmtoolbox()
{
	// TODO: 在此添加命令处理程序代码

	if (parDialog.DoModal() == IDOK)
	{
	ifcRender->CalculatePath();
	ifstream readPath("D:\\tbpath1.txt");
	D3DXVECTOR3 a;
	
	vector<D3DXVECTOR3>tbpath;
	for (int i=0;i<2000;i++)
	{
		readPath>>a.x;
		readPath>>a.y;
		readPath>>a.z;
		fmmPath.push_back(a);
	}
	readPath.close();

	D3DXVECTOR3 pp;
	for (int i=0;i<fmmPath.size();i++)
	{
		pp.x =(fmmPath[i].x*105.3/500-98)/100;
		pp.y = -(fmmPath[i].y*105/500-64)/100;

		pp.z = 0.02f;
		ifcRender->footPoints.push_back(pp);
	}

	method  = 3;
	if (clock==false)
	{
		SetTimer(1,10,NULL);
		clock=true;

	}
	}

}

//多层楼的情况处理
void CTHUIFCerView::OnPathsimulateMf()
{
	// TODO: 在此添加命令处理程序代码

	SetValueDialog dlg;
	dlg.grid_size = grid_size;
	vector<D3DXVECTOR3>secondPath;
	//ifcRender->stepCounter = 0;

	if (parDialog.DoModal() == IDOK)
	{
		//ifcRender->footPoints.clear();
		grid_size = parDialog.grid_size;
		int startPos = parDialog.getStartPos();
		int endPos = parDialog.getEndPos();

		fm = new FastMarching();
		fm->setGridSize(grid_size);
		ifcRender->setGridSize(grid_size);

		ifcRender->CalculatePath_MF();
		method = 2;

		vector<int>x;
		vector<int>y;
		x = ifcRender->getDoorX();
		y = ifcRender->getDoorY();

		if (ifcRender->dstorey[startPos] == ifcRender->dstorey[endPos])
		{
			if (ifcRender->dstorey[startPos] == 1)
				fmmPath = fm->fmm(x[startPos],y[startPos],x[endPos],y[endPos],_T("E:\\grids100.txt"));
			else if(ifcRender->dstorey[startPos] == 2)
				fmmPath = fm->fmm(x[startPos],y[startPos],x[endPos],y[endPos],_T("E:\\grids200.txt"));

			D3DXVECTOR3 pp;

			for (int i=0;i<fmmPath.size();i++)
			{
				/*pp.x =(fmmPath[i].x*2-100)/100;
				pp.y = -(fmmPath[i].y*1.28-64)/100;*/

				pp.x =(fmmPath[i].x*(ifcRender->getLength()/grid_size)-(ifcRender->getLength()/2))/100;
				pp.y = -(fmmPath[i].y*(ifcRender->getWidth()/grid_size)-(ifcRender->getWidth()/2))/100;

				if(ifcRender->dstorey[startPos]== 1)
					pp.z = -0.15f;
				else if(ifcRender->dstorey[startPos] == 2)
					pp.z = -0.5f;
				ifcRender->footPoints.push_back(pp);
			}
		}

		else {
			vector<int>stairx1;
			vector<int>stairy1;
			vector<int>stairx2;
			vector<int>stairy2;
			vector<double>distance;
			vector<D3DXVECTOR3>sPos = ifcRender->stairPos;
			vector<int>storey = ifcRender->dstorey;

			
			for (int i=0;i<sPos.size();i++)
			{
				if (sPos[i].z == storey[startPos])
				{
					stairx1.push_back(sPos[i].x);
					stairy1.push_back(sPos[i].y);
				}
				else if (sPos[i].z = storey[endPos])
				{
					stairx2.push_back(sPos[i].x);
					stairy2.push_back(sPos[i].y);
				}
			}

			if(stairx2[0] == 10)
				stairx2[0] = 25;
			else if(stairx1[0] == 10)
				stairx1[0] = 25;

			D3DXVECTOR3 pp;

			for (int i=0;i<stairx1.size();i++)
			{
				if (storey[startPos] == 1)
				{
					fmmPath = fm->fmm(x[startPos],y[startPos],stairx1[i],stairy1[i],_T("E:\\grids100.txt"));
					distance.push_back(GetPathDistance(fmmPath));
				}
				else if (storey[startPos] == 2)
				{
					fmmPath = fm->fmm(x[startPos],y[startPos],stairx1[i],stairy1[i],_T("E:\\grids200.txt"));
					distance.push_back(GetPathDistance(fmmPath));
				}
				for (int i=0;i<fmmPath.size();i++)
				{

					pp.x =(fmmPath[i].x*(ifcRender->getLength()/grid_size)-(ifcRender->getLength()/2))/100;
					pp.y = -(fmmPath[i].y*(ifcRender->getWidth()/grid_size)-(ifcRender->getWidth()/2))/100;

					if(ifcRender->dstorey[startPos]== 1)
					{
						pp.z = -0.31f;
						fmmPath[i].z = -0.22f;
					}
					else if(ifcRender->dstorey[startPos] == 2)
					{
						pp.z = 0.06f;
						fmmPath[i].z = 0.15f;
					}
					ifcRender->footPoints.push_back(pp);
				}
			}

			for (int i=0;i<stairx2.size();i++)
			{
				if (storey[endPos] == 1)
				{
					secondPath = fm->fmm(stairx2[i],stairy2[i],x[endPos],y[endPos],_T("E:\\grids100.txt"));
					distance[i]+= GetPathDistance(secondPath);
				}
				else if (storey[endPos] == 2)
				{
					secondPath = fm->fmm(stairx2[i],stairy2[i],x[endPos],y[endPos],_T("E:\\grids200.txt"));
					distance[i]+= GetPathDistance(secondPath);
				}
				for (int i=0;i<secondPath.size();i++)
				{

					pp.x =(secondPath[i].x*(ifcRender->getLength()/grid_size)-(ifcRender->getLength()/2))/100;
					pp.y = -(secondPath[i].y*(ifcRender->getWidth()/grid_size)-(ifcRender->getWidth()/2))/100;

					if(ifcRender->dstorey[endPos]== 1)
						pp.z = -0.31f;
					else if(ifcRender->dstorey[endPos] == 2)
						pp.z = 0.06f;
					ifcRender->footPoints.push_back(pp);
				}
			}

			for (int i=0;i<secondPath.size();i++)
			{
				if(ifcRender->dstorey[endPos] == 1)
					secondPath[i].z = -0.22f;
				else if(ifcRender->dstorey[endPos] == 2)
					secondPath[i].z = 0.15f;
				fmmPath.push_back(secondPath[i]);
			}

			
			
			
		}
		

		if (clock==false)
		{
			SetTimer(1,50,NULL);
			clock=true;

		}

		

		
	}

}

double CTHUIFCerView::GetPathDistance(vector<D3DXVECTOR3>fmmPath)
{
	double pathDistance=0;
	double unitDistance = 0;
	for (int i=0;i<fmmPath.size()-1;i++)
	{
		unitDistance = sqrt((fmmPath[i+1].y-fmmPath[i].y)*(fmmPath[i+1].y-fmmPath[i].y)+(fmmPath[i+1].x - fmmPath[i].x)*(fmmPath[i+1].x - fmmPath[i].x));
		pathDistance += unitDistance;
	}
	return pathDistance;
}

void CTHUIFCerView::OnPathsimulateAstaralgorithm()
{
	// TODO: 在此添加命令处理程序代码
	SetValueDialog dlg;
	dlg.grid_size = grid_size;
	ifcRender->footPoints.clear();
	fmmPath.clear();
	//ifcRender->stepCounter = 0;

	if (parDialog.DoModal() == IDOK)
	{
		//ifcRender->footPoints.clear();
		grid_size = parDialog.grid_size;
		int startPos = parDialog.getStartPos();
		int endPos = parDialog.getEndPos();
		CMask *g_mask;
		ifcRender->setGridSize(grid_size);
		ifcRender->CalculatePath();
		//method = 2;

		vector<int>x;
		vector<int>y;
		x = ifcRender->getDoorX();
		y = ifcRender->getDoorY();
		Point2D start;
		Point2D end;
		start.x = x[startPos];
		start.y = y[startPos];
		end.x = x[endPos];
		end.y = y[endPos];
		g_mask = new CMask(grid_size,start,end,"E:\\grids100.txt");
		LARGE_INTEGER nFreq;
		LARGE_INTEGER nBeginTime;
		LARGE_INTEGER nEndTime;
		double time;
		QueryPerformanceFrequency(&nFreq);
		QueryPerformanceCounter(&nBeginTime);
		g_mask->StartFindPath();
		g_mask->FindPath();
		QueryPerformanceCounter(&nEndTime);
		time = (double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart;

		std::vector<Point2D>::iterator iter;
		Point2D tmp;
		D3DXVECTOR3 a;
		for(iter = g_mask->m_FindedPath.begin(); iter != g_mask->m_FindedPath.end(); iter++)
		{
			tmp = (*iter);
			a.x = tmp.x;
			a.y = tmp.y;
			a.z = -0.1f;
			fmmPath.push_back(a);
		}
	}

	D3DXVECTOR3 pp;
	for (int i=0;i<fmmPath.size();i++)
	{
		pp.x =(fmmPath[i].x*(ifcRender->getLength()/grid_size)-(ifcRender->getLength()/2))/100;
		pp.y = -(fmmPath[i].y*(ifcRender->getWidth()/grid_size)-(ifcRender->getWidth()/2))/100;

		pp.z = -0.1f;
		ifcRender->footPoints.push_back(pp);
	}

	double pathDistance;
	pathDistance = GetPathDistance(ifcRender->footPoints);

	if (clock==false)
	{
		SetTimer(1,50,NULL);
		clock=true;

	}
}

void CTHUIFCerView::OnPathsimulateAntcolonyalgorithm()
{
	// TODO: 在此添加命令处理程序代码
	AntDialog dlg;
	dlg.grid_size = grid_size;
	//ifcRender->stepCounter = 0;

	if (antDialog.DoModal() == IDOK)
	{
		//ifcRender->footPoints.clear();
		grid_size = antDialog.grid_size;
		int startPos = antDialog.getStartPos();
		int endPos = antDialog.getEndPos();
		int m_gener = antDialog.m_gener;
		double m_beta = antDialog.m_beta;
		double m_alpha = antDialog.m_alpha;
		antColony = new AntColony();
		antColony->setGridSize(grid_size);
		ifcRender->setGridSize(grid_size);
		ifcRender->CalculatePath();

		vector<int>x;
		vector<int>y;
		x = ifcRender->getDoorX();
		y = ifcRender->getDoorY();

		int StartPoint = x[startPos]*grid_size + y[startPos];
		int TarGetG = x[endPos]*grid_size+y[endPos];

		antColony->setStartPoint(StartPoint);
		antColony->setTarGetG(TarGetG);

		LARGE_INTEGER nFreq;
		LARGE_INTEGER nBeginTime;
		LARGE_INTEGER nEndTime;
		double time;
		QueryPerformanceFrequency(&nFreq);
		QueryPerformanceCounter(&nBeginTime);
		fmmPath = antColony->OnStartsimu(m_gener,m_beta,m_alpha);
		QueryPerformanceCounter(&nEndTime);
		time = (double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart;
		
		D3DXVECTOR3 pp;

		for (int i=0;i<fmmPath.size();i++)
		{
			/*pp.x =(fmmPath[i].x*2-100)/100;
			pp.y = -(fmmPath[i].y*1.28-64)/100;*/

			pp.x =(fmmPath[i].x*(ifcRender->getLength()/grid_size)-(ifcRender->getLength()/2))/100;
			pp.y = -(fmmPath[i].y*(ifcRender->getWidth()/grid_size)-(ifcRender->getWidth()/2))/100;

			pp.z = -0.1f;
			ifcRender->footPoints.push_back(pp);
		}

		double pathDistance;
		pathDistance = GetPathDistance(ifcRender->footPoints);

		if (clock==false)
		{
			/*if(grid_size == 50)
				SetTimer(1,100,NULL);
			else if(grid_size == 100)*/
				SetTimer(1,50,NULL);
			/*else if(grid_size == 200)
				SetTimer(1,25,NULL);*/
			clock=true;

		}
	}
}

void CTHUIFCerView::OnPathsimulateMulti32841()
{
	// TODO: 在此添加命令处理程序代码
	SetValueDialog dlg;
	dlg.grid_size = grid_size;
	vector<D3DXVECTOR3>secondPath;
	//ifcRender->stepCounter = 0;

	if (parDialog.DoModal() == IDOK)
	{
		//ifcRender->footPoints.clear();
		grid_size = parDialog.grid_size;
		int startPos = parDialog.getStartPos();
		int endPos = parDialog.getEndPos();

		fm = new FastMarching();
		fm->setGridSize(grid_size);
		ifcRender->setGridSize(grid_size);

		ifcRender->CalculatePath_MF();
		method = 2;

		vector<int>x;
		vector<int>y;
		x = ifcRender->getDoorX();
		y = ifcRender->getDoorY();

		if (ifcRender->dstorey[startPos] == ifcRender->dstorey[endPos])
		{
			if (ifcRender->dstorey[startPos] == 1)
				fmmPath = fm->fmm(x[startPos],y[startPos],x[endPos],y[endPos],_T("E:\\grids100.txt"));
			else if(ifcRender->dstorey[startPos] == 2)
				fmmPath = fm->fmm(x[startPos],y[startPos],x[endPos],y[endPos],_T("E:\\grids200.txt"));

			D3DXVECTOR3 pp;

			for (int i=0;i<fmmPath.size();i++)
			{
				/*pp.x =(fmmPath[i].x*2-100)/100;
				pp.y = -(fmmPath[i].y*1.28-64)/100;*/

				pp.x =(fmmPath[i].x*(ifcRender->getLength()/grid_size)-(ifcRender->getLength()/2))/100;
				pp.y = -(fmmPath[i].y*(ifcRender->getWidth()/grid_size)-(ifcRender->getWidth()/2))/100;

				if(ifcRender->dstorey[startPos]== 1)
					pp.z = -0.15f;
				else if(ifcRender->dstorey[startPos] == 2)
					pp.z = -0.5f;
				ifcRender->footPoints.push_back(pp);
			}
		}

		else {
			vector<int>stairx1;
			vector<int>stairy1;
			vector<int>stairx2;
			vector<int>stairy2;
			vector<double>distance;
			vector<D3DXVECTOR3>sPos = ifcRender->stairPos;
			vector<int>storey = ifcRender->dstorey;


			for (int i=0;i<sPos.size();i++)
			{
				if (sPos[i].z == storey[startPos])
				{
					stairx1.push_back(sPos[i].x);
					stairy1.push_back(sPos[i].y);
				}
				else if (sPos[i].z = storey[endPos])
				{
					stairx2.push_back(sPos[i].x);
					stairy2.push_back(sPos[i].y);
				}
			}

			if(stairx2[0] == 10)
				stairx2[0] = 25;
			else if(stairx1[0] == 10)
				stairx1[0] = 25;

			D3DXVECTOR3 pp;

			for (int i=0;i<stairx1.size();i++)
			{
				if (storey[startPos] == 1)
				{
					fmmPath = fm->fmm(x[startPos],y[startPos],stairx1[i],stairy1[i],_T("E:\\grids100.txt"));
					distance.push_back(GetPathDistance(fmmPath));
				}
				else if (storey[startPos] == 2)
				{
					fmmPath = fm->fmm(x[startPos],y[startPos],stairx1[i],stairy1[i],_T("E:\\grids200.txt"));
					distance.push_back(GetPathDistance(fmmPath));
				}
				for (int i=0;i<fmmPath.size();i++)
				{

					pp.x =(fmmPath[i].x*(ifcRender->getLength()/grid_size)-(ifcRender->getLength()/2))/100;
					pp.y = -(fmmPath[i].y*(ifcRender->getWidth()/grid_size)-(ifcRender->getWidth()/2))/100;

					if(ifcRender->dstorey[startPos]== 1)
					{
						pp.z = -0.31f;
						fmmPath[i].z = -0.22f;
					}
					else if(ifcRender->dstorey[startPos] == 2)
					{
						pp.z = 0.06f;
						fmmPath[i].z = 0.15f;
					}
					ifcRender->footPoints.push_back(pp);
				}
			}

			for (int i=0;i<stairx2.size();i++)
			{
				if (storey[endPos] == 1)
				{
					secondPath = fm->fmm(stairx2[i],stairy2[i],x[endPos],y[endPos],_T("E:\\grids100.txt"));
					distance[i]+= GetPathDistance(secondPath);
				}
				else if (storey[endPos] == 2)
				{
					secondPath = fm->fmm(stairx2[i],stairy2[i],x[endPos],y[endPos],_T("E:\\grids200.txt"));
					distance[i]+= GetPathDistance(secondPath);
				}
				for (int i=0;i<secondPath.size();i++)
				{

					pp.x =(secondPath[i].x*(ifcRender->getLength()/grid_size)-(ifcRender->getLength()/2))/100;
					pp.y = -(secondPath[i].y*(ifcRender->getWidth()/grid_size)-(ifcRender->getWidth()/2))/100;

					if(ifcRender->dstorey[endPos]== 1)
						pp.z = -0.31f;
					else if(ifcRender->dstorey[endPos] == 2)
						pp.z = 0.06f;
					ifcRender->footPoints.push_back(pp);
				}
			}

			for (int i=0;i<secondPath.size();i++)
			{
				if(ifcRender->dstorey[endPos] == 1)
					secondPath[i].z = -0.22f;
				else if(ifcRender->dstorey[endPos] == 2)
					secondPath[i].z = 0.15f;
				fmmPath.push_back(secondPath[i]);
			}




		}


		if (clock==false)
		{
			SetTimer(1,50,NULL);
			clock=true;

		}

	}
}
