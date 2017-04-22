#include <iostream>
#include <cstdlib>

using namespace std;

//adjacnt list node
struct AdjListNode
{
	int dest;
	struct  AdjListNode* next;
};

struct AdjList
{
	struct AdjListNode* head;
};

class Graph
{
private:
	int V;
	struct AdjList* array;
public:
	Graph(int v)
	{
		this->V = v;
		array = new AdjList[V];
		for(int i=0;i<V;++i)
		{
			array[i].head = newAdjListNode(i);
		}
	}
	//create new adjacency list node
	AdjListNode * newAdjListNode(int dest)
	{
		AdjListNode * newNode = new AdjListNode;
		newNode->dest = dest;
		newNode->next = NULL;
		return newNode;
	}

	//add edge to graph
	/*void addEdge(int src,int dest)
	{
		AdjListNode * newNode= newAdjListNode(dest);
		newNode->next = array[src].head->next;
		array[src].head->next = newNode;
	}
	*/
	 //无向图add的方法，src和dest分别都要add，且没有为arry【ℹ️】申请空间，array[i].head=NULL;
	 void addEdge(int src, int dest)
        {
            AdjListNode* newNode = newAdjListNode(dest);
            newNode->next = array[src].head;
            array[src].head = newNode;
            newNode = newAdjListNode(src);
            newNode->next = array[dest].head;
            array[dest].head = newNode;
        }

	void printGraph()
	{
		int v;
		for (v=0;v<V;++v)
		{
			AdjListNode* pCrawl = array[v].head;
			cout<<"\n adjacency list of vertex"<<v<<"\n"<<v;
			pCrawl = pCrawl->next;
			while(pCrawl)
			{
				cout<<"-> "<<pCrawl->dest;
				pCrawl = pCrawl->next;
			}
			cout<<endl;
		}
	}
};

int main(int argc, char const *argv[])
{
	Graph gh(5);
	gh.addEdge(0,1);
	gh.addEdge(0,2);
	gh.addEdge(1,2);
	gh.addEdge(1,4);
	gh.addEdge(2,3);
	gh.addEdge(3,4);
	//gh.addEdge(4,1);
	gh.printGraph();
	return 0;
}