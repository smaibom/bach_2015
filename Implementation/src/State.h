#ifndef STATE_H
#define STATE_H

#include "Node.h"
#include <memory>

/*
The state class, holds a node, the remaining number if insertions, deletions
and mutations allowed, and the number of matches that lead to the state.
*/
class State{
	public:
		std::shared_ptr<Node> node;
		int deletions;
		int mutations;
		int insertions;
		int path;
};
#endif
