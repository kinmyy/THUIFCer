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
	pList->InsertColumn(0,_T("����"),LVCFMT_LEFT,160,0);
	pList->InsertColumn(1,_T("����"),LVCFMT_LEFT,160,1);
	pList->InsertColumn(2,_T("�����"),LVCFMT_LEFT,166,2);

	pList->ModifyStyle(LVS_TYPEMASK,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//���㹤����
void CStatisticsPlateDlg::OnBnClickedButtonAddupplate()
{
	if (!ifcProject)
	{
		AfxMessageBox(_T("IFC����δ���أ�"));
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

//���ع��ɱ�
void CStatisticsPlateDlg::OnBnClickedButtonAddupplateloadcompose()
{
	CFileDialog openFile(TRUE,_T("*.txt"),NULL,OFN_EXTENSIONDIFFERENT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,_T("txt�ı��ļ�(*.txt)||"));
	if (openFile.DoModal()==IDOK)
	{
		CString FilePathName=openFile.GetPathName();
		CFile OpenFile;
		if (!OpenFile.Open(FilePathName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
		{
			OpenFile.Close();
			::AfxMessageBox(_T("��ʧ�ܣ�"),MB_OK);
			return;
		}
		m_bLoadCompose=TRUE;
		//���ع��ɱ�
		OpenFile.Close();
	}
}

//���ؼ۸��
void CStatisticsPlateDlg::OnBnClickedButtonAddupplateloadprice()
{
	CFileDialog openFile(TRUE,_T("*.txt"),NULL,OFN_EXTENSIONDIFFERENT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,_T("txt�ı��ļ�(*.txt)||"));
	if (openFile.DoModal()==IDOK)
	{
		CString FilePathName=openFile.GetPathName();
		CFile OpenFile;
		if (!OpenFile.Open(FilePathName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
		{
			OpenFile.Close();
			::AfxMessageBox(_T("��ʧ�ܣ�"),MB_OK);
			return;
		}
		m_bLoadPrice=TRUE;
		//���ؼ۸��
		OpenFile.Close();
	}
}

//�������
void CStatisticsPlateDlg::OnBnClickedButtonAddupplateprice()
{
	if (!ifcProject)
	{
		AfxMessageBox(_T("IFC����δ���أ�"));
		return;
	}
	if (!m_bLoadCompose)
	{
		AfxMessageBox(_T("���Ϲ��ɱ�δ���أ�"));
		return;
	}
	if (!m_bLoadPrice)
	{
		AfxMessageBox(_T("�����δ���أ�"));
		return;
	}
}
