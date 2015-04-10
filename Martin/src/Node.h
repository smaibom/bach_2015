#ifndef NODE_H
#define NODE_H

#include <map>
#include <list>
#include <sstream>

struct Node{
	int value;
	std::map<char, Node*> nexts;
	std::list<Node*> epsilons;
};

#endif
