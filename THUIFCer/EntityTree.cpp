// EntityTree.cpp : ʵ���ļ�
//

#pragma once

#include "stdafx.h"
#include "THUIFCer.h"
#include "EntityTree.h"
#include "MainFrm.h"
#include "THUIFCerView.h"
#include "IFCProject.h"
#include "define.h"

// CEntityTree

IMPLEMENT_DYNAMIC(CEntityTree, CTreeCtrl)

CEntityTree::CEntityTree()
{
	HitItemID=0;
	HitItemType=ITEMTYPE_UNKNOWN;
}

CEntityTree::~CEntityTree()
{
}

BEGIN_MESSAGE_MAP(CEntityTree, CTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, &CEntityTree::OnClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CEntityTree::OnNMRclick)
	ON_COMMAND(ID_ENTITYTREE_PROPERTY, &CEntityTree::OnEntitytreeProperty)
	ON_COMMAND(ID_ENTITYTREE_ONLYSHOW, &CEntityTree::OnEntitytreeOnlyshow)
	ON_COMMAND(ID_ENTITYTREE_GROUPSHOW, &CEntityTree::OnEntitytreeGroupshow)
	ON_COMMAND(ID_ENTITYTREE_ALLSHOW, &CEntityTree::OnEntitytreeAllshow)
	ON_COMMAND(ID_ENTITYTREE_ALLHIDE, &CEntityTree::OnEntitytreeAllhide)
	ON_COMMAND(ID_ENTITYTREE_ADDPROPERTY, &CEntityTree::OnEntitytreeAddproperty)
END_MESSAGE_MAP()

// CEntityTree ��Ϣ�������

int CEntityTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CEntityTree::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	DWORD pos = GetMessagePos();
	CPoint pt(LOWORD(pos), HIWORD(pos));
	ScreenToClient(&pt);
	

	hRoot=GetRootItem();
	HTREEITEM hItem=GetNextVisibleItem(hRoot);
	//�������������нڵ�
	while (hItem)
	{
		CRect r;
		GetItemRect(hItem, &r, true);
		r.right = r.left-4;
		r.left = r.left-18;

		//����
		if (r.PtInRect(pt))
		{
			CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
			CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
			CIFCProject *pIFCProject = p3DView->GetDocument()->ifcProject;

			if (!pIFCProject)
				return;
			if (!pIFCProject->OpenFlag())
				return;

			HTREEITEM hParentItem=GetParentItem(hItem);
			if (hParentItem==hRoot) //Ϊʵ��ڵ�
			{
				int GroupID=(int)GetItemData(hItem);
				CString GroupName=pIFCProject->FindEntityName(GroupID);
				CIFCEntity theEntities=pIFCProject->GetEntity(GroupName);
				if (theEntities.stat()==UNCHECKED)
				{
					theEntities.SetAllSelected(TRUE); //ȫ����ʾ
					pIFCProject->SetEntity(GroupName,theEntities); //д��
					SetItemImage(hItem,ITEM_CHECKED,ITEM_CHECKED);
					//�����ӽڵ�
					HTREEITEM hChildItem=GetChildItem(hItem);
					while(hChildItem!=NULL)
					{
						SetItemImage(hChildItem,ITEM_CHECKED,ITEM_CHECKED);
						hChildItem=GetNextSiblingItem(hChildItem);
					}
				}
				else
				{
					theEntities.SetAllSelected(FALSE); //ȫ������ʾ
					pIFCProject->SetEntity(GroupName,theEntities); //д��
					SetItemImage(hItem,ITEM_UNCHECKED,ITEM_UNCHECKED);
					//�����ӽڵ�
					HTREEITEM hChildItem=GetChildItem(hItem);
					while(hChildItem!=NULL)
					{
						SetItemImage(hChildItem,ITEM_UNCHECKED,ITEM_UNCHECKED);
						hChildItem=GetNextSiblingItem(hChildItem);
					}
				}
			}
			HTREEITEM hChildItem=GetChildItem(hItem);
			if (hChildItem==NULL) //Ϊʵ���ڵ�
			{
				int GroupID=(int)GetItemData(hParentItem);
				int InstanceID=(int)GetItemData(hItem);
				CString GroupName=pIFCProject->FindEntityName(GroupID);
				CIFCEntity theEntities=pIFCProject->GetEntity(GroupName);
				CIFCInstance theInstance=theEntities.GetInstance(InstanceID);
				if (theInstance.GetSelected()==TRUE)
				{
					theInstance.SetSelected(FALSE);
					theEntities.SetInstance(InstanceID,theInstance);
					pIFCProject->SetEntity(GroupName,theEntities);
					//����ʵ��ͼ��
					SetItemImage(hItem,ITEM_UNCHECKED,ITEM_UNCHECKED);
					//����ʵ��ͼ��
					if (theEntities.stat()==PARTLYCHECKED)
						SetItemImage(hParentItem,ITEM_PARTLY_CHECKED,ITEM_PARTLY_CHECKED);
					if (theEntities.stat()==UNCHECKED)
						SetItemImage(hParentItem,ITEM_UNCHECKED,ITEM_UNCHECKED);
					if (theEntities.stat()==ALLCHECKED)
						SetItemImage(hParentItem,ITEM_CHECKED,ITEM_CHECKED);
				}
				else
				{
					theInstance.SetSelected(TRUE);
					theEntities.SetInstance(InstanceID,theInstance);
					pIFCProject->SetEntity(GroupName,theEntities);
					//����ͼ��
					SetItemImage(hItem,ITEM_CHECKED,ITEM_CHECKED);
					//����ʵ��ͼ��
					if (theEntities.stat()==PARTLYCHECKED)
						SetItemImage(hParentItem,ITEM_PARTLY_CHECKED,ITEM_PARTLY_CHECKED);
					if (theEntities.stat()==UNCHECKED)
						SetItemImage(hParentItem,ITEM_UNCHECKED,ITEM_UNCHECKED);
					if (theEntities.stat()==ALLCHECKED)
						SetItemImage(hParentItem,ITEM_CHECKED,ITEM_CHECKED);
				}

			}
		}

		hItem = GetNextVisibleItem(hItem);
	}

	//����3ά��ͼ
	CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	((CTHUIFCerView*)pMainFrame->GetActiveView())->SendMessage(ID_UPDATE_IFCVIEW, 0, 0);
	//this->GetWindow(GW_HWNDNEXT)->SendMessage(ID_UPDATE_IFCVIEW, 0, 0);

	*pResult = 0;
}

