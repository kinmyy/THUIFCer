#pragma once

// StatusBar.h : ×´Ì¬À¸

class CMyStatusBar : public CStatusBar
{
public:
	CMyStatusBar();
	BOOL SetIndicators (const UINT* lpIDArray, int nIDCount);
	virtual ~CMyStatusBar();

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
