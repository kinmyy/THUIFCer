// THUIFCerDoc.h : CTHUIFCerDoc ��Ľӿ�
//


#pragma once
#include "IFCProject.h"

class CTHUIFCerDoc : public CDocument
{
protected: // �������л�����
	CTHUIFCerDoc();
	DECLARE_DYNCREATE(CTHUIFCerDoc)

// ����
public:
	CIFCProject * ifcProject;
	CString AppPath;
	CString SchemaPath;

// ����
public:
	CView * GetViewPtr(CRuntimeClass * pClass);
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CTHUIFCerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};