LRESULT CEntityTree::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case ID_UPDATE_CLEAR:
		HitItemID=0;
		HitItemType=ITEMTYPE_UNKNOWN;
		DeleteAllItems();
		break;
	case ID_UPDATE_ENTITYTREE:
		CImageList* pImageList = new CImageList();
		pImageList->Create(16, 16, ILC_COLOR24, 4, 4);

		CBitmap bitmap;

		//0��δѡ�� ITEM_UNCHECKED
		bitmap.LoadBitmap(IDB_BITMAP_UNCHECKED);
		pImageList->Add(&bitmap, (COLORREF)0x000000);
		bitmap.DeleteObject();

		//1������ѡ�� ITEM_PARTLY_CHECKED
		bitmap.LoadBitmap(IDB_BITMAP_PARTLYCHECKED);
		pImageList->Add(&bitmap, (COLORREF)0x000000);
		bitmap.DeleteObject();

		//2��ѡ�� ITEM_CHECKED
		bitmap.LoadBitmap(IDB_BITMAP_CHECKED);
		pImageList->Add(&bitmap, (COLORREF)0x000000);
		bitmap.DeleteObject();

		//3��Root
		bitmap.LoadBitmap(IDB_BITMAP_ROOT);
		pImageList->Add(&bitmap, (COLORREF)0x000000);
		bitmap.DeleteObject();

		//::SetWindowLong(this->m_hWnd, GWL_STYLE, TVS_LINESATROOT|TVS_HASLINES|TVS_HASBUTTONS|::GetWindowLong(this->m_hWnd, GWL_STYLE));

		SetImageList(pImageList, TVSIL_NORMAL);
		DeleteAllItems();

		//��ȡIFC����
		CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
		CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
		CIFCProject *pIFCProject = p3DView->GetDocument()->ifcProject;

		if (!pIFCProject)
			break;
		if (!pIFCProject->OpenFlag())
			break;

		//�������β�νṹ
		EntityTreeXml=*(pIFCProject->GetEntityTree(TRUE));
		TiXmlHandle hXmlRoot(&EntityTreeXml);
		TiXmlElement *pElem;

		//Project�ڵ�
		pElem=hXmlRoot.FirstChild("Project").Element();
		if (!pElem)
			break;
		CString str(pElem->Value()); //������
		hRoot=InsertItem(str);
		SetItemImage(hRoot,ITEM_ROOT,ITEM_ROOT);

		pElem=hXmlRoot.FirstChild("Project").FirstChild("Entity").ToElement();
		if (!pElem)
			break;

		TV_INSERTSTRUCT parentTvstruct;
		TV_INSERTSTRUCT childTvstruct;

		for (const TiXmlElement *sub_tag =pElem; sub_tag; sub_tag = sub_tag->NextSiblingElement())
		{
			//CString GroupName(sub_tag->Attribute("GroupName"));
			//CString GroupNameIFC(sub_tag->Attribute("GroupNameIFC"));
			CString GroupNameCN(sub_tag->Attribute("GroupNameCN"));
			int GroupID=atoi(sub_tag->Attribute("GroupID"));
			//���ڵ�
			parentTvstruct.hParent = hRoot;    //���ӵ����ڵ�
			parentTvstruct.hInsertAfter = TVI_LAST;
			parentTvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_STATE;
			parentTvstruct.item.stateMask = TVIS_STATEIMAGEMASK;
			parentTvstruct.item.state = INDEXTOSTATEIMAGEMASK(1);
			parentTvstruct.item.cChildren = 1; //�����ӽڵ�
			parentTvstruct.item.lParam = GroupID;
			parentTvstruct.item.pszText = (LPTSTR)(LPCTSTR)GroupNameCN;
			parentTvstruct.item.iImage = ITEM_CHECKED;
			parentTvstruct.item.iSelectedImage = ITEM_CHECKED;
			HTREEITEM pHtreeitem=InsertItem(&parentTvstruct);
			for (const TiXmlElement *sub_record =sub_tag->FirstChildElement(); sub_record; sub_record = sub_record->NextSiblingElement())
			{
				int InstanceID=atoi(sub_record->Attribute("InstanceID"));
				CString InstanceName(sub_record->Attribute("InstanceName"));
				
				childTvstruct.hParent=pHtreeitem;
				childTvstruct.hInsertAfter = TVI_LAST;
				childTvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_STATE;
				childTvstruct.item.stateMask = TVIS_STATEIMAGEMASK;
				childTvstruct.item.state = INDEXTOSTATEIMAGEMASK(1);
				childTvstruct.item.cChildren = 0; //Ϊ��ֹ�ӽڵ�
				childTvstruct.item.lParam = InstanceID;
				childTvstruct.item.pszText = (LPWSTR)(LPCWSTR)InstanceName;
				childTvstruct.item.iImage = ITEM_CHECKED;
				childTvstruct.item.iSelectedImage = ITEM_CHECKED;
				InsertItem(&childTvstruct);
			}
		}
		Expand(hRoot, TVE_EXPAND);//hRoot->Expand(true);
		break;
	}
	
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

