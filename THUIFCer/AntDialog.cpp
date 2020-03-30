// AntDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "THUIFCer.h"
#include "AntDialog.h"
#include <string>
#include<fstream>
using namespace std;


// AntDialog 对话框

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

	((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();//消除现有所有内容

	ifstream rr(_T("E:\\doorName.txt"));
	string aa;

	while (!rr.eof())
	{
		std::getline(rr,aa);
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(CString(aa.c_str()));
		((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(CString(aa.c_str()));
	}

	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);//设置第n行内容为显示的内容
	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(0);//设置第n行内容为显示的内容

	DDX_Text(pDX, IDC_GEN, m_gener);
	DDX_Text(pDX, IDC_ATTR, m_beta);
	DDX_Text(pDX, IDC_PHER, m_alpha);
}


BEGIN_MESSAGE_MAP(AntDialog, CDialog)
	ON_BN_CLICKED(IDOK, &AntDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// AntDialog 消息处理程序

void AntDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	startPos = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();//当前选中的行
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