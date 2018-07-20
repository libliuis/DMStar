#include<bits/stdc++.h>
using namespace std;
int m[10][10];//map 0 for free 1 for obstacle
int dx[5]={0,1,-1,0,0};
int dy[5]={0,0,0,1,-1};
int n,m,na;
const int maxna=10;
const int MAXN=1000;
int minmakespan=10000000;
int ansnodenumber[maxna]={-1};
struct searchstrcut
{
	int nodenumber;
    vector<vector<int>> maintainroad;
    int level;
    int daijia=0;
	int x,y;
	int parentnodenumber;
	vector<int> childnumber;
};
vector<searchstrcut> v[maxna];
vector<searchstrcut> vtmp[maxna];
class agent
{
public:
	int number;
	int st=0;
	int nodenumber;
    int sx,sy;
    int gx,gy;
    queue<searchstrcut> q;
    vector<searchstrcut> v;
	agent(int number,int sx,int sy,int gx,int gy);
    ~agent();
};
agent::agent(int number,int sx,int sy,int gx,int gy)
{
	this->number=number;
	this->sx=sx;
	this->sy=sy;
	this->gx=gx;
	this->gy=gy;
}
agent ag[na];
bool check(int x,int y)
{
	if(x>=0&&x<n&&y>=0&&y<m&&m[x][y]==0)
		return true;
	else 
		return false;
}
vector<int> dfs(vector<int> vv,int j,int k,int index)
{
	if(index==na)vtmp[j][k].maintainroad.push_back(vv);
	for(int i=0;i<vtmp[j][k].qualifiednodes[index].size();i++)
	{
		vv.push_back(vtmp[j][k].qualifiednodes[index][i]);
	}
}
int main()
{
	memset(m,0,sizeof(m));
	cin>>n>>m>>na;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			cin>>m[i][j];
		}
	}
	for(int i=0;i<na;i++)
	{
		int sx,sy,gx,gy;
		cin>>sx>>sy>>gx>>gy;  
		ag[i].agent(i,sx,sy,gx,gy);
		searchstrcut tmp=new searchstrcut;
		tmp.nodenumber=0;
		tmp.parentnodenumber=tmp.nodenumber;
		tmp.daijia=0;
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
    		}
    		while(!ag[j].q.empty())
    		{
    			searchstrcut tmp=ag[j].q.front();
    			if(tmp.level!=i)break;
    			ag[j].q.pop();
    			if(tmp.x==ag[j].gx&&tmp.y==ag[j].gy)
    			{
    				int fg=0;
    				for(int xunwen=0;xunwen<na;xunwen++)
    				{
    					for(int ls=0;ls<tmp.qualifiednodes[xunwen].size();ls++)
    					{
    						if(ag[xunwen].v[tmp.qualifiednodes[xunwen][ls]].x==ag[xunwen].sx&&ag[xunwen].v[tmp.qualifiednodes[xunwen][ls]].y==ag[xunwen].sy)
    						{
    							continue;
    						}
    						else 
    						{
    							fg=1;
    							break;
    						}
    					}
						if(fg)break;
						else
						{

						}
    				}
    				searchstruct ntmp=new searchstruct;
            		ntmp.x=tmp.x,ntmp.y=tmp.y;
            		ntmp.parentnodenumber=tmp.parentnodenumber;
            		ntmp.level=tmp.level;
            		ntmp.nodenumber=tmp.nodenumber;
            		ntmp.daijia=tmp.daijia;
            		vtmp[j].push_back(ntmp);
    			}
    			else
    			{
    				for(int k=0;k<5;k++)
    				{
    					searchstruct ntmp=new searchstruct;
            			ntmp.x=tmp.x+dx[k],ntmp.y=tmp.y+dy[k];
            			if(!check(ntmp.x,ntmp.y))continue;
            			ntmp.parentnodenumber=tmp.nodenumber;
            			ntmp.level=tmp.level+1;
            			ntmp.nodenumber=++ag[j].nodenumber;
            			// if(k!=0) minimal path cost if unlined
            			ntmp.daijia=tmp.daijia+1;//minimal makespan
            			tmp.childnumber.push_back(ntmp.nodenumber);
            			vtmp[j].push_back(ntmp);
            			ag[j].v.push_back(ntmp);
    				}
    			}
    		}
    	}
    	for(int j=0;j<na;j++)
    	{
    		for(int k=0;k<vtmp[j].size();k++)
    		{
    			for(int h=0;h<vtmp[j][k].maintainroad.size();h++)
    			{
    				for(int l=0;l<na;l++)
    				{
    					if(l!=j)
    					{
    						int index=vtmp[j][k].maintainroad[h][l];
    						if(ag[l].v[index].level==vtmp[j][k].level)continue;
    						for(int z=0;z<ag[l].v[index].childnumber.size();z++)
    						{
    							searchstruct tmpl=ag[l].v[ag[l].v[index].childnumber[z]];
    						}
    					}
    				}
    			}
    		}
    		// for(int k=0;k<vtmp[j].size();k++)
    		// {
    		// 	for(int h=0;h<na;h++)
    		// 	{
    		// 		if(j!=h)
    		// 		{
    		// 			for(int l=0;l<vtmp[h].size();l++)
    		// 			{
    		// 				for(int hhhh=0;hhhh<ag[j].v[vtmp[j][k].parentnodenumber].qualifiednodes[h].size();hhhh++)
    		// 				{
    		// 					if(ag[j].v[vtmp[j][k].parentnodenumber].qualifiednodes[h][hhhh]==vtmp[h][l].parentnodenumber)
    		// 					{
    		// 						if(vtmp[j][k].x!=vtmp[h][l].x||vtmp[j][k].y!=vtmp[h][l].y)
    		// 						{

    		// 							vtmp[j][k].qualifiednodes[h].push_back(vtmp[h][l].nodenumber);
    		// 						}
    		// 					}
    		// 				}
    		// 			}
    		// 		}
    		// 	}
    		// 	vtmp[j][k].qualifiednodes[j].push_back(vtmp[j][k].nodenumber);
    		// 	vector<int> vv;
    		// 	dfs(vv,j,k,0);
    		// 	ag[j].q.push(vtmp[j][k]);
    		// }
    	}
    	for(int j=0;j<na;j++)
    	{
    		vtmp[j].clear();
    	}
    }
	return 0;
}
