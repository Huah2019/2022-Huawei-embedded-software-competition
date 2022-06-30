# 2022华为嵌入式软件大赛解决方案
## 表现

中部赛区初赛第一，中部赛区复赛第一，总决赛冠军

## 传送带

传送带模型：有$W$个窗口，窗口编号为$0,1,...,W-1$，第$i(0\le i<W-1)$个窗口到第$i+1$个窗口有一条有向边。第$t(0\le t<W )$个窗口到第$0$个窗口有一条有向边，这条边最多只能经过$L$次，此外还存在$k$个窗口$w_0,w_1,...,w_{k-1}(t<w_0<w_1<...<w_{k-1}<W)$满足窗口$w_i$到窗口$w_i$有一条可以最多经过$L$次的有向边。

考虑把传送带转换成有序的窗口序列，初始时$u=0$，窗口序列$wins$为空，接下来执行下述操作：

1. 将窗口$x$放入序列$wins$的最后。
2. 如果存在$y\le x$满足$x$到$y$有一条边且有向边$x\to y$的经过次数$<L$，则让$x=y$，边$x\to y$的经过次数加$1$，并转到$1$，否则转到$3$
3. 如果$x<W-1$，则让$x=x+1$，并转到$1$，否则退出操作过程。

过程结束后得到一个有序的窗口序列$wins$。

随后一条流水线的窗口匹配方案（假设窗口序列为$d_0,d_1,...,d_k$）在传送带上可行的充要条件为：

存在$0\le i_0\le i_1\le ...\le i_k<wins.size()$满足$wins_{i_0}=d_0,wins_{i_1}=d_1,...,wins_{i_k}=d_k$，特别地，如果流水线相应的第$j(0\le j<k)$个仪器和第$j+1$个仪器不是协同关系，还需要满足$i_j<i_{j+1}$。

## 流水图

流水图模型：有$D$个仪器构成一张有向无环图，仪器编号为$0,1,...,D-1$，图有$E$条边，第$i(0\le i<E)$条边有三个参数$z_i\ x_i\ y_i$，表示$x_i$到$y_i$有一条类型为$z_i$的有向边，$z_i=1$表示边为协同边，$z_i=0$表示边不是协同边。对于$j=0,1,...,D-1$，需要给仪器$j$分配一个整数$winId_j$，表示仪器$j$必须安装在窗口$wins_{winId_j}$所对应的车间中。分配的整数需要满足$0\le winId_j<wins.size()$且对于$0\le i<E$必须满足$winId_{x_i}\le winId_{y_i}+[z_i\neq 0]$。

考虑生成有向无环图的拓扑序，如果$x_i$到$y_i$有一条有向边，则拓扑序序列中$x_i$要排在$y_i$的前面。

按照拓扑序从小到大贪心枚举仪器，每次给仪器分配符合条件的最小整数，对于每个仪器$i=0,1,...,D-1$得到$l_i$。

按照拓扑序从大到小贪心枚举仪器，每次给仪器分配符合条件的最大整数，对于每个仪器$i=0,1,...,D-1$得到$r_i$。

## 仪器安装

### 安装方案一

按照拓扑序从小到大安装仪器，枚举仪器$x$时，可以根据之前安装的仪器的仪器得到一个参数$mx$，仪器$x$分配的整数必须满足$winId_x\ge mx$，此外，还需要满足$winId_x\le r_x$才能保证后续的仪器有合法的安装方案。每次给仪器$x$在$[mx,r_x]$区间选择一个花费增量最小的整数分配给它。

初赛得分：4127870865

缺陷：每次选择的都是局部最优解，无法考虑到全局范围内的更优解。

### 安装方案二

在方案一的基础上进行改进，先设$mnInc=inf$，枚举$i=mx,mx+1,...,r_x$，计算如果$x$分配整数$i$的代价$Inc$，随机生成一个$[lowbound,1]$内的实数$r$，如果$r\ge \frac{Inc}{mnInc}$，则$x$分配的整数设为$i$，且$mnInc=Inc$，其中$lowbound$是设定的一个大于$0$小于$1$的参数。

对该方法进行多轮迭代，取最优的解。

分析：当$Inc\le mnInc$，以一定的概率接受局部更优解，并且$Inc$越小接受的概率越大。这样这个函数有一定的概率拒绝局部最优解，从而有机会达到全局范围内的更优解。

初赛得分：3639574619

## 多条主流水线

### 方案一

