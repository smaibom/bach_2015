#ifndef WORLD_H
#define WORLD_H

#include "Node.h"
#include <list>
#include <map>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>

struct Counter{
	int num;
};


struct World{
	std::list<Node*> nodes;
	Counter* count;
	Node* start;
	Node* end;
};



#endif
