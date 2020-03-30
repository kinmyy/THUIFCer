//Ãæ°å

class CPanel : public CDockPageBar
{
	// Construction
public:
	CPanel();
	using CDockPageBar::AddPage;

public:
	BOOL AddPage(CRuntimeClass* pClass,UINT nIDTemplate, LPCTSTR sText, UINT IconID);
	virtual ~CPanel();

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};
