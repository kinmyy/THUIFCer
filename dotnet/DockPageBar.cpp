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
//
// Hint: These classes are intended to be used as base classes. Do not
// simply add your code to these file - instead create a new class
// derived from CDockPageBar classes and put there what you need. See 
// CTestBar classes in the demo projects for examples.
// Modify this file only to fix bugs, and don't forget to send me a copy.
/////////////////////////////////////////////////////////////////////////
// Acknowledgements:
//	o	感谢Cristi Posea的CSizingControlBar
//	o	借鉴了王骏的《轻松实现类VC界面》中的部分代码，在这里表示感谢。
//	o	开发过程中遇到的问题得到了VC知识库论坛中很多人的帮助，这里一并感谢。
//


// DockPageBar.cpp : implementation file
//
#include "stdafx.h"
#include "DockPageBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
// delete the line between menu bar and tool bar

// From <afximpl.h>
struct AUX_DATA
{
	int _unused1, _unused2;
	int _unused3, _unused4;
	int cxBorder2, cyBorder2;
};

extern __declspec(dllimport) AUX_DATA afxData;

class INIT_afxData
{
public:
    INIT_afxData ()
    {
        afxData.cxBorder2 = afxData.cyBorder2 = 0;
    }
} g_afxData2;

/////////////////////////////////////////////////////////////////////////
// CMyButton
#define HTSTUD             22

CMyButton::CMyButton()
{
    bRaised = FALSE;
    bPushed = FALSE;
}

void CMyButton::Paint(CDC* pDC, BOOL isActive)
{
    CRect rc = GetRect();

    if (bPushed)
        pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW),
            ::GetSysColor(COLOR_BTNHIGHLIGHT));
    else
        if (bRaised)
            pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                ::GetSysColor(COLOR_BTNSHADOW));
}

// draw close button
void CCloseButton::Paint(CDC* pDC, BOOL isActive)
{
	CMyButton::Paint(pDC, isActive);

	COLORREF clrOldTextColor = pDC->GetTextColor();

	if(TRUE == isActive)
		pDC->SetTextColor(RGB(255,255,255));
	else
		pDC->SetTextColor(RGB(128,128,128));

    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
    CFont font;
    int ppi = pDC->GetDeviceCaps(LOGPIXELSX);
    int pointsize = MulDiv(75, 96, ppi); // 6 points at 96 ppi
    font.CreatePointFont(pointsize, _T("Marlett"));
    CFont* oldfont = pDC->SelectObject(&font);

    pDC->TextOut(ptOrg.x + 2, ptOrg.y + 2, CString(_T("r"))); // x-like

    pDC->SelectObject(oldfont);
    pDC->SetBkMode(nPrevBkMode);
    pDC->SetTextColor(clrOldTextColor);
}

CStudButton::CStudButton()
{
	bFloat = FALSE;
}

// draw stud
void CStudButton::Paint(CDC* pDC, BOOL isActive)
{
	CMyButton::Paint(pDC, isActive);

	HPEN oldPen;
	CPen pen;
	if(TRUE == isActive)
		pen.CreatePen (PS_SOLID, 1, RGB(255,255,255));
	else
		pen.CreatePen (PS_SOLID, 1, RGB(0,0,0));

	oldPen = (HPEN)pDC->SelectObject (pen);

	if(FALSE == bFloat)
	{
		pDC->MoveTo (ptOrg.x + 4, ptOrg.y + 8);
		pDC->LineTo (ptOrg.x + 4, ptOrg.y + 3);
		pDC->LineTo (ptOrg.x + 8, ptOrg.y + 3);
		pDC->LineTo (ptOrg.x + 8, ptOrg.y + 8);
		
		pDC->MoveTo (ptOrg.x + 7, ptOrg.y + 3);
		pDC->LineTo (ptOrg.x + 7, ptOrg.y + 8);
		
		pDC->MoveTo (ptOrg.x + 2, ptOrg.y + 8);
		pDC->LineTo (ptOrg.x + 11, ptOrg.y + 8);
		
		pDC->MoveTo (ptOrg.x + 6, ptOrg.y + 8);
		pDC->LineTo (ptOrg.x + 6, ptOrg.y + 12);
	}
	else
	{
		pDC->MoveTo (ptOrg.x + 5, ptOrg.y + 4);
		pDC->LineTo (ptOrg.x + 10, ptOrg.y + 4);
		pDC->LineTo (ptOrg.x + 10, ptOrg.y + 8);
		pDC->LineTo (ptOrg.x + 5, ptOrg.y + 8);
		
		pDC->MoveTo (ptOrg.x + 5, ptOrg.y + 7);
		pDC->LineTo (ptOrg.x + 10, ptOrg.y + 7);
		
		pDC->MoveTo (ptOrg.x + 5, ptOrg.y + 2);
		pDC->LineTo (ptOrg.x + 5, ptOrg.y + 11);
		
		pDC->MoveTo (ptOrg.x + 1, ptOrg.y + 6);
		pDC->LineTo (ptOrg.x + 5, ptOrg.y + 6);
	}

	pDC->SelectObject(oldPen);
}

