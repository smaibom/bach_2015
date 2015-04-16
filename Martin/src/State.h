#ifndef STATE_H
#define STATE_H

#include "Node.h"
#include "World.h"
#include <algorithm>
#include <memory>
#include <string>
class State{
	private:
		std::string path;
	public:
		std::shared_ptr<Node> node;
        State() {
        	path = "";
        }
        ~State() {
        	path.clear();
        	node.reset();
        }
        std::string getPath();
        void *setNode(std::shared_ptr<Node> node);
        void setPath(std::string str);
        bool emptyPath();
        void *addPath(char* chr);

		int deletions;
		int mutations;
		int insertions;
};

std::string State::getPath(){
	return path;
}

void State::setPath(std::string str){
	path = str;
}

void *State::setNode(std::shared_ptr<Node> next_node){
	node = next_node;
	next_node.reset();
}

bool State::emptyPath(){
	return (path == "");
}

void *State::addPath(char * chr){
	path += *chr;
}

class States{
	public:

		int deletions;
		int mutations;
		int insertions;


		std::shared_ptr<char> buffer;
		char *chr;
		std::list<std::shared_ptr<State>> states;
		std::shared_ptr<Node> end;
		std::list<std::shared_ptr<State>> next_states;
		void add_state(std::shared_ptr<Node> node);
		States(){
		}
		~States(){
			next_states.clear();
			states.clear();
			end.reset();
			buffer.reset();
		}
		std::string check_values();

	private:
		typedef std::list<std::shared_ptr<Node>> listMyClass;

};

void States::add_state(std::shared_ptr<Node> node){
	std::shared_ptr<State> state(std::make_shared<State>());
	state->setNode(node);
	state->insertions = insertions;
	state->deletions = deletions;
	state->mutations = mutations;
	states.push_back(state);
	state.reset();
	node.reset(); // May very well be overkill
}

std::string States::check_values(){
	std::string result = "";
	if (states.empty()){
		return result;
	}
	std::shared_ptr<State> state = states.back();
	states.pop_back();

	if(state->node == end and !state->emptyPath()){
		result = std::string(state->getPath());
		return result;
	}

	for(listMyClass::iterator listMyClassIter = state->node->epsilons.begin();
	    listMyClassIter != state->node->epsilons.end();
	    listMyClassIter ++)
	{
		std::shared_ptr<State> next_state = std::make_shared<State>();
		next_state->setPath(state->getPath());
		next_state->setNode((std::shared_ptr<Node>)*listMyClassIter);
		next_state->insertions = state->insertions;
		next_state->deletions = state->deletions;
		next_state->mutations = state->mutations;
		states.push_back(next_state);
		next_state.reset();
	}

	if(state->node->nexts[*chr]){
		state->node = state->node->nexts[*chr];
		state->addPath(chr);
		next_states.push_back(state);
	}else if(state->insertions > 0){
		state->insertions -= 1;
		state->addPath(chr);
		next_states.push_back(state);
	}//else if(state->deletions > 0){
//     Gå til alle næste mulige states, med lad være med at tilføj til path
//	}else if(state->mutations > 0){
//     Gå til alle næste mulige states, og tilføj til path
//	}
	state.reset();
	return "";
}

#endif
