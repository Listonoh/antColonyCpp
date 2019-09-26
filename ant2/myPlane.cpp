#pragma once
#include "myPlane.h"
#include <map>
#include <random>
#include <iostream>
#include <algorithm>

MyPlane::MyPlane(double Alpha, double Beta) : ALPHA{ Alpha }, BETA{ Beta } {
	edges = vector<vector<tuple<int, int>>>();
}

int MyPlane::getValue(int from, int to) {
	if (from > to) {
		int temp = to;
		to = from;
		from = temp;
	}
	return edgesValues[make_tuple(from, to)];
}

double MyPlane::getPheromons(int from, int to) {
	if (from > to) {
		int temp = to;
		to = from;
		from = temp;
	}
	return pheromones[make_tuple(from, to)];
}

void MyPlane::setPheromons(int from, int to, double value) {
	if (from > to) {
		int temp = to;
		to = from;
		from = temp;
	}
	pheromones[make_tuple(from, to)] = value;
}

void MyPlane::updatePheromons(const vector<int>& path, double Rho, double Q) {
	//Rho - evaporating coefficient
	//Q - coefficient for increasing pheromones

	//evaporating of pheromones
	for (auto& item : pheromones) { //for c11
		item.second *= (1 - Rho);
	}

	int b, a = path[0];
	tuple<int, int> edge;
	//increasing pheromones based on path
	for (size_t i = 1; i < path.size(); i++) {
		b = path[i];
		if (a > b) {
			edge = make_tuple(b, a);
		}
		else {
			edge = make_tuple(a, b);
		}

		pheromones[edge] += Q / edgesValues[edge];
		a = b;
	}
}

void MyPlane::insEdge(int from, int to, int value) {
	if (bMap[from] == 0) {
		bMap[from] = static_cast<int>(bMap.size());
	};

	if (bMap[to] == 0) {
		bMap[to] = static_cast<int>(bMap.size());
	};
	auto nFrom = bMap[from]-1;//because we start at 0->1 and we want 0->0 this is only here and have no impact later
	auto nTo = bMap[to]-1;

	if (nFrom != nTo) {
		if (edges.size() <= nFrom) {
			edges.emplace_back(vector<tuple<int, int>>());
		}
		if (edges.size() <= nTo){
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
}

//write all
void MyPlane::WA() {
	for (size_t i = 0; i < edges.size(); i++) {
		std::cout << i << ":  ";
		for (auto const& point : edges[i]) {
			cout << "to: " << std::get<0>(point) << ", val :" << std::get<1>(point) << " | ";
		}
		cout << std::endl;
	}
}


int MyPlane::getNextVertex(int vertex, const vector<int>& missingVert) {
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

		if (std::binary_search(missingVert.begin(), missingVert.end(), to)) {
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

	double upperBound = sigma;
	double aRandomDouble = (upperBound) * (static_cast<double>(rand()) / RAND_MAX);

	for (int j = 0; j < nPosib; j++) {
		aRandomDouble -= prob[j];
		if (aRandomDouble <= 0) {
			return trgVert[j];
		};
	}

	return get<0>(edges[vertex][0]);
}


vector<int> MyPlane::getVertexes() {
	vector<int> ret;
	for (int i = 0; i < edges.size(); i++) {
		ret.emplace_back(i);
	}
	return ret;
}