void CEntityTree::OnEntitytreeProperty()
{
	//����������ͼ
	CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	((CPropertyList*)pMainFrame->GetPropertyList())->SendMessage(ID_UPDATE_PROPERTYLIST, HitItemID, HitItemType);
}

void CEntityTree::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	CPoint pos; //��Ļ����
	if (!GetCursorPos(&pos))
		return;
	ScreenToClient(&pos); //תΪ����������
	UINT uFlags;
	HTREEITEM hItem=HitTest(pos,&uFlags);

	if (hItem==NULL)
		return;
	if ((hItem!=NULL)&&(TVHT_ONITEM&uFlags))
		Select(hItem,TVGN_CARET);

	if (hItem==hRoot) //���ڵ�
	{
		HitItemType=ITEMTYPE_ROOT;
	}
	else
	{
		HTREEITEM hParentItem=GetParentItem(hItem);
		if (hParentItem==hRoot) //Ϊʵ��ڵ�
		{
			HitItemID=(int)GetItemData(hItem);
			HitItemType=ITEMTYPE_ENTITY;
		}
		HTREEITEM hChildItem=GetChildItem(hItem);
		if (hChildItem==NULL) //Ϊʵ���ڵ�
		{
			HitItemID=(int)GetItemData(hItem);
			HitItemType=ITEMTYPE_INSTANCE;
		}
	}

	CMenu menu;
	menu.LoadMenu(IDR_MENU_ENTITYTREE);
	CMenu *pMenu=menu.GetSubMenu(0);

	if (HitItemType==ITEMTYPE_ROOT)
	{
		pMenu->EnableMenuItem(ID_ENTITYTREE_GROUPSHOW,MF_GRAYED);
		pMenu->EnableMenuItem(ID_ENTITYTREE_ONLYSHOW,MF_GRAYED);
	}
	if (HitItemType==ITEMTYPE_ENTITY)
	{
		pMenu->EnableMenuItem(ID_ENTITYTREE_ALLSHOW,MF_GRAYED);
		pMenu->EnableMenuItem(ID_ENTITYTREE_ALLHIDE,MF_GRAYED);
		pMenu->EnableMenuItem(ID_ENTITYTREE_ONLYSHOW,MF_GRAYED);
	}
	if (HitItemType==ITEMTYPE_INSTANCE)
	{
		pMenu->EnableMenuItem(ID_ENTITYTREE_ALLSHOW,MF_GRAYED);
		pMenu->EnableMenuItem(ID_ENTITYTREE_ALLHIDE,MF_GRAYED);
		pMenu->EnableMenuItem(ID_ENTITYTREE_GROUPSHOW,MF_GRAYED);
	}

	ClientToScreen(&pos); //����ת����Ļ����
	pMenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);
}

