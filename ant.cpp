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

using namespace std;

class plane {
public:
	plane() {};
	virtual int getNextVertex(int vertex, set<int>& missingVert) = 0;
	virtual void updatePheromons(vector<int>& path, double Ro, double Q) = 0;
};


class my_plane : plane {
	map<int, vector<tuple<int, int>>> edges; //from , <to , value>
	map<tuple<int, int>, int> edgesValues;
	map<tuple<int, int>, double> pheromones;
	std::default_random_engine re;
	double BETA = 1;
	double ALPHA = 1;

	vector<int> getAdjVal(int vertex) {
		auto k = vector<int>();
		k.emplace_back(vertex);
		return k;
	}

public:
	my_plane(double Alpha, double Beta) {
		edges = map<int, vector<tuple<int, int>>>();
		ALPHA = Alpha, BETA = Beta;
		cout << ALPHA;
	};

	int getValue(int from, int to) {
		if (from > to) {
			int temp = to;
			to = from;
			from = temp;
		}
		return edgesValues[make_tuple(from, to)];
	}

	double getPheromons(int from, int to) {
		if (from > to) {
			int temp = to;
			to = from;
			from = temp;
		}
		return pheromones[make_tuple(from, to)];
	}

	void setPheromons(int from, int to, double value) {
		if (from > to) {
			int temp = to;
			to = from;
			from = temp;
		}
		pheromones[make_tuple(from, to)] = value;
	}

	
	void updatePheromons(vector<int>& path, double Ro = 0.01, double Q = 2) override {
		//Ro - evaporating coeficient
		// Q - coeficient for increasing pheromons

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
		//for (auto const& [key, valPhero] : pheromones) {
		for (auto& item : pheromones) { //for c11
			item.second *= (1 - Ro);
		}
		// cout << pheromones[ins] << " this is it \n"; // it works :)

		//increasing pheromones based on path
		std::set<tuple<int, int>>::iterator it = mSet.begin();
		while (it != mSet.end()) {
			pheromones[*it] += Q / edgesValues[*it];
			it++;
		}
	}

	void insEdge2(int from, int to, int value) {
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
	void WA() {
		//for (auto const& [key, val] : edges) { //c++17
		for (auto const& edge : edges) {
			std::cout << edge.first << ":  ";
			for (auto const& point : edge.second) {
				cout << "to: " << std::get<0>(point) << ", val :" << std::get<1>(point) << " | ";
			}
			cout << std::endl;
		}
	};


	int getNextVertex(int vertex, set<int>& missingVert) override {
		int nPosib = edges[vertex].size();
		//cout << nPosib;
		//int* trgVert = new int[nPosib];
		//double *prob = new double[nPosib];
		vector<int> trgVert(nPosib);
		vector<double> prob(nPosib);
		double sigma = 0; //all probabilties 
		int i = 0; //counter

		//initial search for posible edges 
		//for (auto const& [to, value] : edges[vertex]) { // for c++17
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
			// cout << "n";
			i = 0;
			nPosib = edges[vertex].size();
			for (auto const& item : edges[vertex]) {
				auto to = std::get<0>(item);
				auto value = std::get<1>(item);

				double ETAij = (double)pow((double)1 / value, BETA);
				double TAUij = (double)pow(getPheromons(vertex, to), ALPHA);

				// cout << ETAij << "><" << TAUij << "!" << value << " |" ;
				prob[i] = ETAij * TAUij;
				trgVert[i] = to;
				sigma += prob[i];
				i++;
			}
		}

		double upper_bound = sigma;
		// cout << upper_bound << "\n" ;
		// double lower_bound = 0;
		// std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
		// double a_random_double = unif(re);
		double a_random_double = (upper_bound) * ((double)rand() / (double)RAND_MAX);

		//cout << sigma << " " << a_random_double << " " << prob[0] << " " << nPosib << "\t";
		for (int i = 0; i < nPosib; i++)
		{
			a_random_double -= prob[i];
			if (a_random_double <= 0) {
				//cout << " selected: " << i << " from: " << nPosib <<  "\n";
				// return get<0>(edges[vertex][i]);
				// cout << "returning :" <<  trgVert[i] << "\n";
				return trgVert[i];
			};
		}

		//souldnt return here 
		return get<0>(edges[vertex][nPosib]);
	};


