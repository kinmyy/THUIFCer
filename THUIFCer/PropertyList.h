#pragma once


// CPropertyList

class CPropertyList : public CListCtrl
{
	DECLARE_DYNAMIC(CPropertyList)
private:
	TiXmlDocument PropertyListXml;
	CMap<DWORD,DWORD&,COLORREF,COLORREF&> MapItemColor;

public:
	CPropertyList();
	virtual ~CPropertyList();
	void SetItemColor(DWORD iItem,COLORREF color);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
