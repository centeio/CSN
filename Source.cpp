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
#include <fstream>
#include <time.h>
#endif

using namespace std;


struct Graph{
	map<string, int> vertices;
	int N;
	int E;
	vector<vector<int> > adja;
	vector<vector<int> > d;
};


int getVertex(string u, Graph &g){
	map<string, int> ::iterator itr;
	itr = g.vertices.find(u); //length not find
	if (itr == g.vertices.end()){
		g.vertices.insert(pair <string, int>(u, g.vertices.size()));
		return g.vertices.size() - 1;
	}
	else{
		return itr->second;
	}
}


void printGraph(Graph g)
{
	for (unsigned int v = 0; v < g.adja.size(); ++v)
	{
		cout << "\n Adjacency list of vertex "
			<< v << "\n head ";
		for (unsigned int x = 0; x < g.adja.at(v).size(); x++){
			int c = g.adja.at(v).at(x);
			cout << "-> " <<c;
		}
		printf("\n");
	}
}



int geo(int u, int v, Graph &g){ //addapted from https://www.geeksforgeeks.org/minimum-number-of-edges-between-two-vertices-of-a-graph/
	if (g.d[u][v] != -1){
		return g.d[u][v];
	}

	if (g.d[v][u] != -1){
		return g.d[v][u];
	}

	if (g.adja[u].size() == 1){
		if (g.d[g.adja[u][0]][v] != -1)
			return g.d[g.adja[u][0]][v] + 1;
	}

	// visited[n] for keeping track of visited 
	// node in BFS 
	vector<bool> visited(g.N, 0);

	// Initialize distances as 0 
	vector<int> distance(g.N, INT_MAX);

	// queue to do BFS. 
	queue <int> Q;
	distance[u] = 0;

	Q.push(u);
	visited[u] = true;
	while (!Q.empty())
	{
		int x = Q.front();
		Q.pop();

		for (unsigned int i = 0; i<g.adja[x].size(); i++)
		{
			if (distance[g.adja[x][i]] > (distance[x] + 1)){
				distance[g.adja[x][i]] = distance[x] + 1;
			}

			if (visited[g.adja[x][i]])
				continue;

			// update distance for i 
			Q.push(g.adja[x][i]);
			visited[g.adja[x][i]] = true;
			
		}
	}
	g.d[u] = distance;

	return distance[v];
}

bool addEdgeint(int u, int v, Graph &g){
	if (u == v){
		return false;
	}

	//check loop
	if (geo(u, v, g) != INT_MAX)
		return false;

	//check if edge already exists
	for (unsigned int i = 0; i < g.adja.at(v).size(); i++){
		if (g.adja.at(v).at(i) == u)
			return false;
	}

	//add edge to vertex
	g.adja.at(u).push_back(v);
	g.adja.at(v).push_back(u);
	return true;

}

bool addEdge(string stringu, string stringv, Graph &g){
	//check if vertex exists
	int u = getVertex(stringu, g);
	int v = getVertex(stringv, g);

	return addEdgeint(u, v, g);
}

float closenesscent(int i, Graph &g){
	if (g.adja[i].size() == 0)
		return 0;
	float sum = 0;
	for (int j = 0; j < g.N; j++){
		if (i != j){
			sum += (1.0 / (geo(i, j, g)*1.0));
		}
	}
	return sum / ((g.N - 1)*1.0);
}

float mclosenesscent(Graph &g){
	float sum = 0;
	for (int i = 1; i < g.N; i++){
		sum += closenesscent(i,g);
	}
	return sum / (g.N*1.0);
}

Graph readFiles(string filename){
	ifstream gfile;
	string vi, vj;

	Graph g;

	gfile.open(filename);
	if (gfile.fail()) {
		cout << "Couldn't open the file." << endl;
	}
	gfile >> g.N >> g.E;
	for (int i = 0; i < g.N; i++) {
		vector<int> temp;
		vector<int> dists(g.N, -1);
		g.adja.push_back(temp);
		g.d.push_back(dists);
	}

	while (gfile >> vi >> vj) {
		addEdge(vi, vj, g);
	}

	return g;

}

Graph ER(int N, int E){
	Graph g;
	int counter = 0;
	g.N = N;
	g.E = E;

	for (int i = 0; i < g.N; i++) {
		vector<int> temp;
		vector<int> dists(g.N, -1);
		g.adja.push_back(temp);
		g.d.push_back(dists);
	}

	while (counter < g.E){
		int u = rand() % g.N;
		int v = rand() % g.N;

		if (addEdgeint(u, v, g)){
			counter = counter + 1;
		}
	}
	g.E = counter;
	return g;
}

Graph switching(Graph g){
	Graph newg;
	newg.N = g.N;
	newg.E = g.E;
	newg.adja = g.adja;
	int Q = g.N*log(g.N);
	int QE = Q*newg.E;

	for (int i = 0; i < QE; i++){
		int n1, n2;
		do{
			n1 = rand() % newg.N;
			n2 = rand() % newg.N;
		} while (!(n1 != n2 && newg.adja[n1].size() > 0 && newg.adja[n2].size() > 0));

		int posn3 = rand() % newg.adja[n1].size();
		int posn4 = rand() % newg.adja[n2].size();

		int n3 = newg.adja[n1][posn3];
		int n4 = newg.adja[n2][posn4];

		if(addEdgeint(n1, n2, newg))
			newg.adja[n1].erase(newg.adja[n1].begin() + posn3);
		if(addEdgeint(n3, n4, newg))
			newg.adja[n2].erase(newg.adja[n2].begin() + posn4);
	}
	return newg;

}

int main(){
	srand(time(NULL));

	//Graph g = readFiles("C:\\Users\\Carolina\\Documents\\FEUP\\5A\\1S\\CSN\\Lab\\Lab4\\dependency_networks\\Basque_syntactic_dependency_network.txt");
	Graph g = readFiles("C:\\Users\\Carolina\\Documents\\FEUP\\5A\\1S\\CSN\\Lab\\Lab4\\dependency_networks\\test.txt");
	cout << "::::::::real network::::::::\n N: " << g.N << "; E: " << g.E <<"\n";
	Graph er = ER(g.N, g.E);
	cout << "::::::::ER network::::::::\n N: " << er.N << "; E: " << er.E << "\n";
	Graph switchn = switching(g);
	cout << "::::::::Switching network::::::::\n N: " << switchn.N << "; E: " << switchn.E << "\n";


	system("pause");

}

