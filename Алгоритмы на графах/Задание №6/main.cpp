#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int N;
const int MAXN = 100, INF = 1000000;

struct element{
	int row;
	int column;
	element(int x, int y){
		this->row = x;
		this->column = y;
	}

	int get_first_layer_number(){
		return (this->row*N + this->column);
	}
	int get_second_layer_number(){
		return (this->row*N + this->column + N*N);
	}
};

struct edge {
	int a, b, cap, flow;
};

int K, s, t;
vector<int> d, ptr, q;
ifstream fin;
vector<edge> e;
vector<vector<int>> g;

void fill_elements(vector<element>& elements){
	for (int i = 0; i < 2 * K; i++){
		if (i < K)
			fin >> elements[i].row;
		else
			fin >> elements[i - K].column;
	}
}

void input(vector<element>& elementsPVK, vector<element>& elementsPIN){
	fin.open("input.txt");
	fin >> N >> K;
	elementsPVK.assign(K, element(-1, -1));
	elementsPIN.assign(K, element(-1, -1));

	fill_elements(elementsPVK);
	fill_elements(elementsPIN);
	fin.close();
}

void add_edge(int a, int b, int cap = 1) {
	edge e1 = { a, b, cap, 0 };
	edge e2 = { b, a, 0, 0 };
	g[a].push_back((int)e.size());
	e.push_back(e1);
	g[b].push_back((int)e.size());
	e.push_back(e2);
}

void connect_layers(int i, int j){
	int numb_on_first_layer = i*N + j;
	int numb_on_second_layer = i*N + j + N*N;

	add_edge(numb_on_first_layer, numb_on_second_layer);

	int delta1[] = { 0, 0, -1, 1 };
	int delta2[] = { -1, 1, 0, 0 };
	for (int k = 0; k < 4; k++){
		if ((i + delta1[k]<N) && (i + delta1[k]>-1) && (j + delta2[k]<N) && (j + delta2[k]>-1)){
			int temp = (i + delta1[k])*N + (j + delta2[k]);
			add_edge(numb_on_second_layer, temp);
		}
	}
}

void make_graph(vector<element> elementsPVK, vector<element> elementsPIN){
	vector<int> v;
	g.assign(2 * N*N + 2, v);
	for (int i = 0; i < N; i++)
	for (int j = 0; j < N; j++)
		connect_layers(i, j);

	s = 2*N*N;
	t = 2*N*N + 1;
	for (int l = 0; l < K; l++){
		add_edge(s, elementsPVK[l].get_first_layer_number());
		add_edge(elementsPIN[l].get_second_layer_number(), t);
	}
}

bool bfs() {
	int qh = 0, qt = 0;
	q[qt++] = s;
	d.assign(g.size(), -1);
	d[s] = 0;
	while ((qh < qt) && (d[t] == -1)) {
		int v = q[qh++];
		for (size_t i = 0; i<g[v].size(); ++i) {
			int id = g[v][i],
				to = e[id].b;
			if (d[to] == -1 && e[id].flow < e[id].cap) {
				q[qt++] = to;
				d[to] = d[v] + 1;
			}
		}
	}
	return d[t] != -1;
}

int dfs(int v, int flow) {
	if (!flow) 
		return 0;
	if (v == t)
		return flow;
	for (; ptr[v]<(int)g[v].size(); ++ptr[v]) {
		int id = g[v][ptr[v]],
			to = e[id].b;
		if (d[to] != d[v] + 1) 
			continue;
		int pushed = dfs(to, min(flow, e[id].cap - e[id].flow));
		if (pushed) {
			e[id].flow += pushed;
			e[id ^ 1].flow -= pushed;
			return pushed;
		}
	}
	return 0;
}

int dinic() {
	q.resize(g.size());

	int flow = 0;
	for (;;) {
		if (!bfs())
			break;
		ptr.assign(g.size(), 0);
		while (int pushed = dfs(s, INF))
			flow += pushed;
	}
	return flow;
}

void print_answer(int res){
	ofstream fout;
	fout.open("output.txt");
	if (res != K)
		fout << -1;
	else{
		for (int i = 0; i < g[s].size(); i++){
			int temp = e[g[s][i]].b;
			while (temp != t){
				for (int j = 0; j < g[temp].size(); j++){
					if (e[g[temp][j]].flow == 1){
						//Check that we are on the top layer
						if(e[g[temp][j]].a < N*N)
							fout << e[g[temp][j]].a << " ";
						temp = e[g[temp][j]].b;
					}
				}
			}
			fout << endl;
		}
	}
	fout.close();
}

int main(){
	vector<element> elementsPVK, elementsPIN;
	input(elementsPVK,elementsPIN);
	make_graph(elementsPVK, elementsPIN);
	int res = dinic();
	print_answer(res);
	return 0;
}