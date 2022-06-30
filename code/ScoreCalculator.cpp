#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
mt19937_64 gen(233589449);
ll rdint(ll l,ll r){return uniform_int_distribution<ll>(l,r)(gen);}
double rddouble(double l,double r){return uniform_real_distribution<double>(l,r)(gen);}
const ll inf=1e18;
const int MaxinstrumentNum=1000;
#define DEBUG
#ifdef DEBUG
void feedback(string s){printf("%s\n",s.c_str());}
// string inputFile="../case0.in";
// string inputFile="../1.in";
string inputFile="../2.in";
string OutputFile="../solution.txt";
#else
void feedback(string s){}
#endif
const int energyNum=5,instrumentTypeNum=3;
int K,N,L,backToZero,windowNum,instrumentNum;
class TimeCounter
{
    private:
        int64_t beginTime;
        static inline int64_t getCurrentMillisecs()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch())
            .count();
        }
    public:
        TimeCounter(){beginTime=getCurrentMillisecs();}
        int64_t getTime(){ return getCurrentMillisecs()-beginTime;};
}timeCounter;
struct workshopArae
{
    int id,energy;
};
struct window
{
    bool backToSelf;
    int workshop;
    int costCoefficient;
    bool ability[instrumentTypeNum];
};
struct instrument
{
    int type;
    int cost[energyNum];
};
bool supportEnergy[3][5]={{1,1,0,0,0},{1,0,1,0,0},{0,0,0,1,1}};
vector<window>windows;
vector<instrument>instruments;
vector<int>energyTime;
vector<vector<workshopArae>>workshop;
vector<workshopArae>workshopAraes;
class assemblyLineGraph
{
public:
    int nodeNum;
    struct edge
    {
        int from,to,type;
    };
    vector<edge>edges;
    void init(int _nodeNum)
    {
        nodeNum=_nodeNum;
    }
    void addedge(int u,int v,int type)
    {
        edges.push_back({u,v,type});
    }
}G;
class assemblyLine
{
    public:
        int K;
        vector<int>edges,pre;
        vector<int>instrumentSeq()
        {
            if(edges.empty()) return vector<int>();
            vector<int>ans;
            ans.emplace_back(G.edges[edges[0]].from);
            pre.resize(instrumentNum);
            pre[0]=-1;
            for(int i=0;i<edges.size();++i)
            {
                pre[G.edges[edges[i]].to]=i;
                ans.emplace_back(G.edges[edges[i]].to);
            }
            return ans;
        }
        int getpre(int to)
        {
            if(pre[to]==-1||G.edges[pre[to]].type!=1) return -1;
            return G.edges[pre[to]].from;
        }
};
vector<assemblyLine>lines;
void calScore()
{
    freopen(OutputFile.c_str(),"r",stdin);
    vector<vector<int>>seqs;
    for(auto &line:lines) seqs.emplace_back(line.instrumentSeq());
    int D;
    scanf("%d",&D);
    if(D!=instrumentNum)
    {
        feedback("Error: instrument number ilegal Yours: "+to_string(D)+" correct: "+to_string(instrumentNum));
        exit(0);
    }
    vector<int>workshopAraeId(instrumentNum);
    ll Cost=0,installCost=0,matchCost=0;
    for(int i=0;i<D;++i)
    {
        scanf("%d",&workshopAraeId[i]);
        auto &y=workshopAraes[workshopAraeId[i]];
        installCost+=instruments[i].cost[y.energy];
    }
    vector<vector<int>>match_windows;
    vector<int>maxTime(windowNum);
    int T;scanf("%d",&T);
    match_windows.resize(T);
    seqs.resize(T);
    for(int line_id=0;line_id<T;++line_id)
    {
        auto &seq=seqs[line_id];
        auto &match_window=match_windows[line_id];
        int len;scanf("%d",&len);
        match_window.resize(len);
        for(int i=0;i<len;++i)
        { 
            scanf("%d",&match_window[i]);
            auto &y=workshopAraes[workshopAraeId[seq[i]]];
            maxTime[match_window[i]]=max(maxTime[match_window[i]],energyTime[y.energy]);
        }
    }
    for(int i=0;i<windowNum;++i)
        matchCost+=1ll*maxTime[i]*windows[i].costCoefficient;
    for(int line_id=0;line_id<T;++line_id)
    {
        auto &seq=seqs[line_id];
        auto &match_window=match_windows[line_id];
        int len=match_window.size();
        for(int i=0;i<len;++i)
        { 
            int win=match_window[i];
            int ins_id=seq[i];
            bool flag=true;
            if(i)
            {
                auto &e=G.edges[lines[line_id].edges[i-1]];
                if(win==match_window[i-1]&&e.type==1) flag=false;
            }
            if(flag)
                matchCost+=1ll*maxTime[win]*lines[line_id].K;
        }
    }
    Cost=installCost+matchCost;
    feedback("Read solution OK");
    feedback("totCost: "+to_string(Cost));
    feedback("installCost: "+to_string(installCost));
    feedback("matchCost: "+to_string(matchCost));
    freopen("CON","r",stdin);
}
int main()
{
    #ifdef DEBUG
        freopen(inputFile.c_str(),"r",stdin);
    #endif
    auto readData=[&]()
    {
        // scanf("%d",&K);
        energyTime.resize(energyNum);
        for(int i=0;i<energyNum;++i) scanf("%d",&energyTime[i]);
        scanf("%d",&N);
        workshop.resize(N);
        int R;scanf("%d",&R);
        for(int i=0;i<R;++i)
        {
            int x,y;scanf("%d%d",&x,&y);int lim;cin>>lim;
            workshop[x].push_back({i,y});
            workshopAraes.push_back({i,y});
        }
        scanf("%d",&L);
        scanf("%d",&backToZero);
        --backToZero;
        scanf("%d",&windowNum);
        windows.resize(windowNum);
        for(int i=0;i<windowNum;++i)
        {
            scanf("%d%d%d",&windows[i].backToSelf,&windows[i].workshop,&windows[i].costCoefficient);
            for(int j=0;j<instrumentTypeNum;++j)
                scanf("%d",&windows[i].ability[j]);
        }
        scanf("%d",&instrumentNum);
        instruments.resize(instrumentNum);
        for(int i=0;i<instrumentNum;++i)
        {
            scanf("%d",&instruments[i].type);
            for(int j=0;j<energyNum;++j)
                scanf("%d",&instruments[i].cost[j]);
            int x=0;
            for(int j=0;j<energyNum;++j) cin>>x;
        }
        G.init(instrumentNum);
        int edgeNum;
        scanf("%d",&edgeNum);
        for(int i=0;i<edgeNum;++i)
        {
            int z,x,y;scanf("%d%d%d",&z,&x,&y);
            G.addedge(x,y,z);
        }
        int T;scanf("%d",&T);
        lines.resize(T);
        for(int i=0;i<T;++i)
        {
            scanf("%d",&lines[i].K);
            scanf("%d",&edgeNum);
            lines[i].edges.resize(edgeNum);
            for(int j=0;j<edgeNum;++j)
                scanf("%d",&lines[i].edges[j]);
        }
    };
    readData();
    feedback("readData OK!");
    // return 0;
    // for(auto e_id:line.edges)
    //     cout<<G.edges[e_id].from<<' '<<G.edges[e_id].to<<' '<<G.edges[e_id].type<<'\n';

    calScore();
    feedback("installationInstruments OK!");
    feedback("Use Time: "+to_string(timeCounter.getTime())+"ms");
}