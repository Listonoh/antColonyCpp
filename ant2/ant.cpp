#include "Ant.h"
#include "MyPath.h"

//cant be const because i modify it
Ant::Ant(MyPlane& plan) : pl(plan) {
}

MyPath Ant::findPath() const {
	auto missing_vertexes = pl.getVertexes();
	auto max = static_cast<int>(missing_vertexes.size());
	max *= 4; //it shouldn't be more then 2* but its "tree" and tree has n-1 edges

	const auto from = missing_vertexes[0];
	auto path = vector<int>();
	auto location = from;
	path.emplace_back(location);
	missing_vertexes.erase(missing_vertexes.begin());

	auto value = 0;
	auto timer = 0;

	// completing route/path
	while (!missing_vertexes.empty() && timer < max) {
		timer++;

		auto next_vertex = pl.getNextVertex(location, missing_vertexes);
		value += pl.getValue(location, next_vertex);
		auto eraser = std::find(missing_vertexes.begin(), missing_vertexes.end(), next_vertex);
		if (eraser != missing_vertexes.end()) {
			missing_vertexes.erase(eraser);
		};
		path.emplace_back(next_vertex);
		location = next_vertex;
	}

	// returning back home
	missing_vertexes.insert(missing_vertexes.begin(), from);
	while (location != from && timer < max) {
		timer++;
		auto next_vertex = pl.getNextVertex(location, missing_vertexes);
		value += pl.getValue(location, next_vertex);
		path.emplace_back(next_vertex);
		location = next_vertex;
	}

	if (timer < max) {
		return MyPath{value, path};
	}
	else {
		return MyPath{std::numeric_limits<int>::max(), vector<int>{1}};
	}
}
