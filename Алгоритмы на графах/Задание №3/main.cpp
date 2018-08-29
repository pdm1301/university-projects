#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;


const int MAXN = 1000;
vector< vector<int> > g;
vector<int> ans,used;
int N;
ofstream fout;
double p[MAXN][MAXN], a[MAXN][MAXN];
int b[MAXN][MAXN];

void read_input() {
	ifstream fin;
	fin.open("input.txt");
	if (fin.is_open()) {
		fout.open("output.txt");

		fin >> N;
		char buff[100];
		fin.getline(buff, 100);
		for (int i = 0; i < N; i++){
			vector<int> vect;
			g.push_back(vect);
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++){
				b[i][j] = 0;
				fin >> p[i][j];
				if (p[i][j]){
					g[i].push_back(j);
				}
			}
			fin.getline(buff, 100);
		}

	}
	fin.close();
}
void dfs(int v) {
	used[v] = 1;
	for (size_t i = 0; i<g[v].size(); ++i) {
		int to = g[v][i];
		if (!used[to])
			dfs(to);
	}
	ans.push_back(v);
}

void topological_sort() {
	used.assign(N, 0);
	ans.clear();
	for (int i = 0; i<N; ++i)
	if (!used[i])
		dfs(i);
	reverse(ans.begin(), ans.end());
}

void sort_probabilities(){
	for (int i = 0; i < N; i++)
	for (int j = 0; j < N; j++)
		a[i][j] = p[ans[i]][ans[j]];
}

int find_element_in_first_sort(int target){
	int i = 0;
	while (i < ans.size()){
		if (ans[i] == target)
			return i;
		i++;
	}
}

void get_answer(int s, int f){
	vector<int> answer;
	answer.push_back(ans[f]);
	int k = b[s][f];
	while (k != 0){
		answer.push_back(ans[k]);
		int j = k;
		k = b[s][j];
	}
	answer.push_back(ans[s]);

	fout << a[s][f] << " " << answer.size() << endl;
	reverse(answer.begin(), answer.end());
	for (int i = 0; i < answer.size(); i++)
		fout << answer[i] << " ";
	fout.close();
}

void solve_task(){
	int start = find_element_in_first_sort(0);
	int finish = find_element_in_first_sort(N-1);
		for (int k = start + 1; k<N; k++)
			for (int j = k + 1; j<=finish; j++)
				if (a[start][j]<a[start][k] * a[k][j]) {
					a[start][j] = a[start][k] * a[k][j];
					b[start][j] = k;
				}
	get_answer(start, finish);
}

int main() {
	read_input();
	topological_sort();
	sort_probabilities();
	solve_task();
	return 0;
}