
#ifndef FINDPATH_H
#define FINDPATH_H

#include "l_heapnode.h"
#include "NaviPath.h"
#include <cmath>

const int MAX_NAVIGRID = 5000;	// 当搜索过的点超过该数就退出
const int MASK_BIT = 0;			// 阻挡位		

class Point2D
{

public:

	Point2D(){x=0;y=0;}
	Point2D( int x1, int y1 ) : x(x1), y(y1){}
	Point2D( const Point2D &rhs ) : x(rhs.x), y(rhs.y){}
	Point2D &operator =( const Point2D &rhs ){ x=rhs.x, y=rhs.y; return *this; }
	Point2D operator +( const Point2D &rhs ) const
	{
		return Point2D(x+rhs.x, y+rhs.y);
	}
	Point2D operator -( const Point2D &rhs ) const
	{
		return Point2D(x-rhs.x, y-rhs.y);
	}
	Point2D operator *( const Point2D &rhs ) const
	{
		return Point2D(x*rhs.x, y*rhs.y);
	}
	Point2D operator /( const Point2D &rhs ) const
	{
		return Point2D(x/rhs.x, y/rhs.y);
	}

	Point2D operator *( int scale ) const
	{
		return Point2D(x*scale, y*scale);
	}

	Point2D operator /( int scale ) const
	{
		return Point2D(x/scale, y/scale);
	}

	bool operator==( const Point2D &rhs ) const
	{
		return x==rhs.x && y==rhs.y;
	}

	bool operator!=( const Point2D &rhs ) const
	{
		return x!=rhs.x || y!=rhs.y;
	}

	Point2D &operator +=( const Point2D &rhs )
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Point2D &operator -=( const Point2D &rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Point2D &operator *=( float scale )
	{
		x = (int)(x*scale);
		y = (int)(y*scale);
		return *this;
	}

	Point2D &operator /=( float scale )
	{
		x = (int)(x/scale);
		y = (int)(y/scale);
		return *this;
	}

	int LengthSq(){ return x*x+y*y; }
	int Length(){ return (int)sqrt((double)(x*x+y*y)); }

public:

	int x;
	int y;

};

class CMask{
public:
	enum{
		SEARCH_TYPE_ASTAR,
		SEARCH_TYPE_DFS,
		SEARCH_TYPE_BFS,
		SEARCH_TYPE_COUNT
	};
	CMask(int size, Point2D a,Point2D b,const char* filename = NULL);
	~CMask();

	void StartFindPath();											// 开始扩展
	int FindNextNode();												// 取OPEN集扩展下一点
	void FindPath();												// 直接一次找出路径
	void TryGotoNeighbour( stPathHeapNode &from, int dx, int dy ,int deep = 1);	// 尝试访问周围
	int ComputeHeuticsCost( int x,int y );							// 计算H'函数损耗

	bool LineOfSightTest( const Point2D &start, const Point2D &end );

	void ReadFile (int size, Point2D a,Point2D b,const char* filename);
	int GetMask(int wide,int heigt);
	bool IsFinishPath(){return m_bFind;}
	int GetWide(){return m_width;}
	int GetHeight(){return m_height;}

	bool CanWalk( int x, int y )
	{ 
		return m_data[y*m_width+x]!=MASK_BIT;
	}
	int GetSearchTime(){
		if (m_bFind)
		{
			return m_searchTime;
		}
		else
		{
			return -1;
		}
	}


	std::vector<Point2D> m_FindedPath;								// 保存最后的路径
	CNaviPath m_NaviPath;											// 保存最后路径的原始点, 以便平滑坐标
	CPathHeap m_PathHeap;											// open集,记录待扩展的点
	std::vector<stPathHeapNode> m_ClosedNodes;						// 保留close集是为了记录节点信息,最后找到时回溯找路径用
	Point2D m_start;												// 大格坐标
	Point2D m_end;									
	Point2D m_startPos;												// 真正坐标
	Point2D m_endPos;
	int m_searchType;												// 搜索类型


private:
	int m_lMaskPixelWidth;
	int m_lMaskPixelHeight;
	int m_width;
	int m_height;
	int* m_data;

	int m_searchTime;

	int* m_pVisitMap;												// 记录访问过的点, 访问过标记为1
	bool m_bFind;	

};
#endif