// THUIFCerDoc.h : CTHUIFCerDoc 类的接口
//


#pragma once
#include "IFCProject.h"

class CTHUIFCerDoc : public CDocument
{
protected: // 仅从序列化创建
	CTHUIFCerDoc();
	DECLARE_DYNCREATE(CTHUIFCerDoc)

// 属性
public:
	CIFCProject * ifcProject;
	CString AppPath;
	CString SchemaPath;

// 操作
public:
	CView * GetViewPtr(CRuntimeClass * pClass);
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CTHUIFCerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};