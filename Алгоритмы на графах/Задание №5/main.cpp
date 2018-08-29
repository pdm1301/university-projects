#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int N;
int num_of_vertices = 0;
int houses[] = { -1, -1 };
const double INF = 1000000;
const double precision = 0.0001;

struct Point{
	double x;
	double y;
	Point(){
		this->x = 0;
		this->y = 0;
	}
	Point(double x, double y){
		this->x = x;
		this->y = y;
	}
};

struct Line{
	Point first;
	Point second;
	int A;
	int B;
	int C;

	Line(Point f, Point s){
		this->first = f;
		this->second = s;
		this->A = f.y - s.y;
		this->B = s.x - f.x;
		this->C = f.x*s.y - s.x*f.y;
	}
};

struct Cross{
	Point point;
	int line_num;

	Cross(Point p,int num){
		this->point = p;
		this->line_num = num;
	}
};

struct Vertex{
	Point begin, end;
	int num;

	Vertex(Point b, Point e, int n){
		this->begin = b;
		this->end = e;
		this->num = n;
	}
};

vector<Line> lines;
Point vintik, shpuntik;
vector<vector<Cross>> crosses;
vector<vector<Vertex>> vertices;
vector<vector<pair<int, double>>> g;   //int-number of vertex, double-angle


void input_data(){
	ifstream fin;
	fin.open("input.txt");
	fin >> N;

	for (int i = 0; i < N; i++){
		Point f, s;
		fin >> f.x >> f.y >> s.x >> s.y;
		if ((f.x > s.x) || ((f.x == s.x) && (f.y>s.y)))
			lines.push_back(Line(s, f));
		else
			lines.push_back(Line(f, s));
	}
	fin >> vintik.x >> vintik.y >> shpuntik.x >> shpuntik.y;
	fin.close();
}

bool in_range(double a, double b, double x){
	if (b<a)
		return (b <= x) && (x <= a);
	else
		return (a <= x) && (x <= b);
}

bool equals(double x, double y){
	return (abs(x - y) < precision);
}

bool can_make_cross(int i, int j, double& x, double& y){
	int d, d1, d2;
	d = lines[i].A*lines[j].B - lines[i].B*lines[j].A;

	if (d!=0){
		d1 = -(lines[i].C*lines[j].B - lines[i].B*lines[j].C);
		d2 = -(lines[i].A*lines[j].C - lines[i].C*lines[j].A);

		x = (double) d1 / d;
		y = (double) d2 / d;

		if ((in_range(lines[i].first.x, lines[i].second.x, x)) && (in_range(lines[i].first.y, lines[i].second.y, y)) && (in_range(lines[j].first.x, lines[j].second.x, x)) && (in_range(lines[j].first.y, lines[j].second.y, y)))
			return true;
	}
	return false;
}

bool sort_function(const Cross& c1, const Cross& c2){
	if (abs(c1.point.x - c2.point.x) < precision)
		return (c1.point.y < c2.point.y);
	return (c1.point.x < c2.point.x);
}

void findCrosses(){
	vector<Cross> v;
	crosses.assign(N, v);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (j != i) {
				double x, y;
				if (can_make_cross(i, j, x, y))
					crosses[i].push_back(Cross(Point(x, y), j));
			}

	for (int i = 0; i < crosses.size(); i++)
		sort(crosses[i].begin(), crosses[i].end(), sort_function);
}

void add_vertex(Point start, Point finish, int i){
	vertices[i].push_back(Vertex(start, finish, num_of_vertices));
	vertices[i].push_back(Vertex(finish, start, num_of_vertices + 1));
	num_of_vertices += 2;
}

void make_vertices(){
	
	vector<Vertex> v;
	vertices.assign(N, v);

	for (int i = 0; i < crosses.size(); i++){
	
		Point start = lines[i].first;
		Point finish = lines[i].second;
		
		if (!crosses[i].empty()){

			int j = 0;
			Point temp;
			
			while (j < crosses[i].size()){
							
				temp = crosses[i][j].point;
				
				if (!equals(start.x, temp.x) || !equals(start.y, temp.y))
					add_vertex(start, temp, i);
				
				start = temp;
				j++;
			}
			
			//Check that finish of line isn't last cross
			if (!equals(temp.x, finish.x) || !equals(temp.y, finish.y))
				add_vertex(temp, finish, i);
		}
		else
			add_vertex(lines[i].first, finish, i);
	}
}

