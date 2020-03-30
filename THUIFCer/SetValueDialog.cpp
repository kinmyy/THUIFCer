// SetValueDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "THUIFCer.h"
#include "SetValueDialog.h"


// SetValueDialog 对话框

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

	((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();//消除现有所有内容

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

	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);//设置第n行内容为显示的内容
	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(0);//设置第n行内容为显示的内容

	

}



BEGIN_MESSAGE_MAP(SetValueDialog, CDialog)
	ON_BN_CLICKED(IDOK, &SetValueDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// SetValueDialog 消息处理程序

void SetValueDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	startPos = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();//当前选中的行
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