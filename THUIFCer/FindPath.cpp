#include "stdafx.h"
#include <math.h>
#include "FindPath.h"
#include "l_heapnode.h"
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <assert.h>
using namespace std;

static std::vector<stPathHeapNode>::iterator FindHeapNodeInArray( std::vector<stPathHeapNode> &array, unsigned int index )
{
	std::vector<stPathHeapNode>::iterator iter = array.begin();
	for( ; iter!=array.end(); iter++ )
	{
		if( iter->index == index ) return iter;
	}
	return array.end();
}


CMask::CMask(int size, Point2D a,Point2D b,const char* filename)
{
	m_width = 0;
	m_height = 0;
	m_data = NULL;
	m_pVisitMap = NULL;
	m_bFind = false;

	m_searchType = SEARCH_TYPE_ASTAR;

	m_lMaskPixelHeight = 10;
	m_lMaskPixelWidth = 10;

	m_NaviPath.Reset();
	m_searchTime = 0;


	if (filename != NULL)
	{
		ReadFile(size,a,b,filename);
	}
}
CMask::~CMask()
{
	if ( m_data != NULL)
	{
		delete [] m_data;
		m_data = NULL;
	}
	if ( m_pVisitMap != NULL )
	{
		delete [] m_pVisitMap;
		m_pVisitMap = NULL;
	}
}
void CMask::ReadFile(int size, Point2D a,Point2D b,const char* filename)
{
	std::ifstream in("E:\\grids100.txt");
	if(in)
	{
		/*in>>m_width>>m_height;
		in>>m_startPos.x>>m_startPos.y;
		in>>m_endPos.x>>m_endPos.y;*/
		m_width = size;
		m_height = size;
		m_startPos.x = a.x*10;
		m_startPos.y = a.y*10;
		m_endPos.x = b.x*10;
		m_endPos.y = b.y*10;

		m_start.x = m_startPos.x/m_lMaskPixelWidth;
		m_start.y = m_startPos.y/m_lMaskPixelHeight;

		m_end.x = m_endPos.x/m_lMaskPixelWidth;
		m_end.y = m_endPos.y/m_lMaskPixelHeight;

		if ( m_data != NULL )
		{
			delete [] m_data;
		}
		m_data = new int [m_width*m_height];
		for(int i=0; i<m_height; i++)
		{
			for(int j=0; j<m_width; j++)
			{
				in>>m_data[i*m_width+j];
			}
		}
	}
}
int CMask::GetMask(int wide,int heigt)				// 以0,0开始计数
{
	if (m_data!=NULL)
	{
		return m_data[heigt*m_width+wide];
	}
	else
	{
		return -1;
	}
}

// deep=1表示广度优先搜索BFS, 默认值为1, deep=-1表示深度优先搜索
void CMask::TryGotoNeighbour( stPathHeapNode &from, int dx, int dy, int deep)
{
	int x1 = INDEX_TO_X(from.index) + dx;
	int y1 = INDEX_TO_Y(from.index) + dy;

	if( x1<0 || x1>=m_width || y1<0 || y1>=m_height ) return;	//超出地图外
	if( GetMask(x1,y1) == MASK_BIT ) return;					//人物阻挡，不能通过


	unsigned int curindex = MAKEINDEX( x1, y1 );
	if (deep >=0)
	{
		deep = 1;
	}
	else
	{
		deep = -1;
	}
	int aval = from.aval + 1*deep;
	if( m_pVisitMap[y1*m_width+x1] == 1) //已经访问过了
	{
		std::vector<stPathHeapNode>::iterator iter = FindHeapNodeInArray( m_PathHeap.m_AllNodes, curindex );
		if( iter != m_PathHeap.m_AllNodes.end() )
		{
			if( aval*deep < iter->aval*deep )
			{
				iter->aval = aval;
				iter->from = from.index;
				m_PathHeap.AdjustNode( iter );
			}
		}
		return;
	}

	stPathHeapNode tmpnode;
	tmpnode.from = from.index;
	tmpnode.index = curindex;
	tmpnode.aval = aval;
	tmpnode.hval = ComputeHeuticsCost(x1,y1);
	m_PathHeap.PushNode( tmpnode );

	m_pVisitMap[y1*m_width+x1] = 1;

}

void CMask::StartFindPath()
{
	// 检查start点和end点
	if (GetMask(m_end.x,m_end.y) == MASK_BIT || GetMask(m_start.x,m_start.y) == MASK_BIT)
	{
		m_bFind = true;
		return;
	}

	// 初始化
	m_bFind = false;
	m_PathHeap.Init();
	m_ClosedNodes.resize(0);
	m_pVisitMap = new int[m_height*m_width];
	memset(m_pVisitMap,0,m_height*m_width*sizeof(int));

	// 往OPEN集中加入起点
	stPathHeapNode node;
	node.from = node.index = MAKEINDEX(m_end.x, m_end.y);
	node.hval = node.aval = 0;
	m_PathHeap.PushNode( node );
	m_pVisitMap[m_end.y*m_width+m_end.x] = 1;
}

