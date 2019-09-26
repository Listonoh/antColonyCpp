#pragma once
#include <vector>
#include "myPlane.h"
#include "myPath.h"

class Ant {
	MyPlane& pl;

public:
	Ant(MyPlane& plan);
	MyPath findPath();
};