/////////////////////////////////////////////////////////////////////////////
// CPageItem 
#define	ITEMBUTTON_HEIGHT		27

void CPageItem::Draw(CDC *pDC, BOOL bActive)
{
	CRect rect = m_rect;
	COLORREF crOldText;

	rect.top += 2;
	if(bActive)
	{
		rect.bottom -= 2;
		
		CBrush brush(GetSysColor(COLOR_3DFACE));
		pDC->FillRect(rect,&brush);

		CPen pen(PS_SOLID,1,GetSysColor(COLOR_3DDKSHADOW));
		HPEN oldPen = (HPEN)pDC->SelectObject (&pen);
		pDC->MoveTo (rect.left , rect.bottom );
		pDC->LineTo (rect.right+1 ,rect.bottom );

		pDC->MoveTo (rect.right, rect.top);
		pDC->LineTo (rect.right ,rect.bottom+1 );

		pDC->SelectObject (oldPen);

		crOldText = pDC->SetTextColor(RGB(0,0,0));
		m_pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		CPen pen(PS_SOLID, 1, RGB(128,128,128));
		HPEN oldPen = (HPEN)pDC->SelectObject (&pen);

		pDC->MoveTo (rect.right+1, rect.top + 3);
		pDC->LineTo (rect.right+1, rect.bottom -4);

		pDC->SelectObject (oldPen);

		crOldText = pDC->SetTextColor(RGB(128,128,128));
		m_pWnd->ShowWindow(SW_HIDE);
	}

	rect.left += 5;
	rect.right -= 2;
	rect.top +=1;
	
	// draw Icon
	if(rect.Width() > 16 && m_hIcon != NULL)
	{
		::DrawIconEx(pDC->m_hDC,rect.left,rect.top + 3,m_hIcon,16,16,0,NULL,DI_NORMAL);
		rect.left += 22;
	}
	if (!m_sText.IsEmpty())
	{
		// draw text
		rect.top += 2;
		CString sText = m_sText;
		int l = sText.GetLength();
		int i;

		for(i=0;i<10 && pDC->GetTextExtent(sText).cx > rect.Width();i++,l-=2)
			sText = sText.Left(l-2);
		if(i > 0)
		{
			sText = sText.Left(l-2);
			sText += "...";
		}
		int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
		
		pDC->DrawText(sText, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		pDC->SetBkMode(nPrevBkMode);
	}

	pDC->SetTextColor (crOldText);
}

/////////////////////////////////////////////////////////////////////////////
// CDockPageBar

IMPLEMENT_DYNAMIC(CDockPageBar, baseCDockPageBar);

CDockPageBar::CDockPageBar()
{
	m_szMinHorz = CSize(50, 50);
    m_szMinVert = CSize(60, 60);
    m_szMinFloat = CSize(150, 150);

	m_cyGripper = 20;
	m_isActive = FALSE;
	m_Title = "";
	m_nActivePage = -1;
}

CDockPageBar::~CDockPageBar()
{
	POSITION pos;
	CPageItem* pItem;
	for(pos=m_PageList.GetHeadPosition();pos!=NULL;)
	{
		pItem=(CPageItem*)m_PageList.GetNext(pos);
		if(pItem)
		{
			delete pItem;
			pItem=NULL;
		}
	}
	m_PageList.RemoveAll();
}


BEGIN_MESSAGE_MAP(CDockPageBar, baseCDockPageBar)
	//{{AFX_MSG_MAP(CDockPageBar)
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONUP()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDockPageBar message handlers

int CDockPageBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (baseCDockPageBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_font.CreatePointFont(85, _T("Tahoma")))
			SetFont(&m_font);

	m_isActive = FALSE;

	return 0;
}

