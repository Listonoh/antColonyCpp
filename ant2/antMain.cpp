#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include "MyPlane.h"
#include "Ant.h"
#include "MyPath.h"
#include <windows.h>
#include <experimental/filesystem>

using namespace std;

bool is_optimal(const int value, const int iteration, const int max_iteration, int& m_value, int& m_counter) {
	if (iteration > max_iteration) {
		//check if it doesn't exceed maximal iterations
		return true;
	}

	if (m_value == -1) {
		//if there is no optimal Value just take current
		m_value = value;
		return false;
	}

	if (m_value < value) {
		m_counter = 0;
		m_value = value;
		return false;
	}

	m_counter++;
	if (m_counter >= 2) {
		return true;
	}
	return false;
}

MyPath ant_colony_tsp(MyPlane& plan, int max_iteration, double rho, double Q, int n = 20) {
	const auto ant1 = Ant(plan);
	auto best_result = ant1.findPath();
	auto iteration = 0;
	auto m_value = -1;
	auto m_counter = 0;

	while (!is_optimal(best_result.value, iteration, max_iteration, m_value, m_counter)) {
		iteration++;
		cout << "*";
		for (auto i = 0; i < n; i++) {
			const auto result = ant1.findPath();
			cout << "-";
			if (result.value <= best_result.value) best_result = result;
		}
		plan.updatePheromones(best_result.vertexes, rho, Q);
	}
	return best_result;
}

double try_parse_string_to_double(const string& input, const double base, const string& arg_name) {
	try {
		return stod(input);
	}
	catch (const std::exception&) {
		cout << "neplatna hodnota argumentu: " << arg_name << "hodnota" << input << "\n";
	}
	return base;
}

int try_parse_string_to_int(const string& input, const int base, const string& arg_name) {
	try {
		return stoi(input);
	}
	catch (const std::exception&) {
		cout << "neplatna hodnota argumentu: " << arg_name << "hodnota" << input << "\n";
	}
	return base;
}

int main(int argc, char* argv[]) {
	srand(static_cast<int>(std::time(nullptr))); // new random

	if (argc < 2) {
		cout << "usage: please write input file \n"
			<< "example: '" << argv[0] << "' a.in" << "\n";
		return 1;
	}

	// main variables for algorithm
	double alpha = 1;
	double beta = 1;
	auto rho = 0.01;
	double Q = 2;
	auto max = 100;
	//

	auto index = 2;
	while (index + 1 < argc) {
		string arg_name = argv[index];
		string arg_value = argv[index + 1];
		index += 2;
		if (arg_name == "--alpha") {
			alpha = try_parse_string_to_double(arg_value, alpha, arg_name);
		}
		else if (arg_name == "--beta") {
			beta = try_parse_string_to_double(arg_value, beta, arg_name);
		}
		else if (arg_name == "--rho") {
			rho = try_parse_string_to_double(arg_value, rho, arg_name);
		}
		else if (arg_name == "--Q") {
			Q = try_parse_string_to_double(arg_value, Q, arg_name);
		}
		else if (arg_name == "--max") {
			max = try_parse_string_to_int(arg_value, max, arg_name);
		}
		else {
			cout << "Neplatny argument: " << arg_name << "\n";
		}
	}

	string file = argv[1];

	ifstream input(file);

	if (!input) {
		cout << "Unable to open file: " << file;
		return 1;
	}

	auto mp = MyPlane(alpha, beta);

	int from, to, value;

	while (input >> from >> to >> value) {
		mp.insEdge(from, to, value);
	}

	cout << "Before solving \n";
	auto best_solution = ant_colony_tsp(mp, max, rho, Q);
	cout << "\nAfter solving \n";
	auto best_path = best_solution.vertexes;

	cout << "final size: " << best_path.size() << "\n";
	cout << "final value: " << best_solution.value << "\n";
	for (auto i : best_path) {
		cout << i << " ";
	}
	cout << "\n";

	return 0;
}
