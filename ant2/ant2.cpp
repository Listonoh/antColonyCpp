#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <cmath>
#include <random>
#include <ctime>
#include <climits>
#include <new>
#include "my_plane.h"
#include "ant.h"

using namespace std;

bool isOptimal(int value, int iteration, int max_iteration, int &mValue, int &mCounter) {
	if (iteration > max_iteration){
		return true;
	}

	if (mValue == -1) {
		mValue = value;
		return false;
	}

	if (mValue < value) {
		mCounter = 0;
		mValue = value;
		return false;
	}

	mCounter++;
	if (mCounter >= 2){
		return true;
	}
	return false;
}

tuple<int, vector<int>> AntColonyTSP(my_plane* plan, int max_iteration, double rho, double Q, int n = 20) {
	auto ant1 = Ant(plan);
	auto bestResult = ant1.findPath();
	int iteration = 0;
	int mValue = -1;
	int mCounter = 0;

	while (!isOptimal(get<0>(bestResult), iteration, max_iteration, mValue, mCounter)) {
		iteration++;
		for (int i = 0; i < n; i++) {
			ant1 = Ant(plan);
			auto result = ant1.findPath();
			if (get<0>(result) <= get<0>(bestResult)) bestResult = result;
		}
		plan->updatePheromons(get<1>(bestResult), rho, Q);
	}
	return bestResult;
}



int main(int argc, char* argv[]) {
	std::srand((int)std::time(nullptr)); // new random

	if (argc < 2) {
		cout << "usage: please write input file \n"
			<< "example: '" << argv[0] << "' a.in" << "\n";
		return 1;
	}

	double alfa = 1; double beta = 1; double rho = 0.01; double Q = 2; int max = 100;

	cout << "ok" << argc;
	int index = 2;
	while (index + 1 < argc)
	{
		string arg_name = argv[index];
		string arg_value = argv[index + 1];
		index += 2;
		if (arg_name == "--alfa")
		{
			try
			{
				alfa = stod(arg_value);
			}
			catch (const std::exception&)
			{
				cout << "neplatny hodnota argumentu: " << arg_name << "hodnota" << arg_value << "\n";
			}
		}
		else if (arg_name == "--beta")
		{
			try
			{
				beta = stod(arg_value);
			}
			catch (const std::exception&)
			{
				cout << "neplatny hodnota argumentu: " << arg_name << "hodnota" << arg_value << "\n";
			}
		}
		else if (arg_name == "--rho")
		{
			try
			{
				rho = stod(arg_value);
			}
			catch (const std::exception&)
			{
				cout << "neplatny hodnota argumentu: " << arg_name << "hodnota" << arg_value << "\n";
			}

		}
		else if (arg_name == "--Q")
		{
			try
			{
				Q = stod(arg_value);
			}
			catch (const std::exception&)
			{
				cout << "neplatny hodnota argumentu: " << arg_name << "hodnota" << arg_value << "\n";
			}
		}
		else if (arg_name == "--max")
		{
			try
			{
				max = stoi(arg_value);
			}
			catch (const std::exception&)
			{
				cout << "neplatny hodnota argumentu: " << arg_name << "hodnota" << arg_value << "\n";
			}
		}
		else
		{
			cout << "neplatny argument: " << arg_name << "\n";
		};
	}


	string file = argv[1];
	ifstream inpu(file);

	if (!inpu)
	{
		cout << "Unable to open file: " << file;
		return 1;
	}

	auto mp = new my_plane(alfa, beta);

	int from, to, value;

	while (inpu >> from >> to >> value)
	{
		mp->insEdge2(from, to, value);
	}
	//impu.close();
	//mp.WA();
	cout << "Presolving ";
	auto tK = AntColonyTSP(mp, max, rho, Q);
	cout << "Aftersolving " << "\n";
	auto k = get<1>(tK);

	cout << "final size: " << k.size() << "\n";
	cout << "final value: " << get<0>(tK) << "\n";
	for (size_t i = 0; i < k.size(); i++) {
		cout << k[i] << " ";
	}
	cout << "\n";

	delete mp;
	return 0;
}