void CDockPageBar::OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisableIfNoHndler)
{
	if (!HasGripper())
        return;

    BOOL bNeedPaint = FALSE;

    CWnd* pFocus = GetFocus();
    BOOL bActiveOld = m_isActive;

    m_isActive = (pFocus->GetSafeHwnd() && IsChild(pFocus));
	m_isActive = m_isActive || (pFocus == this);
    if (m_isActive != bActiveOld)
        bNeedPaint = TRUE;


    CPoint pt;
    ::GetCursorPos(&pt);

	///////////////////////////////////////////////////////////
	// hit close
    BOOL bHit = (OnNcHitTest(pt) == HTCLOSE);
    BOOL bLButtonDown = (::GetKeyState(VK_LBUTTON) < 0);

    BOOL bWasPushed = m_biHide.bPushed;
    m_biHide.bPushed = bHit && bLButtonDown;

    BOOL bWasRaised = m_biHide.bRaised;
    m_biHide.bRaised = bHit && !bLButtonDown;

    bNeedPaint |= (m_biHide.bPushed ^ bWasPushed) ||
                  (m_biHide.bRaised ^ bWasRaised);

	////////////////////////////////////////////////////////////
	// hit stud
	bHit = (OnNcHitTest(pt) == HTSTUD);
	bWasPushed = m_stud.bPushed;
    m_stud.bPushed = bHit && bLButtonDown;

    bWasRaised = m_stud.bRaised;
    m_stud.bRaised = bHit && !bLButtonDown;

	bNeedPaint |= (m_stud.bPushed ^ bWasPushed) ||
                  (m_stud.bRaised ^ bWasRaised);

    if (bNeedPaint)
        SendMessage(WM_NCPAINT);

}

// draw title bar
void CDockPageBar::NcPaintGripper(CDC *pDC, CRect rcClient)
{
    if (!HasGripper())
        return;

    CRect gripper = rcClient;
    CRect rcbtn = m_biHide.GetRect();


    gripper.DeflateRect(1, 1);
	gripper.top -= m_cyGripper;
	gripper.bottom = gripper.top + 16;

	HFONT oldFont = (HFONT)pDC->SelectObject (m_font);

	int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF crOldText;

	if(TRUE == m_isActive)			// active state
	{
		CBrush brush(RGB(10,36,106));
		pDC->FillRect(&gripper, &brush);

		crOldText = pDC->SetTextColor(RGB(255,255,255));
	}
	else
	{
		CPen pen(PS_SOLID, 1, RGB(128,128,128));
		
		HPEN poldPen = (HPEN)pDC->SelectObject (&pen);
		
		pDC->MoveTo (gripper.TopLeft());
		pDC->LineTo (gripper.right ,gripper.top );
		pDC->LineTo (gripper.BottomRight() );
		pDC->LineTo (gripper.left ,gripper.bottom );
		pDC->LineTo (gripper.TopLeft ());
		pDC->SelectObject (poldPen);

		crOldText = pDC->SetTextColor(RGB(0,0,0));
	}
	
	gripper.left += 4;
	gripper.top += 2;

	// draw caption
	if (!m_Title.IsEmpty())
	{
		CString sText = m_Title;
		int l = sText.GetLength();
		int i;

		for(i=0;i<10 && pDC->GetTextExtent(sText).cx > (gripper.Width() - 30);i++,l-=2)
			sText = sText.Left(l-2);
		if(i > 0)
		{
			sText = sText.Left(l-2);
			sText += "...";
		}
		pDC->TextOut (gripper.left, gripper.top, sText);
	}

	pDC->SetTextColor (crOldText);
	pDC->SetBkMode(nPrevBkMode);
	pDC->SelectObject(oldFont);

	CPoint ptOrgBtn;
    ptOrgBtn = CPoint(gripper.right - 15, gripper.top);
	m_biHide.Move(ptOrgBtn);
    m_biHide.Paint(pDC, m_isActive);

	ptOrgBtn.x -= 17;
	m_stud.Move (ptOrgBtn);
	m_stud.Paint(pDC, m_isActive);
}

void CDockPageBar::NcCalcClient(LPRECT pRc, UINT nDockBarID)
{
	CRect rcBar(pRc); // save the bar rect

    // subtract edges
    baseCDockPageBar::NcCalcClient(pRc, nDockBarID);

    if (!HasGripper())
        return;

    CRect rc(pRc); // the client rect as calculated by the base class

    BOOL bHorz = (nDockBarID == AFX_IDW_DOCKBAR_TOP) ||
                 (nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);

    if (bHorz)
        rc.DeflateRect(0, m_cyGripper, 1, 0);
    else
        rc.DeflateRect(0, m_cyGripper, 0, 0);

    *pRc = rc;
}

void CDockPageBar::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	if (nHitTest == HTCLOSE)
        m_pDockSite->ShowControlBar(this, FALSE, FALSE); // hide
	if (nHitTest == HTSTUD)
		m_stud.bFloat = ~m_stud.bFloat;

	baseCDockPageBar::OnNcLButtonUp(nHitTest, point);
}

