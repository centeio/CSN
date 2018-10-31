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
#include <cstring>
#endif

using namespace std;

ofstream outfile("out_night.txt", ios::out);

int order = 1; //0 for default, 1 for random, 2 for crescent sorting, 3 for descrescent sorting
vector<int> shuffled;

struct Graph {
	map<string, int> vertices;
	int N;
	int E;
	vector<vector<int> > adja;
	vector<vector<int> > d;
	bool opt;
};


int getVertex(string u, Graph &g) {
	map<string, int> ::iterator itr;
	itr = g.vertices.find(u); //length not found
	if (itr == g.vertices.end()) {
		if (order == 1) {
			g.vertices.insert(pair <string, int>(u, shuffled[g.vertices.size()]));
			
		}
		else {
			g.vertices.insert(pair <string, int>(u, g.vertices.size()));
		}
		return g.vertices.find(u)->second;
	}
	else {
		return itr->second;
	}
}


void printGraph(Graph g)
{
	for (unsigned int v = 0; v < g.adja.size(); ++v)
	{
		cout << "\n Adjacency list of vertex "
			<< v << "\n head ";
		for (unsigned int x = 0; x < g.adja.at(v).size(); x++) {
			int c = g.adja.at(v).at(x);
			cout << "-> " << c;
		}
		printf("\n");
	}
}



int geo(int u, int v, Graph &g) { //addapted from https://www.geeksforgeeks.org/minimum-number-of-edges-between-two-vertices-of-a-graph/
	if (g.opt) {
		if (g.d[u][v] != -1) {
			return g.d[u][v];
		}

		if (g.d[v][u] != -1) {
			return g.d[v][u];
		}

		if (g.adja[u].size() == 1) {
			if ((g.d[g.adja[u][0]][v] != -1) && g.d[g.adja[u][0]][v] != INT_MAX)
				return g.d[g.adja[u][0]][v] + 1;
		}
	}


	// visited[n] for keeping track of visited 
	// node in BFS 
	vector<bool> visited(g.N, 0);

	// Initialize distances as 0 
	vector<int> distance(g.N, INT_MAX);

	// queue to do BFS. 
	queue <int> Q;
	distance[u] = 0;

	//cout << "visited, distance: " << visited.size() << " " << distance.size() << "\n";

	Q.push(u);
	visited[u] = true;
	while (!Q.empty())
	{
		int x = Q.front();
		Q.pop();
		//cout << "X: " << x << "\n";

		for (unsigned int i = 0; i < g.adja[x].size(); i++)
		{
			//cout << "i: " << i << "\n";
			//cout << "g.adja[x][i]: " << g.adja[x][i] << "\n";

			if (distance[g.adja[x][i]] > (distance[x] + 1)) {
				distance[g.adja[x][i]] = distance[x] + 1;
			}

			if (visited[g.adja[x][i]])
				continue;

			// update distance for i 
			Q.push(g.adja[x][i]);
			visited[g.adja[x][i]] = true;

		}

	}

	if (g.opt) {
		g.d[u] = distance;
	}
	return distance[v];
}

bool addEdgeint(int u, int v, Graph &g) {
	if (u == v) {
		return false;
	}

	//check if edge already exists
	for (unsigned int i = 0; i < g.adja.at(v).size(); i++) {
		if (g.adja.at(v).at(i) == u)
			return false;
	}

	//add edge to vertex
	g.adja.at(u).push_back(v);
	g.adja.at(v).push_back(u);
	return true;

}

bool addEdge(string stringu, string stringv, Graph &g) {
	//check if vertex exists
	int u = getVertex(stringu, g);
	int v = getVertex(stringv, g);

	return addEdgeint(u, v, g);
}

float closenesscent(int i, Graph &g) {
	if (g.adja[i].size() == 0)
		return 0;
	float sum = 0;
	for (int j = 0; j < g.N; j++) {
		if (i != j) {

			sum += (1.0 / (geo(i, j, g)*1.0));
		}
	}
	return sum / ((g.N - 1)*1.0);
}

float mclosenesscent(Graph &g) {
	float sum = 0;
	for (int i = 1; i < g.N; i++) {
		sum += closenesscent(i, g);
	}
	return sum / (g.N*1.0);
}

bool minclosenesscent(Graph &g, float cx) {
	int N = g.N;
	int M = 0.1*N;
	float sum = 0;
	for (int i = 1; i < M; i++) {
		if ((sum/(N*1.0)) >= cx)
			return 1;
		sum += closenesscent(i, g);
			
	}
	return 0;
}


