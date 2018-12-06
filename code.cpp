#include <bits/stdc++.h>
#define INF 1000000009
using namespace std;

class Graph{
public:

    int N; // nº vertices
    int M; // nº arestas
    map<int, set<int> > adj; // lista de adjacência
    set<int> visited;

    void build(string file_name) {
        int ori, dest;
        FILE *file;

        file = fopen(file_name.c_str(), "r");
        
        while(fscanf(file, "%d %d", &ori, &dest) != EOF) { 
            adj[ori].insert(dest);
            adj[dest].insert(ori);
        }

        N = adj.size();
        M = 0;

        for(auto i: adj) {
            M+=i.second.size();
        }

        M/=2;

        fclose(file);
    }

    void print() {
        for(auto u:adj) {
            cout << u.first << "->";
            for(auto v:adj[u.first]) {
                cout << " " << v;
            }
            cout << "\n";
        }
    }

    int dfs(int u) { // retorna nº de vértices da componente
        visited.insert(u);

        int ans=1;

        for(auto v: adj[u]) {
            if(!visited.count(v)) 
                ans+=dfs(v);
        }

        return ans;
    }

    map<int,int> bfs(int u) { // retorno map(vértice v, dist_min(u,v))
        queue<int> q;
        map<int,int> dis;

        for(auto i: adj) dis[i.first] = INF;

        q.push(u);
        dis[u] = 0;

        while(q.size()) {
            int v = q.front();
            q.pop();

            for(auto w: adj[v]) {
                if(dis[v]+1 < dis[w]) {
                    dis[w] = dis[v]+1;
                    q.push(w);
                }
            }
        }
        return dis;
    }

    set<int> max_componente() { // retorno vertices da maior componente
        set<int> ans;
        int u=-1, sz=-INF;

        visited.clear();

        for(auto v: adj) {
            if(!visited.count(v.first)) {
                int x = dfs(v.first);
                if(sz < x) {
                    sz = x;
                    u =  v.first;
                }
            }
        }

        visited.clear();

        map<int,int> dis = bfs(u);

        ans.insert(u);

        for(auto v: dis) 
            if(v.second != INF) ans.insert(v.first);

        return ans;
    }

    //////// Número de vértices, arestas e componentes ////////
    int vertex_size() { 
        return N;
    }

    int edge_size() { 
        return M;
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

        visited.clear();

        return w;
    }

    int chromatic() {
    	int ans=0;
        
        map<int, int> painted;

        set<int> colour;

        for(int i=1; i<=N; i++) {
        	colour.insert(i);
        }

        for(auto u: adj) {
        	for(auto v: u.second) 
        		if(painted.count(v)) colour.erase(painted[v]);

        	painted[u.first] = *colour.begin();

        	ans = max(ans, painted[u.first]);

        	for(auto v: adj[u.first]) 
        		if(painted.count(v)) colour.insert(painted[v]);
        }

        return ans;

    }
    ///////////////////////////////////////////////////////////

