#ifndef NODE_H
#define NODE_H

#include <map>
#include <list>
#include <sstream>


struct Node{
	int value;
	std::map<char, Node*> nexts;
	std::list<Node*> epsilons;
};

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

#endif
