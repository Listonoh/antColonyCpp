#pragma once
#include "myPlane.h"
#include "myPath.h"

class Ant {
	MyPlane& pl;

public:
	explicit Ant(MyPlane& plan);
	MyPath findPath() const;
};
