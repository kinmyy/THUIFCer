// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "THUIFCer.h"
#include "MainFrm.h"
#include "EntityTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Unable to create the toolbox.\n");
		return -1;      // δ�ܴ���
	}
	
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("δ�ܴ����Ի���\n");
		return -1;		// δ�ܴ���
	}
	

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Unable to create Rebar\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Unable to create the status bar.\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ������ʾ���򽫴��Ƴ�
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY);

	if (!m_wndDockPageLeftTop.Create(_T("LeftTopPanel"), this, CSize(200,600),TRUE,1234))
	{
		TRACE0("δ�ܴ���LeftTop���\n");
		return -1;
	}
	m_wndDockPageLeftTop.SetBarStyle(m_wndDockPageLeftTop.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_wndDockPageLeftTop.EnableDocking (CBRS_ALIGN_ANY);

	if (!m_wndDockPageRightTop.Create(_T("RightTopPanel"), this, CSize(200,600),TRUE,1234))
	{
		TRACE0("δ�ܴ���RightTop���\n");
		return -1;
	}
	m_wndDockPageRightTop.SetBarStyle(m_wndDockPageRightTop.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_wndDockPageRightTop.EnableDocking (CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndDockPageLeftTop, AFX_IDW_DOCKBAR_LEFT);
	/*
	RecalcLayout();
	CRect rect;
	m_wndDockPageLeftTop.GetWindowRect(rect);
	rect.OffsetRect(0,1);   //ƫ��
	DockControlBar(&m_wndDockPageRightTop, AFX_IDW_DOCKBAR_LEFT,rect);
	*/
	DockControlBar(&m_wndDockPageRightTop, AFX_IDW_DOCKBAR_RIGHT);

	if (!m_treeEntitySheet.Create(WS_CHILD|WS_VISIBLE|
		TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), this, 100))
	{
		TRACE0("δ�ܴ���ʵ���б�\n");
		return -1;
	}
	m_wndDockPageLeftTop.AddPage(&m_treeEntitySheet,_T("Entity List"),IDI_ICON_ENTITYTREE);

	if (!m_listPropertySheet.Create(WS_CHILD|WS_VISIBLE,
		CRect(0, 0, 0, 0), this, 100))
	{
		TRACE0("δ�ܴ����������\n");
		return -1;
	}
	m_wndDockPageRightTop.AddPage(&m_listPropertySheet,_T("Property Sheet"),IDI_ICON_PROPERTYLIST);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	AfxGetApp()->m_nCmdShow = SW_SHOWMAXIMIZED;//��󻯴��� 
	m_strTitle = "IFC-Path";
	return CFrameWnd::PreCreateWindow(cs);

	//return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������

CEntityTree *CMainFrame::GetEntityTree()
{
	return &m_treeEntitySheet;
}

CPropertyList *CMainFrame::GetPropertyList()
{
	return &m_listPropertySheet;
}