跑安装方案二的时候同时计算多条流水线的花费增量。

复赛得分：41852505342

### 方案二

任取出一条流水线，跑**安装方案二**安装仪器，安装好仪器后，确定每个窗口的最大时间。接下来去匹配每条流水线：

对于一条有$k$个仪器的流水线，设$f_{i,j}$为流水线中第$i$个仪器匹配的窗口为$wins_j$的最小代价，用前缀和优化可以在$O(kLW)$的时间复杂度内求得安装仪器且确定窗口最大时间后的最优解。

决赛得分：41545479581

## 代码

```cpp
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
```

## 自造测试数据

```txt
841 1181 1717 825 530 
21
45
4 2 77
1 3 70
4 4 63
6 1 81
19 4 95
11 4 58
9 0 60
1 3 81
7 4 41
20 0 34
14 0 75
19 1 40
4 1 52
20 2 49
12 4 86
11 1 1
3 1 65
2 3 24
5 4 48
7 3 55
10 3 71
13 3 71
13 0 3
19 0 19
8 2 16
5 3 20
10 3 75
8 3 42
12 0 94
2 4 2
15 4 47
14 4 56
0 2 65
8 4 48
15 3 64
10 1 63
19 0 57
8 3 65
1 2 37
20 2 62
3 2 16
9 1 83
1 1 86
2 0 18
15 3 58
7 16 57
0 3 3807 1 0 1 
0 20 9451 0 1 0 
0 2 8501 1 1 0 
0 2 323 0 0 1 
0 20 9324 0 1 0 
0 17 8993 0 1 1 
0 13 8118 1 1 0 
0 20 6584 1 0 1 
0 10 981 1 0 0 
0 13 5623 0 0 1 
0 11 1301 0 1 1 
0 0 4161 0 0 1 
0 18 4324 1 0 0 
0 17 12 0 0 1 
0 20 6069 0 1 1 
0 2 3385 0 1 1 
0 7 8471 0 0 1 
0 7 6759 0 1 0 
0 16 7509 0 1 0 
0 11 2538 1 1 0 
0 5 248 1 0 0 
0 11 1841 1 1 0 
0 6 8822 1 0 0 
0 4 1671 0 1 0 
1 16 1544 1 0 1 
1 18 7404 1 0 0 
1 17 5120 0 0 1 
0 11 88 1 0 0 
1 18 6792 1 0 1 
0 15 7431 1 0 1 
0 20 1039 1 1 0 
0 7 8027 1 0 0 
1 6 8665 0 1 1 
0 5 8124 1 1 0 
0 5 6811 0 1 1 
0 5 2353 1 1 0 
0 1 6419 0 0 1 
0 11 4407 0 0 1 
1 15 4363 0 1 0 
1 12 4383 1 0 1 
0 16 3359 1 0 1 
0 6 114 0 1 1 
0 3 7473 1 0 1 
1 7 2285 0 0 1 
0 7 3060 1 0 1 
0 15 6001 0 0 1 
1 7 445 0 1 0 
0 5 4850 0 1 1 
0 5 1949 1 0 0 
0 5 6229 0 1 1 
0 5 2195 1 0 0 
1 7 6531 0 1 1 
1 7 2182 1 0 0 
0 12 2071 0 1 0 
0 10 3890 1 1 0 
0 3 10000 0 1 1 
0 11 7757 1 1 0 
167
1 92487915 82417311 6926898 63634973 80871445 3 3 5 1 4 
0 14698714 60158486 96442612 90593362 79017311 1 5 4 4 4 
1 6665213 50150477 64057616 13363835 54702791 4 4 3 2 5 
0 30378477 76590856 58899697 68128660 87247828 3 5 2 5 5 
2 79766170 49214500 61400842 90366930 82725267 3 2 1 3 2 
2 73407417 98049261 38131589 88709307 82616541 1 1 1 2 4 
2 61570333 94701198 70151684 39381281 71116617 1 3 3 3 5 
2 25313061 79510061 92806297 81514973 36252804 2 4 1 5 1 
0 45995677 70392230 17163833 30085386 76083122 5 5 4 5 5 
1 16061698 20532772 96955610 28026527 81221790 3 5 5 2 5 
2 5016606 47296825 89776310 85489690 16812967 5 5 2 5 4 
2 19601482 80853595 87488263 91125055 61147898 4 4 5 4 5 
0 8705682 73095042 40983975 21849383 63996487 5 5 2 2 2 
0 20587616 77982146 89801692 86915320 6558168 1 1 3 3 4 
1 56592334 7563354 89510191 56470246 30078176 3 1 1 1 4 
2 13369864 170644 17690105 32380279 37896277 1 2 5 3 4 
1 76030917 28293091 6019860 42758647 39238991 2 2 3 1 5 
1 20705388 93767057 2726758 50393143 97733948 2 2 1 3 4 
2 58050711 62212423 43967053 32879186 20269437 4 1 5 1 4 
2 4387308 1922291 190976 69264240 25153546 4 4 1 3 4 
0 61561063 80912657 42049964 73045652 16717403 3 2 5 2 4 
2 67054090 60573863 74702526 27626775 89136742 1 3 5 2 3 
0 10966110 40962422 21002488 23631710 29879767 2 3 3 1 5 
2 37176611 56478908 59503035 96522029 43696917 3 5 1 3 1 
0 2817559 82121259 34809365 53213265 37675795 4 3 4 3 1 
2 83718164 94480230 92776410 92243318 29605053 4 1 2 3 1 
2 66278102 21843592 91900285 66804817 68610464 1 4 5 4 4 
1 65304378 4631208 26571845 63913220 39194431 2 2 1 2 4 
1 27897690 89888802 24441122 10601091 26566070 3 4 5 4 2 
0 92950343 97038759 97306346 28689939 24305257 5 4 3 5 3 
0 8985442 71658353 74848050 95977507 85267413 2 3 2 5 2 
1 20509343 62383892 66121916 34320948 73801446 1 1 4 5 1 
0 91772789 25336877 6842764 81771442 51716731 5 3 2 4 5 
0 60441251 90263012 59432079 6097060 45404075 3 2 2 5 5 
0 58114729 90224178 44102071 80087395 98654320 2 2 5 2 2 
2 78269976 7526287 40117247 50286191 8606385 4 1 5 3 5 
2 47239314 14374529 58444545 50298075 69982282 2 5 1 4 4 
2 74639035 42105634 10443090 29083645 43217959 2 4 3 5 1 
0 99082086 22307034 5957138 2609236 72519576 3 4 2 2 5 
1 7809965 72523590 66786051 42678629 12589009 5 1 3 3 2 
2 22733093 65423429 55095988 38519977 55919163 2 4 5 3 2 
1 58102994 32160801 25425430 98668700 88928371 2 1 2 3 5 
1 89101412 42723899 5781589 8485033 94573809 1 1 1 1 1 
0 84520167 11114294 42729921 46912974 12366118 5 5 1 5 5 
2 66253650 53509480 31891068 16542001 5326220 3 2 3 5 3 
1 99093770 86204034 99412722 9067336 35870952 3 2 2 1 5 
0 96872617 57628704 28358511 96793577 18060212 2 2 2 3 4 
2 36512914 52589028 29018150 61538271 50530662 3 1 2 4 5 
0 68791531 43196159 85876063 69877369 39548743 3 5 3 5 4 
2 98764459 51109230 27349098 96298633 17573623 4 1 3 1 4 
0 3377660 60007085 76825735 60030775 7416053 2 3 1 2 2 
0 49228920 52307097 3177608 25970408 68926044 4 1 5 5 4 
2 71813194 13022248 47940390 87815474 58259865 1 4 3 5 5 
0 25074621 44942472 1929414 59629310 60005888 2 3 3 4 1 
0 83515254 66393681 69178642 17393992 20696670 3 2 5 2 4 
1 64550830 7001593 26249565 85509673 57545705 4 4 4 1 4 
0 3971873 48678619 58750014 22755959 90196102 1 2 4 2 4 
2 218142 39486389 58999114 15201656 30087477 1 2 3 3 4 
2 37342527 93170807 78056281 64311395 18865969 4 3 3 5 4 
2 81890506 35507042 84339071 70813999 33018861 5 1 4 5 4 
1 70303404 45682352 61046169 34878030 27267453 4 4 3 5 4 
1 39633342 83288924 66658561 34496681 67868694 2 4 2 5 1 
2 82548448 74572920 38687318 82244072 49154550 3 5 2 4 1 
0 58617074 9464277 68652118 83382086 70136738 1 1 4 5 3 
1 35119963 66895908 14196193 79379391 42420980 1 1 2 1 1 
0 82254561 811658 62377272 12181802 86069858 3 1 1 4 3 
1 41173811 19958667 71575269 66021936 12238068 2 3 2 4 2 
0 2952213 4824707 52104206 76466048 73324564 2 2 4 5 1 
1 91497439 80203213 27458900 17091703 76227902 5 2 5 4 2 
0 55088148 96997826 73517883 75077429 66208622 4 2 5 4 5 
0 46981992 36761862 79359461 12767615 3572302 4 5 5 1 5 
2 9364020 86102331 81285095 46418768 17433541 2 3 3 4 4 
0 71474333 3193839 71750197 60698772 82673528 3 2 4 1 4 
1 34013462 96722467 58365145 12585910 46035988 3 3 5 2 2 
2 50525794 54095625 90363147 97251225 43186923 4 1 4 1 5 
0 11294314 79856565 88934139 11503431 23829874 5 5 2 5 4 
1 35140329 35221625 36831690 37991696 2075881 1 3 5 1 3 
2 85060299 43446407 28940115 61055743 10997906 2 4 4 5 3 
2 98716936 96928505 32734829 20133069 61121147 3 2 5 2 3 
0 24168524 44944638 91607959 52513225 52891423 3 3 4 5 3 
0 38370114 2912557 30805634 5881402 20691332 2 4 2 2 3 
2 45057929 87671098 50737915 38771902 47686300 2 1 3 2 5 
2 16323250 39299870 3238596 1822463 44577374 1 5 4 5 4 
1 94163389 34273686 46489654 91428808 53320177 2 3 1 4 5 
0 42360621 88546050 66753140 22056544 18231104 1 5 2 3 3 
2 82421803 4893199 78023514 49657946 85557731 1 5 1 1 4 
2 76084835 5300588 19745215 66953314 72788249 5 2 3 2 1 
2 71928355 75447928 31566896 13665878 43087886 5 1 3 2 1 
1 39050221 61934219 76583144 17089758 31996236 1 3 3 2 5 
2 76738490 82742301 36271392 63002201 12779413 3 2 1 3 5 
2 77770298 61527937 87382894 37786172 96069841 3 1 2 1 5 
1 87972215 65706967 29388960 77220108 57381945 1 2 2 5 2 
2 15848096 65953969 81953054 74536405 9057549 5 2 1 5 4 
0 10916665 23361511 44718617 68473639 12688580 5 1 4 3 5 
0 94092555 70003110 11987851 24868425 28322642 2 2 4 2 3 
0 89568946 60168451 76219901 13022567 58519123 3 3 3 5 4 
2 27979973 54329165 29095291 6318739 50926682 3 2 5 2 5 
2 95595954 30794574 60392846 57579020 55655628 5 3 4 5 1 
2 72688342 44538184 15529327 20757190 61025471 3 2 5 5 5 
2 95419781 93579485 4035008 64342460 7433410 5 3 3 3 2 
0 4700477 5227500 67255010 90161711 63942123 5 3 4 4 2 
1 38305932 83704420 95254232 94010936 90705727 4 5 3 1 2 
1 46922940 38476666 94417746 66191933 75959690 4 4 5 2 4 
1 37541999 15530265 25085877 77707345 20715433 1 1 1 5 1 
0 12573015 99989789 12400305 16164263 4510127 1 5 2 2 4 
1 62047631 35983941 92769215 28304676 28989302 3 2 1 2 2 
1 74253373 8000424 74900941 15365134 87408981 5 1 5 2 5 
2 86837952 28438524 50823220 19937570 73319315 5 4 3 4 1 
2 63294051 26687206 76544794 55118052 20521129 1 2 1 1 2 
0 16604892 22552423 57295809 98573844 4190853 1 1 4 1 1 
1 49685559 15812012 42283972 70187424 10493904 3 2 2 2 3 
1 35435385 75162307 80990576 11564550 3663462 5 2 4 5 1 
1 19941827 75597581 80502449 32045916 51046292 1 2 4 3 3 
1 79915697 92577894 40327150 55913399 24874310 1 3 4 2 1 
2 13638732 43615416 20600280 62824203 66978300 4 3 4 3 2 
0 3913975 11329753 80979749 16189820 35302396 5 4 2 2 3 
1 65743435 67403406 9723763 68428126 72590648 2 1 3 1 4 
1 82422078 38641715 64515114 8519563 48694276 1 3 5 2 3 
1 95768335 38921043 53591759 46310117 83362044 5 1 2 4 1 
0 54592340 36011427 84986124 82120075 61844874 1 5 2 2 4 
0 87291790 2291746 500906 28383622 36237339 2 1 2 2 2 
0 74797203 67871823 85978051 64609944 69991541 4 2 1 2 5 
2 96718727 77652293 40831803 2792806 53143919 3 5 1 3 2 
0 58483782 4245124 10008143 51937453 37208707 5 2 5 2 4 
0 4815696 42780701 10536288 91310232 65031961 2 2 3 4 3 
2 96629898 17317507 87320233 63785888 3765867 5 5 3 1 1 
1 83110885 39086757 72943699 42096584 60373816 1 1 1 1 2 
2 12927740 86240808 5700956 91200754 56828347 3 4 1 2 3 
2 51231832 77309379 24566942 63214794 32106976 5 2 3 1 3 
1 44165326 36001800 17559825 13907096 90310876 1 4 1 5 4 
0 18505877 45378168 52494422 17419443 72730947 3 2 1 4 4 
1 70132350 30648112 39546071 65277983 8396592 5 1 4 2 4 
1 22996546 44305079 30515225 95576359 55610291 4 3 5 1 5 
2 55066240 84986208 52672389 45273075 2764091 5 2 5 3 3 
2 32879578 74283446 31061541 19384219 47346291 1 2 1 3 5 
0 5887970 56510067 11790312 72188797 48776363 3 4 5 1 5 
1 13711483 77936796 32858458 18084291 77606853 4 4 3 2 1 
0 4567792 26362102 45688219 39014660 22958965 5 5 2 5 4 
0 83726362 44416521 58506208 24488648 71598819 1 4 4 5 4 
1 47320704 41043306 25979313 56498973 54988415 4 2 1 1 3 
1 75967282 43016138 43251632 69173714 86050995 2 1 2 5 3 
2 69078526 72749658 64556296 99952769 77654076 2 5 1 2 3 
0 38223460 83404639 14558597 95557858 64895682 2 1 4 4 4 
0 10728501 16927131 1110781 67942258 50927502 4 4 1 3 5 
1 71441445 67633700 46607782 14663863 9515103 2 4 1 2 1 
1 98022672 97153535 17891215 84684801 81310439 3 3 3 5 4 
0 65009974 51924081 19624475 50094568 38950178 2 3 1 2 1 
1 63798742 94047083 12143597 24565338 53857344 1 4 3 5 1 
0 16234974 91393263 83523830 53229040 44330445 4 4 2 5 1 
0 16238865 97180689 12675706 77671349 38723540 1 1 3 4 1 
2 5305969 78843199 30139560 780457 10317848 2 2 4 5 4 
2 25100147 76456495 58382861 71335348 75575755 3 4 3 3 1 
0 92532756 13061241 58339142 81748110 71490155 1 4 3 2 3 
0 59182205 63950839 80797543 96110960 9112908 5 2 3 3 2 
0 7013066 22898203 59032717 57121905 2585914 3 4 4 2 5 
2 27409203 61152235 87192532 48422744 12979729 4 5 5 4 2 
2 11279504 11937585 21023636 56264078 67776878 4 5 2 3 4 
2 83714940 9641464 81392560 67718204 89756882 4 4 2 4 4 
0 40290816 5217608 59781028 85505399 94157609 5 4 5 2 1 
2 54393111 88813444 65500514 74740084 85574289 1 1 3 4 3 
0 33303503 5815908 63730588 27037345 62306302 2 1 3 1 1 
2 58915449 51761313 18069741 84919549 12354847 5 3 3 1 5 
1 45309396 3276776 94135963 45778715 81898457 2 5 3 4 3 
0 21673605 70551744 67659400 52835441 46339271 5 3 5 2 2 
2 52782260 78986242 29251325 51228779 77339637 3 4 3 4 5 
2 47710012 1747070 4197056 9301119 61204584 1 5 1 3 2 
0 7787482 62070806 13619758 55627897 25406909 4 2 1 4 3 
298
0 129 130
0 162 129
1 98 129
1 96 130
1 136 96
0 122 162
0 152 136
0 86 96
0 102 122
0 154 136
0 124 122
1 32 154
1 40 98
0 71 129
1 150 136
0 91 124
0 108 152
0 94 108
0 17 122
0 113 162
1 47 94
1 64 96
0 66 98
0 65 86
0 7 47
0 131 154
0 76 113
1 138 91
1 99 154
0 77 124
0 81 99
0 54 64
1 155 17
0 56 17
0 39 155
0 25 17
1 3 155
1 151 39
0 101 47
0 61 99
1 68 81
0 159 65
0 112 25
0 24 131
0 160 131
1 97 24
0 19 81
0 104 77
0 59 112
0 75 19
1 33 39
0 21 68
0 145 151
0 78 33
0 15 151
0 120 59
1 140 151
1 36 104
1 44 101
0 83 145
1 38 83
0 46 140
0 103 104
0 84 36
1 95 78
1 57 46
0 22 145
1 37 44
0 23 120
0 70 84
0 149 145
0 163 44
0 148 95
0 121 103
1 110 103
1 161 70
0 164 46
0 100 149
0 42 46
1 30 57
0 52 84
0 72 149
0 53 110
0 67 53
0 29 23
1 132 70
0 139 52
0 144 132
0 146 132
0 45 139
0 26 29
0 93 139
0 147 144
0 153 72
0 80 153
0 116 53
0 114 93
1 49 52
0 4 114
1 88 72
0 16 132
0 90 49
0 127 147
0 20 127
0 143 132
1 85 93
0 55 114
1 41 153
1 123 55
0 157 49
0 10 85
0 13 147
0 105 116
1 63 143
0 58 63
0 0 88
0 60 157
0 109 60
1 34 105
0 50 157
0 141 55
0 115 90
1 156 109
1 134 41
0 28 141
0 9 55
1 118 109
0 48 10
0 1 115
0 12 141
0 6 115
0 142 34
0 119 1
0 165 0
0 128 58
0 27 128
0 166 12
0 5 156
0 43 119
0 137 142
0 73 12
1 106 156
0 8 128
1 74 106
0 125 142
1 69 27
0 111 73
0 79 8
1 89 166
0 51 89
0 87 43
0 82 119
0 135 166
1 18 87
0 158 166
0 117 89
0 92 111
1 35 5
0 14 73
0 107 79
0 11 158
0 126 158
0 31 135
1 2 35
0 62 2
0 133 35
0 107 146
1 110 113
0 42 112
0 111 165
1 92 71
0 49 113
0 139 70
0 14 92
1 25 76
0 153 164
0 135 146
1 1 85
0 20 138
0 58 16
0 60 66
0 142 56
0 11 119
0 2 97
1 43 156
1 115 46
0 85 159
0 34 33
0 40 162
0 20 94
0 68 61
0 166 3
1 157 143
0 12 42
0 117 25
0 93 96
0 114 129
0 126 142
1 156 20
0 166 77
0 146 162
0 70 39
0 22 99
1 146 44
0 82 130
0 60 140
0 88 147
0 70 47
0 58 77
0 127 114
1 80 146
1 45 65
0 1 108
0 111 28
0 126 65
0 99 7
0 35 33
0 2 135
0 110 65
0 158 138
0 158 165
0 48 23
0 94 136
0 139 59
0 51 49
0 14 68
0 90 146
0 141 53
0 111 97
0 23 61
1 12 60
1 81 136
0 27 83
0 159 17
0 93 152
0 125 99
0 51 20
1 159 138
0 79 108
0 123 162
0 106 94
0 43 136
0 119 129
0 58 53
1 127 95
0 35 112
0 13 57
0 117 6
0 92 79
0 116 91
1 131 71
0 117 147
0 5 146
0 46 7
0 11 46
0 160 98
1 70 33
1 161 77
1 79 49
0 2 51
1 73 134
0 8 153
0 74 20
1 47 124
0 80 67
0 73 137
1 82 44
0 14 16
0 126 14
0 103 17
1 14 55
0 146 113
0 61 152
0 33 136
0 15 78
0 20 3
0 75 151
0 139 130
0 164 108
0 125 24
0 141 33
0 151 122
0 60 45
0 38 7
0 90 149
0 31 148
0 166 53
0 118 146
0 164 86
0 2 103
0 93 38
0 105 19
0 23 131
0 164 97
0 117 20
0 68 17
0 6 109
0 0 149
7
250000
4 26 19 1 0 
350000
8 73 62 47 29 10 5 1 0 
380000
13 111 92 87 85 69 63 57 47 29 10 5 1 0 
440000
3 14 4 3 
90000
11 91 86 80 63 57 47 29 10 5 1 0 
170000
12 115 99 81 70 52 37 34 32 18 5 1 0 
470000
14 127 110 105 91 86 80 63 57 47 29 10 5 1 0 
```

