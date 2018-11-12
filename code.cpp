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
    Graph g;
    g.build("in.txt");

    Graph h = build_max_comp(g);

    // h.print();
    // puts("okay");

    cout << g.agrupamento_vertice(1) << endl;
    cout << g.agrupamento_medio();
}