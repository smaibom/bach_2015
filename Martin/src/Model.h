#ifndef MODEL_H
#define MODEL_H

#include "World.h"
#include "Node.h"
#include <memory>

std::shared_ptr<Node> create_node(std::shared_ptr<World> world);
std::shared_ptr<World> add_or(std::shared_ptr<World> lworld, std::shared_ptr<World> rworld);
std::shared_ptr<World> add_plus(std::shared_ptr<World> world);
std::shared_ptr<World> add_question(std::shared_ptr<World> world);
std::shared_ptr<World> add_star(std::shared_ptr<World> world);
std::shared_ptr<World> connect(std::shared_ptr<World> first, std::shared_ptr<World> last);
std::shared_ptr<World> new_world(char chr, std::shared_ptr<Counter> count);

#endif
