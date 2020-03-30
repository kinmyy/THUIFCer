#pragma once
#include "resource.h"

class CStatisticsWallDlg : public CDialog
{
public:
	CStatisticsWallDlg();
	// 对话框数据
	enum { IDD = IDD_DIALOG_ADDUPWALL };

private:
	CIFCProject *m_pIfcProject;  //在使用之前记得判断指针是否为空
	BOOL         m_bLoadCompose;
	BOOL         m_bLoadPrice;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAddupwall();
	afx_msg void OnBnClickedButtonAddupwallloadcompose();
	afx_msg void OnBnClickedButtonAddupwallloadprice();
	afx_msg void OnBnClickedButtonAddupwallprice();
	afx_msg void OnBnClickedButtonAddupwallcompose();
};