void CEntityTree::OnEntitytreeOnlyshow()
{
	CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
	CIFCProject *pIFCProject = p3DView->GetDocument()->ifcProject;

	if (!pIFCProject)
		return;
	if (!pIFCProject->OpenFlag())
		return;

	HTREEITEM hSelectedItem=GetSelectedItem();
	HTREEITEM hParentItem=GetParentItem(hSelectedItem);

	if (HitItemType==ITEMTYPE_INSTANCE)
	{
		int GroupID=(int)GetItemData(hParentItem);

		pIFCProject->SetAllSelected(FALSE);
		pIFCProject->SetInstanceSelected(GroupID,HitItemID,TRUE);

		//����3ά��ͼ
		((CTHUIFCerView*)pMainFrame->GetActiveView())->SendMessage(ID_UPDATE_IFCVIEW, 0, 0);
		//������
		HTREEITEM hEntityItem=GetChildItem(hRoot);
		HTREEITEM hInstanceItem;
		while (hEntityItem)
		{
			SetItemImage(hEntityItem,ITEM_UNCHECKED,ITEM_UNCHECKED);
			hInstanceItem=GetChildItem(hEntityItem);
			while (hInstanceItem)
			{
				if (hInstanceItem==hSelectedItem)
				{
					SetItemImage(hInstanceItem,ITEM_CHECKED,ITEM_CHECKED);
				}
				else
				{
					SetItemImage(hInstanceItem,ITEM_UNCHECKED,ITEM_UNCHECKED);
				}
				hInstanceItem=GetNextSiblingItem(hInstanceItem);
			}
			hEntityItem=GetNextSiblingItem(hEntityItem);
		}
		int count=0;
		HTREEITEM hItem=GetChildItem(hParentItem);
		while (hItem)
		{
			count++;
			hItem=GetNextSiblingItem(hItem);
		}
		if (count==1)
			SetItemImage(hParentItem,ITEM_CHECKED,ITEM_CHECKED);
		else
			SetItemImage(hParentItem,ITEM_PARTLY_CHECKED,ITEM_PARTLY_CHECKED);
	}
}

