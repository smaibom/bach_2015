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
#include <memory>

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

	//first->end->nexts = last->start->nexts;
	first->end->next = last->start->next;
	first->end->chr = last->start->chr;

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

//	start->nexts[chr] = end;
	start->next = end;
	start->chr = chr;

	world->start=start;
	world->end=end;
	return world;
}

std::string f( int val )
{
   if ( val >= 10 )
      return f( val / 10 ) + ( char )( val % 10 + '0' ) ;
   else
      return std::string( 1, '0' + val ) ;
}

int main(){
	std::shared_ptr<Counter> count = std::make_shared<Counter>();
	count->num=0;
	std::shared_ptr<World> world = std::make_shared<World>();

	// g(a|c)+(tc+)+a+
	/*
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
	*/
//	*world = *connect(add_star(new_world('a', new_count)),connect(new_world('t', new_count),connect(new_world('c', new_count),connect(new_world('t', new_count),connect(new_world('c', new_count), connect(new_world('a', new_count), connect(new_world('a', new_count), new_world('a', new_count))))))));
//	*world = *add_star(new_world('a', count));
//	*world = *connect(add_star(new_world('a', new_count)),add_star(add_or(new_world('g', new_count)));

// 2 mutations, 1 deletion, 1 insertion
// GGGTGCAAGCGTTAAT[2,1,1] 50...350 AGCGTGGGGAGCAAAC[2,1,1]
/*
	*world = *connect(new_world('g', count), connect(new_world('g', count), new_world('g', count)));
	*world = *connect(world, connect(new_world('t', count), connect(new_world('g', count), new_world('c', count))));
	*world = *connect(world, connect(new_world('a', count), connect(new_world('a', count), new_world('g', count))));
	*world = *connect(world, connect(new_world('c', count), connect(new_world('g', count), new_world('t', count))));
	*world = *connect(world, connect(new_world('t', count), connect(new_world('a', count), new_world('a', count))));
	*world = *connect(world, new_world('t', count));
*/

// 1 insertions GGAGTGCAAGCGTT[0,0,1]
	*world = *connect(new_world('g', count), connect(new_world('g', count), new_world('a', count)));
	*world = *connect(world, connect(new_world('g', count), connect(new_world('t', count), new_world('g', count))));
	*world = *connect(world, connect(new_world('c', count), connect(new_world('a', count), new_world('a', count))));
	*world = *connect(world, connect(new_world('g', count), connect(new_world('c', count), new_world('g', count))));
	*world = *connect(world, new_world('t', count));
	*world = *connect(world, new_world('t', count));

//	*world = *connect(new_world('g', count), connect(new_world('g', count), new_world('g', count)));

	std::cout << *world;
	std::shared_ptr<States> states = std::make_shared<States>();
	states->end = world->end;
	states->insertions = 1;
	states->deletions = 0;
	states->mutations = 0;

	char * buffer;
	int length;
	int read_chunk;
	int cnt;

	std::ifstream is ("../data/chr3.fa", std::ifstream::binary);
//	std::ifstream is ("../data/_fas4.fa", std::ifstream::binary);
	if (is) {
		read_chunk = 1024*10000;

		// get length of file:
		is.seekg (0, is.end);
		length = is.tellg();
		is.seekg (0, is.beg);

		if(length < read_chunk){
			std::cout << "Reading entire file in buffer" << std::endl;
			buffer = new char [length];
			read_chunk = length;
		}else{
			std::cout << "Reading file in sections to buffer" << std::endl;		
			buffer = new char [read_chunk];
		}

		// read data as a block:
		while( !is.eof() and (read_chunk > 0)) {
			is.read (buffer,read_chunk);
			std::string output = "";
			int result = 0;
			for(int i = 0; i < read_chunk; i++)
			{
				cnt += 1;
				if(isspace(buffer[i])){
					// DO NOTHING
				}else{
					char chr = tolower(buffer[i]);

					if((states->insertions + states->mutations + states->deletions) > 0){
						states->add_state(world->start);
					}else{
						if(chr == world->start->chr){
							states->add_state(world->start);
						}
					}
					if(states->states.size() > 0){
						states->chr = &chr;
//						std::cout << "YTEA" << chr << std::endl;
						result = states->check_values();
//						std::cout << "aety" << std::endl;

						if(result!=0){
							output += std::string(buffer).substr(i-result, result) + '\[' + f(cnt-result) + ":" + f(cnt) +  "]\n";
						}
					}
				}
				// Printer efter hvert tegn
				std::cout << output;
				output.clear();
			}
			length -= read_chunk;
			if(length < read_chunk)
				read_chunk = length;
		}
		is.close();
	}
}
