// THUIFCerView.h : CTHUIFCerView 类的接口
//


#pragma once
#include "resource.h"
#include "THUIFCerDoc.h"
#include "CIFCRender.h"
#include "Potential.h"
#include <map>
#include "FastMarching.h"
#include "SetValueDialog.h"
#include"AntDialog.h"
#include"AntColony.h"

class CTHUIFCerView : public CFormView
{
protected: // 仅从序列化创建
	CTHUIFCerView();
	DECLARE_DYNCREATE(CTHUIFCerView)

public:
	enum{ IDD = IDD_THUIFCER_FORM };
	HWND m_hwndRenderWindow;       //渲染窗口句柄
	CIFCRender * ifcRender;
	vector<CString>doorName;

// 属性
public:
	CTHUIFCerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CTHUIFCerView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDraw(CDC* /*pDC*/);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void On3dResetside();
	afx_msg void On3dResetfront();
	afx_msg void On3dResettop();
	afx_msg void OnEditOption();
	afx_msg void OnFileSaveEntityxml();
	afx_msg void OnFileSavePropertyxml();
	afx_msg LRESULT OnOpenIfc(WPARAM wParam , LPARAM lParam);
	afx_msg void OnAnalysisCarbon();
	afx_msg void OnFileSaveAsifc();
	afx_msg void OnFileNew();
	afx_msg void OnFileSave();
	afx_msg void OnSummaryPlate();
	afx_msg void OnSummaryWall();
	afx_msg void OnSummaryDoor();
	afx_msg void OnSummaryWindow();
	afx_msg void OnAnalysisSearch();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	//afx_msg void OnPathsimulate();

	//人工势场法路径分析相关属性
public:
	vector<Particle>start;//起始点
	vector<Particle>goal;//目标点
	vector<Particle>obstacle;//静态障碍
	double r;//粒子半径
	int mm;//设置参数标志，0为未设置
	bool reset;
	vector<Potential> Controllor;
	bool IsPause;//是否暂停
	bool clock;
	bool IsReady;
	bool Isgo;
	//FastMarching fm;
	int method;
	vector<D3DXVECTOR3> fmmPath;
	int timer;

	//设置的参数
	double start_x;
	double start_y;
	double end_x;
	double end_y;
	int grid_size;
	SetValueDialog parDialog;
	FastMarching *fm;
	AntDialog antDialog;
	AntColony *antColony;

	//相关函数
public:
	double Dis(Particle p1,Particle p2);
	vector<D3DXVECTOR3> GetDistance(Place p);
	vector<D3DXVECTOR3> GetGroup(int i);
	double GetPathDistance(vector<D3DXVECTOR3>fmmPath);
public:
	afx_msg void OnEnergy();
public:
	afx_msg void OnFmm();
public:
	afx_msg void OnPathsimulatePotential();
public:
	afx_msg void OnPathsimulateFastmarching();
public:
	afx_msg void OnPathsimulateFmtoolbox();
public:
	afx_msg void OnPathsimulateMf();
public:
	afx_msg void OnPathsimulateAstaralgorithm();
public:
	afx_msg void OnPathsimulateAntcolonyalgorithm();
public:
	afx_msg void OnPathsimulateMulti32841();
};

#ifndef _DEBUG  // THUIFCerView.cpp 中的调试版本
inline CTHUIFCerDoc* CTHUIFCerView::GetDocument() const
   { return reinterpret_cast<CTHUIFCerDoc*>(m_pDocument); }
#endif

