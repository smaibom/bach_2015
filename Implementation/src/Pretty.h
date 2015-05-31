#ifndef PRETTY_H
#define PRETTY_H

#include "World.h"
#include "Node.h"
#include "State.h"

std::ostream& operator 	<< (std::ostream &o, const World &w){
	o << "digraph {" << std::endl;
	o << " node [shape=circle]" << std::endl;
    
	if(w.start->next){
		o << " node" << w.start->value  <<" -> "<< "node" << w.start->next->value << "[label=" << w.start->chr << "];" << std::endl;
	}
    if(w.start->epsilon_left)
        o << " node" << w.start->value  << " -> " << "node"<< w.start->epsilon_left->value << "[label=\"&epsilon;\"]" << std::endl;
    if(w.start->epsilon_right)
        o << " node" << w.start->value  << " -> " << "node"<< w.start->epsilon_right->value << "[label=\"&epsilon;\"]" << std::endl;

	o << " node"<<w.end->value << " [shape=doublecircle]" << std::endl;
    
	std::list<std::shared_ptr<Node>> s = w.nodes;
	std::list<std::shared_ptr<Node>>::const_iterator i;   
	for( i = s.begin(); i != s.end(); ++i)
	{
        if ((*i)->value != w.start->value){
            if((*i)->next){
                o << " node" << (*i)->value  <<" -> "<< "node" << (*i)->next->value << "[label=" << (*i)->chr << "];" << std::endl;
            }
            if((*i)->epsilon_left)
                o << " node" << (*i)->value  << " -> " << "node"<< (*i)->epsilon_left->value << "[label=\"&epsilon;\"]" << std::endl;
            if((*i)->epsilon_right)
                o << " node" << (*i)->value  << " -> " << "node"<< (*i)->epsilon_right->value << "[label=\"&epsilon;\"]" << std::endl;
        }
	}

	o << "}" << std::endl;
	return o;
}
/*
std::ostream& operator << (std::ostream &o, const Node &n){
	o << "node" << n.value  <<" -> "<< "node" << n.next->value << "[label=" << n.chr << "]" << std::endl;
    if(n.epsilon_left)
		o << "node" << n.value  << " -> " << "node"<< n.epsilon_left->value << "[label=\"&epsilon;\"]" << std::endl;
    if(n.epsilon_right)
		o << "node" << n.value  << " -> " << "node"<< n.epsilon_right->value << "[label=\"&epsilon;\"]" << std::endl;
	return o;
}
*/
std::ostream& operator << (std::ostream &o, const State &s){
		//o << "[" << s.start << "," << s.path.length() << "]" <<s.path;
		return o;
}

#endif
