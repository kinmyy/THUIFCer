//THUIFCer选项对话框

#pragma once
#include "resource.h"

//多页
class COptionSheet : public CPropertySheet
{
public:
	COptionSheet(CWnd* pParent);
protected:
	DECLARE_MESSAGE_MAP();
};

//对话框，IFC相关设置
class COptionPageIFC : public CPropertyPage
{
public:
	enum {IDD=IDD_DIALOG_OPTIONIFC};

public:
	COptionPageIFC():CPropertyPage(COptionPageIFC::IDD)
	{
	}

protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
};
