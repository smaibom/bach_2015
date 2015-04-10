#ifndef PRETTY_H
#define PRETTY_H

#include "World.h"
#include "Node.h"
#include "State.h"

std::ostream& operator 	<< (std::ostream &o, const World &w){
	o << "digraph {" << std::endl;
	o << "  node [shape=circle]" << std::endl;

// START NODE, FOR PRETTY PRINTING
	o << "  node"<<w.start->value << " [shape=circle]" << std::endl;
	std::list<Node*> start = w.start->epsilons;
	std::list<Node*>::const_iterator ii;

	for( ii = start.begin(); ii != start.end(); ++ii)
	{
		o << "  node" << w.start->value  << " -> " << "node"<< (*ii)->value << "[label=\"&epsilon;\"];" << std::endl;
	}

	typedef std::map<char, Node*>::const_iterator MapIterator;
	for (MapIterator iter = w.start->nexts.begin(); iter != w.start->nexts.end(); iter++)
	{
		Node* value = iter->second;
			o << "  node" << w.start->value  <<" -> "<< "node" << (*value).value << "[label=" << iter->first << "];" << std::endl;
	}
// NORMAL NODES
	std::list<Node*> s = w.nodes;
	std::list<Node*>::const_iterator i;
	for( i = s.begin(); i != s.end(); ++i)
	{
		if(w.start == *i){
			//o << "node"<<(*i)->value << " [shape=doublecircle]" << endl;
		}else if(w.end == *i){
			//o << "node"<<(*i)->value << " [shape=box]" << endl;
		}else{
			std::list<Node*> s = (*i)->epsilons;
			std::list<Node*>::const_iterator ii;
			for( ii = s.begin(); ii != s.end(); ++ii)
			{
				o << "  node" << (*i)->value  << " -> " << "node"<< (*ii)->value << "[label=\"&epsilon;\"];" << std::endl;
			}

			typedef std::map<char, Node*>::const_iterator MapIterator;
			for (MapIterator iter = (*i)->nexts.begin(); iter != (*i)->nexts.end(); iter++)
			{
				Node* value = iter->second;
					o << "  node" << (*i)->value  <<" -> "<< "node" << (*value).value << "[label=" << iter->first << "];" << std::endl;
			}
		}
	}
// END NODE, FOR PRETTY PRINTING
	o << "  node"<<w.end->value << " [shape=doublecircle]" << std::endl;

	std::list<Node*> end = w.end->epsilons;
	std::list<Node*>::const_iterator iii;

	for( iii = end.begin(); iii != end.end(); ++iii)
	{
		o << "  node" << w.end->value  << " -> " << "node"<< (*iii)->value << "[label=\"&epsilon;\"];" << std::endl;
	}

	typedef std::map<char, Node*>::const_iterator MapIterator;
	for (MapIterator iter = w.end->nexts.begin(); iter != w.end->nexts.end(); iter++)
	{
		Node* value = iter->second;
			o << "  node" << w.end->value  <<" -> "<< "node" << (*value).value << "[label=" << iter->first << "];" << std::endl;
	}

	o << "}";
	return o;
}

std::ostream& operator << (std::ostream &o, const Node &n){
	typedef std::map<char, Node*>::const_iterator MapIterator;
	for (MapIterator iter = n.nexts.begin(); iter != n.nexts.end(); iter++)
	{
		Node* value = iter->second;
		if (value->nexts.empty()){
			o << *value;
		}else{
			o << "node" << n.value  <<" -> "<< "node" << (*value).value << "[label=" << iter->first << "]" << std::endl;
		}
	}

	std::list<Node*> s = n.epsilons;
	std::list<Node*>::const_iterator i;
	for( i = s.begin(); i != s.end(); ++i)
	{
		o << "node" << n.value  << " -> " << "node"<< (*i)->value << "[label=\"&epsilon;\"]" << std::endl;
	}
	return o;
}

std::ostream& operator << (std::ostream &o, const State &s){
		o << "[" << s.start << "," << s.path.length() << "]" <<s.path;
		return o;
}

#endif
