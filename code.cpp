#include <bits/stdc++.h>
#define INF 1000000009
using namespace std;

class Graph{
public:

    int N; // nº vertices
    int M; // nº arestas
    map<int, set<int> > adj; // lista de adjacência
    map<int, int> excentricidade;
    map<int, double> centralidade;
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
        int count=0;
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
                count++;
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

    void excentricidade_centralidade(string arq) {

        int k=1, sz = adj.size();
        for(auto ux: adj) {
            int u = ux.first;

            map<int,int> dis = bfs(u);

            /////////////// Excentricidade  //////////////
            int exc=0;
            for(auto i: dis) exc = max(exc, i.second);
            excentricidade[u] = exc;  
            //////////////////////////////////////////////

            /////////////// Centralidade  //////////////
            int n=0; 
            for(auto i:dis){
                if(i.second == INF) continue;
                n+=i.second;
            }
            centralidade[u] = (double)(N-1)/(double)n;
            ////////////////////////////////////////////

            cout << "[" << arq << "] ........ [" << k++ << "|" << sz << "]\n";
        }
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

    double degree_avg() {
        double sum_degree = 0;

        for(auto i: adj) {
            sum_degree +=i.second.size();
        }

        return sum_degree / (double)N;
    }
    ///////////////////////////////////////////////////////////

    //////////////////////// Densidade ////////////////////////
    double density() {
        return 2.0*(double)M / (double)(N * (N-1));
    }
    ///////////////////////////////////////////////////////////

    /////////////// Excentricidade efetiva média //////////////

    int excentricidade_vertice(int u) {
        if(excentricidade.count(u)) {
            return excentricidade[u];
        }

        int ans=0;

        map<int,int> dis = bfs(u);

        for(auto i: dis) ans = max(ans, i.second);

        return excentricidade[u] = ans;
    }

    double excentricidade_media() {
        double sum = 0;

        for(auto u: adj) sum+=excentricidade_vertice(u.first);

        return sum/(double)vertex_size();
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
    
    double centralidade_vertice(int u){
        if(centralidade.count(u)) return centralidade[u];

        map<int,int> dis = bfs(u);
        int n=0; 
        for(auto i:dis){
            if(i.second == INF) continue;
            else{ 
                n+=i.second;
            }
        }
        return centralidade[u] = (double)(N-1)/(double)n;
    }

    double centralidade_media(){
        double n=0.0;
        for(auto u:adj){
            n+=centralidade_vertice(u.first);
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

void results(Graph g, string in_arq) {
    Graph h = build_max_comp(g);    

    h.excentricidade_centralidade(in_arq);

    ofstream myfile;

    myfile.open ("out_"+in_arq);

    myfile << "======================BEGIN======================\n";
    myfile << "GRAFO "                      << in_arq             << endl;
    myfile << "N. de vertices: "            << g.vertex_size()    << endl;
    myfile << "N. de arestas: "             << g.edge_size()      << endl;
    myfile << "N. de componentes conexas: " << g.w_cont()         << endl;
 
    myfile << endl;

    myfile << "ANALISE DA MAIOR COMPONENTE\n";

    myfile << "N. de vertices: " << h.vertex_size() << endl;
    myfile << "N. de arestas: "  << h.edge_size()   << endl;
    myfile << "N. cromatico: "   << h.chromatic()   << endl;
    myfile << "Grau medio: "     << h.degree_avg()  << endl;
    myfile << "Densidade: "      << h.density()     << endl;


    myfile << "Coeficiente de agrupamento medio: " << h.agrupamento_medio()     << endl;
    myfile << "Excentricidade efetiva media: "     << h.excentricidade_media()  << endl;
    myfile << "Diametro efetivo: "                 << h.diametro_efetivo()      << endl;
    myfile << "Raio efetivo: "                     << h.raio_efetivo()          << endl;
    myfile << "Centralidade media: "               << h.centralidade_media()    << endl;
    myfile << "Porcentagem de vertices centrais: " << h.vertices_centrais()     << endl;
    myfile << "Porcentagem de vertices extremos: " << h.vertices_extremos()     << endl;

    myfile << "=======================END=======================\n";
    myfile.close();
}


int main()
{

    string arq = "CA-CondMat.txt";


    Graph g;

    g.build(arq);
    results(g, arq);
}
