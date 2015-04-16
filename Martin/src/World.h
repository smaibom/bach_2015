#ifndef WORLD_H
#define WORLD_H

#include "Node.h"
#include <list>
#include <iostream>
#include <memory>

struct Counter{
	int num;
};


struct World{
	std::list<std::shared_ptr<Node>> nodes;
	std::shared_ptr<Counter> count;
	std::shared_ptr<Node> start;
	std::shared_ptr<Node> end;
};



#endif