int boundclosenesscent(Graph &g, float cx) {
	cout << "looking for lower bound of closeness\n";
	int N = g.N;
	//int M = log(N);
	double sum = 0;
	

	for (int i = 0; i < N; i += 0.1*N) {
		cout << "i: " << i << "\n";
		int M = i + 0.1*N;
		cout << "M: " << M << "\n";
		double tempsum = 0;
		for (int c = i; c < M; c++) {
			tempsum += closenesscent(c, g);
		}
		sum += (tempsum / (N*1.0));
		//cout << sum;
		//Lower bound checking
		if (sum >= cx) {
			cout << "value of lb closeness: " << sum << "\n";
			return 1;
		}
		float max = (sum + 1 - (M / (N*1.0)));
		if ( max < cx) {
			cout << "value of ub closeness: " << sum << "\n";
			return 0;
		}
		cout << "min: " << sum << "max: " << max << "\n";
	}
}

bool sortdec(int u, int v) {
	return u > v;
}

bool sortcresc(int u, int v) {
	return u < v;

}



bool maxclosenesscent(Graph &g, float cx) {
	int N = g.N;
	int M = 0.1*N;
	float sum = 0;
	for (int i = 1; i < M; i++) {
		sum += closenesscent(i, g);
		if ((sum /( N * 1.0)) >= cx)
			return 1;
	}
	sum += g.N - M;

	if (sum / (g.N*1.0) < cx)
		return 0;

	return 1;
}
float apprxclosenesscent(Graph &g) {
	int M = 0.1*g.N;
	float sum = 0;
	for (int i = 1; i < M; i++) {
		sum += closenesscent(i, g);
	}
	return sum / (M*1.0);
}

Graph readFiles(string filename) {
	ifstream gfile;
	string vi, vj;
	int edges = 0;

	Graph g;
	g.opt = true;

	gfile.open(filename);
	if (gfile.fail()) {
		cout << "Couldn't open the file." << endl;
	}
	gfile >> g.N >> g.E;
	if (order == 1) {
		for (int i = 0; i < g.N; ++i)
			shuffled.push_back(i);
		random_shuffle(shuffled.begin(), shuffled.end());
	}

	for (int i = 0; i < g.N; i++) {
		vector<int> temp;
		vector<int> dists(g.N, -1);
		g.adja.push_back(temp);
		g.d.push_back(dists);
	}

	while (gfile >> vi >> vj) {
		if (addEdge(vi, vj, g))
			edges++;
		//cout << edges << "\n";
		outfile << edges << "\n";
	}

	//sort edges?
	if (order == 2 || order == 3) { //crescent
		vector<int> sizes;
		vector<vector<int> > adjacency;

		for (int i = 0; i < g.adja.size(); i++) {
			sizes.push_back(g.adja[i].size());
			vector<int> temp;
			adjacency.push_back(temp);
		}

		if (order == 2)
			sort(sizes.begin(), sizes.end(), sortcresc);
		else if (order == 3)
			sort(sizes.begin(), sizes.end(), sortdec);

		map<int, int> vertices; //map pf old position -> new position
		map<string, int>::iterator it;

		for (it = g.vertices.begin(); it != g.vertices.end(); it++) {
			int index = it->second;
			int newpos = find(sizes.begin(), sizes.end(), g.adja[index].size()) - sizes.begin();
			sizes[newpos] = -1;
			vertices.insert(pair <int, int>(index, newpos));
			adjacency[newpos] = g.adja[index];
			it->second = newpos;
		}

		for (int i = 0; i < adjacency.size(); i++) {
			for (int j = 0; j < adjacency[i].size(); j++) {
				adjacency[i][j] = vertices.find(adjacency[i][j])->second;
			}
		}
		//update g.adja
		g.adja = adjacency;
	}

	return g;

}
Graph ER(int N, int E) {
	Graph g;
	g.opt = true;
	int counter = 0;
	g.N = N;
	g.E = E;

	for (int i = 0; i < g.N; i++) {
		vector<int> temp;
		vector<int> dists(g.N, -1);
		g.adja.push_back(temp);
		g.d.push_back(dists);
	}

	while (counter < g.E) {
		int u = rand() % g.N;
		int v = rand() % g.N;

		if (addEdgeint(u, v, g)) {
			counter = counter + 1;
		}
		outfile << counter << "\n";
		//cout << counter << "\n";
	}
	g.E = counter;
	return g;
}