void CEntityTree::OnEntitytreeGroupshow()
{
	CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
	CIFCProject *pIFCProject = p3DView->GetDocument()->ifcProject;

	if (!pIFCProject)
		return;
	if (!pIFCProject->OpenFlag())
		return;

	HTREEITEM hSelectedItem=GetSelectedItem();
	if (HitItemType==ITEMTYPE_ENTITY)
	{
		pIFCProject->SetAllSelected(FALSE);
		pIFCProject->SetEntitySelected(HitItemID,TRUE);

		//����3ά��ͼ
		((CTHUIFCerView*)pMainFrame->GetActiveView())->SendMessage(ID_UPDATE_IFCVIEW, 0, 0);
		//������
		HTREEITEM hEntityItem=GetChildItem(hRoot);
		HTREEITEM hInstanceItem;
		while (hEntityItem)
		{
			if ((hEntityItem==hSelectedItem))
			{
				SetItemImage(hEntityItem,ITEM_CHECKED,ITEM_CHECKED);
			}
			else
			{
				SetItemImage(hEntityItem,ITEM_UNCHECKED,ITEM_UNCHECKED);
			}
			hInstanceItem=GetChildItem(hEntityItem);
			while(hInstanceItem)
			{
				if (GetParentItem(hInstanceItem)==hSelectedItem)
				{
					SetItemImage(hInstanceItem,ITEM_CHECKED,ITEM_CHECKED);
				}
				else
				{
					SetItemImage(hInstanceItem,ITEM_UNCHECKED,ITEM_UNCHECKED);
				}
				hInstanceItem=GetNextSiblingItem(hInstanceItem);
			}
			hEntityItem=GetNextSiblingItem(hEntityItem);
		}
	}
}

void CEntityTree::OnEntitytreeAllshow()
{
	CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
	CIFCProject *pIFCProject = p3DView->GetDocument()->ifcProject;

	if (!pIFCProject)
		return;
	if (!pIFCProject->OpenFlag())
		return;

	if (HitItemType==ITEMTYPE_ROOT)
	{
		pIFCProject->SetAllSelected(TRUE);

		//����3ά��ͼ
		((CTHUIFCerView*)pMainFrame->GetActiveView())->SendMessage(ID_UPDATE_IFCVIEW, 0, 0);
		//������
		HTREEITEM hEntityItem=GetChildItem(hRoot);
		HTREEITEM hInstanceItem;
		while (hEntityItem)
		{
			SetItemImage(hEntityItem,ITEM_CHECKED,ITEM_CHECKED);
			hInstanceItem=GetChildItem(hEntityItem);
			while(hInstanceItem)
			{
				SetItemImage(hInstanceItem,ITEM_CHECKED,ITEM_CHECKED);
				hInstanceItem=GetNextSiblingItem(hInstanceItem);
			}
			hEntityItem=GetNextSiblingItem(hEntityItem);
		}
	}
}

void CEntityTree::OnEntitytreeAllhide()
{
	CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
	CIFCProject *pIFCProject = p3DView->GetDocument()->ifcProject;

	if (!pIFCProject)
		return;
	if (!pIFCProject->OpenFlag())
		return;

	if (HitItemType==ITEMTYPE_ROOT)
	{
		pIFCProject->SetAllSelected(FALSE);

		//����3ά��ͼ
		((CTHUIFCerView*)pMainFrame->GetActiveView())->SendMessage(ID_UPDATE_IFCVIEW, 0, 0);
		//������
		HTREEITEM hEntityItem=GetChildItem(hRoot);
		HTREEITEM hInstanceItem;
		while (hEntityItem)
		{
			SetItemImage(hEntityItem,ITEM_UNCHECKED,ITEM_UNCHECKED);
			hInstanceItem=GetChildItem(hEntityItem);
			while(hInstanceItem)
			{
				SetItemImage(hInstanceItem,ITEM_UNCHECKED,ITEM_UNCHECKED);
				hInstanceItem=GetNextSiblingItem(hInstanceItem);
			}
			hEntityItem=GetNextSiblingItem(hEntityItem);
		}
	}
}

void CEntityTree::OnEntitytreeAddproperty()
{
	/*���Դ���*/
	//���������
	CMainFrame *pMainFrame=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	CTHUIFCerView *p3DView = ((CTHUIFCerView*)pMainFrame->GetActiveView());
	CIFCProject *pIFCProject = p3DView->GetDocument()->ifcProject;

	if (!pIFCProject)
		return;
	if (!pIFCProject->OpenFlag())
		return;

	HTREEITEM hSelectedItem=GetSelectedItem();
	HTREEITEM hParentItem=GetParentItem(hSelectedItem);

	if (HitItemType==ITEMTYPE_INSTANCE)
	{
		int GroupID=(int)GetItemData(hParentItem);

		pIFCProject->AddNewPropertyTest(HitItemID);
	}
}
