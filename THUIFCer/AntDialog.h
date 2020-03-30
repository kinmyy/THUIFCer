#pragma once
#include "afxwin.h"
#include<vector>
#include <string>
using namespace std;


// AntDialog 对话框

class AntDialog : public CDialog
{
	DECLARE_DYNAMIC(AntDialog)

public:
	AntDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AntDialog();

// 对话框数据
	enum { IDD = IDD_ANTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
DECLARE_MESSAGE_MAP()

public:
	int grid_size;

	int startPos;
	int endPos;

	vector<CString>doorName;

	
public:
	afx_msg void OnBnClickedOk();
	int getStartPos();
	int getEndPos();
public:
	int m_gener;
public:
	double m_beta;
public:
	double m_alpha;
};