BOOL CDockPageBar::HasGripper() const
{
	if (IsFloating())
        return FALSE;
    return TRUE;
}

LRESULT CDockPageBar::OnNcHitTest(CPoint point) 
{
    CRect rcBar;
    GetWindowRect(rcBar);

    UINT nRet = baseCDockPageBar::OnNcHitTest(point);
    if (nRet != HTCLIENT)
        return nRet;

    CRect rc = m_biHide.GetRect();
    rc.OffsetRect(rcBar.TopLeft());
    if (rc.PtInRect(point))
        return HTCLOSE;

	rc = m_stud.GetRect();
    rc.OffsetRect(rcBar.TopLeft());
    if (rc.PtInRect(point))
        return HTSTUD;

    return HTCLIENT;
}


/////////////////////////////////////////////////////////////////////////////
// about PageItem

void CDockPageBar::GetClientRect(LPRECT lpRect, BOOL isMulti)
{
	CWnd::GetClientRect(lpRect);
	
	if(TRUE == isMulti)
		lpRect->bottom -= ITEMBUTTON_HEIGHT;
}

CPageItem* CDockPageBar::GetPageItem(int nIndex)
{
	CPageItem *pItem = NULL;
	POSITION pos = m_PageList.FindIndex(nIndex);
	if(pos)
		pItem = (CPageItem*)m_PageList.GetAt(pos);

	return pItem;
}

void CDockPageBar::SetActivePage(int nIndex)
{
	if(nIndex == m_nActivePage)
		return;
	CPageItem *pItem;

	pItem = (CPageItem*)GetPageItem(nIndex);

	if(!pItem) return;
	m_nActivePage = nIndex;

	m_Title = pItem->m_sText;

	SetWindowText(m_Title);
	SendMessage(WM_NCPAINT);
	Invalidate();
}

void CDockPageBar::OnSize(UINT nType, int cx, int cy) 
{
	UpdateSize();
	Invalidate(FALSE);
}

void CDockPageBar::UpdateSize()
{
	UINT PageCount = m_PageList.GetCount();
	if(PageCount < 1) return;

	CPageItem	*pItem;
	CRect		rect,ClientRect,ItemRect;
	
	if(PageCount == 1) 
	{
		GetClientRect(ClientRect, FALSE);

		pItem = (CPageItem*)GetPageItem(0);;
		if(pItem->m_pWnd)
		{
				pItem->m_pWnd->MoveWindow(ClientRect);
				pItem->m_pWnd->ShowWindow(SW_SHOW);
		}
		return;
	}

	POSITION	pos;

	GetClientRect(rect, FALSE);
	rect.left += 6;
	rect.right -= 6;
	rect.bottom -= 1;
	rect.top = rect.bottom - ITEMBUTTON_HEIGHT + 2;


	GetClientRect(ClientRect, TRUE);

	ItemRect = rect;
	int AreaWidth = 0,ItemWidth,ItemIndex=0;

	for(pos=m_PageList.GetHeadPosition();pos!=NULL;ItemIndex++)
	{
		pItem=(CPageItem*)m_PageList.GetNext(pos);
		if(pItem)
		{
			ItemWidth = pItem->m_TabWidth;
			AreaWidth += ItemWidth;
			ItemRect.right = ItemRect.left+ItemWidth-1;
			pItem->m_rect = ItemRect;
			ItemRect.left = ItemRect.right + 1;
			if(pItem->m_pWnd)
				pItem->m_pWnd->MoveWindow(ClientRect);
		}
	}

	if(AreaWidth > rect.Width())
	{
		ItemRect = rect;
		int AreaWidth,MaxWidth = rect.Width()/PageCount;
		for(pos=m_PageList.GetHeadPosition();pos!=NULL;)
		{
			pItem=(CPageItem*)m_PageList.GetNext(pos);
			if(pItem)
			{
				AreaWidth = pItem->m_TabWidth;
				ItemWidth = (MaxWidth < AreaWidth)?MaxWidth:AreaWidth;
				ItemRect.right = ItemRect.left+ItemWidth;
				pItem->m_rect = ItemRect;
				ItemRect.left = ItemRect.right + 1;
			}
		}
	}
}

