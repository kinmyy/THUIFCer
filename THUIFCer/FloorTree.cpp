// EntityTree.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "THUIFCer.h"
#include "FloorTree.h"
#include "MainFrm.h"
#include "THUIFCerView.h"

/*
extern	STRUCT_INSTANCES	* first_instance;
*/

// CFloorTree

IMPLEMENT_DYNAMIC(CFloorTree, CTreeCtrl)

CFloorTree::CFloorTree()
{

}

CFloorTree::~CFloorTree()
{
}


BEGIN_MESSAGE_MAP(CFloorTree, CTreeCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CEntityTree ��Ϣ�������



int CFloorTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*
	//for test
	hRoot = InsertItem(_T("���ڵ�"));
	InsertItem(_T("Entity"), hRoot);
	*/

	return 0;
}

LRESULT CFloorTree::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case ID_UPDATE_ENTITYTREE:
		DeleteAllItems();
		break;
	default:
		break;
	}

	return CTreeCtrl::WindowProc(message, wParam, lParam);
}
