/*
 * Koden her kan bruges til at lave .dot kode, som kan overføres til http://graphs.grevian.org/graph hvor man kan få en fin lille NFA.
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
#include <stdio.h>
#include <ctype.h>
//a
Node* create_node(World *world){
	Node *node = new Node();
	node->value = world->count->num;
	world->count->num = world->count->num+1;
	world->nodes.push_back(node);
	return node;
}

//a+
World* add_plus(World *world){
	Node *node1 = create_node(world);
	Node *node2 = create_node(world);
	node1->epsilons.push_back(world->start);
	world->end->epsilons.push_back(node2);
	world->end->epsilons.push_back(world->start);
	world->start = node1;
	world->end = node2;
	return world;
}

//a*
World* add_star(World *world){
	Node *start = create_node(world);
	Node *end = create_node(world);
	start->epsilons.push_back(world->start);
	start->epsilons.push_back(end);
	world->end->epsilons.push_back(world->start);
	world->end->epsilons.push_back(end);

	world->start = start;
	world->end=end;
	return world;
}

//a|b
World* add_or(World *lworld, World *rworld){
	World *world = new World();
	world->count = lworld->count;
	Node *start = create_node(world);
	Node *end = create_node(world);
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
World* add_question(World *world){
	world->start->epsilons.push_back(world->end);
	return world;
}

// ab
World* connect(World *first, World *last){
	World *world = new World();
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

World* new_world(char chr, Counter *count){
	World *world = new World();
	world->count = count;
	Node *start = create_node(world);
	Node *end = create_node(world);
	start->nexts[chr] = end;
	world->start=start;
	world->end=end;
	return world;
}

void print(State state){
	std::cout << state.node.value << " /  " << state.path <<  std::endl;
}

int main(){
	Counter *new_count = new Counter();
	new_count->num=0;
	World *world = new World();;

	// g(a|c)+ (tc+)+a+
	*world = *connect(
+			add_plus(connect(new_world('g', new_count), add_plus(add_or(new_world('a', new_count), new_world('c', new_count))))),
			connect(add_plus(connect(new_world('t', new_count), add_plus(new_world('c', new_count)))),
			add_plus(new_world('a', new_count))
			));
//	*world = *connect(add_star(new_world('a', new_count)),connect(new_world('t', new_count),connect(new_world('c', new_count),connect(new_world('t', new_count),connect(new_world('c', new_count), connect(new_world('a', new_count), connect(new_world('a', new_count), new_world('a', new_count))))))));
//	*world = *connect(add_star(new_world('a', new_count)),add_star(add_or(new_world('b', new_count),connect(new_world('c', new_count),new_world('d', new_count)))));
	std::cout << *world;

	States *states = new States();

	std::ifstream file("../data/fas1.fa");
    std::string str;
    while (std::getline(file, str))
    {
    	for(int i = 0; i < str.length(); i++)
    	{
    		states->add_state(world->start);
    		states->checkValue(tolower(str[i]));
    	}
    }
}
