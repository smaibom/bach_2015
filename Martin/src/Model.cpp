/*
 * Koden her kan bruges til at lave .dot kode, som kan overføres til http://graphs.grevian.org/graph hvor man kan få en fin lille NFA.
 * Compile med -std=c++0x, da jeg bruger moderne metoder :)
 *
 *  Created on: 02/03/2015
 *      Author: Martin
 */

#define _epsilon_ '\u0190'

#include "Model.h"
#include "State.h"
#include "Pretty.h"
#include <fstream>
#include <string>
#include <pthread.h>
#include <memory.h>
#include <thread>
//a
std::shared_ptr<Node> create_node(std::shared_ptr<World> world){
	std::shared_ptr<Node>node(new Node());
	node->value = world->count->num;
	world->count->num = world->count->num+1;
	world->nodes.push_back(node);
	return node;
}

//a+
std::shared_ptr<World> add_plus(std::shared_ptr<World> world){
	std::shared_ptr<Node> node1 = create_node(world);
	std::shared_ptr<Node> node2 = create_node(world);
	node1->epsilons.push_back(world->start);
	world->end->epsilons.push_back(node2);
	world->end->epsilons.push_back(world->start);
	world->start = node1;
	world->end = node2;
	return world;
}

//a*
std::shared_ptr<World> add_star(std::shared_ptr<World> world){
	std::shared_ptr<Node> start = create_node(world);
	std::shared_ptr<Node> end = create_node(world);
	start->epsilons.push_back(world->start);
	start->epsilons.push_back(end);
	world->end->epsilons.push_back(world->start);
	world->end->epsilons.push_back(end);

	world->start = start;
	world->end=end;
	return world;
}

//a|b
std::shared_ptr<World> add_or(std::shared_ptr<World> lworld, std::shared_ptr<World> rworld){
	std::shared_ptr<World> world(new World());
	world->count = lworld->count;
	std::shared_ptr<Node> start = create_node(world);
	std::shared_ptr<Node> end = create_node(world);
	world->start=start;
	world->end=end;
	start->epsilons.push_back(lworld->start);
	start->epsilons.push_back(rworld->start);
	rworld->end->epsilons.push_back(end);
	lworld->end->epsilons.push_back(end);
	world->nodes.merge(lworld->nodes);
	world->nodes.merge(rworld->nodes);
	return world;
}

//a?
std::shared_ptr<World> add_question(std::shared_ptr<World> world){
	world->start->epsilons.push_back(world->end);
	return world;
}

// ab
std::shared_ptr<World> connect(std::shared_ptr<World> first, std::shared_ptr<World> last){
	std::shared_ptr<World> world(new World());
	world->count = first->count;
	world->start = first->start;
	world->end = last->end;
	first->end->epsilons.merge(last->start->epsilons);

	// Hacky måde at gøre det på, men first's end node bør være tom, så det ok.
	first->end->nexts = last->start->nexts;

	last->nodes.remove(last->start);
	world->nodes.merge(first->nodes);
	world->nodes.merge(last->nodes);
	return world;
}

std::shared_ptr<World> new_world(char chr, std::shared_ptr<Counter> count){
	std::shared_ptr<World> world(new World());
//	World *world = new World();
	world->count = count;
	std::shared_ptr<Node> start = create_node(world);
	std::shared_ptr<Node> end = create_node(world);
	start->nexts[chr] = end;
	world->start=start;
	world->end=end;
	return world;
}

/*void print(State state){
	std::cout << state.node->value << " /  " << state.path <<  std::endl;
}*/

int main(){
	std::shared_ptr<Counter> new_count(new Counter());
	new_count->num=0;
	std::shared_ptr<World> world(new World());

	// g(a|c)+ (tc+)+a+
	*world = *connect(
			add_plus(connect(new_world('g', new_count), add_plus(add_or(new_world('a', new_count), new_world('c', new_count))))),
			connect(add_plus(connect(new_world('t', new_count), add_plus(new_world('c', new_count)))),
			add_plus(new_world('a', new_count))
			));
//	*world = *connect(add_star(new_world('a', new_count)),connect(new_world('t', new_count),connect(new_world('c', new_count),connect(new_world('t', new_count),connect(new_world('c', new_count), connect(new_world('a', new_count), connect(new_world('a', new_count), new_world('a', new_count))))))));
//	*world = *connect(add_star(new_world('a', new_count)),add_star(add_or(new_world('b', new_count),connect(new_world('c', new_count),new_world('d', new_count)))));
	std::cout << *world;

	std::shared_ptr<States> states(new States());

	std::ifstream file("../data/fas1.fa");
    std::string str;
	#define N 5
    pthread_t my_thread[N];
    while (std::getline(file, str))
    {
    	for(int i = 0; i < str.length(); i++)
    	{
    		states->add_state(world->start);
    		char chr = tolower(str[i]);
    		states->chr = &chr;
    		while(!states->states.empty()){
    			states->g_pages_mutex->lock();
    			std::shared_ptr<std::thread> t1(new std::thread (&States::something,states));
    			states->g_pages_mutex->lock();
    			std::shared_ptr<std::thread> t2(new std::thread (&States::something,states));

    			t1->join();
    			t1.reset();
    			t2->join();
    			t2.reset();
    		}
    		states->states.merge(states->new_states);
    	}
    }
}