double find_angle(Line l1, Line l2){
	double Cos = (l1.A*l2.A + l1.B*l2.B) / (sqrt(l1.A*l1.A + l1.B*l1.B)*sqrt(l2.A*l2.A + l2.B*l2.B));
	double angle = 180 - acos(Cos) * 180 / 3.14159; 
	return angle;
}

bool point_on_line(Line l, Point p){
	return (abs(l.A*p.x + l.B*p.y + l.C) < precision);
}

void find_house(Line l,Point house, Vertex v, int param){
	if (point_on_line(l, house) && (in_range(v.begin.x, v.end.x, house.x)) && (in_range(v.begin.y, v.end.y, house.y)))
		houses[param] = v.num;
}

void find_house(Line l, Point house, int i, int param){
	if (point_on_line(l, house) && (in_range(l.first.x, l.second.x, house.x)) && (in_range(l.first.y, l.second.y, house.y)))
		houses[param] = vertices[i][0].num;
}

void build_graph(){
	vector<pair<int, double>> v;
	g.assign(num_of_vertices + 2, v);
	Point house_holders[] = { vintik, shpuntik };
	for (int i = 0; i < N; i++){

		vector<Vertex> v1 = vertices[i];

		if (crosses[i].empty()){   //No crosses,find houses on line
			for (int m = 0; m < 2; m++)
			if (houses[m] < 0)
				find_house(lines[i], house_holders[m], i, m);
		}
		//Connecting vertices on the same line
		for (int k = 2; k < v1.size(); k+=2){
			g[v1[k - 2].num].push_back(make_pair(v1[k].num, 0));
			g[v1[k + 1].num].push_back(make_pair(v1[k - 1].num, 0));
		}
		
		for (int j = 0; j < crosses[i].size(); j++){
			int c_num = crosses[i][j].line_num;
			double angle = find_angle(lines[i], lines[c_num]);
			vector<Vertex> v2 = vertices[c_num];

			for (int l = 0; l < v1.size(); l++){
				int count = 0;
				int t = 0;
				while (t < v2.size()){

					//We can turn in two directions, count is the number of turns found
					if (count == 2)
						break;
					if (equals(v1[l].end.x, v2[t].begin.x) && equals(v1[l].end.y, v2[t].begin.y)){
						if ((!(v1[l].num % 2) && !(v2[t].num % 2)) || ((v1[l].num % 2) && (v2[t].num % 2)))
							g[v1[l].num].push_back(make_pair(v2[t].num, 180 - angle));
						else
							g[v1[l].num].push_back(make_pair(v2[t].num, angle));
						count++;
					}
					t++;
				}

				for (int m = 0; m < 2; m++)
					if (houses[m] < 0)
						find_house(lines[i], house_holders[m], v1[l], m);
			}
		}
	}

	g[num_of_vertices].push_back(make_pair(houses[0], 0));
	g[num_of_vertices].push_back(make_pair(houses[0] + 1, 0));
	g[houses[1]].push_back(make_pair(num_of_vertices + 1, 0));
	g[houses[1] + 1].push_back(make_pair(num_of_vertices + 1, 0));
}

double dijkstra(){
	int n = g.size();
	int s = n - 2;

	vector<double> d(n, INF);
	vector<int> p(n);
	vector<char> u(n);

	d[s] = 0;
	for (int i = 0; i < n; i++)
	{
		int v = -1;
		for (int j = 0; j < n; ++j)
		if (!u[j] && (v == -1 || d[j] < d[v]))
			v = j;

		if (d[v] == INF)
			break;
		u[v] = true;
		for (size_t j = 0; j < g[v].size(); j++)
		{
			int to = g[v][j].first;
			double len = g[v][j].second;
			if (d[v] + len < d[to])
			{
				d[to] = d[v] + len;
				p[to] = v;
			}
		}
	}

	double result = d[n - 1];
	if (result == INF)
		return -1;
	else
		return result;
}

int main(){
	input_data();
	findCrosses();
	make_vertices();
	build_graph();
	
	ofstream fout;
	fout.open("output.txt");

	fout << dijkstra();
	fout.close();
	return 0;
}