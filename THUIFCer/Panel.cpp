//Ãæ°å

#pragma once
#include "StdAfx.h"
#include "Panel.h"
#include "..\dotnet\DockPageBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLeftPanel
CPanel::CPanel()
{
}

CPanel::~CPanel()
{
}


BEGIN_MESSAGE_MAP(CPanel, CDockPageBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTestBar message handlers

int CPanel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockPageBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CPanel::AddPage(CRuntimeClass *pClass, UINT nIDTemplate, LPCTSTR sText, UINT IconID)
{
	CDialog *pDlg = (CDialog*)pClass->CreateObject();
	if(pDlg != NULL)
	{
		if(pDlg->Create(nIDTemplate,this))
		{
			return AddPage(pDlg, sText, IconID);
		}
	}

	return FALSE;
}