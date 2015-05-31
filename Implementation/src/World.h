#ifndef WORLD_H
#define WORLD_H

#include "Node.h"
#include <list>
#include <iostream>
#include <memory>

/* Simple counter structure, keeps track of nodes being created in Model.cpp */
struct Counter{
    int num;
};

/* A World is a full NFA by itself, with nodes, start and end nodes */
struct World{
    std::list<std::shared_ptr<Node>> nodes;
    std::shared_ptr<Counter> count;
    std::shared_ptr<Node> start;
    std::shared_ptr<Node> end;
};
#endif