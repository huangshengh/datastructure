#include <iostream>
#include <list>
using namespace std;

class Graph
{
	int V;
	list<int>* adj;//使用一个list array来实现邻接表
	void DFEUtil(int v, bool visited[]);
	int pathto[];

public:
	Graph(int V);
	void addEdge(int V,int w);
	void DFS();
	void BFS(int v);
}

void Graph::addEdge(int V,int w)
{
	this->adj[V].push_back(w);
}

void Graph::DFSUtil(int v,bool visited[])
{
	visited[v] = true;
	cout<<v<<" ";

	list<int>::iterator i;
	for(i = adj[v].begin();i!=adj[v].end();++i)
	{
		//这里就是对每个以v开头的邻接表的检查
		if(!visited[*i])
		{
			DFSUtil(*i,visited);
		}
	}
}


void Graph::DFS()
{
	bool *visited = new bool[V];
	for(int i=0;i<V;++i)
		visited[i] = false;

	//对每个顶点都递归调用
	for(int i=0;i<V;++i)
		if(!visited[i])
			DFEUtil(i,visited);
}

//bfs use queue fifo
//dfs use stack lifo
void Graph::BFS(int v)
{
	queue<int> myqueue;
	bool * visited = new visited[V];
	for (int i = 0; i < V; ++i)
	{
		visited[i] = false;
	}

	visited[v] = true;
	queue.push(v);
	while(!myqueue.empty())
	{
		list<int>::iterator i;
		int node = myqueue.front();//fifo
		for(i=adj[node].begin();i!=adj[node].end();++i)
			if(!visited[*i])
			{
				pathto[node] = *i;
				visited[*i] = true;
				myqueue.push(*i);
			}
	}
	myqueue.pop();
}