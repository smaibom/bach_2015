/*
 * Koden her kan bruges til at lave .dot kode, som kan overføres til http://graphs.grevian.org/graph hvor man kan få en fin lille NFA.
 * Compile med -std=c++0x, da der bruges moderne metoder, husk også gerne et optimization flag, fx -Ofast el, -O2.
 *
 *  Created on: 02/03/2015
 *      Author: Martin
 */

/**** Epsilon symbol defined for printing *****/
#define _epsilon_ '\u0190'

/**** Includes *****/
#include "Model.h"
#include "States.h"
#include "Pretty.h"
#include <fstream>
#include <pthread.h>
#include <memory>

/**** Helper functions for creating NFA *****/

/* a */
std::shared_ptr<Node> create_node(std::shared_ptr<World> world){
    std::shared_ptr<Node>node = std::make_shared<Node>();
    node->value = world->count->num;
    world->count->num = world->count->num+1;
    world->nodes.push_back(node);
    return node;
}

/* a+ */
std::shared_ptr<World> add_plus(std::shared_ptr<World> world){
    std::shared_ptr<Node> start = create_node(world);
    std::shared_ptr<Node> end = create_node(world);
    start->epsilon_left = world->start;
    world->end->epsilon_left = end;
    world->end->epsilon_right = world->start;
    world->start = start;
    world->end = end;
    return world;
}

/* a* */
std::shared_ptr<World> add_star(std::shared_ptr<World> world){
    std::shared_ptr<Node> start = create_node(world);
    std::shared_ptr<Node> end = create_node(world);
    start->epsilon_left = world->start;
    start->epsilon_right = end;
    world->end->epsilon_left = world->start;
    world->end->epsilon_right = end;
    world->start = start;
    world->end=end;
    return world;
}

/* a|b */
std::shared_ptr<World> add_or(std::shared_ptr<World> lworld, std::shared_ptr<World> rworld){
    std::shared_ptr<World> world = std::make_shared<World>();
    world->count = lworld->count;
    std::shared_ptr<Node> start = create_node(world);
    std::shared_ptr<Node> end = create_node(world);

    world->start=start;
    world->end=end;
    start->epsilon_left = lworld->start;
    start->epsilon_right = rworld->start;
    rworld->end->epsilon_left = end;
    lworld->end->epsilon_right = end;
    world->nodes.merge(lworld->nodes);
    world->nodes.merge(rworld->nodes);
    return world;
}

/* a? */
std::shared_ptr<World> add_question(std::shared_ptr<World> world){
    world->start->epsilon_left = world->end;
    return world;
}

/* ab */
std::shared_ptr<World> connect(std::shared_ptr<World> first, std::shared_ptr<World> last){
    std::shared_ptr<World> world = std::make_shared<World>();
    world->count = first->count;
    world->start = first->start;
    world->end = last->end;
    first->end->epsilon_left = last->start->epsilon_left;
    first->end->epsilon_right = last->start->epsilon_right;
    first->end->next = last->start->next;
    first->end->chr = last->start->chr;
    last->nodes.remove(last->start);
    world->nodes.merge(first->nodes);
    world->nodes.merge(last->nodes);
    return world;
}

/* Function to create a new node, or a 'world', as a single node can be a NFA by itself. */
std::shared_ptr<World> new_world(char chr, std::shared_ptr<Counter> count){
    std::shared_ptr<World> world = std::make_shared<World>();
    world->count = count;
    std::shared_ptr<Node> start = create_node(world);
    std::shared_ptr<Node> end = create_node(world);

    start->next = end;
    start->chr = chr;

    world->start=start;
    world->end=end;
    return world;
}

/* Helper function for printing 'hits'. Takes an integer and converts to string. */
std::string f( int val )
{
   if ( val >= 10 )
      return f( val / 10 ) + ( char )( val % 10 + '0' ) ;
   else
      return std::string( 1, '0' + val ) ;
}