BOOL CDockPageBar::AddPage(CWnd *pWnd, LPCTSTR sText, UINT IconID)
{
	ASSERT(IsWindow(pWnd->m_hWnd));
	CPageItem *pItem;
	pItem = new CPageItem();
	pItem->m_pWnd = pWnd;
	pItem->m_pWnd->SetParent (this);
	pItem->m_sText = sText;
	CClientDC dc(this);
	
	pItem->m_TabWidth = dc.GetTextExtent (sText).cx;
	if(IconID)
	{
		pItem->m_hIcon = AfxGetApp()->LoadIcon(IconID);
		pItem->m_TabWidth += 18;
	}
	else
		pItem->m_hIcon = NULL;
	pItem->m_TabWidth += 9;
	if(pWnd)
	{
		CRect	rect;
		GetClientRect(rect, m_PageList.GetCount()>1?TRUE:FALSE);
		pWnd->MoveWindow(rect);
		pWnd->ShowWindow(SW_HIDE);
	}
	m_PageList.AddTail(pItem);
	UpdateWindow();
	return TRUE;
}

BOOL CDockPageBar::AddPage(CPageItem *pPageItem)
{
	CPageItem *pItem;
	pItem = new CPageItem();
	memcpy(pItem, pPageItem, sizeof(CPageItem));
	pItem->m_pWnd->SetParent (this);
	
	m_PageList.AddTail(pItem);
	//m_nActivePage = -1;
	UpdateWindow();
	return TRUE;
}

BOOL CDockPageBar::AddPage(CDockPageBar *dockPageBar)
{
	POSITION	pos;
	CPageItem* pItem;
	for(pos = dockPageBar->m_PageList.GetHeadPosition();pos!=NULL;)
	{
		pItem=(CPageItem*)dockPageBar->m_PageList.GetNext(pos);
		if(pItem)
		{
			AddPage(pItem);
		}
	}

	dockPageBar->m_pDockContext->m_pDockSite->FloatControlBar(dockPageBar,CSize(0,0),0);
	dockPageBar->m_PageList.RemoveAll();
	UpdateWindow();
	return dockPageBar->DestroyWindow ();
}

CPageItem* CDockPageBar::DeletePage()
{
	CPageItem *pItem = NULL;
	POSITION pos = m_PageList.FindIndex(m_nActivePage);
	if(!pos)
	{
		return NULL;
	}
	pItem = (CPageItem*)m_PageList.GetAt(pos);
	pItem->m_pWnd->ShowWindow(SW_HIDE);
	m_PageList.RemoveAt (pos);
	m_nActivePage = -1;
	UpdateWindow();
	return pItem;
}

void CDockPageBar::UpdateWindow()
{
	UpdateSize();
	if(m_nActivePage < 0)
 		SetActivePage(0);
 	Invalidate(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// 
void CDockPageBar::OnPaint() 
{
	CPaintDC	dc(this); 
	if(m_PageList.GetCount() == 0)
		return;

	CPageItem	*pItem;
	POSITION	pos;
	int			nItemIndex = 0;
	CRect		rect;
	GetClientRect(rect, FALSE);

	rect.left +=1;
	rect.right -=1;
	rect.bottom -=1;
	rect.top = rect.bottom - ITEMBUTTON_HEIGHT + 4;

	CBrush brush(RGB(247,243,233));
	dc.FillRect(rect,&brush);

	CPen pen(PS_SOLID,1,GetSysColor(COLOR_3DDKSHADOW));
	HPEN oldPen = (HPEN)dc.SelectObject (&pen);
	dc.MoveTo (rect.TopLeft() );
	dc.LineTo (rect.right, rect.top);
	dc.SelectObject (oldPen);

	HFONT oldFont = (HFONT)dc.SelectObject (m_font);
	// draw PageItem
	for(pos=m_PageList.GetHeadPosition();pos!=NULL;nItemIndex++)
	{
		pItem=(CPageItem*)m_PageList.GetNext(pos);
		if(pItem)
		{
			pItem->Draw(&dc,(m_nActivePage==nItemIndex)?TRUE:FALSE);
		}

	}
	dc.SelectObject (oldFont);
}

void CDockPageBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT nItemIndex=0;
	POSITION pos;
	CPageItem *pItem;

	SetFocus();

	for(pos=m_PageList.GetHeadPosition();pos!=NULL;nItemIndex++)
	{
		pItem=(CPageItem*)m_PageList.GetNext(pos);
		if(pItem)
		{
			if(pItem->m_rect.PtInRect(point))
			{
				SetActivePage(nItemIndex);
				CanDrag(point);			// drag PageItem
				return;
			}
		}
	}
	((CDockPageBarContext*)m_pDockContext)->m_isPage = FALSE;
	//baseCDockPageBar::OnLButtonDown(nFlags, point);
	if (m_pDockBar != NULL)
    {
        // start the drag
        ASSERT(m_pDockContext != NULL);
        ClientToScreen(&point);
        m_pDockContext->StartDrag(point);
    }
    else
        CWnd::OnLButtonDown(nFlags, point);
}

void CDockPageBar::EnableDocking(DWORD dwDockStyle)
{
	// must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
    ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);
    // cannot have the CBRS_FLOAT_MULTI style
    ASSERT((dwDockStyle & CBRS_FLOAT_MULTI) == 0);
    // the bar must have CBRS_SIZE_DYNAMIC style
    ASSERT((m_dwStyle & CBRS_SIZE_DYNAMIC) != 0);
	
    m_dwDockStyle = dwDockStyle;
    if (m_pDockContext == NULL)
        m_pDockContext = new CDockPageBarContext(this);
	
    // permanently wire the bar's owner to its current parent
    if (m_hWndOwner == NULL)
        m_hWndOwner = ::GetParent(m_hWnd);
}

