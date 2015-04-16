#ifndef NODE_H
#define NODE_H

#include <map>
#include <list>
#include <memory>

struct Node{
	int value;
	std::map<char, std::shared_ptr<Node>> nexts;
	std::list<std::shared_ptr<Node>> epsilons;
};

#endif
