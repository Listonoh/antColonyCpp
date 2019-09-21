#pragma once
#include <vector>
#include "my_plane.h"
#include "my_path.h"

class Ant {
	my_plane& pl;

public:
	Ant(my_plane& plan);
	my_Path findPath();
};