void CDockPageBar::CanDrag(CPoint pt)
{
	SetCapture();
	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == this)
	{
		MSG msg;
		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage(msg.wParam);
			break;
		}
		switch (msg.message)
		{
		case WM_LBUTTONUP:			
			ReleaseCapture();
			break;
		case WM_MOUSEMOVE:
			Move(msg.pt);
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}
}

void CDockPageBar::Move(CPoint pt)
{
	CRect rect;
	GetClientRect(&rect, FALSE);
	rect.top = rect.bottom - ITEMBUTTON_HEIGHT - 5;
	ScreenToClient(&pt);
	if(rect.PtInRect(pt))
	{
		int nItemIndex=0;
		POSITION pos;
		CPageItem *pItem;
		for(pos=m_PageList.GetHeadPosition();pos!=NULL;nItemIndex++)
		{
			pItem=(CPageItem*)m_PageList.GetNext(pos);
			if(pItem)
			{
				if(pItem->m_rect.PtInRect(pt))
				{
					if(nItemIndex != m_nActivePage)
					{
						POSITION oldPos = m_PageList.FindIndex(m_nActivePage);
						POSITION curPos = m_PageList.FindIndex(nItemIndex);
						CPageItem *pOldItem = (CPageItem*)m_PageList.GetAt(oldPos);
						// exchange PageItem
						m_PageList.SetAt(oldPos, pItem);
						m_PageList.SetAt(curPos, pOldItem);

						m_nActivePage = nItemIndex;
						UpdateWindow();
						break;
					}
				}
			}
		}
	}
	else
	{
		ReleaseCapture();
		StartDrag(pt);
	}
}

void CDockPageBar::StartDrag(CPoint pt)
{
	ClientToScreen(&pt);
	((CDockPageBarContext*)m_pDockContext)->m_isPage = TRUE;
	m_pDockContext->StartDrag(pt);
	((CDockPageBarContext*)m_pDockContext)->m_isPage = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CDockPageBarContext

#define _AfxGetDlgCtrlID(hWnd)          ((UINT)(WORD)::GetDlgCtrlID(hWnd))

#define CX_BORDER   1
#define CY_BORDER   1

#define CBRS_ALIGN_INSERT_PAGE				0x0001L	// drag to another CDockPageBar
#define HORZF(dw) (dw & CBRS_ORIENT_HORZ)
#define VERTF(dw) (dw & CBRS_ORIENT_VERT)

AFX_STATIC void AFXAPI _AfxAdjustRectangle(CRect& rect, CPoint pt)
{
	int nXOffset = (pt.x < rect.left) ? (pt.x - rect.left) :
					(pt.x > rect.right) ? (pt.x - rect.right) : 0;
	int nYOffset = (pt.y < rect.top) ? (pt.y - rect.top) :
					(pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
	rect.OffsetRect(nXOffset, nYOffset);
}

void CDockPageBarContext::StartDrag(CPoint pt)
{
	ASSERT_VALID(m_pBar);
	m_bDragging = TRUE;

	InitLoop();

	// GetWindowRect returns screen coordinates(not mirrored),
	// so if the desktop is mirrored then turn off mirroring
	// for the desktop dc so that we get correct focus rect drawn.
	// This layout change should be remembered, just in case ...

	if (m_pDC->GetLayout() & LAYOUT_RTL)
		m_pDC->SetLayout(LAYOUT_LTR);

	if (m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
	{
		// get true bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptLast = pt;
		CSize sizeHorz = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK);
		CSize sizeVert = m_pBar->CalcDynamicLayout(0, LM_VERTDOCK);
		CSize sizeFloat = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);

		m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
		m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = CRect(rect.TopLeft(), sizeFloat);
		m_rectFrameDragVert = CRect(rect.TopLeft(), sizeFloat);

		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);

		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}
	else if (m_pBar->m_dwStyle & CBRS_SIZE_FIXED)
	{
		// get true bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptLast = pt;
		CSize sizeHorz = m_pBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
		CSize sizeVert = m_pBar->CalcDynamicLayout(-1, LM_VERTDOCK);

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
		m_rectFrameDragVert = m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}
	else
	{
		// get true bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptLast = pt;
		BOOL bHorz = HORZF(m_dwStyle);
		DWORD dwMode = !bHorz ? (LM_HORZ | LM_HORZDOCK) : LM_VERTDOCK;
		CSize size = m_pBar->CalcDynamicLayout(-1, dwMode);

		// calculate inverted dragging rect
		if (bHorz)
		{
			m_rectDragHorz = rect;
			m_rectDragVert = CRect(CPoint(pt.x - rect.Height()/2, rect.top), size);
		}
		else // vertical orientation
		{
			m_rectDragVert = rect;
			m_rectDragHorz = CRect(CPoint(rect.left, pt.y - rect.Width()/2), size);
		}

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = m_rectDragHorz;
		m_rectFrameDragVert = m_rectDragVert;

		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}

	// adjust rectangles so that point is inside
	_AfxAdjustRectangle(m_rectDragHorz, pt);
	_AfxAdjustRectangle(m_rectDragVert, pt);
	_AfxAdjustRectangle(m_rectFrameDragHorz, pt);
	_AfxAdjustRectangle(m_rectFrameDragVert, pt);

	// initialize tracking state and enter tracking loop
	m_dwOverDockStyle = CanDock();
	Move(pt);   // call it here to handle special keys
	Track();
}

