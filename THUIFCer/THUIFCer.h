// THUIFCer.h : THUIFCer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CTHUIFCerApp:
// �йش����ʵ�֣������ THUIFCer.cpp
//

class CTHUIFCerApp : public CWinApp
{
public:
	CTHUIFCerApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL InitApplication();
};

extern CTHUIFCerApp theApp;