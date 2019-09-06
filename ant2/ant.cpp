#include "ant.h"


Ant::Ant(my_plane* plan) {
	pl = plan;
};

tuple<int, vector<int>> Ant::findPath() {
	auto missingVertexes = pl->getVertexes();
	int max = static_cast<int>(missingVertexes.size());
	max *= 4; //it shouldn't be more then 2* but its "tree" and tree has n-1 edges

	auto from = *missingVertexes.begin();
	auto path = vector<int>();
	int location = from;
	path.emplace_back(location);
	missingVertexes.erase(from);

	int value = 0;
	int timer = 0;

	// completing route/path
	while (missingVertexes.size() != 0 && timer < max) {
		timer++;

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

	if (timer < max)
	{
		return make_tuple(value, path);
	}
	else {
		return make_tuple(INT_MAX, vector<int>{1});
	}
};

