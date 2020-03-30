#pragma once
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// SetValueDialog �Ի���

class SetValueDialog : public CDialog
{
	DECLARE_DYNAMIC(SetValueDialog)

public:
	SetValueDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetValueDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETVALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
