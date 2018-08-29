#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int H, V;
int sort_param = 0;
int capacity_of_graph = 0;
int letters = 0;
int num_of_vertices = 0;
ifstream fin;

struct word {
	int constant;
	int a;
	int b;

	word(int a, int b, int c) {
		this->constant = a;
		this->a = b;
		this->b = c;
	}
};

struct vertex{
	int x;
	int y;
	int num;
	int h_num;
	int v_num;
	vertex(int a, int b, int c, int d, int e){
		this->x = a;
		this->y = b;
		this->num = c;
		this->h_num = d;
		this->v_num = e;
	}
};
struct edge{
	int begin;
	int end;
	int w;
};

vector<word> h_words, v_words;
vector<edge> edge_list;
vector<vertex> vertices;
vector<int> p;
vector < pair < int, pair<int, int> > > g;

void read_words(int param) {
	if (param == 0) {
		for (int i = 0; i < H; i++) {
			int a, b, c;
			fin >> a >> b >> c;
			h_words.push_back(word(a, b, c));
			letters += (c - b + 1);
			char buff[100];
			fin.getline(buff, 100);
		}
	}
	else {
		for (int i = 0; i < V; i++) {
			int a, b, c;
			fin >> a >> b >> c;
			v_words.push_back(word(c, a, b));
			letters += (b - a + 1);
			char buff[100];
			fin.getline(buff, 100);
		}
	}
}

bool sort_function(word w1, word w2){
	return (w1.constant < w2.constant);
}

void input() {
	fin.open("input.txt");
	if (fin.is_open()) {
		fin >> H >> V;
		char buff[100];
		fin.getline(buff, 100);

		read_words(0);
		read_words(1);
	}
	sort(h_words.begin(), h_words.end(), sort_function);
	sort(v_words.begin(), v_words.end(), sort_function);
	fin.close();
}

bool in_range(int a, int b, int x){
	if ((x >= a ) && (x <= b ))
		return true;
	else
		return false;
}

//param=0 for horizontal words, param=1 for vertical words
void make_edge(vertex s, vertex t, int param){
	int weight = -1;
	if (param == 0)
		weight =abs( t.y - s.y) - 1;
	else
		weight = abs(t.x - s.x) - 1;

	pair<int, int> v = make_pair(s.num, t.num);
	pair<int, pair<int, int>> edge = make_pair(weight, v);
	capacity_of_graph += weight;
	g.push_back(edge);
}

void make_graph(){
	vector<vertex> start_of_edge;
	vertex temp = vertex(-1, -1, -1, -1, -1);
	start_of_edge.assign(V, temp);

	for (int i = 0; i < H; i++){		
		for (int j = 0; j < V; j++){
			
			bool first = in_range(h_words[i].a, h_words[i].b, v_words[j].constant);
			bool second = in_range(v_words[j].a, v_words[j].b, h_words[i].constant);
			if (first && second){
				vertex new_vertex = vertex(h_words[i].constant, v_words[j].constant, num_of_vertices, i, j);

				if (!vertices.empty()){
					vertex v = vertices[vertices.size() - 1];
					if (v.h_num == new_vertex.h_num)
						make_edge(v, new_vertex, 0);
				}
				vertices.push_back(new_vertex);

				if (start_of_edge[j].v_num==new_vertex.v_num){
					vertex v1 = start_of_edge[j];
					make_edge(v1, new_vertex, 1);
				}
				start_of_edge[j] = new_vertex;
				num_of_vertices++;
			}
		}
	}
}

int dsu_get(int v) {
	return (v == p[v]) ? v : (p[v] = dsu_get(p[v]));
}

void dsu_unite(int a, int b) {
	a = dsu_get(a);
	b = dsu_get(b);
	if (rand() & 1)
		swap(a, b);
	if (a != b)
		p[a] = b;
}

void Kruskal(){
	make_graph();

	int m = g.size();
	int n = vertices.size();
	int cost = 0;
	int answer = 0;
	vector < pair<int, int> > res;
	ofstream fout;
	fout.open("output.txt");
	sort(g.begin(), g.end());
	p.resize(n);

	for (int i = 0; i<n; ++i)
		p[i] = i;

	for (int i = 0; i<m; ++i) {
		int a = g[i].second.first, b = g[i].second.second, l = g[i].first;
		if (dsu_get(a) != dsu_get(b)) {
			cost += l;
			res.push_back(g[i].second);
			dsu_unite(a, b);
		}
		else
		if (l == 0){
			fout << -1;
			fout.close();
			return;
		}
	}
	answer = letters - vertices.size() - (capacity_of_graph - cost);
	fout<<answer;
	fout.close();
}

int main() {
	input();
	Kruskal();
	return 0;
}