#pragma once
#include <map>
#include <set>
#include <vector>
#include <random>
#include <tuple>


using namespace std;

class myPlane {
	map<int, int> bMap;
	vector<vector<tuple<int, int>>> edges; //from , <to , value>
	map<tuple<int, int>, int> edgesValues;
	map<tuple<int, int>, double> pheromones;
	std::default_random_engine re;
	const double BETA = 1;
	const double ALPHA = 1;

public:
	int getNextVertex(int vertex, const std::set<int>& missingVert);

	void updatePheromons(const std::vector<int>& path, double Ro, double Q);

	myPlane(double Alpha, double Beta);

	int getValue(int from, int to);

	double getPheromons(int from, int to);

	void setPheromons(int from, int to, double value);

	void insEdge(int from, int to, int value);

	void WA();

	set<int> getVertexes();
};