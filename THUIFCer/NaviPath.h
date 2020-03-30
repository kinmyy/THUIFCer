//Ѱ·���
#ifndef NAVIPATH_H
#define NAVIPATH_H


#include <vector>
#include <assert.h>
class CMask;
class Point2D;

struct stPathPoint
{
	int x; //������Ļ�ϵ���������
	int y;
};


class CNaviPath
{
	stPathPoint m_RealGoal; //����Ŀ�겻�ɴ m_Points��·�����յ���ܲ���Ŀ��㣬�����������һ��

public:
	CNaviPath(){}
	void Reset( ){ m_Points.resize(0); }
	void AddPathPoint( int x, int y );
	int  FindNextVisiblePoint(CMask* mask, int i );
	Point2D GetPathPoint( int i );
	Point2D GetPathGoal();
	int  GetNumPoint(){ return m_Points.size(); }
	void LooseGoal( int radius ); //�ı�path, ��path��goal�ں�ԭ��Ŀ�����radius�ĵط�
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