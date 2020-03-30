#pragma once
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// SetValueDialog 对话框

class SetValueDialog : public CDialog
{
	DECLARE_DYNAMIC(SetValueDialog)

public:
	SetValueDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetValueDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETVALUE };

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
};