/**
Main function for the program.
Will create a new States object, which processes a NFA structure created in the NFA.
The States is given insertions, mutations and deletions allowed when searching the NFA.

**/
int main(int argc, char* argv[]){
    std::shared_ptr<States> states = std::make_shared<States>();

    states->insertions = 1;
    states->deletions = 1;
    states->mutations = 1;
    
    bool showgraph = false;
    bool showhits = false;
    int optind=1;
    if(argc == 1){
        std::cout << "Usage: nfa_search -s [for digraph print] -n [for pure hits] data_file" << std::endl;
        return 0;
    }    
    while ((optind < argc) && (argv[optind][0]=='-')) {
        std::string sw = argv[optind];
        if (sw=="-h") {
            std::cout << "To use this program, give the location of a text file as the last argument" << std::endl;
            std::cout << "For example, give data/chr1.fa if you want to search through file chr1.fa"<< std::endl; 
            std::cout << "in folder data" << std::endl << std::endl;
            std::cout << "Other arguments are"<< std::endl << " '-s' : Show NFA - for a digraph plot of the NFA" << std::endl;
            std::cout << " Visit http://graphs.grevian.org/graph to plot, or a similar digraph plotter" << std::endl;
            std::cout << " '-n' : Replaces the normal output of the hits with the number of total hits" << std::endl;
            return 0;
        }
        else if (sw=="-s") {
            showgraph = true;
        }else if(sw=="-n") {
            showhits = true;
        }
        else
            std::cout << "Unknown switch: " 
                 << argv[optind] << std::endl;
        optind++;
    }
    
    /*
    Each node created will have a value attached to it, 
    to keep track of the node.  This counter is handled by a single
    counter structure, which is a simple integer.
    */
    std::shared_ptr<Counter> count = std::make_shared<Counter>();
    count->num=0;
    /* the world variable will become the NFA */
    std::shared_ptr<World> world = std::make_shared<World>();

    /******************************************************************/
    /** An Example of all the functions available used to make a NFA **/
    /**    g(a|c)+(tc+)+a+                                           **/
    /******************************************************************/
    /*
    *world = *connect(
            connect(
                new_world('g', count),
                add_plus(
                    add_or(
                        new_world('a', count),
                        new_world('c', count)
                       )
                    )
                   )
            ,
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

    /* The NFA for GGAGTGCAAGCGTT */
    /*
    *world = *connect(new_world('g', count), connect(new_world('g', count), new_world('a', count)));
    *world = *connect(world, connect(new_world('g', count), connect(new_world('t', count), new_world('g', count))));
    *world = *connect(world, connect(new_world('c', count), connect(new_world('a', count), new_world('a', count))));
    *world = *connect(world, connect(new_world('g', count), connect(new_world('c', count), new_world('g', count))));
    *world = *connect(world, new_world('t', count));
    *world = *connect(world, new_world('t', count));
    */
    /* The NFA for TGCAAGCGTTAAT */
    *world = *connect(connect(
                connect(
                    connect(new_world('t', count), new_world('g', count)),
                    connect(new_world('c', count), new_world('a', count))
                  ),
                connect(
                    connect(new_world('a', count), new_world('g', count)),
                    connect(new_world('c', count), new_world('g', count))
                )
            ),
            connect(connect(new_world('t', count), new_world('t', count)), connect(connect(new_world('a', count), new_world('a', count)), new_world('t',count))));

    /* After an NFA is created, the states is told where to terminate */
    states->end = world->end;

    /* if the flag is set, a digraph NFA is printed */
    if(showgraph)
        std::cout << *world;

    /* The buffer for reading data */
    char * buffer;
    /* The length of the file being read */
    int length;
    /* Size of the chunks being read into buffer, can be increased for more RAM usage */
    int read_chunk;

    /* Reads file from last argument in argv */
    std::ifstream is (argv[argc-1], std::ifstream::binary);
    if (is) {
        read_chunk = 1024*250000;

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

        /* keeps track of number of hits */
        int hits = 0;
        /* When printing hits, keep track of location, across buffer chunks */
        int cnt;

        // read data as a block:
        while( !is.eof() and (read_chunk > 0)) {
            is.read (buffer,read_chunk);
            /* output will hold any hits, and print them after each iteration */
            std::string output = "";
            /* feedback code from states, 0 means no hits, anything else is a hit, and the length of the hit */
            int result = 0;            
            for(int i = 0; i < read_chunk; i++)
            {
                cnt += 1;
                if(isspace(buffer[i])){
                    // DO NOTHING if the character being parsed is a whitespace, newline or of that sort
                }else{
                    char chr = tolower(buffer[i]);
                    // new character, new state, start at front of NFA
                    states->add_state(states->states.end(), world->start);
                    states->chr = &chr;
                    // call function in states, to see if the character results in a match
                    result = states->check_values();

                    if(result!=0){
                        if(!showhits)
                            output += std::string(buffer).substr(i-result, result) + "\[" + f(cnt-result) + ":" + f(cnt) +  "]\n";
                        else
                            hits += 1;
                    }
                }
                if(!showhits){
                    // Prints hits after each sign
                    std::cout << output;
                    output.clear();
                }
            }
            length -= read_chunk;
            if(length < read_chunk)
                read_chunk = length;
        }
        if(showhits)
            std::cout << "Hits: " << hits << std::endl;
        is.close();
    }
}
