#include<bits/stdc++.h>
using namespace std;
class point
{
public:
    int x;
    int y;
};
class inter
{
public:
    int bottom;
    int top;
};
class rect: public inter
{
public:
    int x_l,x_r,y_b,y_t;
    inter* x_int;
    inter* y_int;
};
/*
    left=0
    top=1
    right=2
    down=3
*/
class edge: public inter
{
public:
    int rid;
    inter* x_inter;
    inter* y_inter;
    int coor;
    int etype;
};
class lru
{
public:
    int at;
};
class ctree
{
public:
    int r;
    lru* side;
    ctree* lson;
    ctree* rson;
};
class stripe: public inter
{
public:
    inter* x_i;
    inter* y_i;
    vector<inter*> x_uni;
    double ms;
    ctree* tr;
};
class line_seg: public inter
{
public:
    inter* iobj;
    int coord;
    stripe* sig;
};
vector<inter*> partition(vector<int> Y);
vector<inter*> intervals(vector<int> cors);
vector<pair<int,int>> getIntUnion(vector<inter*> ss);
vector<pair<int,int>> getdiffIntervals(vector<inter*> ss,int s,int e);
vector<line_seg*> contour_pieces(edge* h,vector<stripe*> S);
vector<inter*> interUnion(vector<inter*> v1,vector<inter*> v2);
vector<inter*> interSection(vector<inter*> v1,vector<inter*> v2);
vector<inter*> interSetDifference(vector<inter*> v1,vector<inter*> v2);
vector<stripe*> cop(vector<stripe*> &sp,vector<int> &P,inter* i1);
void blacken(vector<stripe*> &sp,vector<inter*> &inr);
vector<stripe*> concat(vector<stripe*> &SL,vector<stripe*> &SR,vector<int> &P,inter* itr);
void STRIPES(vector<edge*> V,inter* x_ext,vector<inter*> &L,vector<inter*> &R,vector<int> &P,vector<stripe*> &S);
void Rectangle_Dac(vector<rect*> &R,vector<stripe*> &S);
void trans(ctree* node,vector<int> &ptr);
bool ec(edge* e1,edge* e2);
double measure(vector<stripe*> &S);
vector<line_seg*> vert_contour(vector<stripe*> &S);