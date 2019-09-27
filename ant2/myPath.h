#ifndef MY_PATH_H
#define MY_PATH_H

#include <vector>

struct MyPath {
	int value = std::numeric_limits<int>::max();
	std::vector<int> vertexes;
};
#endif
