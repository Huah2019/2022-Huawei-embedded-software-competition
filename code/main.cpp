/*
case0.in
totCost: 433280944
installCost: 227655680
matchCost: 205625264
2.in
totCost: 31000338338
installCost: 6994545167
matchCost: 24005793171
2.in 赛后参数
totCost: 30677957787
installCost: 6949735308
matchCost: 23728222479
*/
#pragma GCC optimize(2)
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
mt19937_64 gen(2333);
ll rdint(ll l,ll r){return uniform_int_distribution<ll>(l,r)(gen);}
double rddouble(double l,double r){return uniform_real_distribution<double>(l,r)(gen);}
const ll inf=1e18;
const int MaxinstrumentNum=1000,maxEnergeTime=2000;
#define DEBUG
#define timeMax
#ifdef DEBUG
// string inputDir="../case0.in";
string inputDir="../2.in";
void feedback(string s){printf("%s\n",s.c_str());}
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
class workshopArae
{
public:
    int id,energy,workshop;
    int lim;//能耗
};
class window
{
public:
    bool backToSelf;
    int workshop;
    int costCoefficient;
    bool ability[instrumentTypeNum];
};
class instrument
{
public:
    int type;
    int cost[energyNum];
    int lim[energyNum];//仪器使用每种能源的能耗
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
    vector<vector<edge>>e,re;
    vector<edge>edges;
    void init(int _nodeNum)
    {
        nodeNum=_nodeNum;
        e.resize(nodeNum);
        re.resize(nodeNum);
    }
    void addedge(int u,int v,int type)
    {
        e[u].push_back({u,v,type});
        re[v].push_back({u,v,type});//反向边
        edges.push_back({u,v,type});
    }
    vector<int>topOrder()
    {
        vector<int>d(nodeNum);
        for(int i=0;i<nodeNum;++i)
            for(auto x:e[i])
                ++d[x.to];
        queue<int>q;
        vector<int>ans;
        for(int i=0;i<nodeNum;++i)
            if(d[i]==0) q.push(i);
        while(!q.empty())
        {
            int u=q.front();q.pop();
            ans.push_back(u);
            for(auto x:e[u])
                if(--d[x.to]==0) q.push(x.to);
        }
        return ans;
    }
    void remakeGraph()
    {
        vector<bitset<MaxinstrumentNum>>ok(nodeNum);
        auto ord=topOrder();
        reverse(ord.begin(),ord.end());
        vector<edge>effective_edges;
        for(int i=0;i<nodeNum;++i) ok[i][i]=1;
        for(int u:ord)
        {
            bitset<MaxinstrumentNum>vis;
            for(auto x:e[u])
            {
                ok[x.to][x.to]=0;
                vis|=ok[x.to];
                ok[x.to][x.to]=1;
            }
            for(auto x:e[u])
                if(!vis[x.to])
                {
                    effective_edges.push_back(x);
                    ok[u]|=ok[x.to];
                }
        }
        for(int i=0;i<nodeNum;++i) e[i].clear(),re[i].clear();
        for(auto &x:effective_edges)
        {
            e[x.from].push_back(x);
            re[x.to].push_back(x);
        }
        for(int i=0;i<nodeNum;++i)
        {
            sort(e[i].begin(),e[i].end(),[&](edge &x,edge &y){
                return ok[x.to].count()>ok[y.to].count();
            });
        }
    }
}G;
class assemblyLine
{
    public:
        int K;
        vector<int>edges,seq,pre;
        vector<int>instrumentSeq()
        {
            if(edges.empty()) return vector<int>();
            if(!seq.empty()) return seq;
            seq.emplace_back(G.edges[edges[0]].from);
            pre.resize(instrumentNum,-1);
            for(int i=0;i<edges.size();++i)
            {
                pre[G.edges[edges[i]].to]=edges[i];
                seq.emplace_back(G.edges[edges[i]].to);
            }
            return seq;
        }
        int getpre(int to)
        {
            if(pre[to]!=-1) assert(G.edges[pre[to]].to==to);
            if(pre[to]==-1||G.edges[pre[to]].type!=1) return -1;
            return G.edges[pre[to]].from;
        }
}line;
vector<assemblyLine>lines;
namespace State
{
    vector<bool>markInstrment;
    vector<int>wins;
    vector<int>seq;
    vector<vector<int>>seqs;
    ll dp[1001][1101];
    int pre[1001][1101];
    struct InstallState
    {
        vector<int>winId,workshopAraeId;
        vector<int>enterNum,maxTime;
        vector<vector<int>>mwins;
        vector<ll>linematchCost;
        vector<int>rlim;//每个车间区域剩余的能耗
        ll Cost,installCost,matchCost;
        void init(int instrumentNum,int windowNum)
        {
            // winId.clear();
            // workshopAraeId.clear();
            // enterNum.clear();
            // maxTime.clear();
            winId.resize(instrumentNum);
            workshopAraeId.resize(instrumentNum);
            enterNum.resize(windowNum);
            maxTime.resize(windowNum);
            rlim.resize(workshopAraes.size());
            for(int i=0;i<workshopAraes.size();++i)
                rlim[i]=workshopAraes[i].lim;
            for(int &x:enterNum) x=0;
            for(int &x:maxTime) x=0;
            Cost=installCost=matchCost=0;
        }
        void init()
        {
            init(instrumentNum,windowNum);
        }
        ll getCostIncrement(int ins_id,int win_id,int wsa_id)
        {
            ll newCost=Cost;
            workshopArae &sa=workshopAraes[wsa_id];
            int win=wins[win_id];
            newCost+=instruments[ins_id].cost[sa.energy];
            if(markInstrment[ins_id])
            {
                newCost-=1ll*maxTime[win]*enterNum[win]*K+1ll*maxTime[win]*windows[win].costCoefficient;
                int time=max(maxTime[win],energyTime[sa.energy]);
                bool flag=true;
                int pre=line.getpre(ins_id);
                if(pre!=-1&&wins[winId[pre]]==win) flag=false;
                int num=enterNum[win]+flag;
                newCost+=1ll*time*num*K+1ll*time*windows[win].costCoefficient;
            }
            return newCost-Cost;
        }
        void upd(int ins_id,int win_id,int wsa_id)
        {
            ll newCost=Cost;
            workshopArae &sa=workshopAraes[wsa_id];
            int win=wins[win_id];
            newCost+=instruments[ins_id].cost[sa.energy];
            installCost+=instruments[ins_id].cost[sa.energy];
            if(markInstrment[ins_id])
            {
                newCost-=1ll*maxTime[win]*enterNum[win]*K+1ll*maxTime[win]*windows[win].costCoefficient;
                int time=max(maxTime[win],energyTime[sa.energy]);
                bool flag=true;
                int pre=line.getpre(ins_id);
                if(pre!=-1&&wins[winId[pre]]==win) flag=false;
                int num=enterNum[win]+flag;
                newCost+=1ll*time*num*K+1ll*time*windows[win].costCoefficient;
                maxTime[win]=time;
                enterNum[win]=num;
            }
            winId[ins_id]=win_id;
            workshopAraeId[ins_id]=wsa_id;
            Cost=newCost;
            matchCost=Cost-installCost;
            rlim[wsa_id]-=instruments[ins_id].lim[sa.energy];
        }
        void getLineCost(assemblyLine &line,vector<int>&mwin,ll &matchCost)
        {
            auto &seq=line.seq;
            mwin.clear();
            for(int i=0;i<seq.size();++i)
                for(int j=0;j<wins.size();++j)
                dp[i][j]=inf;
            auto judge=[&](int ins,int win)
            {
                auto &y=workshopAraes[workshopAraeId[ins]];
                if(windows[win].workshop!=y.workshop) return false;
                if(windows[win].ability[instruments[ins].type]==false) return false;
                if(energyTime[y.energy]>maxTime[win]) return false;
                return true;
            };
            for(int j=0;j<wins.size();++j)
                if(judge(seq[0],wins[j]))
                    dp[0][j]=1ll*maxTime[wins[j]]*line.K;
            for(int i=1;i<seq.size();++i)
            {
                ll mn=inf;
                int pj=-1;
                bool flag=line.getpre(seq[i])!=-1;
                for(int j=0;j<wins.size();++j)
                {
                    if(judge(seq[i],wins[j]))
                    {
                        dp[i][j]=min(inf,mn+1ll*maxTime[wins[j]]*line.K);
                        pre[i][j]=pj;
                        if(flag&&dp[i-1][j]<dp[i][j])
                        {
                            dp[i][j]=dp[i-1][j];
                            pre[i][j]=j;
                        }
                    }
                    if(dp[i-1][j]<mn)
                    {
                        mn=dp[i-1][j];
                        pj=j;
                    }
                }
            }
            ll mn=inf;
            int pj;
            for(int j=0;j<wins.size();++j)
                if(dp[seq.size()-1][j]<mn)
                mn=dp[seq.size()-1][j],pj=j;
            for(int i=seq.size()-1;i>=0;--i)
            {
                mwin.push_back(wins[pj]);
                pj=pre[i][pj];
            }
            reverse(mwin.begin(),mwin.end());
            matchCost=mn;
        }
        void getRealCost()
        {
            for(int &x:maxTime) x=0;
            Cost=installCost=matchCost=0;
            for(int i=0;i<workshopAraeId.size();++i)
                installCost+=instruments[i].cost[workshopAraes[workshopAraeId[i]].energy];
            Cost=installCost;
            for(int i=0;i<lines.size();++i)
                for(int j=0;j<lines[i].seq.size();++j)
                {
                    int ins_id=lines[i].seq[j];
                    int wsa_id=workshopAraeId[ins_id];
                    workshopArae &sa=workshopAraes[wsa_id];
                    int win=mwins[i][j];
                    maxTime[win]=max(maxTime[win],energyTime[sa.energy]);
                }
            for(int i=0;i<enterNum.size();++i)
                matchCost+=1ll*maxTime[i]*windows[i].costCoefficient;
            for(int i=0;i<lines.size();++i)
                for(int j=0;j<lines[i].seq.size();++j)
                {
                    bool flag=true;
                    if(lines[i].getpre(lines[i].seq[j])!=-1&&mwins[i][j]==mwins[i][j-1]) flag=false;
                    int win=mwins[i][j];
                    if(flag) matchCost+=1ll*maxTime[win]*lines[i].K;
                }
            Cost+=matchCost;
        }
        void remakeCost()
        {
            if(Cost>=inf) return;
            for(int &x:maxTime) x=0;
            Cost=installCost=matchCost=0;
            for(int i=0;i<workshopAraeId.size();++i)
                installCost+=instruments[i].cost[workshopAraes[workshopAraeId[i]].energy];
            Cost=installCost;
            vector<int>minworkshopTime(workshop.size(),maxEnergeTime+1);
            for(auto &seq:seqs)
                for(int ins_id:seq)
                {
                    int win_id=winId[ins_id],wsa_id=workshopAraeId[ins_id];
                    workshopArae &sa=workshopAraes[wsa_id];
                    int win=wins[win_id];
                    maxTime[win]=max(maxTime[win],energyTime[sa.energy]);
                    minworkshopTime[windows[win].workshop]=min(minworkshopTime[windows[win].workshop],energyTime[sa.energy]);
                }
            for(int i=0;i<enterNum.size();++i)
                matchCost+=1ll*maxTime[i]*windows[i].costCoefficient;
            for(int win=0;win<windowNum;++win)
                if(maxTime[win]==0&&minworkshopTime[windows[win].workshop]!=maxEnergeTime+1)
                maxTime[win]=minworkshopTime[windows[win].workshop];
            linematchCost.resize(seqs.size());
            mwins.resize(seqs.size());
            for(int id=0;id<seqs.size();++id)
            {
                getLineCost(lines[id],mwins[id],linematchCost[id]);
                matchCost+=linematchCost[id];
            }
            Cost+=matchCost;
            getRealCost();
        }
    };
}
using namespace State;
vector<int>getrlim()
{
    vector<int>rlim(workshopAraes.size());
    for(int i=0;i<workshopAraes.size();++i) rlim[i]=workshopAraes[i].lim;
    return rlim;
}
void installationInstruments()
{
    G.remakeGraph();
    vector<vector<int>>seqs;
    for(auto &line:lines) seqs.emplace_back(line.instrumentSeq());
    auto seq=seqs[0];
    K=lines[0].K;
    line=lines[0];
    for(int i=1;i<seqs.size();++i)
        if(seqs[i].size()*lines[i].K>seq.size()*K)
        {
            line=lines[i];
            seq=seqs[i];
            K=lines[i].K;
        }
    vector<int>order=G.topOrder();
    vector<bool>vis(G.nodeNum);
    for(auto &seq:seqs)
        for(int x:seq) vis[x]=true;
    vector<int>wins;
    for(int j=1;j<=L+1;++j)
        for(int i=0;i<=backToZero;++i) wins.push_back(i);
    for(int i=backToZero+1;i<windowNum;++i)
        for(int j=1;j<=1+(windows[i].backToSelf?L:0);++j)
            wins.push_back(i);
    auto getwinID_r=[&](vector<int>&winId)
    {
        vector<int>rlim=getrlim();
        winId=vector<int>(G.nodeNum);
        for(int z=order.size()-1;z>=0;--z)
        {
            int x=order[z];
            int mn=wins.size()-1;
            for(auto e:G.e[x])
            {
                int win_id=winId[e.to];
                mn=min(mn,win_id-(e.type==0));
            }
            bool install=false;
            for(int i=mn;i>=0;--i)
            {
                if(vis[x]&&!windows[wins[i]].ability[instruments[x].type]) continue;
                //如果窗口wins[i]对主流水线上的仪器x的类型没有预加工能力，则不能安装在这个窗口
                int id=windows[wins[i]].workshop;
                int mx=0,yid=-1;
                for(workshopArae y:workshop[id])
                if(supportEnergy[instruments[x].type][y.energy]&&instruments[x].lim[y.energy]<=rlim[y.id])
                {
                    if(rlim[y.id]>mx)
                        mx=rlim[y.id],yid=y.id;
                }
                if(yid!=-1)
                {
                    rlim[id]-=instruments[x].lim[workshopAraes[yid].energy];
                    winId[x]=i;
                    install=true;
                }
                if(install) break;
            }
            if(!install) feedback("Install Fail getR "+to_string(x)+" "+to_string(mn));
        }
        return winId;
    };
    vector<int>winId_r;
    getwinID_r(winId_r);
    vector<int>time_line(instrumentNum);
    int tim=0;
    auto getVeryGoodst=[&](InstallState &st)
    {
        st.init();
        for(int x:order)
        {
            ++tim;
            int mx=0;
            for(auto e:G.re[x])
            {
                int win_id=st.winId[e.from];
                mx=max(mx,win_id+(e.type==0));
            }
            ll CostInc=1e18;
            int win_id=-1,wsa_id=-1;
            for(int i=mx;i<=winId_r[x];++i)
            {
                if(time_line[wins[i]]==tim) continue;
                time_line[wins[i]]=tim;
                if(vis[x]&&!windows[wins[i]].ability[instruments[x].type]) continue;
                //如果窗口wins[i]对主流水线上的仪器x的类型没有预加工能力，则不能安装在这个窗口
                int id=windows[wins[i]].workshop;
                int mx=0,yid=-1;
                for(workshopArae y:workshop[id])
                if(supportEnergy[instruments[x].type][y.energy]&&instruments[x].lim[y.energy]<=st.rlim[y.id])
                {
                    if(st.rlim[y.id]>mx)
                        mx=st.rlim[y.id],yid=y.id;
                }
                if(yid!=-1)
                {
                    win_id=i;
                    wsa_id=yid;
                    break;
                }
            }
            if(win_id==-1)
            { 
                feedback("Install Fail getGood "+to_string(x)+" "+to_string(mx));
                st.Cost=inf;
                break;
            }
            st.upd(x,win_id,wsa_id);
        }
    };
    auto getGoodst=[&](InstallState &st)
    {
        st.init();
        for(int x:order)
        {
            ++tim;
            int mx=0;
            for(auto e:G.re[x])
            {
                int win_id=st.winId[e.from];
                mx=max(mx,win_id+(e.type==0));
            }
            ll CostInc=1e18;
            int win_id=-1,wsa_id=-1;
            for(int i=mx;i<=winId_r[x];++i)
            {
                if(time_line[wins[i]]==tim) continue;
                time_line[wins[i]]=tim;
                if(vis[x]&&!windows[wins[i]].ability[instruments[x].type]) continue;
                //如果窗口wins[i]对主流水线上的仪器x的类型没有预加工能力，则不能安装在这个窗口
                int id=windows[wins[i]].workshop;
                for(workshopArae y:workshop[id])
                if(supportEnergy[instruments[x].type][y.energy]&&instruments[x].lim[y.energy]<=st.rlim[y.id])
                {
                    ll val=st.getCostIncrement(x,i,y.id);
                    // if(rddouble(0.58,1)>1.0*val/CostInc)//赛时参数
                    if(rddouble(0.05,1)>1.0*val/CostInc)
                    {
                        CostInc=val;
                        win_id=i;
                        wsa_id=y.id;
                    }
                }
            }
            if(win_id==-1)
            { 
                // feedback("Install Fail getGood "+to_string(x)+" "+to_string(mx));
                st.Cost=inf;
                break;
            }
            st.upd(x,win_id,wsa_id);
        }
    };
    State::wins=wins;
    State::markInstrment=vis;
    State::seq=seq;
    State::seqs=seqs;
    int64_t usetime=timeCounter.getTime();
    InstallState st;
    getVeryGoodst(st);
    st.remakeCost();
    // feedback("Good Cost: "+to_string(st.Cost));
    InstallState best_st=st;
    if(st.Cost<best_st.Cost) best_st=st; 
    usetime=timeCounter.getTime()-usetime;
    #ifdef timeMax
    while(true)
    #else
    int numb=50;
    while(numb--)
    #endif
    {
        getGoodst(st);
        st.remakeCost();
        if(st.Cost<best_st.Cost) best_st=st;
        if(timeCounter.getTime()+usetime>14300) break;
    }
    feedback("totCost: "+to_string(best_st.Cost));
    feedback("installCost: "+to_string(best_st.installCost));
    feedback("matchCost: "+to_string(best_st.matchCost));
    printf("%d\n",instrumentNum);
    for(int i=0;i<instrumentNum;++i)
        printf(i==instrumentNum-1?"%d\n":"%d ",best_st.workshopAraeId[i]);
    printf("%d\n",seqs.size());
    for(int id=0;id<lines.size();++id)
    {
        auto &mwin=best_st.mwins[id];
        printf("%d ",mwin.size());
        for(int i=0;i<mwin.size();++i)
            printf(i==mwin.size()-1?"%d\n":"%d ",mwin[i]);
    }
}
int main()
{
    #ifdef DEBUG
        freopen(inputDir.c_str(),"r",stdin);
    #endif
    auto readData=[&]()
    {
        // scanf("%d",&K);
        energyTime.resize(energyNum);
        for(int i=0;i<energyNum;++i) cin>>energyTime[i];
        cin>>N;
        workshop.resize(N);
        int R;cin>>R;
        for(int i=0;i<R;++i)
        {
            int x,y,lim;cin>>x>>y>>lim;
            workshop[x].push_back({i,y,x,lim});
            workshopAraes.push_back({i,y,x,lim});
        }
        cin>>L;
        cin>>backToZero;
        --backToZero;
        cin>>windowNum;
        windows.resize(windowNum);
        for(int i=0;i<windowNum;++i)
        {
            cin>>windows[i].backToSelf>>windows[i].workshop>>windows[i].costCoefficient;
            for(int j=0;j<instrumentTypeNum;++j)
                cin>>windows[i].ability[j];
        }
        cin>>instrumentNum;
        instruments.resize(instrumentNum);
        for(int i=0;i<instrumentNum;++i)
        {
            cin>>instruments[i].type;
            for(int j=0;j<energyNum;++j)
                cin>>instruments[i].cost[j];
            for(int j=0;j<energyNum;++j)
                cin>>instruments[i].lim[j];
        }
        G.init(instrumentNum);
        int edgeNum;
        cin>>edgeNum;
        for(int i=0;i<edgeNum;++i)
        {
            int z,x,y;cin>>z>>x>>y;
            G.addedge(x,y,z);
        }
        int T;cin>>T;
        lines.resize(T);
        for(int i=0;i<T;++i)
        {
            cin>>lines[i].K;
            cin>>edgeNum;
            lines[i].edges.resize(edgeNum);
            for(int j=0;j<edgeNum;++j)
                cin>>lines[i].edges[j];
        }
    };
    feedback("start Run!");
    readData();
    feedback("readData OK!");
    installationInstruments();
    feedback("installationInstruments OK!");
    feedback("Use Time: "+to_string(timeCounter.getTime())+"ms");
    return 0;
}