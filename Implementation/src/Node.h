#ifndef NODE_H
#define NODE_H

#include <map>
#include <list>
#include <memory>

struct Node{
	int value;
	char chr;
	std::shared_ptr<Node> next;
    std::shared_ptr<Node> epsilon_left;
    std::shared_ptr<Node> epsilon_right;
};

#endif