    /////////////////////// Grau médio ////////////////////////
    int degree(int u) { 
        if(!adj.count(u)) return -1;
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
    double density() {
        return 2.0*(double)M / (double)(N * (N-1));
    }
    ///////////////////////////////////////////////////////////

    /////////////// Excentricidade efetiva média //////////////

    int excentricidade_vertice(int u) {
        int ans=0;
        map<int,int> dis = bfs(u);

        for(auto i: dis) ans = max(ans, i.second);

        return ans;
    }

    int excentricidade_media() {
        int sum = 0;

        for(auto u: adj) sum+=excentricidade_vertice(u.first);

        return sum/vertex_size();
    } 
    ///////////////////////////////////////////////////////////

    ///////////////////// Diâmetro efetivo ////////////////////

    int diametro_efetivo() {
        int ans = 0;

        for(auto u: adj) ans = max(ans, excentricidade_vertice(u.first));

        return ans;
    }

    ///////////////////////////////////////////////////////////

    /////////////////////// Raio efetivo //////////////////////

    int raio_efetivo() {
        int ans = INF;

        for(auto u: adj) ans = min(ans, excentricidade_vertice(u.first));

        return ans;
    }

    ///////////////////////////////////////////////////////////

    //////////////////// Centralidade média ///////////////////
    

    double centralidade_vertice(int x){
        // cout << "V: " << x << endl;
        map<int,int> aux = bfs(x);
        int n=0; 
        for(auto u:aux){
            if(u.second == INF) continue;
            else{ 
                n+=u.second;
                // cout << "*" << n << "*" << endl;
            }
        }
        // cout << "&&" << double(N-1)/n << endl; 
        return double(N-1)/n;
    }

    double centralidade_media(){
        double n=0.0;
        for(auto u:adj){
            n+=centralidade_vertice(u.first);
            // cout << " ==> " << n << endl;
        }
        return n/(double)N;
    }

    ///////////////////////////////////////////////////////////

    //////////// Porcentagem de vértices centrais /////////////
    double vertices_centrais() {
        int count=0;
        int raio = raio_efetivo();
        for(auto u:adj) 
            if(excentricidade_vertice(u.first) == raio) count++;

        return 100.0 * ( (double)count/vertex_size() );
    }
    ///////////////////////////////////////////////////////////


    //////////// Porcentagem de vértices extremos /////////////
    double vertices_extremos() {
        int count =0;

        for(auto u:adj) {
            if(u.second.size() == 1) count++;
        }

        return 100.0 * ( (double)count/vertex_size() );
    }
    /////////////// Coeficiente de Agrupamento ////////////////

    ////////////////////// de um Vértice //////////////////////

    long double agrupamento_vertice(int x){

        set<int> aux = adj[x];

        long double count = 0;

        for(auto u:aux){
            for(auto v:aux){
                if(adj[u].count(v)==1){
                    count++;  
                }
            }
        }

        return 2.0*count/(long double)(N*(N-1));

    }

    long double agrupamento_medio(){
        long double aux=0.0;
        for(auto u:adj){
            aux +=agrupamento_vertice(u.first);
        }
        return aux/(long double) N;
    }

};

Graph build_max_comp(Graph g) {
    set<int> vertices = g.max_componente();
    Graph h;

    for(auto u:g.adj){
        if(!vertices.count(u.first)) continue;
        for(auto v:g.adj[u.first]) {
            if(!vertices.count(v)) continue;
            h.adj[u.first].insert(v);
            h.adj[v].insert(u.first);
        }
    }

    h.N = h.adj.size();
    h.M = 0;

    for(auto i: h.adj) {
        h.M+=i.second.size();
    }

    h.M/=2;

    return h;
}

int main()
{

	string arq = "in.txt";
	Graph g;

	g.build(arq);


	cout << "Numero cromatico: " << g.chromatic() << endl;
	// string arq;
	// int vtc, vdd = 0;

	// do {
 //        // cin >> arq;
 //        // cin >> vtc;

 //        // cout << "'" << arq << "'\n";
 //        // cout << "'" << vtc << "'\n";
	// 	cout << "\n------ TRABALHO PRATICO ------\n------ TEORIA DOS GRAFOS ------" << endl;
	//     cout << "Insira o nome do arquivo em que esta o grafo: ";
	//     getline(cin, arq);

	//     Graph g;
	//     g.build(arq);
	//     Graph h = build_max_comp(g);

	//     cout << "\n- Numero de vertices: " << g.vertex_size() << endl;
	//     cout << "- Numero de arestas: " << g.edge_size() << endl;
	//     cout << "- Numero de componentes conexas: " << g.w_cont() << endl;

	//     cout << "- Numero cromatico do grafo: " << h.chromatic() << endl; //NUMERO CROMATICO


	//     cout << "- Insira um vertice v para ver o numero de vertices adjacentes a ele: ";
	//     cin >> vtc;

	//     cout << "\n- Grau do vertice: " << h.degree(vtc) << endl;
	//     cout << "- Grau medio do grafo: " << h.degree_avg() << endl;

	//     cout << "- Densidade do grafo: " << h.density() << endl;

	// 	cout << "- Insira um vertice v, para ver seu coeficiente de agrupamento: ";
	//     cin >> vtc;
	//     cout << "\n- Coeficiente de agrupamento do vertice: " << h.agrupamento_vertice(vtc) << endl;
 //        //cout << "hadjashdkjashdkjashdakjdhkjashdakjsdhjkadjhkjash " << h.adj.begin()->first << endl;
	//     cout << "- Coeficiente de agrupamento medio do grafo: " << h.agrupamento_medio() << endl;

	// 	cout << "- Insira um vertice v, para ver sua excentricidade efetiva: ";
	//     cin >> vtc;
	//     cout << "\n- Excentricidade efetiva do vertice: " << h.excentricidade_vertice(vtc) << endl;
	//     cout << "- Excentricidade efetiva do grafo: " << h.excentricidade_media() << endl;    

	//     cout << "- Diametro efetivo do grafo: " << h.diametro_efetivo() << endl;
	// 	cout << "- Raio efetivo do grafo: " << h.raio_efetivo() << endl;

	// 	cout << "- Porcentagem de vertices centrais do grafo: " << h.vertices_centrais() << endl;
	// 	cout << "- Porcentagem de vertices extremos do grafo: " << h.vertices_extremos() << endl;

 //        cout << "- Insira um vertice v, para ver sua centralidade: ";
 //        cin >> vtc;	
 //        cout << "\nCentralidade de " << vtc << ": " << h.centralidade_vertice(vtc) << endl;
 //        cout << "Centralidade media do grafo: "<< h.centralidade_media() << endl;

 //        cout << "\nDeseja analisar outro grafo?(1 - Sim || 0 - NAO)" << endl;
 //        cin >> vdd;


	// } while(vdd);
    
}