BOOL CDockPageBarContext::Track()
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	// set capture to the window which received this message
	m_pBar->SetCapture();
	ASSERT(m_pBar == CWnd::GetCapture());

	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == m_pBar)
	{
		MSG msg;
		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage(msg.wParam);
			break;
		}

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			if (m_bDragging)
				EndDrag();
			else
				EndResize();
			return TRUE;
		case WM_MOUSEMOVE:
			if (m_bDragging)
				Move(msg.pt);
			else
				Stretch(msg.pt);
			break;
		case WM_KEYUP:
			if (m_bDragging)
				OnKey((int)msg.wParam, FALSE);
			break;
		case WM_KEYDOWN:
			if (m_bDragging)
				OnKey((int)msg.wParam, TRUE);
			if (msg.wParam == VK_ESCAPE)
			{
				CancelLoop();
				return FALSE;
			}
			break;
		case WM_RBUTTONDOWN:
			CancelLoop();
			return FALSE;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

	CancelLoop();

	return FALSE;
}

void CDockPageBarContext::Move(CPoint pt)
{
	CPoint ptOffset = pt - m_ptLast;

	if(TRUE == isDockPage(pt))
	{
		m_dwOverDockStyle = CBRS_ALIGN_INSERT_PAGE;
	}
	else
	{
		// offset all drag rects to new position
		m_rectDragHorz.OffsetRect(ptOffset);
		m_rectFrameDragHorz.OffsetRect(ptOffset);
		m_rectDragVert.OffsetRect(ptOffset);
		m_rectFrameDragVert.OffsetRect(ptOffset);

		// if control key is down don't dock
		m_dwOverDockStyle = m_bForceFrame ? 0 : CanDock();
	}

	m_ptLast = pt;

	// update feedback
	if(FALSE == m_isMe)
		DrawFocusRect();
	else
		DrawFocusRect(TRUE);
}

// get the target CDockPageBar on mouse move
BOOL CDockPageBarContext::isDockPage(CPoint pt)
{
	m_isMe = FALSE;
	HWND hWnd = WindowFromPoint(pt);
	CDockPageBar* pBar = (CDockPageBar*)CWnd::FromHandle (hWnd);
	if(pBar->IsKindOf (RUNTIME_CLASS(CDockPageBar)))
	{
		if(pBar != m_pBar)
		{
			CRect rect;
			pBar->GetWindowRect (&rect);
			rect.bottom = rect.top + pBar->m_cyGripper;
			if(rect.PtInRect (pt))
			{
				m_pTgDockPage = pBar;
				pBar->GetWindowRect (&m_addRect);
				return TRUE;
			}
		}
		else
		{
			m_isMe = TRUE;
		}
	}

	m_pTgDockPage = NULL;
	return FALSE;
}

