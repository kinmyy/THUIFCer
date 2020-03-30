// THUIFCerDoc.cpp : CTHUIFCerDoc 类的实现
//

#include "stdafx.h"
#include "THUIFCer.h"
#include "THUIFCerDoc.h"
#include "THUIFCerView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ONOPENIFC	 (WM_USER + 100)

// CTHUIFCerDoc
/*
extern	STRUCT_INSTANCES	* first_instance;
extern	TCHAR	* ifcSchemaName;
*/


IMPLEMENT_DYNCREATE(CTHUIFCerDoc, CDocument)

BEGIN_MESSAGE_MAP(CTHUIFCerDoc, CDocument)
END_MESSAGE_MAP()


// CTHUIFCerDoc 构造/析构

CTHUIFCerDoc::CTHUIFCerDoc()
{
	TCHAR path[MAX_PATH]; //最长260
	GetModuleFileName(NULL, path, MAX_PATH);
	AppPath=CString(path);

	int position=AppPath.ReverseFind('\\');
	AppPath=AppPath.Left(position);
	SchemaPath=AppPath+_T("\\IFC2X3_TC1.exp");

	ifcProject=NULL;
}

CTHUIFCerDoc::~CTHUIFCerDoc()
{
	if (ifcProject)
	{
		delete ifcProject;
		ifcProject=NULL;
	}
}

BOOL CTHUIFCerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("无标题"));
	SetPathName(AppPath+_T("\\NewProject.ifc"),FALSE);

	return TRUE;
}

// CTHUIFCerDoc 序列化

void CTHUIFCerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 写入
	}
	else if (ar.IsLoading())
	{
		//读出

		//初始化
		/*
		InitIFCEngineInteract();
		//
		switch	(RetrieveObjectGroups((TCHAR*)(LPCTSTR)ar.m_strFileName)) {
			case  0:
				//
				//	Retrieve geometrical data
				//
				EnrichObjectGroups();
				break;
			default:
				//char	error[512] = "Model in IFC Engine, could not be created, probably due to missing IFC Schema in executable directory.\nSearched schema location: ";
				//memcpy(&error[strlen(error)], ifcSchemaName, strlen(ifcSchemaName));
				CString strErr;
				strErr=_T("不能加载IFC模型,可能是由于缺少IFC Schema.\n请搜索是否存在:");
				strErr+=ifcSchemaName;
				MessageBox(GetActiveWindow(), strErr, _T("ERROR loading IFC file"), 0);
				break;
		}
		*/
		if (ifcProject)
		{
			delete ifcProject;
			ifcProject=NULL;
		}

		ifcProject = new CIFCProject(ar.m_strFileName , SchemaPath);
		if (!ifcProject->OpenIFCProject())
		{
			CString Err=_T("不能加载IFC模型,可能是由于缺少IFC Schema.\n请搜索是否存在:");
			Err+=SchemaPath;
			MessageBox(GetActiveWindow(), Err, _T("ERROR loading IFC file"), 0);
			ifcProject=NULL;
			return ;
		}
		/*
		CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
		CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
		*/
		//CMainFrame * pMain = (CMainFrame*)AfxGetMainWnd();
		//CTHUIFCerView * p3DView = (CTHUIFCerView *)pMain->GetActiveView();
		//p3DView->SendMessage(WM_ONOPENIFC , 0 , 0);
	}
}


// CTHUIFCerDoc 诊断

#ifdef _DEBUG
void CTHUIFCerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTHUIFCerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTHUIFCerDoc 命令
CView * CTHUIFCerDoc::GetViewPtr(CRuntimeClass * pClass)
{
	CView * pView;
	POSITION pos = GetFirstViewPosition();

	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(pClass))
			break;
	}
	if (pView->IsKindOf(pClass))
	{
		AfxMessageBox(_T("Can't Get The View!"));
		return NULL;
	}

	return pView;
}