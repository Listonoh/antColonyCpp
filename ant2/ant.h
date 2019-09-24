#pragma once
#include <vector>
#include "myPlane.h"
#include "myPath.h"

class Ant {
	myPlane& pl;

public:
	Ant(myPlane& plan);
	myPath findPath();
};