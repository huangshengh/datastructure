#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>
#include <string>
#include <stack>
#include <vector>
#include <queue>
using namespace std;

//static 只能在class内被定义
class Graph
{
private:
	int V;//顶点数目
	int E;//边的数目
	list<int>** adj;//list实现邻接表,adj[]里面每个元素都是list指针
	//list<int>* adj[]，adj是个数组每个元素都是list<int>的指针
public:
	Graph(int v)
	{
		V = v;
		E =0;
		adj = new (list<int>*);
		for(int i=0;i<V;++i)
			adj[i] = new list<int>();
	}
	Graph(char* s)//输入文件目录
	{
		ifstream input(s);
		//文件前两行是v和E
		input>>V;
		input>>E;
		adj = new (list<int>*);
		for(int i=0;i<V;++i)
			adj[i] = new list<int>();
		string text;
		int m,n;
		getline(input,text);
		while(getline(input,text))
		{
			istringstream stream(text);
			stream>>m;
			stream>>n;
			addEdge(m,n);
		}
	}
	int v() { return V;}
	void addEdge(int v,int w);
	void printGraph();
	int degree(int v);//计算v顶点的度数
	int maxdegree();
	int numberofselfloops();
	list<int> adjc(int v);
};

void Graph::addEdge(int v,int w)
{
	(*adj[v]).push_back(w);//push_back会自己申请空间，所以每个list不需要我们申请空间
	(*adj[w]).push_back(v);
	E++;
}

void Graph::printGraph()
{
	int v;
	for (v=0;v<V;++v)
	{
		list<int>* tmp = adj[v];
		list<int>::iterator ie = (*tmp).begin();
		cout<<"\n adjacency list of vertex"<<v<<"\n"<<v;
		while(ie!=(*tmp).end())
		{
			cout<<"-> "<<(*ie);
			++ie;
		}
		cout<<endl;
	}
}

list<int> Graph::adjc(int v)
{
	list<int> tmp = *adj[v];
	return tmp;
}
//度数就是从v出去的边
int Graph::degree (int v)
{
	int size = (*adj[v]).size();
	return size;
}

int Graph::maxdegree ()
{
	int maxd=0;
	for(int i =0;i<V;++i)
	{
		maxd = max(degree(i),maxd);
	}
	return maxd;
}

int Graph::numberofselfloops()
{
	int count =0;
	for(int i=0;i<V;++i)//每个list的第一个元素并不是i，但是adj[i]中的list都是以i开头的
	//所以第i个list存放的就是第i个顶点开始的边
	{
		for(list<int>::iterator ie = (*adj[i]).begin();ie!=(*adj[i]).end();++ie)
			if(*ie==i) count++;
	}
	return count/2;

}

//使用dfs查找图中的路径,dfs的过程中实际上就是一个隐式的栈，因为每个节点都要等他的节点搜索完成，返回后
//才可以返回
class DFS
{
private:
	vector<int> marked;
	vector<int> edgeto;//从起点到一个顶点已知路径上的最后一个顶点
	int S;//start
public:
	DFS(Graph G,int s)
	{
		for(int i = 0;i<G.v();++i)
		{
			marked.push_back(0);
			edgeto.push_back(0);
		}
		S = s;
		dfs(G,S);
	}
	void dfs(Graph G,int v);
	bool hasPathto(int v);
	stack<int> pathTo(int v);
};

void DFS::dfs(Graph G,int v)
{
	marked[v] = 1;
	for(int w:G.adjc(v))
		if(!marked[w])
		{
			edgeto[w] = v;//说明w在v之前还没有被从s出现的路径连接到图中，v是它的上一个点
			dfs(G,w);
		}
}

bool DFS::hasPathto(int v)
{
	return marked[v];
}

stack<int> DFS::pathTo(int v)
{
	stack<int> path;
	if(!hasPathto(v)) return path;
	for(int x = v;x!=S;x=edgeto[x])//从v开始到s，位置v上的值是v上一层的值，这样一值到某一层的上一个值是s
		path.push(x);//先进后出
	path.push(S);
	return path;
}

//使用bfs来搜索图中路径，bfs就是找出最短的路径。dfs是检查两个顶点是否连通的
class BFS
{
private:
	vector<int> marked;
	vector<int> edgeto;
	int S;
public:
	BFS(Graph g,int s)
	{
		for(int i =0;i<g.v();++i)
		{
			marked.push_back(0);
			edgeto.push_back(0);
		}
		S = s;
		bfs(g,S);
	}

	void bfs(Graph g,int s);
	bool hasPathto(int v);
	stack<int> pathTo(int v);
};

//bfs是使用的是queue，先进先出，这样每个节点的搜完之后就出去了，不管它下面的节点。
void BFS::bfs(Graph g,int s)
{
	queue<int> myque;
	marked[s] =1;
	myque.push(s);
	while(!myque.empty())
	{
		int v = myque.front();
		myque.pop();
		for(int w:g.adjc(v))
			if(!marked[w])
			{
				edgeto[w] = v;
				marked[w] =1;
				myque.push(w);
			}
	}
}

bool BFS::hasPathto(int v)
{
	return marked[v];	
}

stack<int> BFS::pathTo(int v)
{
	stack<int> path;
	if(!hasPathto(v)) return path;
	for(int x = v;x!=S;x=edgeto[x])//从v开始到s，位置v上的值是v上一层的值，这样一值到某一层的上一个值是s
		path.push(x);//先进后出
	path.push(S);
	return path;
}


int main(int argc, char *argv[])
{
	Graph gh(argv[1]);
	/*gh.addEdge(0,1);
	gh.addEdge(0,2);
	gh.addEdge(1,2);
	gh.addEdge(1,4);
	gh.addEdge(2,3);
	gh.addEdge(3,4);
	*/
	gh.printGraph();
	cout<<" the degree of 1 is "<<gh.degree(1)<<endl;
	cout <<" the maxdegree of the graph is "<<gh.maxdegree()<<endl;
	cout<<" the number of selfloop is "<<gh.numberofselfloops()<<endl;
	//以下部分是测试dfs
	DFS search(gh,0);
	stack<int> path = search.pathTo(3);
	if(!path.empty())
		cout<<"the path from 0 to 4 is:"<<endl;
	while(!path.empty())
	{
		cout<<path.top()<<" ";
		path.pop();
	}
	cout<<endl;

	//一下测试bfs
	BFS bsearch(gh,0);
	stack<int> bpath = bsearch.pathTo(3);
	if(!bpath.empty())
		cout<<"the path from 0 to 4 is:"<<endl;
	while(!bpath.empty())
	{
		cout<<bpath.top()<<" ";
		bpath.pop();
	}
	cout<<endl;

	return 0;
}