//welcome to https://hb2018wfy.github.io!
#include<cstdio> 
#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<ctime>
#include<queue>
#include<cmath>
using namespace std;
const int N=210,T=15000,TT=4*T;
int n,m;
int ID[2][N][N],lim;
char Map[N][N],base_Map[N][N];//all 0-index
struct node{ 
	int x,y,g,c,d,v;//燃油、弹药、防御、价值
}base_a[N*N],base_b[N*N],a[N*N],b[N*N];
const int dx[4]={-1,1,0,0},dy[4]={0,0,-1,1};
struct plane{
	int x,y,max_g,max_c,g,c,t,to,las;
	plane(){
		x=y=max_g=max_c=g=c=t=to=0;las=-1;
	}
}base_p[11],p[11];
struct command{
	int type,id,dir,count;
};
vector<command> ans[15100],max_ans[15100];
int cnt_a,cnt_b,cnt_p,max_val;
void input(){
	cin>>n>>m;
	//cerr<<n<<' '<<m<<'\n';
	for(int i=0;i<n;i++)cin>>base_Map[i];
	cin>>cnt_a;
	for(int i=0;i<cnt_a;i++)cin>>base_a[i].x>>base_a[i].y>>base_a[i].g>>base_a[i].c>>base_a[i].d>>base_a[i].v,ID[0][base_a[i].x][base_a[i].y]=i;
	cin>>cnt_b;
	for(int i=0;i<cnt_b;i++)cin>>base_b[i].x>>base_b[i].y>>base_b[i].g>>base_b[i].c>>base_b[i].d>>base_b[i].v,ID[1][base_b[i].x][base_b[i].y]=i,lim+=base_b[i].v;
	cin>>cnt_p;
	for(int i=0;i<cnt_p;i++)cin>>base_p[i].x>>base_p[i].y>>base_p[i].max_g>>base_p[i].max_c;

}

bool vis[11][N][N];
void cp(){
	memset(vis,0,sizeof(vis));
	memcpy(Map,base_Map,sizeof(Map));
	memcpy(a,base_a,sizeof(a));
	memcpy(b,base_b,sizeof(b));
	memcpy(p,base_p,sizeof(p));
}
void output(){
	for(int i=0;i<T;i++){
		for(int j=0;j<max_ans[i].size();j++){
			if(max_ans[i][j].type==0){
				cout<<"move"<<' '<<max_ans[i][j].id<<' '<<max_ans[i][j].dir<<'\n';
			}
			else if(max_ans[i][j].type==1){
				cout<<"attack"<<' '<<max_ans[i][j].id<<' '<<max_ans[i][j].dir<<' '<<max_ans[i][j].count<<'\n';
			}
			else if(max_ans[i][j].type==2){
				cout<<"fuel"<<' '<<max_ans[i][j].id<<' '<<max_ans[i][j].dir<<'\n';
			}
			else if(max_ans[i][j].type==3){
				cout<<"missile"<<' '<<max_ans[i][j].id<<' '<<max_ans[i][j].dir<<'\n';
			}
		}
		cout<<"OK"<<'\n';
		cout.flush();
	}
}
vector<int> v[11];
void rebuild(int u){
	v[u].resize(TT);
	for(int j=0;j<T;j++){
		v[u][j]=rand()%4;
		while(j&&(v[u][j]==v[u][j-1]||v[u][j]==(v[u][j-1]^1)))v[u][j]=rand()%4;
	}
}
int lm[N][2][2];
int dis(int x,int y,int xx,int yy){
	return abs(xx-x)+abs(y-yy);
}

