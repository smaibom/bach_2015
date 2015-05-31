#ifndef NODE_H
#define NODE_H

#include <list>
#include <memory>

/* 
A node is the buildingblock of the NFA, it holds a unique value as key.
The char is the transition value from current node to another node, in next.
Our NFA type can only support two epsilon transitions from any given node.
*/
struct Node{
    int value;
    char chr;
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> epsilon_left;
    std::shared_ptr<Node> epsilon_right;
};

#endif
