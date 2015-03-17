/*
 * node.h
 *
 *  Created on: 02/03/2015
 *      Author: Martin
 */
#define _epsilon_ '\u0190'

#include <iostream>
#include <map>
#include <list>

using namespace std;

struct Counter{
	int num;
};

struct Node{
	int value;
	map<char, Node*> nexts;
	list<Node*> epsilons;
};

std::ostream& operator << (std::ostream &o, const Node &n){
	typedef map<char, Node*>::const_iterator MapIterator;
	for (MapIterator iter = n.nexts.begin(); iter != n.nexts.end(); iter++)
	{
		Node* value = iter->second;
		if (value->nexts.empty()){
			o << *value;
		}else{
			o << "node" << n.value  <<" -> "<< "node" << (*value).value << "[label=" << iter->first << "]" << endl;
		}
	}

	list<Node*> s = n.epsilons;
	std::list<Node*>::const_iterator i;
	for( i = s.begin(); i != s.end(); ++i)
	{
		o << "node" << n.value  << " -> " << "node"<< (*i)->value << "[label=e]" << endl;
	}
	return o;
}

struct World{
	list<Node*> nodes;
	Counter* count;
	Node* start;
	Node* end;
};

std::ostream& operator << (std::ostream &o, const World &w){
	o << "digraph {" << endl;
	o << "  node [shape=circle]" << endl;

// START NODE, FOR PRETTY PRINTING
	o << "  node"<<w.start->value << " [shape=box]" << endl;
	list<Node*> start = w.start->epsilons;
	std::list<Node*>::const_iterator ii;

	for( ii = start.begin(); ii != start.end(); ++ii)
	{
		o << "  node" << w.start->value  << " -> " << "node"<< (*ii)->value << "[label=e];" << endl;
	}

	typedef map<char, Node*>::const_iterator MapIterator;
	for (MapIterator iter = w.start->nexts.begin(); iter != w.start->nexts.end(); iter++)
	{
		Node* value = iter->second;
			o << "  node" << w.start->value  <<" -> "<< "node" << (*value).value << "[label=" << iter->first << "];" << endl;
	}
// NORMAL NODES
	list<Node*> s = w.nodes;
	std::list<Node*>::const_iterator i;
	for( i = s.begin(); i != s.end(); ++i)
	{
		if(w.start == *i){
			//o << "node"<<(*i)->value << " [shape=doublecircle]" << endl;
		}else if(w.end == *i){
			//o << "node"<<(*i)->value << " [shape=box]" << endl;
		}else{
			list<Node*> s = (*i)->epsilons;
			std::list<Node*>::const_iterator ii;
			for( ii = s.begin(); ii != s.end(); ++ii)
			{
				o << "  node" << (*i)->value  << " -> " << "node"<< (*ii)->value << "[label=e];" << endl;
			}

			typedef map<char, Node*>::const_iterator MapIterator;
			for (MapIterator iter = (*i)->nexts.begin(); iter != (*i)->nexts.end(); iter++)
			{
				Node* value = iter->second;
					o << "  node" << (*i)->value  <<" -> "<< "node" << (*value).value << "[label=" << iter->first << "];" << endl;
			}
		}
	}
// END NODE, FOR PRETTY PRINTING
	o << "  node"<<w.end->value << " [shape=doublecircle]" << endl;

	list<Node*> end = w.end->epsilons;
	std::list<Node*>::const_iterator iii;

	for( iii = end.begin(); iii != end.end(); ++iii)
	{
		o << "  node" << w.end->value  << " -> " << "node"<< (*iii)->value << "[label=e];" << endl;
	}

	typedef map<char, Node*>::const_iterator MapIterator;
	for (MapIterator iter = w.end->nexts.begin(); iter != w.end->nexts.end(); iter++)
	{
		Node* value = iter->second;
			o << "  node" << w.end->value  <<" -> "<< "node" << (*value).value << "[label=" << iter->first << "];" << endl;
	}

	o << "}";
	return o;
}

Node* create_node(World *world){
	Node *node = new Node();
	node->value = world->count->num;
	world->count->num = world->count->num+1;
	world->nodes.push_back(node);
	return node;
}

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

World* add_star(World *world){
	Node *node1 = create_node(world);
	Node *node2 = create_node(world);
	Node *node3 = create_node(world);
	node1->epsilons.push_back(world->start);
	node1->epsilons.push_back(node3);
	node2->epsilons.push_back(node1);
	node2->epsilons.push_back(node3);

	world->end->epsilons.push_back(node2);
	world->start = node1;
	world->end=node3;
	return world;
}

World* add_or(World *lworld, World *rworld){
	World *world = new World();
	world->count = lworld->count;
	Node *start = create_node(world);
	Node *end = create_node(world);
	Node *right = create_node(world);
	Node *left = create_node(world);
	world->start=start;
	world->end=end;
	start->epsilons.push_back(lworld->start);
	start->epsilons.push_back(right);
	right->epsilons.push_back(rworld->start);
	rworld->end->epsilons.push_back(end);
	lworld->end->epsilons.push_back(left);
	left->epsilons.push_back(end);
	world->nodes.merge(lworld->nodes);
	world->nodes.merge(rworld->nodes);
	return world;
}


World* add_question(World *world){
	world->start->epsilons.push_back(world->end);
	return world;
}

int main(){
	Counter *count = new Counter();
	count->num=0;
	World *world1 = new World();
	world1->count = count;
	World *world2 = new World();
	world2->count = count;
	Node *node1 = create_node(world1);
	Node *node2 = create_node(world1);

	Node *node3 = create_node(world2);
	Node *node4 = create_node(world2);
	node1->nexts['a'] = node2;
	world1->start=node1;
	world1->end=node2;

	node3->nexts['b'] = node4;
	world2->start=node3;
	world2->end=node4;

	World *w = add_or(world1, world2);
//	cout << *w;
//	World *ww = add_star(w);
//	cout << *ww;
	World *ww = add_question(w);
	cout << *ww;
	return 0;
}
