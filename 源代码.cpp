#include<iostream>
#include<malloc.h>
#define Max_size 31 //����ĵ�ͼ���߽� 
#define Max 10010 //������������ֵ 
using namespace std;
/*----------------------------���¶���ڵ㼰·������---------------------------*/
typedef struct
{
    int i,j,d; //i,jΪ���м��д����Ͻǣ�0��0����ʼ,d�����ò�ͬ�㷨�в�ͬ 
}Man;
typedef struct
{
    Man data[Max_size];
    int length;                           
}PathType;
/*----------------------------���¶����ڽӱ�����---------------------------*/
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
/*----------------------------���¶���ջ���ͼ��亯��---------------------------*/
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
/*----------------------------���¶���������ͼ��亯��---------------------------*/
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
/*----------------------------���¶���ȫ�ֱ���---------------------------*/                               
PathType minpath;    // ���·�� 
char flag1;          // ��־1 
bool flag2 = false;  // ��־2 
int n,m,start_r,start_l,end_r,end_l,count,ans;
int visited[Max_size][Max_size];     //��¼�Ƿ񾭹� 
int maze[Max_size][Max_size];       //�Թ���ͼ
void create_maze(){        //������ͼ 
	cout << "�������Թ���������������";
	cin >> n >> m;
	cout << "�������Թ���0����ͨ·��1�����ϰ�����" << endl;
	for(int i = 0;i < n;i ++){
		for(int j = 0;j < m;j ++){
			cin >> maze[i][j];
		}
		getchar();
	}
}
void CreateAdj(Graph *&G){        //�����ڽӱ� 
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
    for(i = 0; i < n; i++)      //��ͼת��Ϊ�ڽӱ�          
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
void DisplayAdj(Graph *G)  // չʾ�ڽӱ� 
{
    int i, j;
    ArcNode *p;
    cout << "����������ͼ�ڽӱ�����:" << endl; 
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
void non_recursive_stack(int start_r,int start_l,int end_r,int end_l) //�ǵݹ飨ջ���ⷨ 
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
	Stpush(st,e); //��ջ 
	maze[start_r][start_l]=-1;
	count = 0;
	while(!isEmptyStack(st))
	{
		i=st.data[st.top].i;
		j=st.data[st.top].j;
		d=st.data[st.top].d; //��¼��һ���ķ��� 
		if(i==end_r&&j==end_l) //�ҵ��յ� 
		{
			if(!flag2){
				flag2 = true;
				cout << "�÷ǵݹ飨ջ���������е��Թ�·��:" << endl;
			}
			cout<< "��" << ++count << "���Թ�ͨ·����(ǰ��λ�����꣬����λdΪ0���ϣ�Ϊ1���ң�Ϊ2���£�Ϊ3����)��\n";
			for(k=0;k<=st.top;k++) //��ջ�ױ�����ջ������ͨ· 
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
		while(d<4&&flag==0) // �����ĸ������Լ��߽��ж� 
		{
			if(d == -1)d++;
			if(d == 0){if(i == 0){d++;continue;}i=st.data[st.top].i-1;j=st.data[st.top].j;}
			if(d == 1){if(j == m-1){d++;continue;}i=st.data[st.top].i;j=st.data[st.top].j+1;}
			if(d == 2){if(i == n-1){d++;continue;}i=st.data[st.top].i+1;j=st.data[st.top].j;}
			if(d == 3){if(j == 0){d++;continue;}i=st.data[st.top].i;j=st.data[st.top].j-1;}
			if(maze[i][j]==0 && i < n && j < m){flag=1;}
			d++;
		}
		if(flag==1) //�ҵ����ߵķ�����ջ 
		{
			st.data[st.top].d=d;
			e.i=i;
			e.j=j;
			e.d=-1;
			Stpush(st,e);
			maze[i][j]=-1;
		}
		else //û�п��ߵķ����ջ 
		{
			maze[st.data[st.top].i][st.data[st.top].j]=0;
			Stpop(st);
		}
	 }
 	if(!flag2){
 		 cout << "û��ͨ·" << endl;
	     return; 
	 } 
 	cout << "���·��Ϊ��" << ans << "��ͨ·������Ϊ��"<< minpath.top << ",·��Ϊ��" << endl;
 	for(k=0;k<=minpath.top;k++){
		if(k == minpath.top)cout<<"("<<minpath.data[k].i<<","<<minpath.data[k].j<<","<<-1<<")";
		else cout<<"("<<minpath.data[k].i<<","<<minpath.data[k].j<<","<<minpath.data[k].d-1<<")->";
		if((k+1)%5==0){cout<<"\n";}
 	} 
    cout<<"\n";
 	return;
}
void non_recursive_queue(int xi,int yi,int xe,int ye,PathType path) { // �ǵݹ飨���У��ⷨ 
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
		front++; //���� ����Ȼ�ǳ��ӣ����ýڵ����������У������ĺô��Ƿ����ҵ�·���� 
		elem = queue[front];
		if(elem.i == xe&&elem.j == ye) { //�ҵ��յ� 
			d = 0;
			int i = front;
			flag2 = true;
			while(queue[i].d != -1) {
				path.data[d++]=queue[i];
				i = queue[i].d ;
			}
			path.data[d] = queue[i];
			cout << "�ö��������Թ������·��Ϊ(ǰ��λ�����꣬����λdΪ0���ϣ�Ϊ1���ң�Ϊ2���£�Ϊ3����)��" << endl;
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
		while(d<4) //�ҵ����ڽڵ���� 
		{
			d++;
			if(d == 0){if(queue[front].i == 0)continue;i=queue[front].i-1;j=queue[front].j;}
			if(d == 1){if(queue[front].j == m-1)continue;i=queue[front].i;j=queue[front].j+1;}
			if(d == 2){if(queue[front].i == n-1)continue;i=queue[front].i+1;j=queue[front].j;}
			if(d == 3){if(queue[front].j == 0)continue;i=queue[front].i;j=queue[front].j-1;}
			if(maze[i][j]==0 && i < n && j < m){
			maze[i][j]==-1;
			rear++; //��� 
			queue[rear].i= i;
			queue[rear].j= j;
			queue[rear].d = front; //d��¼���Ǹýڵ�ĸ��ڵ���±꣨�ٶ��ɸ��ڵ��ߵ��ӽڵ㣩 
			}
		}
	}
}
void recursive(int xi,int yi,int di,int xe,int ye,PathType path) // �ݹ�ⷨ 
{
	int i,j;
	if(xi==xe&&yi==ye){ //�ҵ��յ� 
		if(!flag2){
			flag2 = true;
			cout << "�õݹ��������е��Թ�·��:" << endl;
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
		cout<< "��" << ++count << "���Թ�ͨ·����(ǰ��λ�����꣬����λdΪ0���ϣ�Ϊ1���ң�Ϊ2���£�Ϊ3����)��\n";
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
			while(di<4){  // �����ĸ������Լ��߽��ж� 
				path.data[path.length].i=xi; 
				path.data[path.length].j=yi;
				path.data[path.length].d=di; //d��¼����һ���ķ��� 
				path.length++;
				if(di == 0 && xi == 0 || di == 1 && yi == m-1 || di == 2 && xi == n-1 || di ==3 && yi == 0){path.length--;di++;continue;}
				switch(di){
					case 0:{i=xi-1,j=yi;break;}
					case 1:{i=xi,j=yi+1;break;}
					case 2:{i=xi+1,j=yi;break;}
					case 3:{i=xi,j=yi-1;break;}
				} 
				maze[xi][yi]=-1;	//��ֵ-1˵�����߹� 
				recursive(i,j,di,xe,ye,path);
				path.length--;		//���� 
				maze[xi][yi]=0;	    
				di++;               //������һ������ 
			} 
		}
	}	
} 
void DFS(Graph *G, int xi, int yi,int d, int xe, int ye, PathType path)  //ͼ������������� 
{
    ArcNode *p;
    visited[xi][yi] = 1;                                     
    path.data[path.length].i = xi;                             
    path.data[path.length].j = yi;
	path.data[path.length].d = d;     //d��¼��һ������                         
    path.length++;
    if(xi == xe && yi == ye)     //�ҵ��յ�                            
    {
    	if(!flag2){
			flag2 = true;
			cout << "��DFS�������е��Թ�·��:" << endl;
		}
    	if(path.length < minpath.length){
			for(int k=0;k<path.length;k++)
			{
				minpath.data[k] = path.data[k];
			}
			minpath.length = path.length;	
			ans = count+1;
		}
    	cout<< "��" << ++count << "���Թ�ͨ·����(ǰ��λ�����꣬����λdΪ0���ϣ�Ϊ1���ң�Ϊ2���£�Ϊ3����)��\n";
        for(int k = 0; k < path.length; k++)
        {
        	if(k == path.length-1)cout<<"("<<path.data[k].i<<","<<path.data[k].j<<","<<-1<<")";
			else cout<<"("<<path.data[k].i<<","<<path.data[k].j<<","<<path.data[k+1].d<<")->"; //��Ϊd��¼������һ������һ���ķ���������һ������Ҫ��k+1���� 
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
    visited[xi][yi] = 0;       //����                 
}
void BFS(Graph *G,int xi,int yi,int xe,int ye,PathType path) { //ͼ�۹���������� 
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
		front++; //���� ����Ȼ�ǳ��ӣ����ýڵ����������У������ĺô��Ƿ����ҵ�·���� 
		elem = queue[front];
		if(elem.i == xe&&elem.j == ye) { //�ҵ��յ� 
			d = 0;
			int i = front;
			flag2 = true;
			while(queue[i].d != -1) {
				path.data[d++]=queue[i];
				i = queue[i].d ;
			}
			path.data[d] = queue[i];
			cout << "��BFS�����Թ������·��Ϊ(ǰ��λ�����꣬����λdΪ0���ϣ�Ϊ1���ң�Ϊ2���£�Ϊ3����)��" << endl;
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
				rear++; //��� 
				queue[rear].i= p->i;
				queue[rear].j= p->j;
				queue[rear].d = front; //d��¼���Ǹýڵ�ĸ��ڵ���±꣨�ٶ��ɸ��ڵ��ߵ��ӽڵ㣩 
			}
			p=p->nextarc;
		}
	}
	if(!flag2) {
			cout << "û��ͨ·" << endl;
	}
}
void check(){ //���ݹ�ⷨ�Ƿ���ͨ· 
	if(!flag2){
		cout << "û��ͨ·" << endl;
	} 
	else{
		cout << "���·��Ϊ��" << ans << "��ͨ·������Ϊ��"<< minpath.length-1 << ",·��Ϊ��" << endl;
		for(int k=0;k<minpath.length;k++){
			if(k == minpath.length-1)cout<<"("<<minpath.data[k].i<<","<<minpath.data[k].j<<","<<-1<<")";
			else cout<<"("<<minpath.data[k].i<<","<<minpath.data[k].j<<","<<minpath.data[k+1].d<<")->";//��Ϊd��¼������һ������һ���ķ���������һ������Ҫ��k+1����
			if((k+1)%5==0){cout<<"\n";}
		} 
		cout<<"\n";
	} 
} 
void check1(){ //���DFS�ⷨ�Ƿ���ͨ· 
	if(!flag2){
		cout << "û��ͨ·" << endl;
	} 
	else{
		cout << "���·��Ϊ��" << ans << "��ͨ·������Ϊ��"<< minpath.length-1 << ",·��Ϊ��" << endl;
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
		cout<<"�������������:";
		cin>>start_r>>start_l;
		cout<<"�������յ�����:";
		cin>>end_r>>end_l;
		while(1){
			cout << "��ѡ����ⷽ����" << endl;
			cout << "1Ϊ�ǵݹ�ջ�ķ������" << endl;
			cout << "2Ϊ�ǵݹ���еķ������" << endl;
			cout << "3Ϊ�ݹ�ķ������" << endl;
			cout << "4Ϊ����ͼ��DFS�ķ������" << endl;
			cout << "5Ϊ����ͼ��BFS�ķ������" << endl;
			cout << "������1-5��" ;
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
						cout << "û��ͨ·" << endl;
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
			cout << "�Ƿ��������������������y or n��";
			cin >> flag3;
			if(flag3 == 'n')break;
		}
		cout << "�Ƿ�����½��Թ�����y or n��";
		cin  >> flag1;
		if(flag1 == 'n')break;
	} 
    return 0;
}

