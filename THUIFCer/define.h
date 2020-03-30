
/************************************************************************/
/* 宏定义                                                               */
/************************************************************************/

//CEntityTree:实体树某类实例被选中情况：全部不选中、部分被选中、全部被选中、为根节点
#define		ITEM_UNCHECKED			0
#define		ITEM_PARTLY_CHECKED		1
#define		ITEM_CHECKED			2
#define     ITEM_ROOT               3

//CEntityTree:实体树节点类型：未知、根节点、实体、实例
#define     ITEMTYPE_UNKNOWN        0
#define     ITEMTYPE_ROOT           1
#define     ITEMTYPE_ENTITY         2
#define     ITEMTYPE_INSTANCE       3

//CIFCEntity:实体类中成员被选中情况：全部不选中、部分被选中、全部被选中
#define     UNCHECKED               0
#define     PARTLYCHECKED           1
#define     ALLCHECKED              2
