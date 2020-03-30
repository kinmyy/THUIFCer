#include "stdafx.h"
#include "NaviPath.h"
#include "FindPath.h"


bool CNaviPath::IsPointInSegment( int x, int y, int x1, int y1, int x2, int y2 )
{
	if( x1 <= x2 )
	{
		if( x<x1 || x>x2 ) return false;
	}
	else if( x<x2 || x>x1 ) return false;

	if( y1 <= y2 )
	{
		if( y<y1 || y>y2 ) return false;
	}
	else if( y<y2 || y>y1 ) return false;

	int r = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);
	if( r == 0 ) return true;
	int dist = (x-x1)*(y2-y1) - (y-y1)*(x2-x1);
	dist = dist*dist / r;

	return dist<=2;
}

bool CNaviPath::IsPointOnPath( const Point2D &pt )
{
	for( int i=0; i<m_Points.size()-1; i++ )
	{
		if( IsPointInSegment(pt.x, pt.y, m_Points[i].x, m_Points[i].y, m_Points[i+1].x, m_Points[i+1].y) ) return true;
	}
	return false;
}

int  CNaviPath::GetPathLength()
{
	int len = 0;
	for( int i=0; i<m_Points.size()-1; i++ )
	{
		stPathPoint &p1 = m_Points[i];
		stPathPoint &p2 = m_Points[i+1];

		len += (int)sqrt((double)( (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) ));
	}

	return len;
}

void CNaviPath::AddPathPoint( int x, int y )
{
	if( m_Points.size() > 0 )
	{
		stPathPoint &tmp = m_Points[m_Points.size()-1];
		if( x==tmp.x && y==tmp.y ) return;
	}
	stPathPoint pt;
	pt.x = x;
	pt.y = y;

	m_Points.push_back(pt);
}

int  CNaviPath::FindNextVisiblePoint(CMask* mask, int i )
{
	assert( m_Points.size()>1 && i<m_Points.size() );
	Point2D begin(m_Points[i].x, m_Points[i].y);

	int next = i+1;
	while( next < m_Points.size() )
	{
		bool b = mask->LineOfSightTest( begin, Point2D(m_Points[next].x, m_Points[next].y) );
		// assert( b == mask->LineOfSightTest1(begin, Point2D(m_Points[next].x, m_Points[next].y)) );
		if( !b ) break;
		next++;
	}
	return next-1;
}

void CNaviPath::LooseGoal( int radius ) //改变path, 让path的goal在和原来目标距离radius的地方
{
	if( radius <= 0 ) return;

	int i;
	for( i=0; i<m_Points.size(); i++ )
	{
		int dx = m_Points[i].x - m_RealGoal.x;
		int dy = m_Points[i].y - m_RealGoal.y;
		if( dx*dx+dy*dy <= radius*radius )
		{
			break;
		}
	}

	if( i < m_Points.size() )
	{
		m_Points.resize(i+1);
	}
}

void CNaviPath::SmoothPath(CMask* mask)
{
	int begin = 0;
	while(1)
	{
		int end = FindNextVisiblePoint(mask,begin);
		if( end == begin ) break; //到终点了
		if( end > begin+1 ) //之间有多余的路径点
		{
			m_Points.erase( m_Points.begin()+begin+1, m_Points.begin()+end );
		}
		begin++;
	}
}

Point2D CNaviPath::GetPathPoint( int i )
{ 
	assert( i < m_Points.size() );
	return Point2D(m_Points[i].x, m_Points[i].y); 
}

Point2D CNaviPath::GetPathGoal()
{
	assert( m_Points.size() > 1 );
	stPathPoint &pt = m_Points[ m_Points.size()-1 ];
	return Point2D( pt.x, pt.y );
}
