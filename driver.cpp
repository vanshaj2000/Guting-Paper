#include "includes/dtypes.h"
using namespace std;
int mini=-1000;
int maxi=1000;
bool custom(edge* e1,edge* e2)
{
    return e1->coor<e2->coor;
}
vector<point*> ptUnion(vector<point*> t1,vector<point*> t2)
{
    vector<point*> ans=t1;
    for(int i=0;i<t2.size();i++)
    {
        int flag=0;
        for(int j=0;j<t1.size();j++)
        {
            if(t2[i]->x==t1[j]->x&&t2[i]->y==t1[j]->y)
                flag=1;
        }
        if(!flag)
            ans.push_back(t2[i]);
    }
    return ans;
}
vector<point*> ptIntersection(vector<point*> t1,vector<point*> t2)
{
    vector<point*> ans;
    for(int i=0;i<t2.size();i++)
    {
        int flag=0;
        for(int j=0;j<t1.size();j++)
        {
            if(t2[i]->x==t1[j]->x&&t2[i]->y==t1[j]->y)
                flag=1;
        }
        if(flag)
            ans.push_back(t2[i]);
    }
    return ans;
}
vector<int> setCon(set<int> &s)
{
    vector<int> res;
    for(auto it=s.begin();it!=s.end();it++)
        res.push_back(*it);
    return res;
}
vector<point*> un(vector<rect*> R)
{
    set<pair<int,int>> s;
    for(int i=0;i<R.size();i++)
    {
        rect* temp=R[i];
        inter* x=temp->x_int;
        inter* y=temp->y_int;
        vector<int> v1={x->top,x->bottom};
        vector<int> v2={y->top,y->bottom};
        for(int a=0;a<2;a++)
        {
            for(int b=0;b<2;b++)
                s.insert({v1[a],v2[b]});
        }
    }
    vector<point*> ans;
    for(auto it=s.begin();it!=s.end();it++)
    {
        point* obj=new point();
        obj->x=it->first;
        obj->y=it->second;
        ans.push_back(obj);
    }
    return ans;
}
vector<int> y_set(vector<rect*> R)
{
    set<int> s;
    for(int i=0;i<R.size();i++)
    {
        s.insert(R[i]->y_int->top);
        s.insert(R[i]->y_int->bottom);
    }
    return setCon(s);
}
vector<inter*> partition(vector<int> Y)
{
    vector<inter*> res;
    sort(Y.begin(),Y.end());
    for(int i=0;i<Y.size()-1;i++)
    {
        inter* obj=new inter();
        obj->bottom=Y[i];
        obj->top=Y[i+1];
        res.push_back(obj);
    }
    return res;
}
vector<int> x_proj(vector<point*> Pts)
{
    return {};
}
vector<inter*> intervals(vector<int> cors)
{
    vector<inter*> ans;
    for(int i=0;i<cors.size();i=i+2)
    {
        inter* obj=new inter();
        obj->bottom=cors[i];
        obj->top=cors[i+1];
        ans.push_back(obj);
    }
    return ans;
}
vector<stripe*> stripes(vector<rect*> R,rect* f)
{
    vector<stripe*> ans;
    vector<int> temp=y_set(R);
    vector<int> temp2={f->y_int->top,f->y_int->bottom};
    vector<int> Y(temp.size()+2,0);
    auto it=set_union(temp.begin(),temp.end(),temp2.begin(),temp2.end(),Y.begin());
    int s=it-Y.begin();
    Y.resize(s);
    vector<inter*> part=partition(Y);
    for(int i=0;i<part.size();i++)
    {
        stripe* obj=new stripe();
        obj->x_i=f->x_int;
        obj->y_i=part[i];
        vector<point*> tt;
        vector<int> v1={obj->x_i->bottom,obj->x_i->top};
        vector<int> v2={obj->y_i->bottom,obj->y_i->top};
        for(int i=0;i<2;i++)
        {
            for(int j=0;j<2;j++)
            {
                point* ch=new point();
                ch->x=v1[i];
                ch->y=v2[j];
                tt.push_back(ch);
            }
        }
        vector<point*> we=ptIntersection(tt,un(R));
        obj->x_uni=intervals(x_proj(we));
        ans.push_back(obj);
    }
    return ans;
}
vector<pair<int,int>> getIntUnion(vector<inter*> ss)
{
    if(ss.size()==0)
        return {};
    vector<pair<int,int>> pr;
    for(int i=0;i<ss.size();i++)
        pr.push_back({ss[i]->bottom,ss[i]->top});
    sort(pr.begin(),pr.end());
    vector<pair<int,int>> res;
    res.push_back(pr[0]);
    for(int i=1;i<pr.size();i++)
    {
        pair<int,int> pp=res[res.size()-1];
        if(pr[i].first>=pp.second)
            res.push_back(pr[i]);
        else if(pr[i].second>pp.second)
        {
            res.pop_back();
            res.push_back({pp.first,pr[i].first});
            res.push_back(pr[i]);
        }
    }
    return res;
}
vector<pair<int,int>> getdiffIntervals(vector<inter*> ss,int s,int e)
{
    vector<pair<int,int>> res=getIntUnion(ss);
    vector<pair<int,int>> ans;
    for(int i=0;i<res.size();i++)
    {
        if(res[i].first>=s&&res[i].second<=e)
        {
            if(res[i].second<=e)
                ans.push_back(res[i]);
            else
                ans.push_back({res[i].first,e});
        }
    }
    return ans;
}
void trans(ctree* node,vector<int> &ptr)
{
    if(node==NULL)
        return;
    if(node->lson==NULL&&node->rson==NULL)
        ptr.push_back(node->r);
    if(node->lson!=NULL)
        trans(node->lson,ptr);
    if(node->rson!=NULL)
        trans(node->rson,ptr);
}
vector<line_seg*> contour_pieces(edge* h,vector<stripe*> S)
{
    vector<line_seg*> ans;
    struct stripe* s;
    if(h->etype==3)
    {
        for(int i=0;i<S.size();i++)
        {
            struct stripe* temp=S[i];
            if(temp->y_i->top==h->coor)
            {
                s=temp;
                break;
            }
        }
    }
    else
    {
        for(int i=0;i<S.size();i++)
        {
            struct stripe* temp=S[i];
            if(temp->y_i->bottom==h->coor)
            {
                s=temp;
                break;
            }
        }
    }
    vector<int> pp;
    trans(s->tr,pp);
    if(pp.size()<=1)
    {
        line_seg* oo=new line_seg();
        oo->coord=h->coor;
        oo->iobj=h->x_inter;
        oo->sig=s;
        return {oo};
    }
    vector<inter*> itrs=intervals(pp);
    vector<inter*> tt=interSetDifference({h->x_inter},interSection({h->x_inter},itrs));
    for(int i=0;i<tt.size();i++)
    {
        line_seg* nl=new line_seg();
        nl->coord=h->coor;
        nl->iobj=tt[i];
        nl->sig=s;
        ans.push_back(nl);
    }
    return ans;
}
vector<inter*> interUnion(vector<inter*> v1,vector<inter*> v2)
{
    vector<inter*> temp=v1;
    for(int i=0;i<v2.size();i++)
        temp.push_back(v2[i]);
    vector<pair<int,int>> res=getIntUnion(temp);
    vector<inter*> ans;
    for(int i=0;i<res.size();i++)
    {
        inter* obj=new inter();
        obj->bottom=min(res[i].first,res[i].second);
        obj->top=max(res[i].first,res[i].second);
        ans.push_back(obj);
    }
    return ans;
}
vector<inter*> interSection(vector<inter*> v1,vector<inter*> v2)
{
    vector<pair<int,int>> pr1=getIntUnion(v1);
    vector<pair<int,int>> pr2=getIntUnion(v2);
    vector<pair<int,int>> res;
    vector<inter*> ans;
    for(int i=0;i<pr1.size();i++)
    {
        pair<int,int> temp=pr1[i];
        for(int j=0;j<pr2.size();j++)
        {
            pair<int,int> t2=pr2[j];
            if(temp.first<=t2.second&&temp.first>=t2.first)
            {
                if(temp.second<=t2.second)
                {
                    if(temp.first!=temp.second)
                        res.push_back(temp);
                    break;
                }
                else
                {
                    if(temp.first!=t2.second)
                        res.push_back({temp.first,t2.second});
                    temp={t2.second,temp.second};
                }
            }
            else if(temp.first<t2.first&&temp.second>t2.first)
            {
                if(temp.second>t2.second)
                {
                    if(t2.first!=t2.second)
                        res.push_back(t2);
                    temp={t2.second,temp.second};
                }
                else
                {
                    if(t2.first!=temp.second)
                        res.push_back({t2.first,temp.second});
                    break;
                }
            }
        }
    }
    for(int i=0;i<res.size();i++)
    {
        inter* obj=new inter();
        obj->bottom=min(res[i].first,res[i].second);
        obj->top=max(res[i].first,res[i].second);
        ans.push_back(obj);
    }
    return ans;
}
vector<inter*> interSetDifference(vector<inter*> v1,vector<inter*> v2)
{
    vector<pair<int,int>> pr1=getIntUnion(v1);
    vector<pair<int,int>> pr2=getIntUnion(v2);
    vector<pair<int,int>> res;
    vector<inter*> ans;
    for(int i=0;i<pr1.size();i++)
    {
        pair<int,int> temp=pr1[i];
        int flag=0;
        for(int j=0;j<pr2.size();j++)
        {
            pair<int,int> t2=pr2[j];
            if(temp.first>=t2.first&&temp.first<=t2.second)
            {
                if(temp.second<=t2.second)
                {
                    flag++;
                    break;
                }
                else
                    temp={t2.second,temp.second};
            }
            else if(temp.first<t2.first)
            {
                if(temp.second<t2.first)
                    break;
                else
                {
                    if(temp.second>t2.second)
                    {
                        if(temp.first!=t2.first)
                            res.push_back({temp.first,t2.first});
                        temp={t2.second,temp.second};
                    }
                    else
                    {
                        temp={temp.first,t2.first};
                        break;
                    }
                }
            }
        }
        if(!flag)
        {
            if(temp.first!=temp.second)
                res.push_back(temp);
        }
    }
    for(int i=0;i<res.size();i++)
    {
        inter* obj=new inter();
        obj->bottom=min(res[i].first,res[i].second);
        obj->top=max(res[i].first,res[i].second);
        ans.push_back(obj);
    }
    return ans;
}
vector<stripe*> cop(vector<stripe*> &sp,vector<int> &P,inter* i1)
{
    vector<inter*> pp=partition(P);
    vector<stripe*> ss;
    for(int i=0;i<pp.size();i++)
    {
        stripe* obj=new stripe();
        obj->x_i=i1;
        obj->y_i=pp[i];
        ss.push_back(obj);
    }
    for(int i=0;i<ss.size();i++)
    {
        for(int j=0;j<sp.size();j++)
        {
            if(sp[j]->y_i->bottom<=ss[i]->y_i->bottom&&sp[j]->y_i->top>=ss[i]->y_i->top)
            {
                ss[i]->x_uni=sp[j]->x_uni;//D
                ss[i]->ms=sp[j]->ms;
                ss[i]->tr=sp[j]->tr;
            }
        }
    }
    return ss;
}
void blacken(vector<stripe*> &sp,vector<inter*> &inr)
{
    for(int i=0;i<sp.size();i++)
    {
        for(int j=0;j<inr.size();j++)
        {
            inter* it=inr[j];
            if(it->bottom<=sp[i]->y_i->bottom&&it->top>=sp[i]->y_i->top)
            {
                sp[i]->x_uni={sp[i]->x_i};//E
                sp[i]->ms=sp[i]->x_i->top-sp[i]->x_i->bottom;
                sp[i]->tr=NULL;
            }
        }
    }
}
vector<stripe*> concat(vector<stripe*> &SL,vector<stripe*> &SR,vector<int> &P,inter* itr)
{
    vector<stripe*> S;
    vector<inter*> pp=partition(P);
    for(int i=0;i<pp.size();i++)
    {
        stripe* obj=new stripe();
        obj->x_i=itr;
        obj->y_i=pp[i];
        obj->ms=0;
        S.push_back(obj);
    }
    for(int i=0;i<S.size();i++)
    {
        stripe* s1;
        stripe* s2;
        for(int j=0;j<SL.size();j++)
        {
            if(S[i]->y_i->bottom==SL[j]->y_i->bottom&&S[i]->y_i->top==SL[j]->y_i->top)
                s1=SL[j];
        }
        for(int j=0;j<SR.size();j++)
        {
            if(S[i]->y_i->bottom==SR[j]->y_i->bottom&&S[i]->y_i->top==SR[j]->y_i->top)
                s2=SR[j];
        }
        S[i]->x_uni=s1->x_uni;//F
        for(int j=0;j<s2->x_uni.size();j++)
            S[i]->x_uni.push_back(s2->x_uni[j]);//F
        S[i]->ms=s1->ms+s2->ms;
        if(s1->tr!=NULL&&s2->tr!=NULL)
        {
            ctree* o1=new ctree();
            lru* o2=new lru();
            S[i]->tr=o1;
            S[i]->tr->side=o2;
            S[i]->tr->side->at=-1;
            S[i]->tr->r=s1->x_i->top;
            S[i]->tr->lson=s1->tr;
            S[i]->tr->rson=s2->tr;
        }
        else if(s1->tr!=NULL&&s2->tr==NULL)
            S[i]->tr=s1->tr;
        else if(s1->tr==NULL&&s2->tr!=NULL)
            S[i]->tr=s2->tr;
        else
            S[i]->tr=NULL;
    }
    return S;
}
void STRIPES(vector<edge*> V,inter* x_ext,vector<inter*> &L,vector<inter*> &R,vector<int> &P,vector<stripe*> &S)
{
    if(V.size()==1)
    {
        edge* v=V[0];
        if(v->etype==0)
            L={v->y_inter};
        else
            R={v->y_inter};
        P={mini,v->y_inter->bottom,v->y_inter->top,maxi};
        vector<inter*> part=partition(P);
        for(int i=0;i<part.size();i++)
        {
            stripe* obj=new stripe();
            obj->x_i=x_ext;
            obj->y_i=part[i];
            obj->ms=0;
            obj->tr=NULL;
            S.push_back(obj);
        }
        for(int i=0;i<S.size();i++)
        {
            if(((S[i]->y_i->bottom)==(v->y_inter->bottom))&&((S[i]->y_i->top)==(v->y_inter->top)))
            {
                if(v->etype==0)
                {
                    inter* tt=new inter();
                    tt->bottom=v->coor;
                    tt->top=x_ext->top;
                    S[i]->x_uni={tt};//B
                    S[i]->ms=(x_ext->top)-(v->coor);
                    ctree* o1=new ctree();
                    lru* o2=new lru();
                    S[i]->tr=o1;
                    S[i]->tr->side=o2;
                    S[i]->tr->side->at=0;
                    S[i]->tr->r=v->coor;
                    S[i]->tr->rson=NULL;
                    S[i]->tr->lson=NULL;
                }
                else
                {
                    inter* tt=new inter();
                    tt->bottom=x_ext->bottom;
                    tt->top=v->coor;
                    S[i]->x_uni={tt};//C
                    S[i]->ms=v->coor-x_ext->bottom;
                    ctree* o1=new ctree();
                    lru* o2=new lru();
                    S[i]->tr=o1;
                    S[i]->tr->side=o2;
                    S[i]->tr->side->at=2;
                    S[i]->tr->r=v->coor;
                    S[i]->tr->rson=NULL;
                    S[i]->tr->lson=NULL;
                }
            }
        }
    }
    else if(V.size()>1)
    {
        sort(V.begin(),V.end(),custom);
        int s=V.size()/2;
        vector<edge*> V1;
        vector<edge*> V2;
        for(int i=0;i<V.size();i++)
        {
            if(i<s)
                V1.push_back(V[i]);
            else
                V2.push_back(V[i]);
        }
        int x=(V1[V1.size()-1]->coor+V2[0]->coor)/2;
        vector<inter*> L1;
        vector<inter*> R1;
        vector<int> P1;
        vector<stripe*> S1;
        vector<inter*> L2;
        vector<inter*> R2;
        vector<int> P2;
        vector<stripe*> S2;
        inter* i1=new inter();
        inter* i2=new inter();
        i1->bottom=x_ext->bottom;
        i1->top=x;
        i2->bottom=x;
        i2->top=x_ext->top;
        STRIPES(V1,i1,L1,R1,P1,S1);
        STRIPES(V2,i2,L2,R2,P2,S2);
        vector<inter*> LR=interSection(L1,R2);
        L=interSetDifference(L1,LR);
        L=interUnion(L,L2);
        R=interSetDifference(R2,LR);
        R=interUnion(R,R1);
        vector<int> pp(P1.size()+P2.size()+1);
        auto it=set_union(P1.begin(),P1.end(),P2.begin(),P2.end(),pp.begin());
        int ss=it-pp.begin();
        pp.resize(ss);
        P=pp;
        vector<stripe*> SL=cop(S1,P,i1);
        vector<stripe*> SR=cop(S2,P,i2);
        vector<inter*> tt1=interSetDifference(R2,LR);
        vector<inter*> tt2=interSetDifference(L1,LR);
        blacken(SL,tt1);
        blacken(SR,tt2);
        S=concat(SL,SR,P,x_ext);
    }
}
void Rectangle_Dac(vector<rect*> &R,vector<stripe*> &S)
{
    vector<edge*> VRX;
    vector<inter*> L;
    vector<inter*> Ri;
    vector<int> pts;
    for(int i=0;i<R.size();i++)
    {
        struct rect* temp=R[i];
        edge* e1=new edge();
        edge* e2=new edge();
        e1->etype=0;
        e2->etype=2;
        e1->x_inter=0;
        e2->x_inter=0;
        e1->y_inter=temp->y_int;
        e2->y_inter=temp->y_int;
        e1->coor=temp->x_l;
        e2->coor=temp->x_r;
        VRX.push_back(e1);
        VRX.push_back(e2);
    }
    inter* itrTemp=new inter();
    itrTemp->bottom=mini;
    itrTemp->top=maxi;
    STRIPES(VRX,itrTemp,L,Ri,pts,S);
}
double measure(vector<stripe*> &S)
{
    double ans=0;
    for(int i=0;i<S.size();i++)
        ans+=(S[i]->ms)*(S[i]->y_i->top-S[i]->y_i->bottom);
    return ans;
}
vector<line_seg*> contour_driver(vector<rect*> &R,vector<stripe*> &S)
{
    vector<line_seg*> meg;
    for(int i=0;i<R.size();i++)
    {
        edge* e1=new edge();
        edge* e2=new edge();
        e1->etype=1;
        e2->etype=3;
        e1->coor=R[i]->y_t;
        e2->coor=R[i]->y_b;
        e1->x_inter=R[i]->x_int;
        e2->x_inter=R[i]->x_int;
        e1->y_inter=0;
        e2->y_inter=0;
        vector<line_seg*> temp=contour_pieces(e1,S);
        vector<line_seg*> temp2=contour_pieces(e2,S);
        for(int i=0;i<temp.size();i++)
            meg.push_back(temp[i]);
        for(int i=0;i<temp2.size();i++)
            meg.push_back(temp2[i]);
    }
    return meg;
}
int main()
{
    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    #endif
    int n;
    cin>>n;
    vector<rect*> R;
    for(int i=0;i<n;i++)
    {
        rect* obj=new rect();
        cin>>obj->x_l>>obj->x_r>>obj->y_b>>obj->y_t;
        inter* obj1=new inter();
        inter* obj2=new inter();
        obj1->bottom=obj->x_l;
        obj1->top=obj->x_r;
        obj->x_int=obj1;
        obj2->bottom=obj->y_b;
        obj2->top=obj->y_t;
        obj->y_int=obj2;
        R.push_back(obj);
    }
    vector<stripe*> S;
    Rectangle_Dac(R,S);
    cout<<measure(S)<<endl;
    vector<line_seg*> lsg=contour_driver(R,S);
    double csum=0;
    for(int i=0;i<lsg.size();i++)
    {
        cout<<lsg[i]->coord<<" "<<lsg[i]->iobj->bottom<<" "<<lsg[i]->iobj->top<<endl;
        csum+=(lsg[i]->iobj->top-lsg[i]->iobj->bottom);
    }
    cout<<csum;
    return 0;
}