void CMask::FindPath()
{

	// 检查start点和end点
	if (GetMask(m_end.x,m_end.y) == MASK_BIT || GetMask(m_start.x,m_start.y) == MASK_BIT)
	{
		m_bFind = true;
		return;
	}

	m_searchTime = GetTickCount();
	// 初始化
	m_bFind = false;
	m_PathHeap.Init();
	m_ClosedNodes.resize(0);
	m_pVisitMap = new int[m_height*m_width];
	memset(m_pVisitMap,0,m_height*m_width*sizeof(int));
	m_NaviPath.Reset();

	// 往OPEN集中加入起点
	stPathHeapNode node;
	node.from = node.index = MAKEINDEX(m_end.x, m_end.y);
	node.hval = node.aval = 0;
	m_PathHeap.PushNode( node );
	m_pVisitMap[m_end.y*m_width+m_end.x] = 1;

	while (m_PathHeap.HasNode())
	{
		FindNextNode();
		if (m_bFind)
		{
			break;
		}
	}
	m_searchTime = GetTickCount() - m_searchTime;
}

int CMask::FindNextNode()
{
	if(m_bFind == true)
	{
		return 1;		// 已搜索完 
	}
	if ( m_PathHeap.HasNode() )
	{
		stPathHeapNode node;
		m_PathHeap.PopNode( node );
		m_ClosedNodes.push_back( node );
		if( m_ClosedNodes.size() >= MAX_NAVIGRID ) 
		{
			m_bFind = true;
			return -1; // 访问超过MAX_NAVIGRID点即退出
		}
			
		if( node.index == MAKEINDEX(m_start.x,m_start.y) ) // 找到
		{
			m_bFind = true;
			m_FindedPath.resize(0);
			m_FindedPath.push_back(m_start);

			m_NaviPath.Reset();
			m_NaviPath.AddPathPoint( m_startPos.x, m_startPos.y );
			while( node.from != node.index )
			{
				// 保存路径
				int xPos = INDEX_TO_X(node.index)*m_lMaskPixelWidth + m_lMaskPixelWidth/2;
				int yPos = INDEX_TO_Y(node.index)*m_lMaskPixelHeight + m_lMaskPixelHeight/2;
				m_NaviPath.AddPathPoint( xPos, yPos );

				// 演示DEMO用
				int x = INDEX_TO_X(node.index);
				int y = INDEX_TO_Y(node.index);
				Point2D tmpp(x,y);
				m_FindedPath.push_back(tmpp);
				std::vector<stPathHeapNode>::iterator iter = FindHeapNodeInArray(m_ClosedNodes, node.from);
				assert( iter != m_ClosedNodes.end() );
				node = *iter;
			}
			m_NaviPath.AddPathPoint( m_endPos.x, m_endPos.y );
			m_FindedPath.push_back(m_end);
			assert( m_FindedPath.size() > 1 );

			m_NaviPath.SmoothPath(this);	
			return 1;	// 正确找到
		}
		else  // 还没找到,扩展
		{
			int deep=0;
			if (m_searchType == SEARCH_TYPE_ASTAR || m_searchType == SEARCH_TYPE_BFS)
			{
				deep = 1;
			}
			else
			{
				deep = -1;
			}
			TryGotoNeighbour( node, -1, 0 ,deep);
			TryGotoNeighbour( node, +1, 0 ,deep);
			TryGotoNeighbour( node, 0, -1 ,deep);
			TryGotoNeighbour( node, 0, +1 ,deep);
			return 0;	// 继续扩展
		}
	}
	else
	{
		return -1;
	}
}


// 计算中间点与目标点的代价, A*算法最关键的函数
int CMask::ComputeHeuticsCost( int x,int y )
{
	if (m_searchType == SEARCH_TYPE_ASTAR)	// a*
	{
		//return (abs(x-m_start.x) + abs(y-m_start.y))*2/3;
		return 10*(abs(x-m_start.x) + abs(y-m_start.y));
	}
	else									// dfs,bfs
	{
		return 0;	
	}
}

//返回到下一个格子
static Point2D LineIntersectGrid( const Point2D &pt1, const Point2D &pt2, const Point2D &start, const Point2D &dp )
{
	int y1 = pt1.y*dp.x;
	int y2 = pt2.y*dp.x;
	if( dp.x > 0 )
	{
		int y = dp.y*(pt2.x-start.x) + start.y*dp.x;
		if( y == y2 ) return Point2D(1, 1);
		else if( y>=y1 && y<y2 ) return Point2D(1, 0);
	}
	else if( dp.x < 0 )
	{
		int y = dp.y*(pt1.x-start.x) + start.y*dp.x;

		if( y == y2 ) return Point2D(0, 1);
		else if( y>y2 && y<=y1 ) return Point2D(-1, 0);
	}

	if( dp.y > 0 ) return Point2D(0, 1);
	else return Point2D(0, -1);
}


bool CMask::LineOfSightTest( const Point2D &start, const Point2D &end )
{
	Point2D step( m_lMaskPixelWidth, m_lMaskPixelHeight );
	Point2D startgrid( start.x/m_lMaskPixelWidth, start.y/m_lMaskPixelHeight );
	Point2D endgrid( end.x/m_lMaskPixelWidth, end.y/m_lMaskPixelHeight );
	Point2D dp = end - start;

	Point2D curgrid = startgrid;
	if( !CanWalk(curgrid.x, curgrid.y) ) return false;
	while( curgrid != endgrid )
	{
		Point2D pt1 = curgrid * step;
		curgrid += LineIntersectGrid( pt1, pt1+step, start, dp );
		if( !CanWalk(curgrid.x, curgrid.y) ) return false;
	}
	return true;
}
