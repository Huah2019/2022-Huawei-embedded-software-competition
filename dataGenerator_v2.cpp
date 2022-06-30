#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
mt19937_64 gen(time(0));
ll rdint(ll l,ll r){return uniform_int_distribution<ll>(l,r)(gen);}
const int energyNum=5,instrumentTypeNum=3;
int main()
{
    freopen("1.in","w",stdout);
    int K=500'000;//[1,1'000'000]
    cout<<K<<'\n';
    for(int i=0;i<energyNum;++i) //[1,2000]
        cout<<rdint(1,2'000)<<(i==energyNum-1?'\n':' ');
    int N=rdint(20,30);//[1,100]
    cout<<N<<'\n';
    int R=rdint(N,N*5);//[N,N*5]
    cout<<R<<'\n';
    for(int i=0;i<R;++i)
    {
        int x=rdint(0,N-1),y=rdint(0,energyNum-1);
        cout<<x<<' '<<y<<'\n';
    }
    int L=rdint(1,10);
    cout<<L<<'\n';
    int backToZero=rdint(0,30);//[0,100]
    cout<<backToZero<<'\n';
    int W=rdint(max(1,backToZero),100);//[1,100]
    cout<<W<<'\n';
    for(int i=0;i<W;++i)
    {
        bool backToSelf=false;
        int workshop,costCoefficient;
        bool ability[3];
        if(i>backToZero&&rdint(1,100)>66) backToSelf=true;
        workshop=rdint(0,N-1);
        costCoefficient=rdint(1,10000);
        for(int j=0;j<3;++j) ability[j]=rdint(0,1);
        if(ability[0]&&ability[1]&&ability[2])
            ability[rdint(0,2)]=false;
        if(!ability[0]&&!ability[1]&&!ability[2])
            ability[rdint(0,2)]=true;
        cout<<backToSelf<<' '<<workshop<<' '<<costCoefficient;
        for(int j=0;j<3;++j)
            cout<<' '<<ability[j];
        cout<<'\n';
    }
    int D=rdint(1,300);//[1,1'000]
    cout<<D<<'\n';
    for(int i=0;i<D;++i)
    {
        int type=rdint(0,2);
        cout<<type;
        for(int j=0;j<energyNum;++j)
            cout<<' '<<rdint(1,100000000);
        cout<<'\n';
    }
    vector<int>p(D);
    iota(p.begin(),p.end(),0);
    random_shuffle(p.begin(),p.end());
    vector<pair<int,int>>edges;
    int id=0;
    map<pair<int,int>,int>vis;
    map<int,int>fa;
    for(int i=1;i<p.size();++i)
    {
        int j=rdint(max(0,i-20),i-1);
        edges.push_back({i,j});
        fa[i]=j;
        vis[{i,j}]=++id;
    }
    int edgeNum=rdint(max(2,(int)vis.size()),300);//[1,1000]
    while(vis.size()<edgeNum)
    {
        int i=rdint(0,D-1);
        int j=rdint(0,D-1);
        while(true)
        {
            if(i<j) swap(i,j);
            if(i==j||vis.count({i,j}))
            {
                i=rdint(0,D-1);
                j=rdint(0,D-1);
            }
            else break;
        }
        edges.push_back({i,j});
        vis[{i,j}]=++id;
    }
    cout<<edgeNum<<'\n';
    for(auto x:edges)
    {
        int z=rdint(0,100)>80;
        cout<<z<<' '<<p[x.first]<<' '<<p[x.second]<<'\n';
    }
    vector<int>eid;
    int u=p.size()-1;
    while(fa.count(u))
    {
        int v=fa[u];
        eid.push_back(vis[{u,v}]-1);
        u=v;
    }
    cout<<eid.size()<<'\n';
    for(int i=0;i<eid.size();++i)
        cout<<eid[i]<<(i==eid.size()-1?'\n':' ');
}