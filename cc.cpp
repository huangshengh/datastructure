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

//寻找图中的连通子图
class CC
{
private:
	vector<int> marked;
	vector<int> id;
	int count=0;
public:
	CC(Graph g)
	{
		for(int i=0;i<g.v();++i)
		{
			marked.push_back(0);
			id.push_back(0);//同一个连通分量下的顶点v作为索引值的值都是同一个
		}
		for(int s = 0;s<g.v();++s)
		{
			if(!marked[s])
			{
				++count;
				dfs(g,s);
				//++count;
				//因为每个连通分量都会一直dfs到底，所以如果只有一个连通分量
				//一次dfs就会全部标记完成，所以count的数量就是实际的连通分量的数量
			}
		}
	}
	void dfs(Graph g,int v)
	{
		marked[v] = 1;
		id[v] = count;
		for(int w:g.adjc(v))
			if(!marked[w])
				dfs(g,w);
	}
	bool connected(int v,int w) { return id[v]==id[w]; }
	int vid(int v) { return id[v]; }
	int icount() { return count;}
	void path()
	{
		int size = id.size();
		for(int i=1;i<=count;++i)
		{
			cout << "the "<<i<<" is:"<<endl;
			for(int s =0;s<size;++s)
				if(id[s]==i)
					cout<<s<<' ';
			cout<<endl;
		}
	}

};

int main(int argc, char *argv[])
{
	Graph g(argv[1]);
	CC c(g);
	cout<<"the grap has "<<c.icount()<<" components"<<endl;
	c.path();
	return 0;
}