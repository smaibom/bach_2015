#ifndef PRETTY_H
#define PRETTY_H

#include "World.h"
#include "Node.h"
#include "State.h"

std::ostream& operator 	<< (std::ostream &o, const World &w){
	o << "digraph {" << std::endl;
	o << " node [shape=circle]" << std::endl;
	o << " node"<<w.end->value << " [shape=doublecircle]" << std::endl;
	std::list<std::shared_ptr<Node>> s = w.nodes;
	std::list<std::shared_ptr<Node>>::const_iterator i;
	for( i = s.begin(); i != s.end(); ++i)
	{
		if((*i)->next){
			o << "  node" << (*i)->value  <<" -> "<< "node" << (*i)->next->value << "[label=" << (*i)->chr << "];" << std::endl;
		}
		std::list<std::shared_ptr<Node>> s = (*i)->epsilons;
		std::list<std::shared_ptr<Node>>::const_iterator ii;
		for( ii = s.begin(); ii != s.end(); ++ii)
		{
			o << "  node" << (*i)->value  <<" -> "<< "node"<< (*ii)->value << "[label=\"&epsilon;\"];" << std::endl;
		}
	}

	o << "}" << std::endl;
	return o;
}

std::ostream& operator << (std::ostream &o, const Node &n){
	o << "node" << n.value  <<" -> "<< "node" << n.next->value << "[label=" << n.chr << "]" << std::endl;

	std::list<std::shared_ptr<Node>> s = n.epsilons;
	std::list<std::shared_ptr<Node>>::const_iterator i;
	for( i = s.begin(); i != s.end(); ++i)
	{
		o << "node" << n.value  << " -> " << "node"<< (*i)->value << "[label=\"&epsilon;\"]" << std::endl;
	}
	return o;
}

std::ostream& operator << (std::ostream &o, const State &s){
		//o << "[" << s.start << "," << s.path.length() << "]" <<s.path;
		return o;
}

#endif