void CDockPageBarContext::EndDrag()
{
	CancelLoop();

	// drag to another CDockPageBar
	if((NULL != m_pTgDockPage) && (m_dwOverDockStyle == CBRS_ALIGN_INSERT_PAGE))
	{
		if(TRUE == m_isPage)	// drag PageItem
		{
			CPageItem * pItem = (CPageItem *)((CDockPageBar*)m_pBar)->DeletePage ();
			m_pTgDockPage->AddPage(pItem);
		}
		else					// drag a CDockPageBar object to another CDockPageBar
		{
			m_pTgDockPage->AddPage((CDockPageBar*)m_pBar);
		}
		return;
	}

	CControlBar* pBar;
	if(TRUE == m_isPage)		// drag PageItem (dock or float)
		pBar = CreateNewBar();
	else
		pBar = m_pBar;			// drag CDockPageBar

	if(NULL == pBar)
		return;

	if (m_dwOverDockStyle != 0)
	{
		CDockBar* pDockBar = GetDockBar(m_dwOverDockStyle);
		ASSERT(pDockBar != NULL);

		CRect rect = (m_dwOverDockStyle & CBRS_ORIENT_VERT) ?
			m_rectDragVert : m_rectDragHorz;

		UINT uID = _AfxGetDlgCtrlID(pDockBar->m_hWnd);
		if (uID >= AFX_IDW_DOCKBAR_TOP &&
			uID <= AFX_IDW_DOCKBAR_BOTTOM)
		{
			m_uMRUDockID = uID;
			m_rectMRUDockPos = rect;
			pDockBar->ScreenToClient(&m_rectMRUDockPos);
		}

		// dock it at the specified position, RecalcLayout will snap
		m_pDockSite->DockControlBar(pBar, pDockBar, &rect);
		m_pDockSite->RecalcLayout();
	}
	else if ((m_dwStyle & CBRS_SIZE_DYNAMIC) || (HORZF(m_dwStyle) && !m_bFlip) ||
			(VERTF(m_dwStyle) && m_bFlip))
	{
		m_dwMRUFloatStyle = CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI);
		m_ptMRUFloatPos = m_rectFrameDragHorz.TopLeft();
		m_pDockSite->FloatControlBar(pBar, m_ptMRUFloatPos, m_dwMRUFloatStyle);
	}
	else // vertical float
	{
		m_dwMRUFloatStyle = CBRS_ALIGN_LEFT | (m_dwDockStyle & CBRS_FLOAT_MULTI);
		m_ptMRUFloatPos = m_rectFrameDragVert.TopLeft();
		m_pDockSite->FloatControlBar(pBar, m_ptMRUFloatPos, m_dwMRUFloatStyle);
	}
}

CDockPageBar* CDockPageBarContext::CreateNewBar()
{
	CPageItem * pItem = (CPageItem *)((CDockPageBar*)m_pBar)->DeletePage ();

	CDockPageBar* pBar = new CDockPageBar();
	
	if (!(pBar->Create(_T("New Bar"), m_pDockSite, 12345)))
	{
		TRACE0("Failed to create mybar\n");
		return NULL;      // fail to create
	}
	
	pBar->SetBarStyle(m_pBar->GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	
	pBar->EnableDocking(CBRS_ALIGN_ANY);
	pBar->AddPage (pItem);

	return pBar;
}

void CDockPageBarContext::DrawFocusRect(BOOL bRemoveRect)
{
	ASSERT(m_pDC != NULL);

	// default to thin frame
	CSize size(CX_BORDER, CY_BORDER);

	// determine new rect and size
	CRect rect;
	CBrush* pWhiteBrush = CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));
	CBrush* pDitherBrush = CDC::GetHalftoneBrush();
	CBrush* pBrush = pWhiteBrush;

	if (HORZF(m_dwOverDockStyle))
		rect = m_rectDragHorz;
	else if (VERTF(m_dwOverDockStyle))
		rect = m_rectDragVert;
	else
	{
		// use thick frame instead
		size.cx = GetSystemMetrics(SM_CXFRAME) - CX_BORDER;
		size.cy = GetSystemMetrics(SM_CYFRAME) - CY_BORDER;
		if ((HORZF(m_dwStyle) && !m_bFlip) || (VERTF(m_dwStyle) && m_bFlip))
			rect = m_rectFrameDragHorz;
		else
			rect = m_rectFrameDragVert;
		pBrush = pDitherBrush;
	}
	if (bRemoveRect)
		size.cx = size.cy = 0;

	if(m_dwOverDockStyle == CBRS_ALIGN_INSERT_PAGE)
		rect = m_addRect;		// rect of target CDockPageBar

	// draw it and remember last size
	m_pDC->DrawDragRect(&rect, size, &m_rectLast, m_sizeLast,
		pBrush, m_bDitherLast ? pDitherBrush : pWhiteBrush);
	m_rectLast = rect;
	m_sizeLast = size;
	m_bDitherLast = (pBrush == pDitherBrush);
}
