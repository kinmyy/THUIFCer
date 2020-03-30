#pragma once


// CEntityTree

class CFloorTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CFloorTree)

public:
	CFloorTree();
	virtual ~CFloorTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


