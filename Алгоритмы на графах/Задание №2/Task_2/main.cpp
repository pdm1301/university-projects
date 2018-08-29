#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;

const int MAXN = 100;
int N, X1, Y1, X2, Y2;

struct Vertex {
	int x1, y1, x2, y2;
	Vertex() {
		x1 = y1 = x2 = y2 = 0;
	}
	Vertex(int arg1, int arg2, int arg3, int arg4) {
		x1 = arg1;
		y1 = arg2;
		x2 = arg3;
		y2 = arg4;
	}

	bool operator == (Vertex const & a) {
		if ((x1 == a.x1) && (x2 == a.x2) && (y1 == a.y1) && (y2 == a.y2))
			return true;
		else
			return false;
	}
};
Vertex start;
queue<Vertex> Q;
vector< vector<int> > maze1;
vector< vector<int> > maze2;
vector<int> dist,used;
int num = 0;
ifstream fin;

int getVertexNumber(Vertex v) {
	int res = v.x1*N*N*N + v.y1*N*N + v.x2*N + v.y2;
	return res;
}

bool canGoToCell(int x, int y, int numOfMaze) {
	bool inRange = (x < N) && (x >-1) && (y < N) && (y >-1);
	if (inRange){
		if (numOfMaze == 1){
			if (!maze1[x][y])
				return 1;
			else
				return 0;
		}
		else{
			if (!maze2[x][y])
				return 1;
			else
				return 0;
		}
	}
	return 0;
}

Vertex goToCell(Vertex v,int delta1,int delta2){
	int x1 = v.x1;
	int x2 = v.x2;
	int y1 = v.y1;
	int y2 = v.y2;

	if (canGoToCell(x1 + delta1, y1 + delta2, 1)){
		x1 += delta1;
		y1 += delta2;
	}
	if (canGoToCell(x2 + delta1, y2 + delta2, 2)){
		x2 += delta1;
		y2 += delta2;
	}
		
	Vertex temp = Vertex(x1, y1, x2, y2);
	return temp;
}

void fillMaze(int numberOfMaze){
	char buff[100];
	fin.getline(buff, 100);

	if (numberOfMaze == 1){
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++) {
				int temp;
				fin >> temp;
				maze1[i].push_back(temp);
			}
	}
	else{
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++) {
				int temp;
				fin >> temp;
				maze2[i].push_back(temp);
			}
	}
}
void readVertices(){
	
	fin.open("input.txt");
	if (fin.is_open()) {
		
		fin >> N >> X1 >> Y1 >> X2 >> Y2;
		start = Vertex(X1, Y1, X2, Y2);

		vector<int> vect;
		maze1.assign(N, vect);
		maze2.assign(N, vect);

		fillMaze(1);
		fillMaze(2);

		fin.close();
	}
}
void addVertexToQueue(Vertex v, Vertex to){
	used[getVertexNumber(to)] = 1;
	Q.push(to);
	dist[getVertexNumber(to)] = dist[getVertexNumber(v)] + 1;
	num++;
}

void bfs(){
	dist.assign(N*N*N*N, 0);
	used.assign(N*N*N*N, 0);
	
	Q.push(start);
	used[getVertexNumber(start)] = 1;
	dist[getVertexNumber(start)] = 0;
	while (!Q.empty()){
		Vertex v = Q.front();
		Q.pop();
		int delta1[] = { -1, 1, 0, 0 };
		int delta2[] = { 0, 0, -1, 1 };
		for (int i = 0; i < 4; i++){
			Vertex to = goToCell(v,delta1[i],delta2[i]);
			if (!used[getVertexNumber(to)])
				addVertexToQueue(v, to);
		}
	}
}


void solveTask(){
	bfs();
	Vertex target = Vertex(0, 0, 0, 0);
	int temp = getVertexNumber(target);
	cout << dist[temp];
}
int main(){
	readVertices();
	solveTask();
	system("pause");
	return 0;
}