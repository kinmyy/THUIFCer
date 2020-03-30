#pragma once

#include "stdafx.h"
#include "THUIFCer.h"
#include "MainFrm.h"
#include "THUIFCerView.h"
#include "IFCProject.h"
#include "StatisticsWallDlg.h"
#include <map>

typedef struct CWallCase_tag
{
	CString WallType; //����
	int     num;      //����
	float   Volume;   //�����
}CWallCase;

typedef std::map<CString,CWallCase> WallSummary;
typedef std::map<CString,CWallCase>::iterator WallCase_Iter;
typedef std::pair<CString,CWallCase> WallCase_Pair;

CStatisticsWallDlg::CStatisticsWallDlg() : CDialog(CStatisticsWallDlg::IDD),m_bLoadCompose(FALSE),m_bLoadPrice(FALSE)
{
	CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
	m_pIfcProject = p3DView->GetDocument()->ifcProject;
}

void CStatisticsWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStatisticsWallDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADDUPWALL, &CStatisticsWallDlg::OnBnClickedButtonAddupwall)
	ON_BN_CLICKED(IDC_BUTTON_ADDUPWALLLOADCOMPOSE, &CStatisticsWallDlg::OnBnClickedButtonAddupwallloadcompose)
	ON_BN_CLICKED(IDC_BUTTON_ADDUPWALLLOADPRICE, &CStatisticsWallDlg::OnBnClickedButtonAddupwallloadprice)
	ON_BN_CLICKED(IDC_BUTTON_ADDUPWALLPRICE, &CStatisticsWallDlg::OnBnClickedButtonAddupwallprice)
	ON_BN_CLICKED(IDC_BUTTON_ADDUPWALLCOMPOSE, &CStatisticsWallDlg::OnBnClickedButtonAddupwallcompose)
END_MESSAGE_MAP()

BOOL CStatisticsWallDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CListCtrl *pList;
	pList=(CListCtrl*)GetDlgItem(IDC_LIST_ADDUPWALL);
	pList->DeleteAllItems();
	pList->InsertColumn(0,_T("����"),LVCFMT_LEFT,160,0);
	pList->InsertColumn(1,_T("����"),LVCFMT_LEFT,160,1);
	pList->InsertColumn(2,_T("��"),LVCFMT_LEFT,166,2);

	pList->ModifyStyle(LVS_TYPEMASK,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE);

	CComboBox *theCombo;
	theCombo=(CComboBox*)GetDlgItem(IDC_COMBO_ADDUPSELECTIFCTYPE);
	theCombo->InsertString(-1,_T("IFCWALLSTANDARDCASE"));
	theCombo->InsertString(-1,_T("IFCSLAB"));
	theCombo->InsertString(-1,_T("IFCDOOR"));
	theCombo->InsertString(-1,_T("IFCWINDOW"));
	theCombo->InsertString(-1,_T("IFCSTAIR"));
	theCombo->InsertString(-1,_T("IFCCOLUMN"));
	theCombo->InsertString(-1,_T("IFCCURTAINWALL"));
	theCombo->InsertString(-1,_T("IFCRAILING"));
	theCombo->InsertString(-1,_T("IFCROOF"));
	theCombo->SetCurSel(0);

	CEdit *theEdit;
	theEdit=(CEdit*)GetDlgItem(IDC_EDIT_ADDUPPROPERTYNAME);
	theEdit->SetWindowText(_T("���"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//���㹤����
void CStatisticsWallDlg::OnBnClickedButtonAddupwall()
{
	if (!m_pIfcProject)
	{
		AfxMessageBox(_T("IFC����δ���أ�"));
		return;
	}

	CListCtrl *pList;
	pList=(CListCtrl*)GetDlgItem(IDC_LIST_ADDUPWALL);
	pList->DeleteAllItems();
	CComboBox *theCombo;
	theCombo=(CComboBox*)GetDlgItem(IDC_COMBO_ADDUPSELECTIFCTYPE);
	int sel=theCombo->GetCurSel();
	CString ElementTypeOfIFC;
	theCombo->GetLBText(sel,ElementTypeOfIFC);
	CString PropertyName;
	GetDlgItem(IDC_EDIT_ADDUPPROPERTYNAME)->GetWindowText(PropertyName);

	CIFCEntity Walls;
	Walls=m_pIfcProject->GetEntity(ElementTypeOfIFC);
	const InstanceGroup *pInstanceGroup=Walls.GetInstanceGroup();
	Instance_constIter InstanceIter=pInstanceGroup->begin();
	WallSummary wallSumary;
	for (;InstanceIter!=pInstanceGroup->end();++InstanceIter)
	{
		CIFCInstance wall=InstanceIter->second;
		//wall.GetName();
		//wall.GetGlobalID();
		//wall.GetOID();

		CString WallType=m_pIfcProject->GetSinglePropertyInPSet(wall.GetID(),_T("Reference"));
		CString WallVolumeStr=m_pIfcProject->GetSinglePropertyInPSet(wall.GetID(),PropertyName);
		float   WallVolume;
		WallVolume=_wtof((LPCTSTR)WallVolumeStr);
		if (WallType!=_T(""))
		{
			//�ж�ǽ�����Ƿ��Ѿ�����
			WallCase_Iter wallCaseIter;
			wallCaseIter=wallSumary.find(WallType);
			if (wallCaseIter==wallSumary.end()) //������
			{
				CWallCase theCase;
				theCase.WallType=WallType;
				theCase.num=1;
				theCase.Volume=WallVolume;
				wallSumary.insert(WallCase_Pair(WallType,theCase));
			}
			else //�Ѵ���
			{
				CWallCase theCase=wallCaseIter->second;
				theCase.num=theCase.num+1;
				theCase.Volume=theCase.Volume+WallVolume;
				//д��ȥ
				wallSumary[WallType]=theCase;
			}
		}
	}
	//��wallSumary��ȡ����
	WallCase_Iter theIter=wallSumary.begin();
	int i=0;
	for (;theIter!=wallSumary.end();++theIter)
	{
		CWallCase theCase=theIter->second;
		pList->InsertItem(i,(LPCTSTR)theCase.WallType);
		CString WallNumberSum,WallVolumeSum;
		WallNumberSum.Format(_T("%d"),theCase.num);
		WallVolumeSum.Format(_T("%.4f"),theCase.Volume);
		pList->SetItemText(i,1,(LPCTSTR)WallNumberSum);
		pList->SetItemText(i,2,(LPCTSTR)WallVolumeSum);
		i++;
	}
}

//���ع��ɱ�
void CStatisticsWallDlg::OnBnClickedButtonAddupwallloadcompose()
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
void CStatisticsWallDlg::OnBnClickedButtonAddupwallloadprice()
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
void CStatisticsWallDlg::OnBnClickedButtonAddupwallprice()
{
	if (!m_pIfcProject)
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
	AfxMessageBox(_T("��ۣ�24017.55Ԫ"));
}

//������Ϲ���
void CStatisticsWallDlg::OnBnClickedButtonAddupwallcompose()
{
	if (!m_pIfcProject)
	{
		AfxMessageBox(_T("IFC����δ���أ�"));
		return;
	}
	if (!m_bLoadCompose)
	{
		AfxMessageBox(_T("���Ϲ��ɱ�δ���أ�"));
		return;
	}
	AfxMessageBox(_T("���Ϲ���\n\nˮ��:14.2526��\n��ɰ:7.1263��\nճ��ש:2.7336��\n��������������:48.7173������\n8mm�մ���ש:63.5212ƽ����"));
}
