#ifndef STATE_H
#define STATE_H

#include "Node.h"
#include "World.h"
#include <algorithm>
#include <memory>
#include <string>
#include <deque>

class State{
	public:
		std::shared_ptr<Node> node;
		int deletions;
		int mutations;
		int insertions;
		int path;
};

class States{
	public:
		int deletions;
		int mutations;
		int insertions;
		std::vector<State>::iterator it;

		char *chr;
		int ret;
		std::vector<State> states;
		std::shared_ptr<Node> end;

		void add_state(std::shared_ptr<Node> node);
		States(){
			states.reserve(1000);
		}
		~States(){
			// deconstructor
		}
		int check_values();
};

void States::add_state(std::shared_ptr<Node> node){
	states.emplace_back();
	states.back().node = node;
	states.back().path = 0;
	states.back().insertions = insertions;
	states.back().deletions = deletions;
	states.back().mutations = mutations;
}

int States::check_values(){
	ret = 0;
	it = states.begin();
	while (it != states.end() )
	{
		if( (*it).node == end ){
				if(ret < (*it).path)
					ret = (*it).path;
			states.erase(it++);
		}else{
			if((*it).node->chr == *chr){
				(*it).node = (*it).node->next;
				(*it).path += 1;
				++it;
			}else{
				if((*it).insertions > 0){
					states.emplace( states.begin());
					++it;
					states.front().path = (*it).path + 1;
					states.front().node = (*it).node;
					states.front().insertions = (*it).insertions -1;
					states.front().deletions = (*it).deletions;
					states.front().mutations = (*it).mutations;
				}
				if((*it).mutations > 0){
					states.emplace( states.begin());
					++it;
					states.front().path = (*it).path + 1;
					states.front().node = (*it).node->next;
					states.front().insertions = (*it).insertions;
					states.front().deletions = (*it).deletions;
					states.front().mutations = (*it).mutations -1;			
				}
				if((*it).deletions > 0){
					states.emplace( states.begin());
					++it;
					states.front().path = (*it).path;
					states.front().node = (*it).node->next;
					states.front().insertions = (*it).insertions;
					states.front().deletions = (*it).deletions-1;
					states.front().mutations = (*it).mutations;				
				}
				states.erase(it);
			}
		}
	}
	return ret;
}

#endif
