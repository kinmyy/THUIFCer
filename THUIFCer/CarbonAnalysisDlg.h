#pragma once
#include "resource.h"

class CCarbonAnalysisDlg : public CDialog
{
public:
	CCarbonAnalysisDlg();
	// �Ի�������
	enum { IDD = IDD_DIALOG_CARBON };
	CString m_PropertyName;

private:
	CIFCProject *ifcProject; //��ʹ��֮ǰ�ǵ��ж�ָ���Ƿ�Ϊ��

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCarbonButtonCalculate();
};
