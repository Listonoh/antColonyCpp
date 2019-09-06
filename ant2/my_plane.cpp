#pragma once
#include "my_plane.h"
#include <map>
#include <random>
#include <iostream>

vector<int> my_plane::getAdjVal(int vertex) {
	auto k = vector<int>();
	k.emplace_back(vertex);
	return k;
};

my_plane::my_plane(const double Alpha, const double Beta) {
	edges = map<int, vector<tuple<int, int>>>();
	ALPHA = Alpha, BETA = Beta;
};

int my_plane::getValue(int from, int to) {
	if (from > to) {
		int temp = to;
		to = from;
		from = temp;
	}
	return edgesValues[make_tuple(from, to)];
};

double my_plane::getPheromons(int from, int to) {
	if (from > to) {
		int temp = to;
		to = from;
		from = temp;
	}
	return pheromones[make_tuple(from, to)];
};

void my_plane::setPheromons(int from, int to, double value) {
	if (from > to) {
		int temp = to;
		to = from;
		from = temp;
	}
	pheromones[make_tuple(from, to)] = value;
};


void my_plane::updatePheromons(vector<int>& path, double Rho, double Q) {
	//Rho - evaporating coefficient
	//Q - coefficient for increasing pheromones

	set<tuple<int, int>> mSet;
	//filing path

	int b, a = path[0];
	tuple<int, int> ins;
	for (size_t i = 1; i < path.size(); i++)
	{
		b = path[i];
		if (a > b) {
			ins = make_tuple(b, a);
		}
		else {
			ins = make_tuple(a, b);
		}

		mSet.insert(ins);
		a = b;
	}

	//evaporating of pheromones
	for (auto& item : pheromones) { //for c11
		item.second *= (1 - Rho);
	}

	//increasing pheromones based on path
	std::set<tuple<int, int>>::iterator it = mSet.begin();
	while (it != mSet.end()) {
		pheromones[*it] += Q / edgesValues[*it];
		it++;
	}
};

void my_plane::insEdge2(int from, int to, int value) {
	if (from != to) {
		edges[from].emplace_back(make_tuple(to, value));
		edges[to].emplace_back(make_tuple(from, value));

		if (from > to) {
			int temp = from;
			from = to;
			to = temp;
		};

		edgesValues[make_tuple(from, to)] = value; // from < to 
		pheromones[make_tuple(from, to)] = 1;
	}
};

//write all
void my_plane::WA() {
	for (auto const& edge : edges) {
		std::cout << edge.first << ":  ";
		for (auto const& point : edge.second) {
			cout << "to: " << std::get<0>(point) << ", val :" << std::get<1>(point) << " | ";
		}
		cout << std::endl;
	}
};


int my_plane::getNextVertex(int vertex, set<int>& missingVert) {
	int nPosib = edges[vertex].size();
	vector<int> trgVert(nPosib);
	vector<double> prob(nPosib);
	double sigma = 0; //all probabilities
	int i = 0; //counter

	//initial search for possible edges
	for (auto const& item : edges[vertex]) {
		auto to = std::get<0>(item);
		auto value = std::get<1>(item);
		//if is not in missing then skip him
		if (missingVert.find(to) == missingVert.end()) {
			nPosib--;
			continue;
		}

		double ETAij = (double)pow((double)1 / value, BETA);
		double TAUij = (double)pow(getPheromons(vertex, to), ALPHA);

		prob[i] = ETAij * TAUij;
		trgVert[i] = to;
		sigma += prob[i];
		i++;
	}

	//if no edge is selected we choose randomly from the rest
	if (sigma == 0) {
		i = 0;
		nPosib = edges[vertex].size();
		for (auto const& item : edges[vertex]) {
			auto to = std::get<0>(item);
			auto value = std::get<1>(item);

			double ETAij = (double)pow((double)1 / value, BETA);
			double TAUij = (double)pow(getPheromons(vertex, to), ALPHA);

			prob[i] = ETAij * TAUij;
			trgVert[i] = to;
			sigma += prob[i];
			i++;
		}
	}

	double upper_bound = sigma;
	double a_random_double = (upper_bound) * ((double)rand() / (double)RAND_MAX);

	for (int i = 0; i < nPosib; i++)
	{
		a_random_double -= prob[i];
		if (a_random_double <= 0) {
			return trgVert[i];
		};
	}

	return get<0>(edges[vertex][nPosib]);
};


set<int> my_plane::getVertexes() {
	set<int> ret;
	for (auto const& edge : edges)
	{
		auto key = edge.first;
		ret.insert(key);
	}
	return ret;
};
