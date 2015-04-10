#ifndef STATE_H
#define STATE_H

#include "Node.h"
#include "World.h"
#include <String>
#include <algorithm>



class State{
	public:
		int start;
		Node node;
		std::string path;
};

class States{
	public:
		std::list<State*> states;
		void checkValue(char chr);
		void add_state(Node *node);
};

void States::add_state(Node *node){
	State *state = new State();
	state->node = *node;
	states.push_back(state);
}

void States::checkValue(char chr){
	std::list<State*> new_states;
	while(!states.empty()){
		State *state = states.back();
		states.pop_back();

		if(state->node.value == 3 and state->path != ""){ // Hacky måde, 3 er slutpunktet, taget fra NFA'en
			std::cout << state->path <<  std::endl;
		}

		typedef std::list<Node*> listMyClass;

		for( listMyClass::iterator listMyClassIter = state->node.epsilons.begin();
		    listMyClassIter != state->node.epsilons.end();
		    listMyClassIter ++)
		{
			State *new_state = new State();
			new_state->path = state->path;
			new_state->node = *(*listMyClassIter);
			states.push_front(new_state);
		}

		if(state->node.nexts[chr]){
			state->node = *state->node.nexts[chr];
			state->path += chr;
			new_states.push_back(state);
		}else{
			delete state;
		}
		//else{
		//	std::cout << "øv : path so far '" << state.path << "' - died on: "  << chr << std::endl;
		//}
		}
	states = new_states;
}

#endif
