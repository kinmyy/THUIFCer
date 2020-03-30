// PropertyList.cpp : 实现文件
//
#pragma once

#include "stdafx.h"
#include "THUIFCer.h"
#include "PropertyList.h"
#include "MainFrm.h"
#include "THUIFCerView.h"
#include "IFCProject.h"

// CPropertyList

IMPLEMENT_DYNAMIC(CPropertyList, CListCtrl)

CPropertyList::CPropertyList()
{

}

CPropertyList::~CPropertyList()
{
}


BEGIN_MESSAGE_MAP(CPropertyList, CListCtrl)
	ON_WM_CREATE()
	//ON_NOTIFY(NM_CUSTOMDRAW,IDC_LIST_IFC,OnNMCustomdraw)
END_MESSAGE_MAP()



// CPropertyList 消息处理程序



LRESULT CPropertyList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case ID_UPDATE_CLEAR:
		DeleteAllItems();
		break;
	case ID_UPDATE_PROPERTYLIST:
		/*
		CImageList* pImageList = new CImageList();
		pImageList->Create(16, 16, ILC_COLOR24, 4, 4);
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP_UNCHECKED);
		pImageList->Add(&bitmap, (COLORREF)0x000000);
		bitmap.DeleteObject();
		SetImageList(pImageList, LVSIL_SMALL);
		*/
		DeleteAllItems();

		int HitItemID=(int)wParam;
		int HitItemType=(int)lParam;

		CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
		CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
		CIFCProject *pIFCProject = p3DView->GetDocument()->ifcProject;

		if (!pIFCProject)
			break;
		if (!pIFCProject->OpenFlag())
			break;

		switch (HitItemType)
		{
		case ITEMTYPE_ROOT:
			//获取工程的属性
			break;
		case ITEMTYPE_ENTITY:
			//获取实体的属性
			break;
		case ITEMTYPE_INSTANCE:
			//获取实例的属性
			PropertyListXml=*(pIFCProject->GetPropertySet(HitItemID));
			TiXmlHandle hXmlRoot(&PropertyListXml);
			TiXmlElement *pElem;

			//PSet属性集的根节点
			pElem=hXmlRoot.FirstChild("InstancePropertySet").FirstChild("PropertySet").ToElement();
			if (!pElem)
				break;

			int i=0; //分组号
			int j=0;
			for (const TiXmlElement *sub_tag =pElem; sub_tag; sub_tag = sub_tag->NextSiblingElement())
			{
				CString PropertySetName(sub_tag->Attribute("PropertySetName"));

				LVGROUP lvGrp;   
				ZeroMemory(&lvGrp,sizeof(LVGROUP));
				lvGrp.cbSize    = sizeof(LVGROUP);
				lvGrp.iGroupId  = i;
				lvGrp.mask      = LVGF_ALIGN|LVGF_STATE|LVGF_HEADER|LVGF_GROUPID;
				lvGrp.state     = LVGS_NORMAL;
				lvGrp.uAlign    = LVGA_HEADER_LEFT;
				lvGrp.cchHeader = 200;
				lvGrp.pszHeader = (LPWSTR)(LPCWSTR)PropertySetName;
				EnableGroupView(TRUE);
				LRESULT lIndex = InsertGroup(i, &lvGrp);

				for (const TiXmlElement *sub_record =sub_tag->FirstChildElement(); sub_record; sub_record = sub_record->NextSiblingElement())
				{
					CString Name(sub_record->Attribute("Name"));
					CString Value(sub_record->Attribute("Value"));
					CString Description(sub_record->Attribute("Description"));

					LVITEM lvi;
					lvi.mask       = LVIF_GROUPID|LVIF_TEXT;
					lvi.iGroupId   = i;
					lvi.iItem      = j;
					lvi.iSubItem   = 0;
					lvi.pszText    = (LPWSTR)(LPCWSTR)Name;
					InsertItem(&lvi);

					SetItemText(j,1,Value);
					SetItemText(j,2,Description);
					j++;
				}
				i++;
			}

			break;
		}

		break;
	}

	return CListCtrl::WindowProc(message, wParam, lParam);
}

int CPropertyList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*
	LVCOLUMN lvcolumn;
	lvcolumn.fmt=LVCFMT_LEFT; //左对齐
	lvcolumn.mask=LVCF_TEXT|LVCF_FMT|LVCF_WIDTH;
	lvcolumn.cx=20;
	lvcolumn.pszText=_T("Name");
	lvcolumn.iSubItem=0;
	InsertColumn(0,&lvcolumn);

	lvcolumn.pszText=_T("Value");
	lvcolumn.iSubItem=1;
	InsertColumn(1,&lvcolumn);

	lvcolumn.pszText=_T("Description");
	lvcolumn.iSubItem=2;
	InsertColumn(2,&lvcolumn);
	*/
	InsertColumn(0,_T("Name"),LVCFMT_LEFT,100,0);
	InsertColumn(1,_T("Value"),LVCFMT_LEFT,100,1);
	InsertColumn(2,_T("Description"),LVCFMT_LEFT,50,2);

	ModifyStyle(LVS_TYPEMASK,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE);

	//分组测试
	/*
	LVGROUP lvGrp;   
	ZeroMemory(&lvGrp,sizeof(LVGROUP));   
	lvGrp.cbSize    = sizeof(LVGROUP);   
	lvGrp.iGroupId  = 1;   
	lvGrp.mask      = LVGF_ALIGN|LVGF_STATE|LVGF_HEADER|LVGF_GROUPID;   
	lvGrp.state     = LVGS_NORMAL;   
	lvGrp.uAlign    = LVGA_HEADER_LEFT;   
	lvGrp.cchHeader = 260;   
	lvGrp.pszHeader = _T("Group");   

	EnableGroupView(TRUE);   
	LRESULT lIndex = InsertGroup(0, &lvGrp);   

	LVITEM lvi;   
	lvi.mask       = LVIF_GROUPID|LVIF_TEXT;   
	lvi.iGroupId   = 1;                           //   IMPORTANT   
	lvi.iItem      = 0;   
	lvi.iSubItem   = 0;   
	lvi.pszText    = _T("item");   
	InsertItem(&lvi);   

	lvi.iItem=1;
	InsertItem(&lvi);   

	lvGrp.iGroupId=2;
	InsertGroup(1,   &lvGrp);
	lvi.iGroupId=2;
	lvi.iItem=0;
	InsertItem(&lvi);
	lvi.iItem=1;
	InsertItem(&lvi);
	lvi.iItem=2;
	InsertItem(&lvi);
	*/

	return 0;
}

void CPropertyList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = CDRF_DODEFAULT;
	NMLVCUSTOMDRAW * lplvdr=(NMLVCUSTOMDRAW*)pNMHDR;
	NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
	AfxMessageBox(_T("call"));

	switch(lplvdr->nmcd.dwDrawStage)//判断状态
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT://画ITEM之前就要进行颜色的改变
		COLORREF ItemColor;
		if(MapItemColor.Lookup(nmcd.dwItemSpec, ItemColor))
		//根据在SetItemColor(DWORD iItem, COLORREF color) 设置的//ITEM号和COLORREF 在摸板中查找，然后进行颜色赋值。
		{
			lplvdr->clrTextBk = ItemColor;
			*pResult = CDRF_DODEFAULT;
		}
	default:
		break;
	}
}

void CPropertyList::SetItemColor(DWORD iItem,COLORREF color)
{
	MapItemColor.SetAt(iItem, color);//设置某行的颜色。
	RedrawItems(iItem, iItem);//重新染色
	SetFocus();//设置焦点
	UpdateWindow();
}