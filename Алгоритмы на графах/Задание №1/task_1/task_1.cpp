#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;


const int MAXN = 1000;
vector< vector<int> > g;
bool used[MAXN],exit_dfs;          //���������� exit_dfs ����� ��� ������ �� ���� ����������� ������� dfs ��� ���������� �����
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
					if (g[to].size() > 1) {   //��������� ���� ������,��� "����" ��������� ������� �������
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
				IS_BRIDGE(v1, v2);   //��� ��� ��� �������� ����� (v1,v2) ����� ����,�� ���������� ��� ������� � �����
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
			int count;                      //count - ���-�� ������ ��������� � i-��
			fin >> count;
			vector<int> vect;
			for (int i = 0; i < count; i++) { // ��������� count ������ �� �����
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
			int v2 = g[v1][0];                            //������� ����� ����������� ������� v1 � v2
			if ((g[v1].size()>1) && (g[v2].size()>1) && (v1<v2)) {   //���� � ������� v1 ��� v2 ������ 1 �����,�� ������ ����� ������� �����,
				vector<int>::iterator it = find(g[v2].begin(), g[v2].end(), v1);
				if (it != g[v2].end()) {
					g[v1].erase(g[v1].begin());               //������� �� ����� ����������� ���������
					g[v2].erase(it);
					exit_dfs = false;
					find_bridges(v1, v2);                           //���� �����
					g[v1].push_back(v2);                  //��������������� ����� ������� v1 � v2
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