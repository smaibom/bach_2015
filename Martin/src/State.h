#ifndef STATE_H
#define STATE_H

#include "Node.h"
#include "World.h"
#include <String>
#include <algorithm>
#include <mutex>
#include <memory>
#include <thread>

class State{
	private:
		std::mutex *path_mutex;
		std::string path;
	public:
		std::shared_ptr<Node> node;
        State() {
        	path_mutex = new std::mutex;
        	path = "";
        }
        ~State() {
        	path.clear();
        	delete path_mutex;
        	node.reset();
        }
        std::string* getPath();
        void *setNode(std::shared_ptr<Node> node);
        void *setPath(std::string* str);
        bool emptyPath();
        void *addPath(char* chr);
};

std::string* State::getPath(){
	return &path;
}

void *State::setPath(std::string* str){
	path_mutex->lock();
	path = *str;
	path_mutex->unlock();
}

void *State::setNode(std::shared_ptr<Node> new_node){
	node = new_node;
}

bool State::emptyPath(){
	return path == "";
}

void *State::addPath(char * chr){
	path_mutex->lock();
	path += *chr;
	path_mutex->unlock();
}

class States{
	friend void* proxy_function(void*);
	friend void task1(std::string msg);
	public:
		char *chr;
		std::list<std::shared_ptr<State>> states;
		std::list<std::shared_ptr<State>> new_states;
		void add_state(std::shared_ptr<Node> node);
		std::shared_ptr<std::mutex> g_pages_mutex;
		States(){
			std::shared_ptr<std::mutex> mutex(new std::mutex);
			g_pages_mutex = mutex;
		}
		~States(){
			g_pages_mutex.reset();

		}
		void something();

	private:
		void create_thread();
		typedef std::list<std::shared_ptr<Node>> listMyClass;

};

void States::add_state(std::shared_ptr<Node> node){
	std::shared_ptr<State> state(new State());
	state->setNode(node);
	states.push_back(state);
	state.reset();
}

void States::something(){
	if (states.empty()){
		g_pages_mutex->unlock();
		return;
	}
	std::shared_ptr<State> state = states.back();
	states.pop_back();

	g_pages_mutex->unlock();

	if(state->node->value == 3 and !state->emptyPath()){ // Hacky måde, 3 er slutpunktet, taget fra NFA'en
		std::cout << *state->getPath() <<  std::endl;
	}

	for(listMyClass::iterator listMyClassIter = state->node->epsilons.begin();
	    listMyClassIter != state->node->epsilons.end();
	    listMyClassIter ++)
	{
		std::shared_ptr<State> new_state(new State());
		new_state->setPath(state->getPath());
		new_state->setNode((std::shared_ptr<Node>)*listMyClassIter);
      	g_pages_mutex->lock();
		states.push_back(new_state);
		new_state.reset();
       	g_pages_mutex->unlock();
	}

	if(state->node->nexts[*chr]){
		state->node = state->node->nexts[*chr];
		state->addPath(chr);
      	g_pages_mutex->lock();
		new_states.push_back(state);
		state.reset();
       	g_pages_mutex->unlock();
	}else{
       	g_pages_mutex->lock();
		state.reset();
       	g_pages_mutex->unlock();
	}
}

#endif
