#pragma once
#include "resource.h"

class CStatisticsPlateDlg : public CDialog
{
public:
	CStatisticsPlateDlg();
	// 对话框数据
	enum { IDD = IDD_DIALOG_ADDUPPLATE };

private:
	CIFCProject *ifcProject; //在使用之前记得判断指针是否为空
	BOOL        m_bLoadCompose;
	BOOL        m_bLoadPrice;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAddupplate();
	afx_msg void OnBnClickedButtonAddupplateloadcompose();
	afx_msg void OnBnClickedButtonAddupplateloadprice();
	afx_msg void OnBnClickedButtonAddupplateprice();
};