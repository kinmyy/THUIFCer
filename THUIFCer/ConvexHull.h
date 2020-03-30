#pragma once

#include <vector>
#define DOUBLE_EPS 1.0e-9
#define IS_ZERO(v) (-DOUBLE_EPS<=(v) && (v)<= DOUBLE_EPS)
#define PI  3.141592653

using namespace std;
class ConvexHull
{
public:
	ConvexHull(void);
public:
	~ConvexHull(void);

//²ÎÊý
public:
	vector<D3DXVECTOR3> allpoints;
	vector<int> convex_points_index;
	bool finished;
	size_t last_i;
	D3DXVECTOR3 base;

//º¯Êý
public:
	void quick_sort(int top, int bottom);
	int loc(int top,int bottom);
	void exchange(int px,int py);
	void clear();
	D3DXVECTOR3 get_point(size_t i);
	size_t get_points_count();
	D3DXVECTOR3 get_convex_point(size_t i);
	size_t get_convex_points_count();
	void add_point(double x,double y);
	//void delete_point(size_t i);
	D3DXVECTOR3 find_base_point();
	void adjust_points_order(D3DXVECTOR3 &base);
	void prepare();
	bool isfinished();
	bool is_valid_convex_point(D3DXVECTOR3 &p,D3DXVECTOR3 &q,D3DXVECTOR3 &r);
	bool has_next_point();
	int get_next_point_index();
	bool search_next_point();
	vector<D3DXVECTOR3> solve();
	double distance(D3DXVECTOR3 p, D3DXVECTOR3 q);
	vector<D3DXVECTOR3> getConvexHell();

};
