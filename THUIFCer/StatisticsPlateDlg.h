#pragma once
#include "resource.h"

class CStatisticsPlateDlg : public CDialog
{
public:
	CStatisticsPlateDlg();
	// �Ի�������
	enum { IDD = IDD_DIALOG_ADDUPPLATE };

private:
	CIFCProject *ifcProject; //��ʹ��֮ǰ�ǵ��ж�ָ���Ƿ�Ϊ��
	BOOL        m_bLoadCompose;
	BOOL        m_bLoadPrice;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAddupplate();
	afx_msg void OnBnClickedButtonAddupplateloadcompose();
	afx_msg void OnBnClickedButtonAddupplateloadprice();
	afx_msg void OnBnClickedButtonAddupplateprice();
};