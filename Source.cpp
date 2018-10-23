#ifndef YOUR_HEADER_H
#define YOUR_HEADER_H
#include <iostream> 
#include <map> 
#include <iterator> 
#include <vector>
#include <string>
#include <queue>
#include <list>
#include <climits>
#endif

using namespace std;


vector<int> adj[5];
map<string, int> vertices;
int N;

int getVertex(string u){
	map<string, int> ::iterator itr;
	itr = vertices.find(u); //length not find
	if (itr == vertices.end()){
		vertices.insert(pair <string, int>(u, vertices.size()));
		return vertices.size();
	}
	else{
		return itr->second;
	}
}

void addEdge(vector<int> adj[], string stringu, string stringv){
	//check if vertex exists
	int u = getVertex(stringu);
	int v = getVertex(stringv);

	//check loop

	//check if edge already exists
	for (auto x : adj[v]){
		if (x == u)
			return;
	}
	
	//add edge to vertex
	adj[u].push_back(v);
	adj[v].push_back(u);
}

void readFiles(){
	
}

void printGraph(vector<int> adj[], int V)
{
	for (int v = 0; v < V; ++v)
	{
		cout << "\n Adjacency list of vertex "
			<< v << "\n head ";
		for (auto x : adj[v])
			cout << "-> " << x;
		printf("\n");
	}
}


int main()
{
	N = 5;
	addEdge(adj, "0", "1");
	addEdge(adj, "0", "4");
	addEdge(adj, "1", "2");
	addEdge(adj, "1", "3");
	addEdge(adj, "1", "4");
	addEdge(adj, "2", "3");
	addEdge(adj, "3", "4");
	printGraph(adj, N);

}

float geodistance(int u, int v){ /*adapted from https://www.geeksforgeeks.org/breadth-first-search-or-bfs-for-a-graph/ */
	// Mark all the vertices as not visited 
	int *visited = new int[N];
	for (int i = 0; i < N; i++)
		visited[i] = -1;

	// Create a queue for BFS 
	list<int> queue;

	// Mark the current node as visited and enqueue it 
	visited[u] = true;
	queue.push_back(u);

	// 'i' will be used to get all adjacent 
	// vertices of a vertex 
	vector<int>::iterator i;

	while (!queue.empty())
	{
		// Dequeue a vertex from queue and print it 
		u = queue.front();
		queue.pop_front();

		// Get all adjacent vertices of the dequeued 
		// vertex s. If a adjacent has not been visited,  
		// then mark it visited and enqueue it 
		for (i = adj[u].begin(); i != adj[u].end(); ++i)
		{
			if (visited[*i] == -1)
			{
				if (*i == v){
					int distance = 0;
					while (u != v){
						distance++;
						u = visited[u];
					}
					return distance*1.0;
				}
				visited[*i] = u;
				queue.push_back(*i);
			}
		}
	}

	return INT_MAX*1.0;
}

float closenesscent(int i){
	float sum = 0;
	for (int j = 0; j < N; j++){
		if (i != j){
			sum += (1.0 / (geodistance(i, j)*1.0));
		}
	}
	return sum / ((N - 1)*1.0);
}

float mclosenesscent(){
	float sum = 0;
	for (int i = 1; i < N; i++){
		sum += closenesscent(i);
	}
	return sum / (N*1.0);
}

