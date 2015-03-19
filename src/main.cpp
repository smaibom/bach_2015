/*
 * Koden her kan bruges til at lave .dot kode, som kan overføres til http://graphs.grevian.org/graph hvor man kan få en fin lille NFA.
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
		o << "node" << n.value  << " -> " << "node"<< (*i)->value << "[label=\"&epsilon;\"]" << endl;
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
	o << "  node"<<w.start->value << " [shape=circle]" << endl;
	list<Node*> start = w.start->epsilons;
	std::list<Node*>::const_iterator ii;

	for( ii = start.begin(); ii != start.end(); ++ii)
	{
		o << "  node" << w.start->value  << " -> " << "node"<< (*ii)->value << "[label=\"&epsilon;\"];" << endl;
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
				o << "  node" << (*i)->value  << " -> " << "node"<< (*ii)->value << "[label=\"&epsilon;\"];" << endl;
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
		o << "  node" << w.end->value  << " -> " << "node"<< (*iii)->value << "[label=\"&epsilon;\"];" << endl;
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

int main(){
	Counter *count = new Counter();
	count->num=0;
	// a
	World *a = new_world('a', count);
	// a*
	World *a_s = add_star(a);

	// b
	World *b = new_world('b', count);

	// cd
	World *c = new_world('c', count);
	World *d = new_world('d', count);
	World *cd = connect(c,d);

	// b|cd
	World *bcd = add_or(b, cd);

	// (b|cd)*
	bcd = add_star(bcd);

	// a*(b|cd)*
	World *full = connect(a_s, bcd);
	cout << *full;

/*
	// a
	World *a = new World();
	a->count = count;
	Node *nodea = create_node(a);
	Node *nodeaa = create_node(a);
	nodea->nexts['a'] = nodeaa;
	a->start=nodea;
	a->end=nodeaa;
	// a*
	a = add_star(a);

	// b
	World *b = new World();
	b->count = count;
	Node *nodeb = create_node(b);
	Node *nodebb = create_node(b);
	nodeb->nexts['a'] = nodebb;
	b->start=nodeb;
	b->end=nodebb;
	// b*
	b = add_star(b);
	World *full = connect(a, b);
	cout << *full;
*/
	return 0;
}
