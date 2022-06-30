#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
mt19937_64 gen(time(0));
ll rdint(ll l,ll r){return uniform_int_distribution<ll>(l,r)(gen);}
const int energyNum=5,instrumentTypeNum=3;
int main()
{
    freopen("2.in","w",stdout);
    // int K=500'000;//[1,1'000'000]
    // cout<<K<<'\n';
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
    vector<pair<int,int>>edges;
    vector<int>f(D);
    iota(f.begin(),f.end(),0);
    function<int(int)>getf=[&](int x){return f[x]==x?x:f[x]=getf(f[x]);};
    vector<vector<bool>>vis(D,vector<bool>(D));
    int edgeNum=rdint(D-1,300);//[1,1000]
    int k=D;
    for(int i=0;i<edgeNum;++i)
    {
        int u=rdint(0,D-2),v=rdint(u+1,min(D-1,u+10));
        while(u==v||vis[min(u,v)][max(u,v)]) u=rdint(0,D-2),v=rdint(u+1,min(D-1,u+10));
        if(getf(u)!=getf(v))
        {
            f[getf(u)]=getf(v);
            --k;
        }
        if(u>v) swap(u,v);
        edges.push_back({u,v});
        vis[u][v]=true;
    }
    while(k>1)
    {
        int u=rdint(0,D-2),v=rdint(u+1,min(D-1,u+10));
        while(getf(u)==getf(v))
            u=rdint(0,D-2),v=rdint(u+1,min(D-1,u+10));
        f[getf(u)]=getf(v);
        --k;
        if(u>v) swap(u,v);
        edges.push_back({u,v});
        vis[u][v]=true;
    }

    cout<<edges.size()<<'\n';
    for(auto &[u,v]:edges)
    {
        int type=rdint(0,20)<=3;
        cout<<type<<' '<<u<<' '<<v<<'\n';
    }

    vector<vector<int>>to(D);
    for(int i=0;i<edges.size();++i)
        to[edges[i].first].push_back(i);

    // int T=rdint(1,10);
    int T=10;
    cout<<T<<'\n';
    while(T--)
    {
        int K=rdint(1,50)*10'000;//[1,1'000'000]
        cout<<K<<' ';
        vector<int>eid;
        int s=rdint(0,20);
        while(to[s].empty()) s=rdint(0,20);
        while(to[s].size())
        {
            int x=rdint(0,to[s].size()-1);
            eid.push_back(to[s][x]);
            s=edges[to[s][x]].second;
        }
        cout<<eid.size();
        for(int x:eid)
            cout<<' '<<x;
        cout<<'\n';
    }
}