// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "StatusBar.h"
#include "Panel.h"
#include "EntityTree.h"
#include "PropertyList.h"

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:
	CEntityTree *GetEntityTree();
	CPropertyList *GetPropertyList();

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMyStatusBar     m_wndStatusBar;            //状态栏
	CToolBar         m_wndToolBar;
	CReBar           m_wndReBar;
	CDialogBar       m_wndDlgBar;
	CPanel           m_wndDockPageLeftTop;      //左上面板
	CPanel           m_wndDockPageRightTop;     //右上面板
	CEntityTree      m_treeEntitySheet;         //实体树控件
	CPropertyList    m_listPropertySheet;       //属性控件

	// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};
