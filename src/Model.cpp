/*
 * Koden her kan bruges til at lave .dot kode, som kan overføres til http://graphs.grevian.org/graph hvor man kan få en fin lille NFA.
 *
 *  Created on: 02/03/2015
 *      Author: Martin
 */
#define _epsilon_ '\u0190'

#include "Model.h"

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

/*
 std::string parse(std::string s) {
	std::string parse_me;
	std::string::iterator pos;


	pos = std::find(s.begin(), s.end(), ')');
	std::string s1(s.begin(), pos);
	if (!s1.empty() and s1.length() < s.length())
	{
		std::string rest(pos + 1, s.end());
		std::string parsed = parse(s1);
		std::string first(s.begin(), pos-parsed.length() - 1);
//		std::cout << first << " " << parse(s1) << " " << rest;
		parse_me = first + " " + parse(s1) + " " + rest;
		parse(parse_me);
	}

	std::reverse(s.begin(), s.end());
	std::string s2(s.begin(), std::find(s.begin(), s.end(), '('));
	if (!s2.empty() and s2.length() < s.length())
	{
		std::reverse(s2.begin(), s2.end());
		return s2;
	}


	std::reverse(s.begin(), s.end());
	std::cout << s;
	return "";
}
*/

int main(){
/*
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
	std::cout << *full;
*/
// Den korte version:
	Counter *new_count = new Counter();
	new_count->num=0;
	std::cout << *connect(add_star(new_world('a', new_count)),add_or(new_world('b', new_count),connect(new_world('c', new_count),new_world('d', new_count))));
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
//	std::string s = "(abe(ka(a)t)*sutter)";
//	parse(s);
 */
}
