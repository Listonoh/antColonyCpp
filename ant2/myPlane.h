#pragma once
#include <vector>
#include <random>
#include <tuple>
#include <unordered_map>


using namespace std;

class MyPlane {
	unordered_map<int, int> b_map;
	vector<vector<tuple<int, int>>> edges; //from , <to , value>
	unordered_map<long long int, int> edges_values;
	unordered_map<long long int, double> pheromones;
	std::default_random_engine re;
	const double beta = 1;
	const double alpha = 1;

public:
	int getNextVertex(int vertex, const std::vector<int>& missing_vertex);

	void updatePheromones(const std::vector<int>& path, double ro, double Q);

	MyPlane(double alpha, double beta);
	static long long int getKey(int from, int to);

	int getValue(int from, int to);

	double getPheromones(int from, int to);

	void setPheromones(int from, int to, double value);

	void insEdge(int from, int to, int value);

	void WA();

	vector<int> getVertexes() const;
};
