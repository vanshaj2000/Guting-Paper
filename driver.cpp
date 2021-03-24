#include "includes/dtypes.h"
using namespace std;
int mini=INT_MIN;
int maxi=INT_MAX;
bool ec(edge* e1,edge* e2)//Comparator for sorting the vertical edges to handle multiple x coordinates case
{
    if(e1->coor!=e2->coor)
        return e1->coor<e2->coor;
    else
        return e1->etype<e2->etype;
}
vector<inter*> partition(vector<int> Y)//Function to find the different intervals from an array of coordinates
{
    vector<inter*> res;
    sort(Y.begin(),Y.end());//Sorting the points
    for(int i=0;i<Y.size()-1;i++)//Then taking consecutive intervals
    {
        inter* obj=new inter();
        obj->bottom=Y[i];
        obj->top=Y[i+1];
        res.push_back(obj);
    }
    return res;
}
vector<inter*> intervals(vector<int> cors)//Used for getting the suninterval queries where cors are the coordinates of the leaf nodes ofa stripe
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
vector<pair<int,int>> getIntUnion(vector<inter*> ss)//This function finds the array of intervals without overlapping intervals 
{
    if(ss.size()==0)
        return {};
    vector<pair<int,int>> pr;
    for(int i=0;i<ss.size();i++)
        pr.push_back({ss[i]->bottom,ss[i]->top});
    sort(pr.begin(),pr.end());//Sorting acc to starting time
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
void trans(ctree* node,vector<int> &ptr)//Function to find the leaf nodes and store them in an array of ptr
{
    if(node==NULL)//If tree is empty, return NULL
        return;
    if(node->lson==NULL&&node->rson==NULL)//If its a leaf node then push in the array
        ptr.push_back(node->r);
    if(node->lson!=NULL)
        trans(node->lson,ptr);
    if(node->rson!=NULL)
        trans(node->rson,ptr);
}
vector<line_seg*> contour_pieces(edge* h,vector<stripe*> S)//This function returns an array of line segments that are the horizontal contour pieces
{
    vector<line_seg*> ans;
    struct stripe* s;
    if(h->etype==3)//If the edge is a top edge
    {
        for(int i=0;i<S.size();i++)
        {
            struct stripe* temp=S[i];
            if(temp->y_i->top==h->coor)//To get the stripe whose top coincides with the edge
            {
                s=temp;
                break;
            }
        }
    }
    else//If its a bottom edge
    {
        for(int i=0;i<S.size();i++)
        {
            struct stripe* temp=S[i];
            if(temp->y_i->bottom==h->coor)//To get the stripe whose top coincides with the edge
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
vector<line_seg*> vert_contour(vector<stripe*> &S)//This function find the vertical contour pieces
{
    vector<line_seg*> ans;
    for(int i=0;i<S.size();i++)//We go to every stripe and its leaf nodes and from every coordinate in it make a line segment above its immediate stripe
    {
        stripe* s=S[i];
        if(s->tr!=NULL)
        {
            vector<int> pp;
            trans(s->tr,pp);//store the leaf nodes
            for(int i=0;i<pp.size();i++)
            {
                line_seg* o1=new line_seg();//Creation of the line segment
                o1->iobj=s->y_i;
                o1->coord=pp[i];
                ans.push_back(o1);
            }
        }
    }
    return ans;
}
vector<inter*> interUnion(vector<inter*> v1,vector<inter*> v2)//Finds the union of 2 sets of intervals
{
    vector<inter*> temp=v1;
    for(int i=0;i<v2.size();i++)
        temp.push_back(v2[i]);//Push both of them in a single array
    vector<pair<int,int>> res=getIntUnion(temp);//And call the getIntUnion
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
vector<inter*> interSection(vector<inter*> v1,vector<inter*> v2)//Finds the intersection of 2 sets of intervals
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
vector<inter*> interSetDifference(vector<inter*> v1,vector<inter*> v2)//Finds the set fifference of 2 sets of intervals
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
vector<stripe*> cop(vector<stripe*> &sp,vector<int> &P,inter* i1)//This is the copy function descibed in the algorithm
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
    for(int i=0;i<ss.size();i++)//Copy an array of original stripes and make a copy array called ss
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
void blacken(vector<stripe*> &sp,vector<inter*> &inr)//This is the blacken function described in the algorithm
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
vector<stripe*> concat(vector<stripe*> &SL,vector<stripe*> &SR,vector<int> &P,inter* itr)//This is the concatenation function described in the paper
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
        if(s1->tr!=NULL&&s2->tr!=NULL)//Tree construction
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
void STRIPES(vector<edge*> V,inter* x_ext,vector<inter*> &L,vector<inter*> &R,vector<int> &P,vector<stripe*> &S)//This is the main function to create stripes
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
        sort(V.begin(),V.end(),ec);
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
void Rectangle_Dac(vector<rect*> &R,vector<stripe*> &S)//This is the main function to call stripes with sending the set of all vertical edges
{
    vector<edge*> VRX;
    vector<inter*> L;
    vector<inter*> Ri;
    vector<int> pts;
    for(int i=0;i<R.size();i++)//Creation of vertical edges from every rectangle
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
    sort(VRX.begin(),VRX.end(),ec);//Sorting to handle multiple x coordinates case
    STRIPES(VRX,itrTemp,L,Ri,pts,S);//Calling stripes
}
double measure(vector<stripe*> &S)//Function to find the measure
{
    double ans=0;
    for(int i=0;i<S.size();i++)
        ans+=(S[i]->ms)*(S[i]->y_i->top-S[i]->y_i->bottom);
    return ans;
}
vector<line_seg*> contour_driver(vector<rect*> &R,vector<stripe*> &S)//Driver for finding contour pieces
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
        vector<line_seg*> temp=contour_pieces(e1,S);//Finding the contour pieces for every horizontal edge
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
    int n;
    cin>>n;
    vector<rect*> R;
    for(int i=0;i<n;i++)//Input of rectangle coordinates as x_l x_r y_b y_t
    {
        rect* obj=new rect();
        cin>>obj->x_l>>obj->x_r>>obj->y_b>>obj->y_t;
        obj->x_l=min(obj->x_l,obj->x_r);
        obj->x_r=max(obj->x_l,obj->x_r);
        obj->y_b=min(obj->y_b,obj->y_t);
        obj->y_t=max(obj->y_b,obj->y_t);
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
    ofstream msr;//Oftreams for visualisations through python
    msr.open("msr.txt");
    msr<<measure(S)<<endl;
    msr.close();
    vector<line_seg*> lsg=contour_driver(R,S);//Finding horizontal contours
    vector<line_seg*> lvert=vert_contour(S);//Finding vertical contours
    double csum=0;
    ofstream os;//Oftreams for visualisations through python
    os.open("otp.txt");
    ofstream os2;//Oftreams for visualisations through python
    os2.open("rec.txt");
    for(int i=0;i<R.size();i++)//Writing rectangle coordinates
    {
        os2<<R[i]->x_l<<" "<<R[i]->y_b<<" "<<R[i]->x_l<<" "<<R[i]->y_t<<endl;
        os2<<R[i]->x_l<<" "<<R[i]->y_b<<" "<<R[i]->x_r<<" "<<R[i]->y_b<<endl;
        os2<<R[i]->x_r<<" "<<R[i]->y_b<<" "<<R[i]->x_r<<" "<<R[i]->y_t<<endl;
        os2<<R[i]->x_l<<" "<<R[i]->y_t<<" "<<R[i]->x_r<<" "<<R[i]->y_t<<endl;
    }
    for(int i=0;i<lsg.size();i++)//Writing horizontal contour pieces coordinates
    {
        os<<lsg[i]->iobj->bottom<<" "<<lsg[i]->coord<<" "<<lsg[i]->iobj->top<<" "<<lsg[i]->coord<<endl;
        csum+=(lsg[i]->iobj->top-lsg[i]->iobj->bottom);
    }
    for(int i=0;i<lvert.size();i++)//Writing vertical contour pieces coordinates
    {
        os<<lvert[i]->coord<<" "<<lvert[i]->iobj->bottom<<" "<<lvert[i]->coord<<" "<<lvert[i]->iobj->top<<endl;
        csum+=(lvert[i]->iobj->top-lvert[i]->iobj->bottom);
    }
    //cout<<csum;
    os.close();
    os2.close();
    return 0;
}