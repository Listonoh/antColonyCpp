#pragma once
#include "myPlane.h"
#include <map>
#include <random>
#include <iostream>

vector<int> myPlane::getAdjVal(int vertex) {
	auto k = vector<int>();
	k.emplace_back(vertex);
	return k;
};

myPlane::myPlane(double Alpha, double Beta) {
	edges = vector<vector<tuple<int, int>>>();
	ALPHA = Alpha, BETA = Beta;
};

int myPlane::getValue(int from, int to) {
	if (from > to) {
		int temp = to;
		to = from;
		from = temp;
	}
	return edgesValues[make_tuple(from, to)];
};

double myPlane::getPheromons(int from, int to) {
	if (from > to) {
		int temp = to;
		to = from;
		from = temp;
	}
	return pheromones[make_tuple(from, to)];
};

void myPlane::setPheromons(int from, int to, double value) {
	if (from > to) {
		int temp = to;
		to = from;
		from = temp;
	}
	pheromones[make_tuple(from, to)] = value;
};


void myPlane::updatePheromons(const vector<int>& path, double Rho, double Q) {
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

void myPlane::insEdge(int from, int to, int value) {
	if (bMap[from] == 0)
	{
		bMap[from] = static_cast<int>(bMap.size());
	};

	if (bMap[to] == 0)
	{
		bMap[to] = static_cast<int>(bMap.size());
	};
	auto nFrom = bMap[from]-1;//because we start at 0->1 and we want 0->0 this is only here and have no impact later
	auto nTo = bMap[to]-1;

	if (nFrom != nTo) {
		if (edges.size() <= nFrom)
		{
			edges.emplace_back(vector<tuple<int, int>>());
		}
		if (edges.size() <= nTo)
		{
			edges.emplace_back(vector<tuple<int, int>>());
		}

		edges[nFrom].emplace_back(make_tuple(nTo, value));
		edges[nTo].emplace_back(make_tuple(nFrom, value));

		if (nFrom > nTo) {
			int temp = nFrom;
			nFrom = nTo;
			nTo = temp;
		};

		edgesValues[make_tuple(nFrom, nTo)] = value; // from < to
		pheromones[make_tuple(nFrom, nTo)] = 1;
	}
};

//write all
void myPlane::WA() {
	for (size_t i = 0; i < edges.size(); i++)
	{
		std::cout << i << ":  ";
		for (auto const& point : edges[i]) {
			cout << "to: " << std::get<0>(point) << ", val :" << std::get<1>(point) << " | ";
		}
		cout << std::endl;
	}
};


int myPlane::getNextVertex(int vertex, const set<int>& missingVert) {
	int nPosib = static_cast<int>(edges[vertex].size());
	vector<int> trgVert(nPosib);
	vector<double> prob(nPosib);
	double sigma = 0; //all probabilities at start 0
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

		double ETAij = static_cast<double>(pow(static_cast<double>(1) / value, BETA));
		double TAUij = static_cast<double>(pow(getPheromons(vertex, to), ALPHA));

		prob[i] = ETAij * TAUij;
		trgVert[i] = to;
		sigma += prob[i];
		i++;
	}

	//if no edge is selected we choose randomly from the rest
	if (sigma == 0) {
		i = 0;
		nPosib = static_cast<int>(edges[vertex].size());
		for (auto const& item : edges[vertex]) {
			auto to = std::get<0>(item);
			auto value = std::get<1>(item);

			double ETAij = static_cast<double>(pow(static_cast<double>(1) / value, BETA));
			double TAUij = static_cast<double>(pow(getPheromons(vertex, to), ALPHA));

			prob[i] = ETAij * TAUij;
			trgVert[i] = to;
			sigma += prob[i];
			i++;
		}
	}

	double upper_bound = sigma;
	double a_random_double = (upper_bound) * (static_cast<double>(rand()) / RAND_MAX);

	for (int j = 0; j < nPosib; j++)
	{
		a_random_double -= prob[j];
		if (a_random_double <= 0) {
			return trgVert[j];
		};
	}

	return get<0>(edges[vertex][nPosib]);
};


set<int> myPlane::getVertexes() {
	set<int> ret;
	for (int i = 0; i < edges.size(); i++)
	{
		ret.insert(i);
	}
	return ret;
};
