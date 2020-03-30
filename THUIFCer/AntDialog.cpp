// AntDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "THUIFCer.h"
#include "AntDialog.h"
#include <string>
#include<fstream>
using namespace std;


// AntDialog �Ի���

IMPLEMENT_DYNAMIC(AntDialog, CDialog)

AntDialog::AntDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AntDialog::IDD, pParent)
	,grid_size(100)
	,m_gener(200)
	,m_beta(10)
	,m_alpha(8)
{

}

AntDialog::~AntDialog()
{
}

void AntDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_GRIDSIZE,grid_size);

	CString strTemp;

	((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();//����������������

	ifstream rr(_T("E:\\doorName.txt"));
	string aa;

	while (!rr.eof())
	{
		std::getline(rr,aa);
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(CString(aa.c_str()));
		((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(CString(aa.c_str()));
	}

	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);//���õ�n������Ϊ��ʾ������
	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(0);//���õ�n������Ϊ��ʾ������

	DDX_Text(pDX, IDC_GEN, m_gener);
	DDX_Text(pDX, IDC_ATTR, m_beta);
	DDX_Text(pDX, IDC_PHER, m_alpha);
}


BEGIN_MESSAGE_MAP(AntDialog, CDialog)
	ON_BN_CLICKED(IDOK, &AntDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// AntDialog ��Ϣ�������

void AntDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	startPos = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();//��ǰѡ�е���
	endPos = ((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel();
	OnOK();
}

int AntDialog::getStartPos()
{
	return startPos;
}

int AntDialog::getEndPos()
{
	return endPos;
}