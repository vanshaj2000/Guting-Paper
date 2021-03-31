#include<bits/stdc++.h>
using namespace std;
int main()
{
    srand(time(0));
    int xl,xr,yb,yt;
    int t;
    cin>>t;
    while(t--)
    {
        xl=rand()%100;
        xr=rand()%200;
        while(xr<=xl)
            xr=rand()%200;
        yb=rand()%100;
        yt=rand()%200;
        while(yt<=yb)
            yt=rand()%200;
        cout<<xl<<" "<<xr<<" "<<yb<<" "<<yt<<"\n";
    }
    return 0;
}