// SetValueDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "THUIFCer.h"
#include "SetValueDialog.h"


// SetValueDialog �Ի���

IMPLEMENT_DYNAMIC(SetValueDialog, CDialog)

SetValueDialog::SetValueDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SetValueDialog::IDD, pParent)
	,grid_size(100)
{
	

}

SetValueDialog::~SetValueDialog()
{
}

void SetValueDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_GRIDSIZE,grid_size);

	CString strTemp;

	((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();//����������������

	/*for(int i=1;i<=doorName.size();i++)
	{
		strTemp.Format(_T("%d"),i);
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(strTemp);
	}*/
	ifstream rr("E:\\doorName.txt");
	string aa;

	while (!rr.eof())
	{
		std::getline(rr,aa);
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(CString(aa.c_str()));
		((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(CString(aa.c_str()));
	}

	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);//���õ�n������Ϊ��ʾ������
	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(0);//���õ�n������Ϊ��ʾ������

	

}



BEGIN_MESSAGE_MAP(SetValueDialog, CDialog)
	ON_BN_CLICKED(IDOK, &SetValueDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// SetValueDialog ��Ϣ�������

void SetValueDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	startPos = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();//��ǰѡ�е���
	endPos = ((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel();
	OnOK();
}

int SetValueDialog::getStartPos()
{
	return startPos;
}

int SetValueDialog::getEndPos()
{
	return endPos;
}