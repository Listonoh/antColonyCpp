#include "MyPlane.h"
#include <map>
#include <random>
#include <iostream>
#include <algorithm>

MyPlane::MyPlane(const double alpha, const double beta) : beta{beta}, alpha{alpha} {
}

long long int MyPlane::getKey(const int from, const int to) {
	long long int key = from;
	key = key << 32;
	key += to;
	return key;
}

int MyPlane::getValue(int from, int to) {
	if (from > to) {
		const auto temp = to;
		to = from;
		from = temp;
	}
	return edges_values[getKey(from, to)];
}

double MyPlane::getPheromones(int from, int to) {
	if (from > to) {
		const auto temp = to;
		to = from;
		from = temp;
	}
	return pheromones[getKey(from, to)];
}

void MyPlane::setPheromones(int from, int to, const double value) {
	if (from > to) {
		const auto temp = to;
		to = from;
		from = temp;
	}
	pheromones[getKey(from, to)] = value;
}

void MyPlane::updatePheromones(const vector<int>& path, const double Rho, const double Q) {
	//Rho - evaporating coefficient
	//Q - coefficient for increasing pheromones

	//evaporating of pheromones
	for (auto& item : pheromones) {
		//for c11
		item.second *= 1 - Rho;
	}

	auto from = path[0];
	long long int edge;
	//increasing pheromones based on path
	for (size_t i = 1; i < path.size(); i++) {
		const auto to = path[i];
		if (from > to) {
			edge = getKey(to, from);
		}
		else {
			edge = getKey(from, to);
		}

		pheromones[edge] += Q / edges_values[edge];
		from = to;
	}
}

void MyPlane::insEdge(const int from, const int to, int value) {
	if (b_map[from] == 0) {
		b_map[from] = static_cast<int>(b_map.size());
	}

	if (b_map[to] == 0) {
		b_map[to] = static_cast<int>(b_map.size());
	}
	auto n_from = b_map[from] - 1; //because we start at 0->1 and we want 0->0 this is only here and have no impact later
	auto n_to = b_map[to] - 1;

	if (n_from != n_to) {
		if (edges.size() <= n_from) {
			edges.emplace_back(vector<tuple<int, int>>());
		}
		if (edges.size() <= n_to) {
			edges.emplace_back(vector<tuple<int, int>>());
		}

		edges[n_from].emplace_back(make_tuple(n_to, value));
		edges[n_to].emplace_back(make_tuple(n_from, value));

		if (n_from > n_to) {
			const auto temp = n_from;
			n_from = n_to;
			n_to = temp;
		}

		edges_values[getKey(n_from, n_to)] = value; // from < to
		pheromones[getKey(n_from, n_to)] = 1;
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


int MyPlane::getNextVertex(int vertex, const vector<int>& missing_vertex) {
	auto n_possible = static_cast<int>(edges[vertex].size());
	vector<int> trg_vertex(n_possible);
	vector<double> prob(n_possible);
	double sigma = 0; //all probabilities at start 0
	auto i = 0; //counter

	//initial search for possible edges
	for (auto const& item : edges[vertex]) {
		auto to = std::get<0>(item);
		const auto value = std::get<1>(item);
		//if is not in missing then skip him

		if (!std::binary_search(missing_vertex.begin(), missing_vertex.end(), to)) {
			n_possible--;
			continue;
		}

		const auto ETAij = static_cast<double>(pow(static_cast<double>(1) / value, beta));
		const auto TAUij = static_cast<double>(pow(getPheromones(vertex, to), alpha));

		prob[i] = ETAij * TAUij;
		trg_vertex[i] = to;
		sigma += prob[i];
		i++;
	}

	//if no edge is selected we choose randomly from the rest
	if (sigma == 0) {
		i = 0;
		n_possible = static_cast<int>(edges[vertex].size());
		for (auto const& item : edges[vertex]) {
			const auto to = std::get<0>(item);
			const auto value = std::get<1>(item);

			const auto ETAij = static_cast<double>(pow(static_cast<double>(1) / value, beta));
			const auto TAUij = static_cast<double>(pow(getPheromones(vertex, to), alpha));

			prob[i] = ETAij * TAUij;
			trg_vertex[i] = to;
			sigma += prob[i];
			i++;
		}
	}

	const auto upper_bound = sigma;
	auto a_random_double = upper_bound * (static_cast<double>(rand()) / RAND_MAX);

	for (auto j = 0; j < n_possible; j++) {
		a_random_double -= prob[j];
		if (a_random_double <= 0) {
			return trg_vertex[j];
		}
	}

	return get<0>(edges[vertex][0]);
}


vector<int> MyPlane::getVertexes() const {
	vector<int> ret;
	ret.reserve(edges.size());
	for (auto i = 0; i < edges.size(); i++) {
		ret.emplace_back(i);
	}
	return ret;
}
