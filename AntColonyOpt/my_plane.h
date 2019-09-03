#pragma once
#include <map>
#include <set>
#include <vector>
#include <random>
#include <tuple>


using namespace std;

class my_plane {
	map<int, vector<tuple<int, int>>> edges; //from , <to , value>
	map<tuple<int, int>, int> edgesValues;
	map<tuple<int, int>, double> pheromones;
	std::default_random_engine re;
	double BETA = 1;
	double ALPHA = 1;

	vector<int> getAdjVal(int vertex);

public:
	int getNextVertex(int vertex, std::set<int>& missingVert);

	void updatePheromons(std::vector<int>& path, double Ro, double Q);

	my_plane(double Alpha, double Beta);

	int getValue(int from, int to);

	double getPheromons(int from, int to);

	void setPheromons(int from, int to, double value);

	void insEdge2(int from, int to, int value);

	void WA();

	set<int> getVertexes();
};