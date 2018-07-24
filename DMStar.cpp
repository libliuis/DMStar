#include<bits/stdc++.h>
using namespace std;
int maps[10][10];//map 0 for free 1 for obstacle
int dx[5]={0,1,-1,0,0};
int dy[5]={0,0,0,1,-1};
int n,m,na;
const int maxna=10;//maximal number of agents
const int MAXN=10;// maximal makespan and search level
const int MAXMAKESPAN=100000000;
struct searchstrcut
{
	int nodenumber;
    vector<vector<int> > maintainroad;
    int level;
    int daijia;
	int x,y;
	int stopped;
	int parentnodenumber;
	vector<int> childnumber;
};
vector<searchstrcut> vtmp[maxna];
class agent
{
public:
	int number;
	int st;
	int nodenumber;
    int sx,sy;
    int gx,gy;
    int ansnodenumber;
    int makespan;
    queue<searchstrcut> q;
    vector<searchstrcut> v;
	void change(int number=0,int sx=0,int sy=0,int gx=0,int gy=0,int st=0)
	{
        this->number=number;
        this->sx=sx;
        this->sy=sy;
        this->gx=gx;
        this->gy=gy;
        this->st=st;
        this->makespan=MAXMAKESPAN;
    };
}ag[maxna];
bool check(int x,int y)
{
	if(x>=0&&x<n&&y>=0&&y<m&&maps[x][y]==0)
		return true;
	else
		return false;
}
void dfs(int j,int k,int h,int s,vector<int>&roadtmp)
{
	if(s==na)//finish creating one qualified condition
	{
		int index=vtmp[j][k].nodenumber;
		ag[j].v[index].maintainroad.push_back(roadtmp);
		return;
	}
	if(s==j)
	{
		roadtmp.push_back(vtmp[j][k].nodenumber);
		dfs(j,k,h,s+1,roadtmp);
		return;
	}
	int index=vtmp[j][k].maintainroad[h][s];
	if(ag[s].v[index].stopped==1)
	{
		if(vtmp[j][k].x!=ag[s].v[index].x||vtmp[j][k].y!=ag[s].v[index].y)
		{
			int flag=0;
			for(int z=0;z<s;z++)
			{
				if(ag[z].v[roadtmp[z]].x==ag[s].v[index].x&&ag[z].v[roadtmp[z]].y==ag[s].v[index].y)
				{
					flag=1;
					break;
				}
			}
			if(flag)return;
		}
		else return;
		roadtmp.push_back(index);
		dfs(j,k,h,s+1,roadtmp);
		roadtmp.pop_back();
		return;
	}
	for(int i=0;i<int(ag[s].v[index].childnumber.size());i++)
	{
		int nb=ag[s].v[index].childnumber[i];
		//check whether the road is in collision
		if(vtmp[j][k].x!=ag[s].v[nb].x||vtmp[j][k].y!=ag[s].v[nb].y)
		{
			int flag=0;
			for(int z=0;z<s;z++)
			{
				if(ag[z].v[roadtmp[z]].x==ag[s].v[nb].x&&ag[z].v[roadtmp[z]].y==ag[s].v[nb].y)
				{
					flag=1;
					break;
				}
			}
			if(flag)continue;
		}
		else continue;
		roadtmp.push_back(nb);
		dfs(j,k,h,s+1,roadtmp);
		roadtmp.pop_back();
	}
	return;
}
void findsolution(int a,int nb)
{
	int roadnums=ag[a].v[nb].maintainroad.size();
	for(int i=0;i<roadnums;i++)
	{
		int flag=1;
		int tmpsolutiontime=0;
		for(int j=0;j<na;j++)
		{
			int index=ag[a].v[nb].maintainroad[i][j];
			if(ag[j].v[index].stopped==0)
			{
				flag=0;
				break;
			}
			else tmpsolutiontime=max(tmpsolutiontime,ag[j].v[index].daijia);
		}
		if(flag) // a feasible solution
		{
            if(ag[a].makespan>tmpsolutiontime)
            {
                for(int j=0;j<na;j++)
                {
//                    if(ag[j].makespan>tmpsolutiontime)
//                    {
                    ag[j].makespan=tmpsolutiontime;
                    ag[j].ansnodenumber=ag[a].v[nb].maintainroad[i][j];
//                    }
                }
            }
		}
	}
}
void backtracepath(int a,int index,stack<pair<int,int> > &optimalpath)
{
	pair<int,int>ptmp;
	ptmp.first=ag[a].v[index].x;
	ptmp.second=ag[a].v[index].y;
	optimalpath.push(ptmp);
	if(index==0)return;
	index=ag[a].v[index].parentnodenumber;
	backtracepath(a,index,optimalpath);
	return;
}
int main()
{
    freopen("in.txt","r",stdin);
	memset(maps,0,sizeof(maps));
	//cout<<"input height width number of agents:"<<endl;
	cin>>n>>m>>na;
	//cout<<"input map:"<<endl;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			cin>>maps[i][j];
		}
	}
	//cout<<"input sx sy gx gy:"<<endl;
	for(int i=0;i<na;i++)
	{
		int sx,sy,gx,gy;
		cin>>sx>>sy>>gx>>gy;
		ag[i].change(i,sx,sy,gx,gy);
		searchstrcut tmp;
		tmp.nodenumber=0;
		tmp.parentnodenumber=tmp.nodenumber;
		tmp.daijia=0;
		tmp.stopped=0;
		tmp.level=0;
		tmp.x=sx;
		tmp.y=sy;
		vector<int>vec;
		for(int j=0;j<na;j++)
		{
			vec.push_back(0);
		}
		tmp.maintainroad.push_back(vec);
		ag[i].q.push(tmp);
		ag[i].v.push_back(tmp);
	}
	//cout<<"start"<<endl;
	int flag=0;
    for(int i=0;i<MAXN;i++) //search level
    {
    	if(flag==na)break;
    	for(int j=0;j<na;j++)
    	{
    		if(ag[j].q.empty()&&ag[j].st==0)
    		{
    			flag++;
    			ag[j].st=1;
    			continue;
    		}
    		while(!ag[j].q.empty())
    		{
    			searchstrcut tmp=ag[j].q.front();
    			if(tmp.level!=i)break;
    			ag[j].q.pop();
    			if(tmp.maintainroad.size()==0)continue;
    			if(tmp.daijia>ag[j].makespan)continue;
    			//this node has no qualified expand
    			if(tmp.x==ag[j].gx&&tmp.y==ag[j].gy)
    			{
  					findsolution(j,tmp.nodenumber);
                    //check otheragentfinishment if not reput in queue else compute the makespan and record the nodenumber;
    			}
    			else //expand qualified node
    			{
    				for(int k=0;k<5;k++)
    				{
    					searchstrcut ntmp;
            			ntmp.x=tmp.x+dx[k],ntmp.y=tmp.y+dy[k];
            			if(!check(ntmp.x,ntmp.y))continue;
//            			cout<<i<<" "<<j<<" "<<k<<endl;
            			ntmp.parentnodenumber=tmp.nodenumber;
            			ntmp.level=tmp.level+1;
            			ntmp.nodenumber=++ag[j].nodenumber;
            			ntmp.stopped=0;
            			// if(k!=0) minimal path cost if unlined
            			ntmp.daijia=tmp.daijia+1;//minimal makespan
            			ag[j].v[tmp.nodenumber].childnumber.push_back(ntmp.nodenumber);
            			if(ntmp.x==ag[j].gx&&ntmp.y==ag[j].gy)
            			{
                            ntmp.stopped=1;
            			}
            			ag[j].v.push_back(ntmp);
            			ntmp.maintainroad=tmp.maintainroad;
//            			cout<<ntmp.maintainroad.size()<<endl;
            			vtmp[j].push_back(ntmp);
    				}
    			}
    		}
    	}
    	//update each node's maintainroad
    	for(int j=0;j<na;j++)
    	{
    		for(int k=0;k<int(vtmp[j].size());k++)
    		{
    			for(int h=0;h<int(vtmp[j][k].maintainroad.size());h++)
    			{
    				vector<int>roadtmp;
    				dfs(j,k,h,0,roadtmp);
    			}
    			ag[j].q.push(ag[j].v[vtmp[j][k].nodenumber]);
    		}
    	}
    	for(int j=0;j<na;j++)
    	{
    		vtmp[j].clear();
    	}
    }
    if(ag[0].makespan==100000000)cout<<"No Solution ERROR"<<endl;
    else
    {
	    cout<<ag[0].makespan<<endl;//minimal makespan
	    for(int i=0;i<na;i++)   //each agent's optimal aciton path
	    {
	    	cout<<"agent"<<i+1<<"'s optimal path:"<<endl;
	    	stack<pair<int,int> >optimalpath;
	    	backtracepath(i,ag[i].ansnodenumber,optimalpath);
	    	while(!optimalpath.empty())
	    	{
	    		cout<<"("<<optimalpath.top().first<<","<<optimalpath.top().second<<") ";
	    		optimalpath.pop();
	    	}
	    	cout<<endl;
	    }
	}
	return 0;
}
