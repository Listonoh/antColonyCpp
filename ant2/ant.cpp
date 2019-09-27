#include "ant.h"
#include "myPath.h"

//cant be const because i modify it
Ant::Ant(MyPlane& plan) : pl(plan) {
}

MyPath Ant::findPath() {
	auto missingVertexes = pl.getVertexes();
	int max = static_cast<int>(missingVertexes.size());
	max *= 4; //it shouldn't be more then 2* but its "tree" and tree has n-1 edges

	auto from = missingVertexes[0];
	auto path = vector<int>();
	int location = from;
	path.emplace_back(location);
	missingVertexes.erase(missingVertexes.begin());

	int value = 0;
	int timer = 0;

	// completing route/path
	while (missingVertexes.size() != 0 && timer < max) {
		timer++;

		int nextVertex = pl.getNextVertex(location, missingVertexes);
		value += pl.getValue(location, nextVertex);
		auto earser = std::find(missingVertexes.begin(), missingVertexes.end(), nextVertex);
		if (earser != missingVertexes.end()) {
			missingVertexes.erase(earser);
		};
		path.emplace_back(nextVertex);
		location = nextVertex;
	}

	// returning back home
	missingVertexes.insert(missingVertexes.begin(),from);
	while (location != from && timer < max) {
		timer++;
		int nextVertex = pl.getNextVertex(location, missingVertexes);
		value += pl.getValue(location, nextVertex);
		path.emplace_back(nextVertex);
		location = nextVertex;
	}

	if (timer < max) {
		return MyPath{ value, path };
	}
	else {
		return MyPath{std::numeric_limits<int>::max(), vector<int>{1} };
	}
}