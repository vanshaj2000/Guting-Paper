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
    return {};
}//Conversion function for x_proj
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
                s=temp;
        }
    }
    else
    {
        for(int i=0;i<S.size();i++)
        {
            struct stripe* temp=S[i];
            if(temp->y_i->bottom==h->coor)
                s=temp;
        }
    }
    vector<pair<int,int>> temp=getdiffIntervals(s->x_uni,h->bottom,h->top);
    vector<inter*> J;
    int r1=h->bottom;
    int r2=h->top;
    for(int i=0;i<temp.size();i++)
    {
        if(temp[i].first!=r1)
        {
            inter* obj=new inter();
            obj->bottom=r1;
            obj->top=temp[i].first;
            J.push_back(obj);
        }
        r1=temp[i].second;
    }
    if(r1!=r2)
    {
        inter* obj=new inter();
        obj->bottom=r1;
        obj->top=r2;
        J.push_back(obj);
    }
    for(int i=0;i<J.size();i++)
    {
        line_seg* ob=new line_seg();
        ob->iobj=J[i];
        ob->coord=h->coor;
        ans.push_back(ob);
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
                //cout<<ss[i]->ms<<endl;
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
                //if(sp[i]->ms>0)
                    //cout<<sp[i]->ms<<endl;
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
    //cout<<"Hello"<<endl;
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
        //if(S[i]->ms>0)
            //cout<<S[i]->ms<<endl;
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
    //cout<<"Hello"<<" "<<V.size()<<endl;
    if(V.size()==1)
    {
        edge* v=V[0];
        //cout<<v->etype<<endl;
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
            //cout<<S[i]->y_i->bottom<<" "<<S[i]->y_i->top<<endl;
            if(((S[i]->y_i->bottom)==(v->y_inter->bottom))&&((S[i]->y_i->top)==(v->y_inter->top)))
            {
                if(v->etype==0)
                {
                    inter* tt=new inter();
                    tt->bottom=v->coor;
                    tt->top=x_ext->top;
                    S[i]->x_uni={tt};//B
                    S[i]->ms=(x_ext->top)-(v->coor);
                    //cout<<S[i]->ms<<endl;
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
                    //cout<<S[i]->ms<<endl;
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
        //cout<<"Hemlo"<<endl;
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
        //cout<<"bbb"<<endl;
        STRIPES(V1,i1,L1,R1,P1,S1);
        //cout<<"hemloji"<<" "<<V1.size()<<endl;
        STRIPES(V2,i2,L2,R2,P2,S2);
        //cout<<"hemlouu"<<" "<<V2.size()<<endl;
        vector<inter*> LR=interSection(L1,R2);
        L=interSetDifference(L1,LR);
        L=interUnion(L,L2);
        R=interSetDifference(R2,LR);
        R=interUnion(R,R1);
        //cout<<P1.size()<<" "<<P2.size()<<endl;
        vector<int> pp(P1.size()+P2.size()+1);
        auto it=set_union(P1.begin(),P1.end(),P2.begin(),P2.end(),pp.begin());
        int ss=it-pp.begin();
        pp.resize(ss);
        P=pp;
        //cout<<P.size()<<endl;
        vector<stripe*> SL=cop(S1,P,i1);
        vector<stripe*> SR=cop(S2,P,i2);
        //cout<<SL.size()<<" "<<SR.size()<<endl;
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
    //cout<<VRX.size()<<endl;
    STRIPES(VRX,itrTemp,L,Ri,pts,S);
}
double measure(vector<stripe*> &S)
{
    double ans=0;
    for(int i=0;i<S.size();i++)
        ans+=(S[i]->ms)*(S[i]->y_i->top-S[i]->y_i->bottom);
    return ans;
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
    cout<<measure(S);
    return 0;
}