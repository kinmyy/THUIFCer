//寻路结果
#ifndef NAVIPATH_H
#define NAVIPATH_H


#include <vector>
#include <assert.h>
class CMask;
class Point2D;

struct stPathPoint
{
	int x; //虚拟屏幕上的像素坐标
	int y;
};


class CNaviPath
{
	stPathPoint m_RealGoal; //由于目标不可达， m_Points中路经的终点可能不是目标点，所以在这里存一下

public:
	CNaviPath(){}
	void Reset( ){ m_Points.resize(0); }
	void AddPathPoint( int x, int y );
	int  FindNextVisiblePoint(CMask* mask, int i );
	Point2D GetPathPoint( int i );
	Point2D GetPathGoal();
	int  GetNumPoint(){ return m_Points.size(); }
	void LooseGoal( int radius ); //改变path, 让path的goal在和原来目标距离radius的地方
	void SmoothPath(CMask* mask);

	void LimitPathPoint( int number )
	{
		assert( number >= 2 );
		if( number >= m_Points.size() ) return;

		m_Points.resize( number );
	}

	bool IsPointOnPath( const Point2D &pt );
	int  GetPathLength();

	static bool IsPointInSegment( int x, int y, int x1, int y1, int x2, int y2 );

	std::vector<stPathPoint> m_Points;
};

#endif