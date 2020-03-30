// THUIFCerView.h : CTHUIFCerView ��Ľӿ�
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
protected: // �������л�����
	CTHUIFCerView();
	DECLARE_DYNCREATE(CTHUIFCerView)

public:
	enum{ IDD = IDD_THUIFCER_FORM };
	HWND m_hwndRenderWindow;       //��Ⱦ���ھ��
	CIFCRender * ifcRender;
	vector<CString>doorName;

// ����
public:
	CTHUIFCerDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CTHUIFCerView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

	//�˹��Ƴ���·�������������
public:
	vector<Particle>start;//��ʼ��
	vector<Particle>goal;//Ŀ���
	vector<Particle>obstacle;//��̬�ϰ�
	double r;//���Ӱ뾶
	int mm;//���ò�����־��0Ϊδ����
	bool reset;
	vector<Potential> Controllor;
	bool IsPause;//�Ƿ���ͣ
	bool clock;
	bool IsReady;
	bool Isgo;
	//FastMarching fm;
	int method;
	vector<D3DXVECTOR3> fmmPath;
	int timer;

	//���õĲ���
	double start_x;
	double start_y;
	double end_x;
	double end_y;
	int grid_size;
	SetValueDialog parDialog;
	FastMarching *fm;
	AntDialog antDialog;
	AntColony *antColony;

	//��غ���
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

#ifndef _DEBUG  // THUIFCerView.cpp �еĵ��԰汾
inline CTHUIFCerDoc* CTHUIFCerView::GetDocument() const
   { return reinterpret_cast<CTHUIFCerDoc*>(m_pDocument); }
#endif

