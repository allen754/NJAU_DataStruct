#include<iostream>
#include<malloc.h>
#define Max_size 31 //定义的地图最大边界 
#define Max 10010 //定义的最大数据值 
using namespace std;
/*----------------------------以下定义节点及路径类型---------------------------*/
typedef struct
{
    int i,j,d; //i,j为几行几列从左上角（0，0）开始,d的作用不同算法中不同 
}Man;
typedef struct
{
    Man data[Max_size];
    int length;                           
}PathType;
/*----------------------------以下定义邻接表类型---------------------------*/
typedef struct ANode                       
{
    int i,j,d;                              
    struct ANode *nextarc;                 
}ArcNode;
typedef struct Vnode                       
{
    ArcNode *firstarc;                   
}VNode;
typedef struct
{
    VNode adjlist[Max_size][Max_size];            
}Graph;                                 
/*----------------------------以下定义栈类型及其函数---------------------------*/
typedef struct{
	Man data[Max];
	int top;
}St;
void InitSt(St &s){
	s.top = -1;
}
void Stpush(St &s,Man value){
	if (s.top == Max - 1)cout << "false";
	else s.data[++s.top] = value;
}
void Stpop(St &s){
	if (s.top != -1 && s.top < Max)s.top--;
	else cout << "false";
}
bool isEmptyStack(St &s){
	if (s.top == -1)return true;
	return false;
}
/*----------------------------以下定义队列类型及其函数---------------------------*/
typedef struct
{
	Man data[Max];
	int front;
	int rear;
}SqQueue;
void initqueue(SqQueue &q){
	q.front = q.rear = 0;
}
bool push(SqQueue &q, Man value){
	if ((q.rear + 1)%Max == q.front)return false;
	q.data[q.rear++] = value;
	return true;
}
bool out(SqQueue &q, Man &e){
	if (q.front == q.rear)return false;
	q.front++;
	return true;
}
Man getfront(SqQueue q){
	return q.data[q.front];
}
bool isEqueue(SqQueue q){
	if (q.front == q.rear)return true;
	return false;
} 
/*----------------------------以下定义全局变量---------------------------*/                               
PathType minpath;    // 最短路径 
char flag1;          // 标志1 
bool flag2 = false;  // 标志2 
int n,m,start_r,start_l,end_r,end_l,count,ans;
int visited[Max_size][Max_size];     //记录是否经过 
int maze[Max_size][Max_size];       //迷宫地图
void create_maze(){        //创建地图 
	cout << "请输入迷宫的行数和列数：";
	cin >> n >> m;
	cout << "请输入迷宫（0代表通路，1代表障碍）：" << endl;
	for(int i = 0;i < n;i ++){
		for(int j = 0;j < m;j ++){
			cin >> maze[i][j];
		}
		getchar();
	}
}
void CreateAdj(Graph *&G){        //建立邻接表 
	create_maze();
    int i, j;
    int di;
    int i1, j1;
    ArcNode *p;
    G = (Graph *)malloc(sizeof(Graph));
    for(i = 0; i < n; i++)              
    {
        for(j = 0; j < m; j++)
        {
            G->adjlist[i][j].firstarc = NULL;
        }
    }
    for(i = 0; i < n; i++)      //地图转化为邻接表          
    {
        for(j = 0; j < m; j++)
        {
            if(maze[i][j] == 0)
            {
                di = 0;
                while(di < 4)
                {
                	if(di == 0 && i == 0 || di == 1 && j == m-1 || di == 2 && i == n-1 || di ==3 && j == 0){di++;continue;}
                    switch(di)
                    {
                    case 0:{i1 = i - 1;j1 = j;break;}
                    case 1:{i1 = i;j1 = j + 1;break;}
                    case 2:{i1 = i + 1;j1 = j;break;}
                    case 3:{i1 = i;j1 = j - 1;break;}
                    }
                    if(maze[i1][j1] == 0)                         
                    {
                        p = (ArcNode *)malloc(sizeof(ArcNode)); 
                        p->i = i1;
                        p->j = j1;
                        p->d = di;
                        p->nextarc = G->adjlist[i][j].firstarc; 
                        G->adjlist[i][j].firstarc = p;
                    }
                    di++;
                }
            }
        }
    }
}
void DisplayAdj(Graph *G)  // 展示邻接表 
{
    int i, j;
    ArcNode *p;
    cout << "构建的无向图邻接表如下:" << endl; 
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            p = G->adjlist[i][j].firstarc;
            if(p == NULL)continue;
			printf("[%d,%d]->", i, j);
            while(p != NULL)
            {
            	if(p->nextarc == NULL)printf("(%d,%d)", p->i, p->j);
                else printf("(%d,%d)->", p->i, p->j);
                p = p->nextarc;
            }
            cout << endl;
        }
    }
}
void non_recursive_stack(int start_r,int start_l,int end_r,int end_l) //非递归（栈）解法 
{
	int i,j,k,d,flag;
	St st,minpath;
	Man e;
	InitSt(minpath);
	minpath.top = Max;
	InitSt(st);
	e.i=start_r;
	e.j=start_l;
	e.d=-1; 
	Stpush(st,e); //入栈 
	maze[start_r][start_l]=-1;
	count = 0;
	while(!isEmptyStack(st))
	{
		i=st.data[st.top].i;
		j=st.data[st.top].j;
		d=st.data[st.top].d; //记录上一步的方向 
		if(i==end_r&&j==end_l) //找到终点 
		{
			if(!flag2){
				flag2 = true;
				cout << "用非递归（栈）求解的所有的迷宫路径:" << endl;
			}
			cout<< "第" << ++count << "条迷宫通路如下(前两位是坐标，第三位d为0向上，为1向右，为2向下，为3向左)：\n";
			for(k=0;k<=st.top;k++) //从栈底遍历到栈顶即是通路 
			{
				if(k == st.top)cout<<"("<<st.data[k].i<<","<<st.data[k].j<<","<<st.data[k].d<<")";
				else cout<<"("<<st.data[k].i<<","<<st.data[k].j<<","<<st.data[k].d-1<<")->";
				if((k+1)%5==0){cout<<"\n";}
			}
			cout<<"\n";
			if(st.top < minpath.top){
				for(k=0;k<=st.top;k++)
				{
					minpath.data[k] = st.data[k];
				}
				minpath.top = st.top;	
				ans = count;
			}
			maze[st.data[st.top].i][st.data[st.top].j]=0;
			Stpop(st);
			continue;
		}
		flag=0;
		while(d<4&&flag==0) // 遍历四个方向以及边界判断 
		{
			if(d == -1)d++;
			if(d == 0){if(i == 0){d++;continue;}i=st.data[st.top].i-1;j=st.data[st.top].j;}
			if(d == 1){if(j == m-1){d++;continue;}i=st.data[st.top].i;j=st.data[st.top].j+1;}
			if(d == 2){if(i == n-1){d++;continue;}i=st.data[st.top].i+1;j=st.data[st.top].j;}
			if(d == 3){if(j == 0){d++;continue;}i=st.data[st.top].i;j=st.data[st.top].j-1;}
			if(maze[i][j]==0 && i < n && j < m){flag=1;}
			d++;
		}
		if(flag==1) //找到可走的方向入栈 
		{
			st.data[st.top].d=d;
			e.i=i;
			e.j=j;
			e.d=-1;
			Stpush(st,e);
			maze[i][j]=-1;
		}
		else //没有可走的方向出栈 
		{
			maze[st.data[st.top].i][st.data[st.top].j]=0;
			Stpop(st);
		}
	 }
 	if(!flag2){
 		 cout << "没有通路" << endl;
	     return; 
	 } 
 	cout << "最短路径为第" << ans << "条通路，长度为："<< minpath.top << ",路径为：" << endl;
 	for(k=0;k<=minpath.top;k++){
		if(k == minpath.top)cout<<"("<<minpath.data[k].i<<","<<minpath.data[k].j<<","<<-1<<")";
		else cout<<"("<<minpath.data[k].i<<","<<minpath.data[k].j<<","<<minpath.data[k].d-1<<")->";
		if((k+1)%5==0){cout<<"\n";}
 	} 
    cout<<"\n";
 	return;
}
void non_recursive_queue(int xi,int yi,int xe,int ye,PathType path) { // 非递归（队列）解法 
	int d;
	Man queue[Max];
	int front = -1;
	int rear = front;
	rear++;
	queue[rear].i =xi;
	queue[rear].j =yi;
	queue[rear].d =-1;
	visited[xi][yi] = 1;
	while(front != rear) {
		int i,j;
		Man elem;
		front++; //出队 （虽然是出队，但该节点仍在数组中，这样的好处是方便找到路径） 
		elem = queue[front];
		if(elem.i == xe&&elem.j == ye) { //找到终点 
			d = 0;
			int i = front;
			flag2 = true;
			while(queue[i].d != -1) {
				path.data[d++]=queue[i];
				i = queue[i].d ;
			}
			path.data[d] = queue[i];
			cout << "用队列求解的迷宫的最短路径为(前两位是坐标，第三位d为0向上，为1向右，为2向下，为3向左)：" << endl;
			for(int i = d; i >= 0; i --) {
				int di;
				if(i == 0)printf("(%d,%d,-1)",path.data[i].i,path.data[i].j);
				else{
					if(path.data[i].i - path.data[i-1].i == 1)di = 0;
					if(path.data[i].j - path.data[i-1].j == -1)di = 1;
					if(path.data[i].i - path.data[i-1].i == -1)di = 2;
					if(path.data[i].j - path.data[i-1].j == 1)di = 3;
					printf("(%d,%d,%d)->",path.data[i].i,path.data[i].j,di);
				}
			}
			cout << endl;
			return;
		}
		int d = -1;
		while(d<4) //找到相邻节点入队 
		{
			d++;
			if(d == 0){if(queue[front].i == 0)continue;i=queue[front].i-1;j=queue[front].j;}
			if(d == 1){if(queue[front].j == m-1)continue;i=queue[front].i;j=queue[front].j+1;}
			if(d == 2){if(queue[front].i == n-1)continue;i=queue[front].i+1;j=queue[front].j;}
			if(d == 3){if(queue[front].j == 0)continue;i=queue[front].i;j=queue[front].j-1;}
			if(maze[i][j]==0 && i < n && j < m){
			maze[i][j]==-1;
			rear++; //入队 
			queue[rear].i= i;
			queue[rear].j= j;
			queue[rear].d = front; //d记录的是该节点的父节点的下标（假定由父节点走到子节点） 
			}
		}
	}
}
void recursive(int xi,int yi,int di,int xe,int ye,PathType path) // 递归解法 
{
	int i,j;
	if(xi==xe&&yi==ye){ //找到终点 
		if(!flag2){
			flag2 = true;
			cout << "用递归求解的所有的迷宫路径:" << endl;
		}
		flag2 = true;
		path.data[path.length].i=xi; 
		path.data[path.length].j=yi;
		path.length++;
		if(path.length < minpath.length){
			for(int k=0;k<path.length;k++)
			{
				minpath.data[k] = path.data[k];
			}
			minpath.length = path.length;	
			ans = count+1;
		}
		cout<< "第" << ++count << "条迷宫通路如下(前两位是坐标，第三位d为0向上，为1向右，为2向下，为3向左)：\n";
		for(int k=0;k<path.length;k++){
			if(k == path.length-1)cout<<"("<<path.data[k].i<<","<<path.data[k].j<<","<<-1<<")";
			else cout<<"("<<path.data[k].i<<","<<path.data[k].j<<","<<path.data[k].d<<")->";
			if((k+1)%5==0){cout<<"\n";}
		}
		cout<<endl;
	}
	else{
		if(maze[xi][yi]==0){
			int di=0;
			while(di<4){  // 遍历四个方向以及边界判断 
				path.data[path.length].i=xi; 
				path.data[path.length].j=yi;
				path.data[path.length].d=di; //d记录走下一步的方向 
				path.length++;
				if(di == 0 && xi == 0 || di == 1 && yi == m-1 || di == 2 && xi == n-1 || di ==3 && yi == 0){path.length--;di++;continue;}
				switch(di){
					case 0:{i=xi-1,j=yi;break;}
					case 1:{i=xi,j=yi+1;break;}
					case 2:{i=xi+1,j=yi;break;}
					case 3:{i=xi,j=yi-1;break;}
				} 
				maze[xi][yi]=-1;	//赋值-1说明已走过 
				recursive(i,j,di,xe,ye,path);
				path.length--;		//回溯 
				maze[xi][yi]=0;	    
				di++;               //进行下一步操作 
			} 
		}
	}	
} 
void DFS(Graph *G, int xi, int yi,int d, int xe, int ye, PathType path)  //图论深度优先搜索 
{
    ArcNode *p;
    visited[xi][yi] = 1;                                     
    path.data[path.length].i = xi;                             
    path.data[path.length].j = yi;
	path.data[path.length].d = d;     //d记录上一步方向                         
    path.length++;
    if(xi == xe && yi == ye)     //找到终点                            
    {
    	if(!flag2){
			flag2 = true;
			cout << "用DFS求解的所有的迷宫路径:" << endl;
		}
    	if(path.length < minpath.length){
			for(int k=0;k<path.length;k++)
			{
				minpath.data[k] = path.data[k];
			}
			minpath.length = path.length;	
			ans = count+1;
		}
    	cout<< "第" << ++count << "条迷宫通路如下(前两位是坐标，第三位d为0向上，为1向右，为2向下，为3向左)：\n";
        for(int k = 0; k < path.length; k++)
        {
        	if(k == path.length-1)cout<<"("<<path.data[k].i<<","<<path.data[k].j<<","<<-1<<")";
			else cout<<"("<<path.data[k].i<<","<<path.data[k].j<<","<<path.data[k+1].d<<")->"; //因为d记录的是上一步到这一步的方向，所以下一步方向要是k+1来找 
			if((k+1)%5==0){cout<<"\n";}
        }
        cout << endl;
    }
    p = G->adjlist[xi][yi].firstarc;
    while(p != NULL)
    {
        if(visited[p->i][p->j] == 0)                            
            DFS(G, p->i, p->j, p->d, xe, ye, path);
        p = p->nextarc;
    }
    visited[xi][yi] = 0;       //回溯                 
}
void BFS(Graph *G,int xi,int yi,int xe,int ye,PathType path) { //图论广度优先搜索 
	int d;
	Man queue[Max];
	int front = -1;
	int rear = front;
	rear++;
	queue[rear].i =xi;
	queue[rear].j =yi;
	queue[rear].d =-1;
	visited[xi][yi] = 1;
	while(front != rear) {
		Man elem;
		front++; //出队 （虽然是出队，但该节点仍在数组中，这样的好处是方便找到路径） 
		elem = queue[front];
		if(elem.i == xe&&elem.j == ye) { //找到终点 
			d = 0;
			int i = front;
			flag2 = true;
			while(queue[i].d != -1) {
				path.data[d++]=queue[i];
				i = queue[i].d ;
			}
			path.data[d] = queue[i];
			cout << "用BFS求解的迷宫的最短路径为(前两位是坐标，第三位d为0向上，为1向右，为2向下，为3向左)：" << endl;
			for(int i = d; i >= 0; i --) {
				int di;
				if(i == 0)printf("(%d,%d,-1)",path.data[i].i,path.data[i].j);
				else{
					if(path.data[i].i - path.data[i-1].i == 1)di = 0;
					if(path.data[i].j - path.data[i-1].j == -1)di = 1;
					if(path.data[i].i - path.data[i-1].i == -1)di = 2;
					if(path.data[i].j - path.data[i-1].j == 1)di = 3;
					printf("(%d,%d,%d)->",path.data[i].i,path.data[i].j,di);
				}
			}
			cout << endl;
			return;
		}
		ArcNode *p = G->adjlist[elem.i][elem.j].firstarc;
		while(p!=NULL) {
			if(visited[p->i][p->j]==0) {
				visited[p->i][p->j]=1;
				rear++; //入队 
				queue[rear].i= p->i;
				queue[rear].j= p->j;
				queue[rear].d = front; //d记录的是该节点的父节点的下标（假定由父节点走到子节点） 
			}
			p=p->nextarc;
		}
	}
	if(!flag2) {
			cout << "没有通路" << endl;
	}
}
void check(){ //检查递归解法是否有通路 
	if(!flag2){
		cout << "没有通路" << endl;
	} 
	else{
		cout << "最短路径为第" << ans << "条通路，长度为："<< minpath.length-1 << ",路径为：" << endl;
		for(int k=0;k<minpath.length;k++){
			if(k == minpath.length-1)cout<<"("<<minpath.data[k].i<<","<<minpath.data[k].j<<","<<-1<<")";
			else cout<<"("<<minpath.data[k].i<<","<<minpath.data[k].j<<","<<minpath.data[k+1].d<<")->";//因为d记录的是上一步到这一步的方向，所以下一步方向要是k+1来找
			if((k+1)%5==0){cout<<"\n";}
		} 
		cout<<"\n";
	} 
} 
void check1(){ //检查DFS解法是否有通路 
	if(!flag2){
		cout << "没有通路" << endl;
	} 
	else{
		cout << "最短路径为第" << ans << "条通路，长度为："<< minpath.length-1 << ",路径为：" << endl;
		for(int k=0;k<minpath.length;k++){
			if(k == minpath.length-1)cout<<"("<<minpath.data[k].i<<","<<minpath.data[k].j<<","<<-1<<")";
			else cout<<"("<<minpath.data[k].i<<","<<minpath.data[k].j<<","<<minpath.data[k].d<<")->";
			if((k+1)%5==0){cout<<"\n";}
		} 
		cout<<"\n";
	} 
} 
int main()
{
	while(1){
		Graph *G;
		int x;
		CreateAdj(G);                                           
		PathType path;
		path.length=0;
		count = 0; 
		minpath.length = Max;
		flag2 = false;
		cout<<"请输入起点坐标:";
		cin>>start_r>>start_l;
		cout<<"请输入终点坐标:";
		cin>>end_r>>end_l;
		while(1){
			cout << "请选择解题方法：" << endl;
			cout << "1为非递归栈的方法求解" << endl;
			cout << "2为非递归队列的方法求解" << endl;
			cout << "3为递归的方法求解" << endl;
			cout << "4为借助图论DFS的方法求解" << endl;
			cout << "5为借助图论BFS的方法求解" << endl;
			cout << "请输入1-5：" ;
			cin >> x;
			switch(x){
				case 1:
					non_recursive_stack(start_r, start_l,end_r,end_l);
					flag2 = false;
					count = 0;
					path.length=0;
					minpath.length = Max;
					break;
				case 2:
					non_recursive_queue(start_r, start_l,  end_r, end_l,path);
					if(!flag2) {
						cout << "没有通路" << endl;
					}
					flag2 = false;
					count = 0;
					path.length=0;
					minpath.length = Max;
					break;
				case 3:
					recursive(start_r,start_l,-1,end_r,end_l,path);
					check1();
					flag2 = false;
					count = 0;
					path.length=0;
					minpath.length = Max;
					break;
				case 4:
					DisplayAdj(G); 
					DFS(G, start_r, start_l, 0, end_r, end_l, path);
					check();
					flag2 = false;
					count = 0;
					path.length=0;
					minpath.length = Max;
					break;
				case 5:
					DisplayAdj(G); 
					BFS(G, start_r, start_l,  end_r, end_l,path);
					flag2 = false;
					count = 0;
					path.length=0;
					minpath.length = Max;
					break;
			} 
			char flag3;
			cout << "是否继续尝试其他方法？（y or n）";
			cin >> flag3;
			if(flag3 == 'n')break;
		}
		cout << "是否继续新建迷宫？（y or n）";
		cin  >> flag1;
		if(flag1 == 'n')break;
	} 
    return 0;
}

