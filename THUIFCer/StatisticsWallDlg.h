#pragma once
#include "resource.h"

class CStatisticsWallDlg : public CDialog
{
public:
	CStatisticsWallDlg();
	// �Ի�������
	enum { IDD = IDD_DIALOG_ADDUPWALL };

private:
	CIFCProject *m_pIfcProject;  //��ʹ��֮ǰ�ǵ��ж�ָ���Ƿ�Ϊ��
	BOOL         m_bLoadCompose;
	BOOL         m_bLoadPrice;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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