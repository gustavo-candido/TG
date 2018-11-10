#include <bits/stdc++.h>
using namespace std;

class Graph{
	int N; // nº vertices
	int M; // nº arestas
	map<int, set<int> > adj; // lista de adjacência
	set<int> visited;

	void build(string file_name) {
		int ori, dest;

		freopen(file_name.c_str(), "r", stdin);
		
		while(scanf("%d %d", &ori, &dest) != EOF) { 
			adj[ori].insert(dest);
			adj[dest].insert(ori);
		}

		N = adj.size();
		M = 0;

		for(auto i: adj) {
			M+=i.second.size();
		}

		M/=2;
	}

	//////// Número de vértices, arestas e componentes ////////
	int vertex_size() { 
		return N;
	}

	int edge_size() { 
		return M;
	}

	void dfs(int u) { 
		visited.insert(u);

		for(auto v: adj[u]) {
			if(!visited.count(v)) 
				dfs(v);
		}
	}

	int w_cont() { 
		int w=0;

		visited.clear();

		for(auto u: adj) {
			if(!visited.count(u.first)){
				dfs(u.first);
				w++;
			}
		}

		return w;
	}

	int chromatic() {
		return -1;
	}
	///////////////////////////////////////////////////////////

	/////////////////////// Grau médio ////////////////////////
	int degree(int u) { 
		return adj[u].size();
	}

	int degree_avg() {
		int sum_degree = 0;

		for(auto i: adj) {
			sum_degree +=i.second.size();
		}

		return sum_degree / N;
	}
	///////////////////////////////////////////////////////////

	//////////////////////// Densidade ////////////////////////
	int density() {
		return 2*M / (N * (N-1));
	}
	///////////////////////////////////////////////////////////
};

int main()
{
	Graph g;
	g.build();
}