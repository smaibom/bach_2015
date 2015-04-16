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
#include <pthread.h>
#include <memory.h>

//a
std::shared_ptr<Node> create_node(std::shared_ptr<World> world){
	std::shared_ptr<Node>node = std::make_shared<Node>();
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
	std::shared_ptr<World> world = std::make_shared<World>();
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
	std::shared_ptr<World> world = std::make_shared<World>();
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
	std::shared_ptr<World> world = std::make_shared<World>();
	world->count = count;
	std::shared_ptr<Node> start = create_node(world);
	std::shared_ptr<Node> end = create_node(world);
	start->nexts[chr] = end;
	world->start=start;
	world->end=end;
	return world;
}

int main(){
	std::shared_ptr<Counter> count = std::make_shared<Counter>();
	count->num=0;
	std::shared_ptr<World> world = std::make_shared<World>();

	// g(a|c)+(tc+)+a+
	*world = *
		connect(
			add_plus(
				connect(
				new_world('g', count),
				add_plus(
					add_or(
							new_world('a', count),
							new_world('c', count))))),
			connect(
				add_plus(
					connect(
						new_world('t', count),
						add_plus(
							new_world('c', count)))),
				add_plus(new_world('a', count))
			)
		);
//	*world = *connect(add_star(new_world('a', new_count)),connect(new_world('t', new_count),connect(new_world('c', new_count),connect(new_world('t', new_count),connect(new_world('c', new_count), connect(new_world('a', new_count), connect(new_world('a', new_count), new_world('a', new_count))))))));
//	*world = *add_star(new_world('a', count));
//	*world = *connect(add_star(new_world('a', new_count)),add_star(add_or(new_world('g', new_count)));


// GGGTGCAAGCGTTAAT[2,1,1] 50...350 AGCGTGGGGAGCAAAC[2,1,1]
/*
	*world = *connect(new_world('g', count), connect(new_world('g', count), new_world('g', count)));
	*world = *connect(world, connect(new_world('t', count), connect(new_world('g', count), new_world('c', count))));
	*world = *connect(world, connect(new_world('a', count), connect(new_world('a', count), new_world('g', count))));
	*world = *connect(world, connect(new_world('c', count), connect(new_world('g', count), new_world('t', count))));
	*world = *connect(world, connect(new_world('t', count), connect(new_world('a', count), new_world('a', count))));
	*world = *connect(world, new_world('t', count));
*/

//	*world = *connect(new_world('g', count), connect(new_world('g', count), new_world('g', count)));

	std::cout << *world;
	std::shared_ptr<States> states = std::make_shared<States>();
	states->end = world->end;
	states->insertions = 1;
	states->deletions = 0;
	states->mutations = 0;

	char * buffer;
	int length;

	std::ifstream is ("../data/fas1.fa", std::ifstream::binary);
//	std::ifstream is ("C:/Users/Martin/Desktop/scan_for_matches/fasta/chr3.fa", std::ifstream::binary);
	if (is) {
		// get length of file:
		is.seekg (0, is.end);
		length = is.tellg();
		is.seekg (0, is.beg);

		buffer = new char [length];

		std::cout << "Reading " << length << " characters... ";
		// read data as a block:
		is.read (buffer,length);

		if (is)
		  std::cout << "all characters read successfully.\n";
		else
		  std::cout << "error: only " << is.gcount() << " could be read\n";
		is.close();

		// ...buffer contains the entire file...
	}

	std::string output = "";

	for(int i = 0; i < length; i++)
	{
		if(isspace(buffer[i])){
			// DO NOTHING
		}else{
			std::string result = "";
			states->add_state(world->start);

			// Alt i over case
			char chr = tolower(buffer[i]);
			states->chr = &chr;
			while(!states->states.empty()){
				result = states->check_values();
				if(result==""){
					// Dont print
				}else{
					output += result + '\n';
					result.clear();
				}
			}
			states->states.merge(states->next_states);
		}
		// Printer efter hvert tegn
		std::cout << output;
		output.clear();
	}
	delete[] buffer;
}
