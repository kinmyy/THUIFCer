//THUIFCerѡ��Ի���

#pragma once
#include "resource.h"

//��ҳ
class COptionSheet : public CPropertySheet
{
public:
	COptionSheet(CWnd* pParent);
protected:
	DECLARE_MESSAGE_MAP();
};

//�Ի���IFC�������
class COptionPageIFC : public CPropertyPage
{
public:
	enum {IDD=IDD_DIALOG_OPTIONIFC};

public:
	COptionPageIFC():CPropertyPage(COptionPageIFC::IDD)
	{
	}

protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
};
