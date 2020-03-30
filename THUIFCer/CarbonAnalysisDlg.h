#pragma once
#include "resource.h"

class CCarbonAnalysisDlg : public CDialog
{
public:
	CCarbonAnalysisDlg();
	// 对话框数据
	enum { IDD = IDD_DIALOG_CARBON };
	CString m_PropertyName;

private:
	CIFCProject *ifcProject; //在使用之前记得判断指针是否为空

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCarbonButtonCalculate();
};
