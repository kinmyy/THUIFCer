
#pragma once


// CEntityTree

class CEntityTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CEntityTree)

protected:
	TiXmlDocument EntityTreeXml;
	HTREEITEM hRoot;
	int HitItemID;
	int HitItemType;

public:
	CEntityTree();
	virtual ~CEntityTree();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEntitytreeProperty();
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEntitytreeOnlyshow();
	afx_msg void OnEntitytreeGroupshow();
	afx_msg void OnEntitytreeAllshow();
	afx_msg void OnEntitytreeAllhide();
	afx_msg void OnEntitytreeAddproperty();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


