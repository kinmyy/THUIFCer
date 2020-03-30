/////////////////////////////////////////////////////////////////////////
//
// CDockPageBar            Version 1.0
//
// Created: Mar 16, 2004
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 2004 by Cuick. All rights reserved.
//
// This code is free for personal and commercial use, providing this 
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// Cuick@163.net
/////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCKPAGEBAR_H__5125E626_E472_4F93_AC8E_20F121A6D1B9__INCLUDED_)
#define AFX_DOCKPAGEBAR_H__5125E626_E472_4F93_AC8E_20F121A6D1B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DockPageBar.h : header file
//

///////////////////////////////////////////////////////////////////////////
//  标题栏按钮
class CMyButton
{
public:
    CMyButton();

    void Move(CPoint ptTo) {ptOrg = ptTo; };
    virtual CRect GetRect() { return CRect(ptOrg, CSize(13, 13)); };
    virtual void Paint(CDC* pDC, BOOL isActive);

    BOOL    bPushed;
    BOOL    bRaised;

protected:
    CPoint  ptOrg;
};
//关闭按钮
class CCloseButton : public CMyButton
{
public:
	virtual void Paint(CDC* pDC, BOOL isActive);
};
//图钉按钮
class CStudButton : public CMyButton
{
public:
	CStudButton();
	virtual void Paint(CDC* pDC, BOOL isActive);
	BOOL	bFloat;
};

///////////////////////////////////////////////////////////////////////////
//  标签页
class CPageItem
{
public:
	CWnd*		m_pWnd;			//窗口,可以为NULL
	CString		m_sText;		//文字
	HICON		m_hIcon;		//图标
	UINT		m_TabWidth;		//标签宽度
	CRect		m_rect;			//标签位置
	
public:
	void Draw(CDC *pDC,BOOL bActive);
};

/////////////////////////////////////////////////////////////////////////////
// CDockPageBar control bar

#ifndef baseCDockPageBar
#define baseCDockPageBar CSizingControlBar
#endif

class CDockPageBar : public baseCDockPageBar
{
	DECLARE_DYNAMIC(CDockPageBar);
// Construction
public:
	CDockPageBar();

// Attributes
public:
	int m_cyGripper;
// Operations
public:
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDockPageBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void EnableDocking(DWORD dwDockStyle);
	CPageItem* DeletePage();
	BOOL AddPage(CDockPageBar* dockPageBar);
	BOOL AddPage(CPageItem *pPageItem);
	BOOL AddPage(CWnd *pWnd, LPCTSTR sText, UINT IconID);

	virtual BOOL HasGripper() const;
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual ~CDockPageBar();

	// Generated message map functions
protected:
	void Move(CPoint pt);
	void CanDrag(CPoint pt);
	void StartDrag(CPoint pt);
	void UpdateWindow();
	virtual void UpdateSize();
	void GetClientRect(LPRECT lpRect, BOOL isMulti);
	void SetActivePage(int nIndex);
	
	// implementation helpers
	virtual void NcCalcClient(LPRECT pRc, UINT nDockBarID);
	virtual void NcPaintGripper(CDC* pDC, CRect rcClient);

	int				m_nActivePage;
	CString			m_Title;
	BOOL			m_isActive;
	//{{AFX_MSG(CDockPageBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CPtrList		m_PageList;
	CPageItem*		GetPageItem(int nIndex);
	CFont			m_font;

	CCloseButton	m_biHide;
	CStudButton		m_stud;
};

/////////////////////////////////////////////////////////////////////////////
// CDockPageBarContext dockcontext
#ifndef LAYOUT_LTR
#define LAYOUT_LTR			0x00000000
#endif

class CDockPageBarContext : public CDockContext
{
public:
// Construction
	CDockPageBarContext(CControlBar* pBar) : CDockContext(pBar) {}
	BOOL			m_isMe;
	BOOL			m_isPage;
	CDockPageBar*	m_pTgDockPage;

// Drag Operations
	virtual void StartDrag(CPoint pt);
	virtual BOOL Track();
	virtual void Move(CPoint pt);
	virtual void EndDrag();
	void DrawFocusRect(BOOL bRemoveRect = FALSE);
protected:
	CDockPageBar* CreateNewBar();
	BOOL isDockPage(CPoint pt);
	CRect	m_addRect;
};

#endif // !defined(AFX_DOCKPAGEBAR_H__5125E626_E472_4F93_AC8E_20F121A6D1B9__INCLUDED_)