	set<int> getVertexes() {
		set<int> ret;
		for (auto const& edge : edges)
		{
			auto key = edge.first;
			ret.insert(key);
		}
		return ret;
	}



};



class ant {
	vector<int> path;
public:
	virtual tuple<int, vector<int>> findPath() = 0;
};

class AntTSP : ant {
	my_plane* pl;
public:

	AntTSP(my_plane* plan) {
		pl = plan;
	};

	tuple<int, vector<int>> findPath() override {
		auto missingVertexes = pl->getVertexes();
		int max = missingVertexes.size();
		max *= 4; //it shouldnt be more then 2* but its "tree" and tree has n-1 edges
		// cout << max << "\n";

		int r = rand() % max;
		auto from = *missingVertexes.begin();
		auto path = vector<int>();
		int location = from;
		path.emplace_back(location);
		missingVertexes.erase(from);

		// cout << "\n count of vertexes: "<< missingVertexes.size() << "\n";

		int value = 0;
		int timer = 0;

		// completing route/path
		while (missingVertexes.size() != 0 && timer < max) {
			timer++;
			// cout <<  " > "<<location << " " << missingVertexes.size() << "\n";
			int nextVertex = pl->getNextVertex(location, missingVertexes);
			value += pl->getValue(location, nextVertex);
			missingVertexes.erase(nextVertex);
			path.emplace_back(nextVertex);
			location = nextVertex;
		}

		// returning back home
		missingVertexes.insert(from);
		while (location != from && timer < max) {
			timer++;
			int nextVertex = pl->getNextVertex(location, missingVertexes);
			value += pl->getValue(location, nextVertex);
			path.emplace_back(nextVertex);
			location = nextVertex;
		}


		// cout << timer << "\n";
		// cout << "size of path: " << path.size() << " value of path: " << value <<" \n" ;
		if (timer < max)
		{
			return make_tuple(value, path);
		}
		else {
			// return make_tuple(value, path);
			return make_tuple(INT_MAX, vector<int>{1});
		}
	};
};


///-------------------main
int mValue = -1;
int mCounter = 0;
bool isOptimal(int value, int iteration, int max_iteration ) {
	if (iteration > max_iteration)
	{
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
	if (mCounter >= 2)
	{
		return true;
	}

	return false;
}

tuple<int, vector<int>> AntColonyTSP(my_plane* plan,  int max_iteration, double rho, double Q, int n = 20) {
	auto ant1 = new AntTSP(plan);
	// tuple<int, vector<int>> bestResult = ant1->findPath();
	//cout << "pre findpath";
	auto bestResult = ant1->findPath();
	//cout << "path found";
	int iteration = 0;

	while (!isOptimal(get<0>(bestResult), iteration, max_iteration)) {
		iteration++;
		//cout << iteration;
		for (int i = 0; i < n; i++) {
			delete(ant1);
			ant1 = new AntTSP(plan);
			auto result = ant1->findPath();
			if (get<0>(result) <= get<0>(bestResult)) bestResult = result;
		}
		plan->updatePheromons(get<1>(bestResult), rho, Q);
	}
	return bestResult;
}



int main(int argc, char* argv[]) {
	std::srand((int)std::time(nullptr)); // new random

	if (argc < 2){
	  cout << "usage: please write input file \n" 
	  << "example: '" << argv[0] << "' a.in" << "\n"; 
	  return 1;
	}

	double alfa = 1; double beta = 1; double rho = 0.01; double Q = 2; int max = 100;

	cout << "ok" << argc;
	int index = 2;
	while (index+1 < argc)
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

	cout << "alfa " << alfa << "\n";

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
	// cout << mp->getAdjVal(1)[0];
}