Graph switching(Graph oldg) {
	Graph g = oldg;
	g.opt = false;
	int Q = log(g.E);
	int QE = Q * g.E;
	//cout << "number of iterations: " << QE << "\n";
	
	cout << "new distance matrix for new graph \n";
	for (int i = 0; i < g.d.size(); i++) {
		for (int j = 0; j < g.d[i].size(); j++) {
			g.d[i][j] = -1;
		}
	}

	for (int i = 0; i < QE; i++) {
	    //cout << i << "\n";
		int n1, n2, n3, n4, posn3, posn4;
		//outfile << i << "\n";
		//cout << i << "\n";

			
		n1 = rand() % g.N;
		n2 = rand() % g.N;
		//cout << i << ": " << "n1: " << n1 << "n2: " << n2 << "\n";
		if (!(n1 != n2 && g.adja[n1].size() > 0 && g.adja[n2].size() > 0))
			continue;			

		posn3 = rand() % g.adja[n1].size();
		posn4 = rand() % g.adja[n2].size();

		n3 = g.adja[n1][posn3];
		n4 = g.adja[n2][posn4];
		//cout << i << ": " << "n3: " << n3 << "n4: " << n4 << "\n";
		if (n3 == n4 || n2 == n3 || n1 == n4)
			continue;

		//erase edges
		//cout << i << ": " << "erase the edges in g old \n";
		//cout << i << ": " << "erase edge 1 \n";
		g.adja[n1].erase(g.adja[n1].begin() + posn3);
		g.adja[n3].erase(find(g.adja[n3].begin(), g.adja[n3].end(), n1));
		//cout << i << ": " << "erase edge 2 \n";
		g.adja[n2].erase(g.adja[n2].begin() + posn4);
		g.adja[n4].erase(find(g.adja[n4].begin(), g.adja[n4].end(), n2));

		//cout << i << ": " << "adding edge 1 + control \n";
		if (!addEdgeint(n1, n2, g)) {
			//cout << i << ": " << " edge 1 switch failed: restoring previous status \n";
			addEdgeint(n1, n3, g);
			addEdgeint(n2, n4, g);
			continue;
		}
		//cout << i << ": " << "adding edge 2 + control \n";
		if (!addEdgeint(n3, n4, g)) {
			//find position of n2 in adjacency of n1
			//erase edge
			g.adja[n1].erase(find(g.adja[n1].begin(), g.adja[n1].end(), n2));
			g.adja[n2].erase(find(g.adja[n2].begin(), g.adja[n2].end(), n1));
			//cout << i << ": " << " edge 1 switch failed: restoring previous status";
			addEdgeint(n1, n3, g);
			addEdgeint(n2, n4, g);
		}
	}
	return g;

}


int main() {
	srand(time(NULL));
	order = 2;
	
	Graph g = readFiles("C:\\Users\\fpier\\OneDrive - uniroma1.it\\MS Data Science\\Complex Social Network\\HW\\HW3\\dependency_networks\\Basque_syntactic_dependency_network.txt");
	//Graph g = readFiles("C:\\Users\\fpier\\OneDrive - uniroma1.it\\MS Data Science\\Complex Social Network\\HW\\HW3\\dependency_networks\\test.txt");
	float x = apprxclosenesscent(g);
	cout << "::::::::real network::::::::\n N: " << g.N << "; E: " << g.E << "; C: " << x << "\n";
	//outfile << "::::::::real network::::::::\n N: " << g.N << "; E: " << g.E << "; C: " << x << "\n";

	//int T = 21;

	Graph sw;
	float count_sw=0;
	//for (int i = 0; i < T; i++) {
	//cout << "::::::::Switching network::::::::\n";
	sw = switching(g);
	sw.opt = true;
	cout << "graph created \n";
	count_sw += boundclosenesscent(sw,x);
		//if (ub < x)
		//	cout << "0";
		//if (lb >= x)
		//	count_sw += 1;

//	}
    cout << "Sw: p(X_nh > X) = " << count_sw <<"\n";


	


	//cout << "::::::::Switching network::::::::\n N: " << g.N << "; E: " << g.E << "; C: " << mclosenesscent(g) << "\n";
	//outfile << "::::::::Switching network::::::::\n N: " << g.N << "; E: " << g.E << "; C: " << mclosenesscent(g) << "\n";


	//printGraph(switchn);
	//int N = g.N;
	//int E = g.E;
	//delete &g;
	//Graph er = ER(12207, 25558);
	//cout << "::::::::ER network::::::::\n N: " << er.N << "; E: " << er.E << "; C: " << mclosenesscent(er) << "\n";
	//outfile << "::::::::ER network::::::::\n N: " << er.N << "; E: " << er.E << "; C: " << mclosenesscent(er) << "\n";


	//printGraph(er);
	//delete &er;
	
	
	vector<vector<int> > a;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			//a[i][j] = -1;
			memset(&a, 0, sizeof(a[0][0]) * i * j);
		}
	}

	system("pause");

}

