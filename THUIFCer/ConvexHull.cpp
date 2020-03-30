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

//ȡ�õ㼯�еĵ�i����
D3DXVECTOR3 ConvexHull::get_point(size_t i)
{
	return allpoints[i];
}

//ȡ�������㼯�����е����Ŀ
size_t ConvexHull::get_points_count()
{
	return allpoints.size();
}

//ȡ��͹���ĵ�i����
D3DXVECTOR3 ConvexHull::get_convex_point(size_t i)
{
	return allpoints[convex_points_index[i]];
}

//��ȡ͹��������
size_t ConvexHull::get_convex_points_count()
{
	return convex_points_index.size();
}

//������ĵ㼯�����һ����
void ConvexHull::add_point(double x,double y)
{
	D3DXVECTOR3 temp;
	temp.x = x;
	temp.y = y;
	temp.z = 0;
	allpoints.push_back(temp);

	//����Ѿ���ɵı��
	finished = false;
	convex_points_index.clear();
}

//������ĵ㼯�����һ����
//void ConvexHull::delete_point(size_t i)
//{
//	allpoints.erase(i);
//
//	//����Ѿ���ɵı��
//	finished =  false;
//	convex_points_index.clear();
//}

//Ѱ�һ�׼�㡣ԭ��x ����С������ x ��ȵ�ʱ�� y ����С
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

//����
double ConvexHull::distance(D3DXVECTOR3 p, D3DXVECTOR3 q)
{
	double x1 = q.x - p.x, y1 = q.y - p.y;
	return sqrt(x1*x1 + y1*y1);
}

//����������Ի�׼��ļ�����Ƕ�����
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

	//��֤��׼��������ڵ�һ��λ��
	allpoints[base.z].z = -1;

	//�Ի�׼������е��������
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

//���͹���Ĺ����Ƿ����
bool ConvexHull::isfinished()
{
	return finished;
}

//ʹ���������ж� p, q, r�Ƿ��ܹ���͹���ı߽硣
bool ConvexHull::is_valid_convex_point(D3DXVECTOR3 &p,D3DXVECTOR3 &q,D3DXVECTOR3 &r)
{
	double x1= q.x - p.x, y1 = q.y - p.y;
	double x2 = r.x - q.x, y2 = r.y - q.y;
	return (x1*y2 - x2*y1 >= 0) ;
}

//�Ƿ�����һ������Ҫ����
bool ConvexHull::has_next_point()
{
	return last_i + 1 < allpoints.size();
}

//���ش��������һ���������
int ConvexHull::get_next_point_index()
{
	return last_i+1;
}

//����һ��������Ѱ����һ�����ʵĵ�
bool ConvexHull::search_next_point()
{
	if (!has_next_point())
	{
		finished = true;
		return false;
	}

	//���Ŀǰ��õ�͹������С��2������ֱ�Ӱ���һ������ӵ�͹�����㼯����
	//TODO: ��һ���ںܶ�����¿����Ż�������������Ҫ���������ж�
	if (convex_points_index.size()<2)
	{
		convex_points_index.push_back(++last_i);
		return true;
	}

	D3DXVECTOR3 &p1 = allpoints[convex_points_index[convex_points_index.size() - 2]];
	D3DXVECTOR3 &p2 = allpoints[convex_points_index[convex_points_index.size() - 1]];
	//ȡ��Ҫ�������һ����
	D3DXVECTOR3 &p3 = allpoints[last_i + 1];

	//��� p1 p2 p3 ���ܹ���͹���ı߽�
	//����Ѿ���õ�͹���㼯�ϵ����һ����ȥ������Ϊ����Ȼ����͹���Ķ��㣩
	if(!is_valid_convex_point(p1, p2, p3))
	{
		convex_points_index.pop_back();
		return true;
	}
	else  //��� p1 p2 p3 �ܹ���͹���ı߽����p3��ӵ�͹���㼯�ϣ�����������һ��
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