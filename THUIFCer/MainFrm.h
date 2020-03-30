// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "StatusBar.h"
#include "Panel.h"
#include "EntityTree.h"
#include "PropertyList.h"

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:
	CEntityTree *GetEntityTree();
	CPropertyList *GetPropertyList();

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMyStatusBar     m_wndStatusBar;            //״̬��
	CToolBar         m_wndToolBar;
	CReBar           m_wndReBar;
	CDialogBar       m_wndDlgBar;
	CPanel           m_wndDockPageLeftTop;      //�������
	CPanel           m_wndDockPageRightTop;     //�������
	CEntityTree      m_treeEntitySheet;         //ʵ�����ؼ�
	CPropertyList    m_listPropertySheet;       //���Կؼ�

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};