int score[N],score_las[N],ck[N];
void init(){
	cp();
	memset(score,0,sizeof(score));
	for(int i=0;i<cnt_p;i++){
		lm[i][0][0]=lm[i][1][0]=0,lm[i][0][1]=n,lm[i][1][1]=m;
	}
	
	for(int i=0;i<cnt_p;i++)if(!ck[i])rebuild(i);	
}
bool inside(int i,int x,int y){return x>=lm[i][0][0]&&x<lm[i][0][1]&&y>=lm[i][1][0]&&y<lm[i][1][1];}
int min_dis[2][N][N],D[2][N][N];
void bfs(){
	queue<pair<int,int>> q;
	//for(int i=0;i<cnt_p;i++)p[i].to=0;
	memset(min_dis,127,sizeof(min_dis));
	memset(D,-1,sizeof(D));
	for(int i=0;i<cnt_a;i++){
		if(!a[i].g)continue;
		min_dis[0][a[i].x][a[i].y]=0;
		q.push(make_pair(a[i].x,a[i].y));
		for(int j=0;j<4;j++)if(inside(0,a[i].x+dx[j],a[i].y+dy[j])&&Map[a[i].x+dx[j]][a[i].y+dy[j]]!='#')D[0][a[i].x][a[i].y]=j;
	}
	while(!q.empty()){
		pair<int,int> u=q.front();q.pop();
		int x=u.first,y=u.second;
		for(int i=0;i<4;i++){
			int nx=x+dx[i],ny=y+dy[i];
			if(!inside(0,nx,ny)||Map[nx][ny]=='#')continue;
			if(min_dis[0][x][y]+1<min_dis[0][nx][ny]){
				min_dis[0][nx][ny]=min_dis[0][x][y]+1;
				D[0][nx][ny]=i^1;
				q.push(make_pair(nx,ny));
			}
		}
	}
	for(int i=0;i<cnt_a;i++){
		if(!a[i].c)continue;
		min_dis[1][a[i].x][a[i].y]=0;
		q.push(make_pair(a[i].x,a[i].y));
		for(int j=0;j<4;j++)if(inside(0,a[i].x+dx[j],a[i].y+dy[j])&&Map[a[i].x+dx[j]][a[i].y+dy[j]]!='#')D[1][a[i].x][a[i].y]=j;
	}
	while(!q.empty()){
		pair<int,int> u=q.front();q.pop();
		int x=u.first,y=u.second;
		for(int i=0;i<4;i++){
			int nx=x+dx[i],ny=y+dy[i];
			if(!inside(0,nx,ny)||Map[nx][ny]=='#')continue;
			if(min_dis[1][x][y]+1<min_dis[1][nx][ny]){
				min_dis[1][nx][ny]=min_dis[1][x][y]+1;
				D[1][nx][ny]=i^1;
				q.push(make_pair(nx,ny));
			}
		}
	}
}
int get_val(){
	int val=0;
	for(int i=0;i<T;i++)ans[i].clear();
	for(int t=0;t<T;t++){
		//cerr<<"check1"<<'\n';
		bfs();
		int cnt=0;
		
		for(int i=0;i<cnt_p;i++){
			vis[i][p[i].x][p[i].y]=1;
			if(Map[p[i].x][p[i].y]=='*'){
				int u=ID[0][p[i].x][p[i].y];
				int val_g=min(p[i].max_g-p[i].g,a[u].g);
				p[i].g+=val_g;
				a[u].g-=val_g;
				ans[t].push_back((command){2,i,val_g});

				int val_c=min(p[i].max_c-p[i].c,a[u].c);
				p[i].c+=val_c;
				a[u].c-=val_c;
				ans[t].push_back((command){3,i,val_c});
				
			}
			if(p[i].g<=0)continue;
			else cnt++;
			for(int j=0;j<4;j++){
					int nx=dx[j]+p[i].x,ny=dy[j]+p[i].y;
					if(!inside(i,nx,ny))continue;
					if(Map[nx][ny]=='#'){
						int u=ID[1][nx][ny];
						if(b[u].d<=p[i].c){
							ans[t].push_back((command){1,i,j,b[u].d});
							Map[nx][ny]='.';
							p[i].c-=b[u].d;b[u].d=0;
							val+=b[u].v;
							score[i]+=b[u].d;
						}
						else{
							ans[t].push_back((command){1,i,j,p[i].c});
							b[u].d-=p[i].c;p[i].c=0;
							score[i]+=p[i].c;
						}
					}	
				}
			
			//get g?
            if(p[i].g==p[i].max_g)p[i].to=0;
			if(!p[i].c)p[i].to=2;
			if(p[i].g<min(p[i].max_g/5,min_dis[0][p[i].x][p[i].y]+10))p[i].to=1;
			//cerr<<"check1.5"<<'\n';
			if(p[i].to&&D[p[i].to-1][p[i].x][p[i].y]!=-1){
				int u=D[p[i].to-1][p[i].x][p[i].y];
				//cerr<<u<<'\n';
				ans[t].push_back((command){0,i,u});
				p[i].x+=dx[u];p[i].y+=dy[u];
				p[i].las=u;
				p[i].g--;
				continue;
			}
			//------------------------
			//cerr<<"check2"<<'\n';
			while(p[i].t<TT&&((!inside(i,p[i].x+dx[v[i][p[i].t]],p[i].y+dy[v[i][p[i].t]]))||Map[p[i].x+dx[v[i][p[i].t]]][p[i].y+dy[v[i][p[i].t]]]=='#'))p[i].t++;//,cerr<<p[i].t<<'\n';
			
			if(p[i].t>=TT)continue;
			
			
			p[i].x+=dx[v[i][p[i].t]];p[i].y+=dy[v[i][p[i].t]];
				
			ans[t].push_back((command){0,i,v[i][p[i].t]});
			if(rand()%2==0&&vis[i][p[i].x+dx[v[i][p[i].t]]][p[i].y+dy[v[i][p[i].t]]])p[i].t++;
			p[i].g--;
		}
		if(!cnt)break;
	}
	
	return val;
}
double Rand() { return (double)rand() / RAND_MAX; }
void simulateAnneal() {
	while(clock()<120000&&max_val!=lim){
		double t=100000;
		while(t>0.001&&max_val!=lim&&clock()<120000){
			init();
			int val=get_val();
			if(val>max_val){
				max_val=val;
				swap(ans,max_ans);
				cerr<<max_val<<'\n';
			}
			cerr<<"get_val="<<val<<'\n';
			for(int i=0;i<cnt_p;i++){
				double delta=score[i]-score_las[i];
				score_las[i]=score[i];
				if (exp( delta / t) > Rand()) ck[i]=1;
				else ck[i]=0;
			}
			t*=0.95;
		}
		cerr<<"max_val="<<max_val<<'\n';
	}
	
}
int main(int argc,char *argv[3]){
	
	freopen(argv[1],"r",stdin);
	freopen(argv[2],"w",stdout);
	srand(time(0));
	input();
	simulateAnneal();
	output();
	return 0;
}
/*
110 ok
106710 ok
261533 ok
28499 ok
32360 23724
20782 ok
389832 381491
268109 266313
30168 ok
23936 8731
*/

