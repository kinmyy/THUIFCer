#include "StdAfx.h"
#include "ConvexHull.h"

ConvexHull::ConvexHull(void)
{
	finished = false;
	last_i = 0;
}

ConvexHull::~ConvexHull(void)
{
	finished = false;
	last_i = 0;
	allpoints.clear();
	convex_points_index.clear();
}

void ConvexHull::clear()
{
	finished = false;
	last_i = 0;
	allpoints.clear();
	convex_points_index.clear();
}

void ConvexHull::quick_sort(int top, int bottom)
{
	int pos;
	if(top < bottom)
	{
		pos = loc(top,bottom);
		quick_sort(top,pos-1);
		quick_sort(pos+1,bottom);
	}
}

int ConvexHull::loc(int top,int bottom)
{
	double x = allpoints[top].z;
	int j,k;
	j = top+1;
	k = bottom;
	while(1)
	{
		while(j<bottom && allpoints[j].z<x)
			j++;
		while(k>top && allpoints[k].z>=x)
			k--;
		if(j>=k)
			break;
		exchange(j,k);
	}
	exchange(top,k);
	return k;
}

void ConvexHull::exchange(int px,int py)
{
	D3DXVECTOR3 k;
	k = allpoints[px];
	allpoints[px] = allpoints[py];
	allpoints[py] = k;
}

//取得点集中的第i个点
D3DXVECTOR3 ConvexHull::get_point(size_t i)
{
	return allpoints[i];
}

//取得整个点集的所有点的数目
size_t ConvexHull::get_points_count()
{
	return allpoints.size();
}

//取得凸包的第i个点
D3DXVECTOR3 ConvexHull::get_convex_point(size_t i)
{
	return allpoints[convex_points_index[i]];
}

//获取凸包顶点数
size_t ConvexHull::get_convex_points_count()
{
	return convex_points_index.size();
}

//向待求解的点集中添加一个点
void ConvexHull::add_point(double x,double y)
{
	D3DXVECTOR3 temp;
	temp.x = x;
	temp.y = y;
	temp.z = 0;
	allpoints.push_back(temp);

	//清除已经完成的标记
	finished = false;
	convex_points_index.clear();
}

//向待求解的点集中添加一个点
//void ConvexHull::delete_point(size_t i)
//{
//	allpoints.erase(i);
//
//	//清除已经完成的标记
//	finished =  false;
//	convex_points_index.clear();
//}

//寻找基准点。原则：x 尽量小，或者 x 相等的时候 y 尽量小
D3DXVECTOR3 ConvexHull::find_base_point()
{
	size_t idx = 0;
	for (size_t i = 0;i< allpoints.size();i++)
	{
		if ((allpoints[i].x<allpoints[idx].x)|| (allpoints[i].x == allpoints[idx].x && allpoints[i].y < allpoints[idx].y))
		{
			idx = i;
		}
	}
	allpoints[idx].z = idx;
	return allpoints[idx];
}

//距离
double ConvexHull::distance(D3DXVECTOR3 p, D3DXVECTOR3 q)
{
	double x1 = q.x - p.x, y1 = q.y - p.y;
	return sqrt(x1*x1 + y1*y1);
}

//对其他点相对基准点的极坐标角度排序
void ConvexHull::adjust_points_order(D3DXVECTOR3 &base)
{
	for(size_t i = 0; i < allpoints.size(); i++)
	{
		
		double t = (base.y - allpoints[i].y) / distance(base, allpoints[i]);
		if(IS_ZERO(t)) allpoints[i].z = PI/2;
		else if(IS_ZERO(t-1)) allpoints[i].z = 0;
		else if(IS_ZERO(t+1)) allpoints[i].z = PI;
		else allpoints[i].z = acos(t);
		//DPRINTF((" r = %lf\n"), points_[i].r);
	}

	//保证基准点排序后处于第一个位置
	allpoints[base.z].z = -1;

	//以基准点对所有点进行排序
	quick_sort(0, allpoints.size() - 1);


}

void ConvexHull::prepare()
{
	convex_points_index.clear();
	last_i = 0;
	finished = false;

	adjust_points_order(find_base_point());

	if (allpoints.size() != 0)
	{
		convex_points_index.push_back(0);
	}
}

//求解凸包的工作是否完成
bool ConvexHull::isfinished()
{
	return finished;
}

//使用向量积判断 p, q, r是否能构成凸包的边界。
bool ConvexHull::is_valid_convex_point(D3DXVECTOR3 &p,D3DXVECTOR3 &q,D3DXVECTOR3 &r)
{
	double x1= q.x - p.x, y1 = q.y - p.y;
	double x2 = r.x - q.x, y2 = r.y - q.y;
	return (x1*y2 - x2*y1 >= 0) ;
}

//是否还有下一个点需要处理
bool ConvexHull::has_next_point()
{
	return last_i + 1 < allpoints.size();
}

//返回待处理的下一个点的索引
int ConvexHull::get_next_point_index()
{
	return last_i+1;
}

//进行一步操作，寻找下一个合适的点
bool ConvexHull::search_next_point()
{
	if (!has_next_point())
	{
		finished = true;
		return false;
	}

	//如果目前求得的凸包顶点小于2个，则直接把下一个点添加到凸包顶点集合中
	//TODO: 这一步在很多情况下可以优化掉。不过还需要增加其他判断
	if (convex_points_index.size()<2)
	{
		convex_points_index.push_back(++last_i);
		return true;
	}

	D3DXVECTOR3 &p1 = allpoints[convex_points_index[convex_points_index.size() - 2]];
	D3DXVECTOR3 &p2 = allpoints[convex_points_index[convex_points_index.size() - 1]];
	//取出要处理的下一个点
	D3DXVECTOR3 &p3 = allpoints[last_i + 1];

	//如果 p1 p2 p3 不能构成凸包的边界
	//则把已经求得的凸包点集合的最后一个点去掉（因为它必然不是凸包的顶点）
	if(!is_valid_convex_point(p1, p2, p3))
	{
		convex_points_index.pop_back();
		return true;
	}
	else  //如果 p1 p2 p3 能构成凸包的边界则把p3添加到凸包点集合，并继续找下一个
	{
		convex_points_index.push_back(++last_i);
		return true;
	}
}

vector<D3DXVECTOR3> ConvexHull::solve()
{
	vector<D3DXVECTOR3>temp = allpoints;
	D3DXVECTOR3 t = find_base_point();
	int k = 0;
	allpoints.clear();
	for (int i=0;i<temp.size();i++)
	{
		if (temp[i].x == t.x && temp[i].y == t.y)
		{
			k++;
			if (k==1)
			{
				allpoints.push_back(temp[i]);
			}
		}
		else
		{
			allpoints.push_back(temp[i]);
		}
	}
	for(prepare();search_next_point();)
		;

	return allpoints;
}

vector<D3DXVECTOR3> ConvexHull::getConvexHell()
{
	return allpoints;
}