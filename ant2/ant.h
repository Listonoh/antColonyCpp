#pragma once
#include <vector>
#include "my_plane.h"

using namespace std;

class Ant {
	my_plane* pl;

public:
	Ant(my_plane* plan);

	tuple<int, vector<int>> findPath();
};
