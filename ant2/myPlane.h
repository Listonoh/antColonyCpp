#pragma once
#include <map>
#include <set>
#include <vector>
#include <random>
#include <tuple>


using namespace std;

class MyPlane {
	map<int, int> b_map;
	vector<vector<tuple<int, int>>> edges; //from , <to , value>
	map<tuple<int, int>, int> edges_values;
	map<tuple<int, int>, double> pheromones;
	std::default_random_engine re;
	const double beta = 1;
	const double alpha = 1;

public:
	int getNextVertex(int vertex, const std::vector<int>& missing_vertex);

	void updatePheromones(const std::vector<int>& path, double ro, double Q);

	MyPlane(double alpha, double beta);

	int getValue(int from, int to);

	double getPheromones(int from, int to);

	void setPheromones(int from, int to, double value);

	void insEdge(int from, int to, int value);

	void WA();

	vector<int> getVertexes() const;
};
