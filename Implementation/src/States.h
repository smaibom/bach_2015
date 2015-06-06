#include "State.h"
#include <vector>
#include <algorithm>
#include <string>
#include <deque>
#include <vector>

/*
Class for holding states, and checking states against a given character.
Holds the default number of deletions, mutations and insertions allowed.
Along with an iterator for moving through a vector of State objects.
*/
class States{
    public:
        int deletions;
        int mutations;
        int insertions;
        std::vector<State>::iterator it;
        char *chr;
        /* Return value, used to given path length when a match is found */
        int ret;
        /* Holds all usable State Objects, unmatched states are removed */
        std::vector<State> states;
        /*
        The final state of the NFA, when a State has a note similar to this
        it means there's a match, which may be empty depending on the NFA.
        */
        std::shared_ptr<Node> end;

        /* Function for adding State Objects to the structure */
        void add_state(std::vector<State>::iterator position, std::shared_ptr<Node> node);
        void add_state(std::vector<State>::iterator position, std::shared_ptr<Node> node, int pat, int ins, int del, int mut);

        States(){
            /* 
			Preallocates some memory
			This needs to be big enough for all states,
			more mismatches need more memory 
			*/
            states.reserve(10000);
        }
        ~States(){
            // deconstructor
        }
        /* Function that loops through the Vector states. */
        int check_values();
};

/* Function that creates a new node at the given location */
void States::add_state(std::vector<State>::iterator position, std::shared_ptr<Node> node){
    if(node->next){
        states.emplace(position);
        (*position).node = node;
        (*position).path = 0;
        (*position).insertions = insertions;
        (*position).deletions = deletions;
        (*position).mutations = mutations;
    }
    /*
    if(node->epsilon_left){
        add_state(position, node->epsilon_left);
    }
    if(node->epsilon_right){
        add_state(position, node->epsilon_right);        
    }
    */
}

/* Same function as above, but with handles for insertions, mutations and the like */
void States::add_state(std::vector<State>::iterator position,
                       std::shared_ptr<Node> node,
                       int pat, int ins, int del, int mut)
{
    if(node->next){
        states.emplace(position);
        (*position).node = node;
        (*position).path = pat;
        (*position).insertions = ins;
        (*position).deletions = del;
        (*position).mutations = mut;
    }
    /*
    if(node->epsilon_left){
        add_state(position, node->epsilon_left,  pat,  ins,  del,  mut);
    }
    if(node->epsilon_right){
        add_state(position, node->epsilon_right,  pat,  ins,  del,  mut);        
    }
    */
}

/*
Loops through all the states available, checking if there's a match with the
current character, if there's a match, new states will be made, and added
behind the iterator, making sure they're not processed immediately.
NOTE: Does not currently support epsilon transitions.
*/
int States::check_values(){
    ret = 0;
    it = states.begin();
    while (it != states.end() )
    {
        // Since I started to use vectors my usual way of handling epsilons broke, so currently there's no epsilon transition support
        // The problem is that I need to add the epsilon transition nodes at a place the iterator will reach them this loop, 
        // unlike insertions mutations and deletions which are ready for the next iteration once handled here.
        // I plan on adding support, but right now my focus is on the report, and not messing with runtime.
        
        if( (*it).node == end ){
                if(ret < (*it).path)
                    ret = (*it).path;
            states.erase(it++);
        }else{
/*
            if((*it).node->epsilon_left){
                add_state(it+1, (*it).node->epsilon_left, (*it).path, (*it).insertions, (*it).deletions, (*it).mutations);
            }
            if((*it).node->epsilon_right){
                add_state(it+1, (*it).node->epsilon_right, (*it).path, (*it).insertions, (*it).deletions, (*it).mutations);        
            }
*/          
            if((*it).node->chr == *chr){
                (*it).node = (*it).node->next;
                (*it).path += 1;
                ++it;
            }else{
                if((*it).insertions > 0){
                    add_state(it, (*it).node, (*it).path+1, (*it).insertions-1, (*it).deletions, (*it).mutations);
                    ++it;
                }
                if((*it).mutations > 0){
                    add_state(it, (*it).node->next, (*it).path+1, (*it).insertions, (*it).deletions, (*it).mutations-1);
                    ++it;
                }
                if((*it).deletions > 0){
                    add_state(it, (*it).node->next, (*it).path, (*it).insertions, (*it).deletions-1, (*it).mutations);
                    ++it;
                }
                states.erase(it);
            }
        }
    }
    return ret;
}