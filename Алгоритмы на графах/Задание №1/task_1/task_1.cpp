#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;


const int MAXN = 1000;
vector< vector<int> > g;
bool used[MAXN],exit_dfs;          //переменна€ exit_dfs нужна дл€ выхода из всех рекурсивных вызовов dfs при нахождении моста
int timer, N, tin[MAXN], fup[MAXN];
ofstream fout;

int min(int a, int b) {
	if (a < b)
		return a;
	return b;
}

void IS_BRIDGE(int v, int to){
	fout << v << " " << to << endl;
}

bool dfs(int v, int p = -1) {
	used[v] = true;
	tin[v] = fup[v] = timer++;
	for (size_t i = 0; i<g[v].size(); ++i) {
		int to = g[v][i];
		if (to == p)  continue;
		if (used[to])
			fup[v] = min(fup[v], tin[to]);
		else {
			dfs(to, v);
			if (!exit_dfs){
				fup[v] = min(fup[v], fup[to]);
				if (fup[to] > tin[v]){
					if (g[to].size() > 1) {   //обработка того случа€,что "мост" св€зывает вис€чую вершину
						IS_BRIDGE(v, to);
						exit_dfs = true;
					}
				}
			}
		}
	}
	return false;
}

void find_bridges(int v1, int v2) {
	timer = 0;
	for (int i = 0; i < N; ++i)
		used[i] = false;
	for (int i = 0; i < N; ++i)
		if (!used[i]){
				if (dfs(i)){
				IS_BRIDGE(v1, v2);   //“ак как при удалении ребра (v1,v2) нашли мост,то записываем эти вершины в ответ
				return;
			}
		}
}

void read_graph() {
	ifstream fin;
	fin.open("input.txt");
	if (fin.is_open()) {
		fout.open("output.txt");
		fin >> N;
		char buff[MAXN];
		fin.getline(buff, MAXN);
		for (int i = 0; i < N; i++) {
			int count;                      //count - кол-во вершин св€занных с i-ой
			fin >> count;
			vector<int> vect;
			for (int i = 0; i < count; i++) { // считываем count вершин из файла
				int temp;
				fin >> temp;
				vect.push_back(temp);
			}
			fin.getline(buff, MAXN);
			g.push_back(vect);
		}
	}
	fin.close();
}

void solve_task() {
	exit_dfs = false;
	for (int v1 = 0; v1 < N; v1++) {
		int s = g[v1].size();
		for (int j = 0; j < s; j++) {
			int v2 = g[v1][0];                            //”дал€ем ребро св€зывающее вершины v1 и v2
			if ((g[v1].size()>1) && (g[v2].size()>1) && (v1<v2)) {   //≈сли у вершины v1 или v2 только 1 св€зь,то удалив ребро получим точку,
				vector<int>::iterator it = find(g[v2].begin(), g[v2].end(), v1);
				if (it != g[v2].end()) {
					g[v1].erase(g[v1].begin());               //котора€ не будет компонентой св€зности
					g[v2].erase(it);
					exit_dfs = false;
					find_bridges(v1, v2);                           //»щем мосты
					g[v1].push_back(v2);                  //¬осстанавливаем св€зь вершины v1 и v2
					g[v2].push_back(v1);
					
				}
			}
		}
	}
	fout.close();
}
int main() {
	read_graph();
	solve_task();
	return 0;
}