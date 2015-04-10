#ifndef MODEL_H
#define MODEL_H

#include "World.h"
#include "Node.h"

Node* create_node(World *world);
World* add_or(World *lworld, World *rworld);
World* add_plus(World* world);
World* add_question(World* world);
World* add_star(World* world);
World* connect(World* first, World* last);
World* new_world(char chr, Counter *count);

#endif
