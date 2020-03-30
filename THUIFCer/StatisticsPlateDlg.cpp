#pragma once

#include "stdafx.h"
#include "THUIFCer.h"
#include "MainFrm.h"
#include "THUIFCerView.h"
#include "IFCProject.h"
#include "StatisticsPlateDlg.h"

CStatisticsPlateDlg::CStatisticsPlateDlg() : CDialog(CStatisticsPlateDlg::IDD),m_bLoadCompose(FALSE),m_bLoadPrice(FALSE)
{
	CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
	ifcProject = p3DView->GetDocument()->ifcProject;
}

void CStatisticsPlateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStatisticsPlateDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADDUPPLATE, &CStatisticsPlateDlg::OnBnClickedButtonAddupplate)
	ON_BN_CLICKED(IDC_BUTTON_ADDUPPLATELOADCOMPOSE, &CStatisticsPlateDlg::OnBnClickedButtonAddupplateloadcompose)
	ON_BN_CLICKED(IDC_BUTTON_ADDUPPLATELOADPRICE, &CStatisticsPlateDlg::OnBnClickedButtonAddupplateloadprice)
	ON_BN_CLICKED(IDC_BUTTON_ADDUPPLATEPRICE, &CStatisticsPlateDlg::OnBnClickedButtonAddupplateprice)
END_MESSAGE_MAP()

BOOL CStatisticsPlateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CListCtrl *pList;
	pList=(CListCtrl*)GetDlgItem(IDC_LIST_ADDUPPLATE);
	pList->DeleteAllItems();
	pList->InsertColumn(0,_T("类型"),LVCFMT_LEFT,160,0);
	pList->InsertColumn(1,_T("数量"),LVCFMT_LEFT,160,1);
	pList->InsertColumn(2,_T("体积和"),LVCFMT_LEFT,166,2);

	pList->ModifyStyle(LVS_TYPEMASK,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//计算工程量
void CStatisticsPlateDlg::OnBnClickedButtonAddupplate()
{
	if (!ifcProject)
	{
		AfxMessageBox(_T("IFC工程未加载！"));
		return;
	}

	CListCtrl *pList;
	pList=(CListCtrl*)GetDlgItem(IDC_LIST_ADDUPPLATE);
	pList->DeleteAllItems();

	CIFCEntity Slabs;
	Slabs=ifcProject->GetEntity(_T("IFCSLAB"));
	const InstanceGroup *pInstanceGroup=Slabs.GetInstanceGroup();
	Instance_constIter InstanceIter=pInstanceGroup->begin();
	for (;InstanceIter!=pInstanceGroup->end();++InstanceIter)
	{
		CIFCInstance slab=InstanceIter->second;
		slab.GetName(); //
		slab.GetGlobalID();
		slab.GetOID();

		/*
		CString SlabType=ifcProject->GetSinglePropertyInPSet(slab.GetID(),m_PropertyName);
		if (CarbonValue!=_T(""))
		{
			pList->InsertItem(i,(LPCTSTR)entities.GetNameCN());
			pList->SetItemText(i,1,(LPCTSTR)entities.GetNameIFC());
			pList->SetItemText(i,2,(LPCTSTR)instance.GetName());
			pList->SetItemText(i,3,(LPCTSTR)instance.GetGlobalID());
			CString OID;
			OID.Format(_T("%d"),instance.GetOID());
			OID.Insert(0,'#');
			pList->SetItemText(i,4,(LPCTSTR)OID);
			pList->SetItemText(i,5,(LPCTSTR)CarbonValue);
			i++;
			SingleCarbon=_wtof((LPCTSTR)CarbonValue);
			AllCarbon=AllCarbon+SingleCarbon;
		}
		*/
	}
}

//加载构成表
void CStatisticsPlateDlg::OnBnClickedButtonAddupplateloadcompose()
{
	CFileDialog openFile(TRUE,_T("*.txt"),NULL,OFN_EXTENSIONDIFFERENT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,_T("txt文本文件(*.txt)||"));
	if (openFile.DoModal()==IDOK)
	{
		CString FilePathName=openFile.GetPathName();
		CFile OpenFile;
		if (!OpenFile.Open(FilePathName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
		{
			OpenFile.Close();
			::AfxMessageBox(_T("打开失败！"),MB_OK);
			return;
		}
		m_bLoadCompose=TRUE;
		//加载构成表
		OpenFile.Close();
	}
}

//加载价格表
void CStatisticsPlateDlg::OnBnClickedButtonAddupplateloadprice()
{
	CFileDialog openFile(TRUE,_T("*.txt"),NULL,OFN_EXTENSIONDIFFERENT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,_T("txt文本文件(*.txt)||"));
	if (openFile.DoModal()==IDOK)
	{
		CString FilePathName=openFile.GetPathName();
		CFile OpenFile;
		if (!OpenFile.Open(FilePathName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
		{
			OpenFile.Close();
			::AfxMessageBox(_T("打开失败！"),MB_OK);
			return;
		}
		m_bLoadPrice=TRUE;
		//加载价格表
		OpenFile.Close();
	}
}

//计算造价
void CStatisticsPlateDlg::OnBnClickedButtonAddupplateprice()
{
	if (!ifcProject)
	{
		AfxMessageBox(_T("IFC工程未加载！"));
		return;
	}
	if (!m_bLoadCompose)
	{
		AfxMessageBox(_T("材料构成表未加载！"));
		return;
	}
	if (!m_bLoadPrice)
	{
		AfxMessageBox(_T("定额表未加载！"));
		return;
	}
}
