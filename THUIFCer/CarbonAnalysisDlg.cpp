#pragma once

#include "stdafx.h"
#include "THUIFCer.h"
#include "MainFrm.h"
#include "THUIFCerView.h"
#include "IFCProject.h"
#include "CarbonAnalysisDlg.h"

CCarbonAnalysisDlg::CCarbonAnalysisDlg() : CDialog(CCarbonAnalysisDlg::IDD)
{
	CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
	ifcProject = p3DView->GetDocument()->ifcProject;
	m_PropertyName=_T("C_CarbonEmission");//̼�ŷ���
}

void CCarbonAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_CARBON_EDIT,m_PropertyName);
}

BEGIN_MESSAGE_MAP(CCarbonAnalysisDlg, CDialog)
	ON_BN_CLICKED(IDC_CARBON_BUTTON_CALCULATE, &CCarbonAnalysisDlg::OnBnClickedCarbonButtonCalculate)
END_MESSAGE_MAP()

BOOL CCarbonAnalysisDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CListCtrl *pList;
	pList=(CListCtrl*)GetDlgItem(IDC_CARBON_LIST);
	pList->DeleteAllItems();
	pList->InsertColumn(0,_T("Component Type"),LVCFMT_LEFT,80,0);//��������
	pList->InsertColumn(1,_T("IFC Type"),LVCFMT_LEFT,100,1);//IFC����
	pList->InsertColumn(2,_T("Component Name"),LVCFMT_LEFT,100,2);//��������
	pList->InsertColumn(3,_T("Component GUID"),LVCFMT_LEFT,120,3);//����GUID
	pList->InsertColumn(4,_T("Component OID"),LVCFMT_LEFT,80,4);//����OID
	pList->InsertColumn(5,_T("Value"),LVCFMT_LEFT,180,5);//ֵ

	pList->ModifyStyle(LVS_TYPEMASK,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCarbonAnalysisDlg::OnBnClickedCarbonButtonCalculate()
{
	if (!ifcProject)
	{
		AfxMessageBox(_T("IFC Project Load Failed!"));//IFC����δ���أ�
		return;
	}
	GetDlgItem(IDC_CARBON_EDIT)->GetWindowText(m_PropertyName);

	CListCtrl *pList;
	pList=(CListCtrl*)GetDlgItem(IDC_CARBON_LIST);
	pList->DeleteAllItems();

	const EntityGroup *pGroups;
	pGroups=ifcProject->GetEntityGroup();
	Entities_constIter entitiesIter=pGroups->begin();
	int i=0;
	float SingleCarbon=0,AllCarbon=0; //̼�ŷ�
	for (;entitiesIter!=pGroups->end();++entitiesIter)
	{
		CIFCEntity entities=entitiesIter->second;
		entities.GetNameCN();  //
		entities.GetNameIFC(); //
		const InstanceGroup *pInstanceGroup=entities.GetInstanceGroup();
		Instance_constIter InstanceIter=pInstanceGroup->begin();
		for (;InstanceIter!=pInstanceGroup->end();++InstanceIter)
		{
			CIFCInstance instance=InstanceIter->second;
			instance.GetName(); //
			instance.GetGlobalID();
			instance.GetOID();
			CString CarbonValue=ifcProject->GetSinglePropertyInPSet(instance.GetID(),m_PropertyName);
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
		}
	}
	CString OutputStr;
	OutputStr.Format(_T("%.2f"),AllCarbon);
	AfxMessageBox(_T("̼�ŷ�����Ϊ��")+OutputStr+_T("kg"));
}
