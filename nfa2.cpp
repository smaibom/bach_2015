#include <string.h>
#include <stdio.h>
#include <iostream>
#include <map> 
#include <list>

class Node{
    public:
        Node(int);
        int number;
        void add_epsilon(Node*);
        bool accepting;
        std::map<char,Node*> nodes;
        std::list<Node*> epsilon_trans;
};

Node::Node(int cur){
    number = cur;
    accepting = false;
}

void Node::add_epsilon(Node* node){
    std::list<Node*>::iterator it;
    it = epsilon_trans.begin();
    epsilon_trans.insert(it,node);
}

//Create a ? subexpression
Node* optional(Node* start, char reg, int cur){
    Node* next = new Node(cur);
    start->nodes[reg] = next;
    start->add_epsilon(next);
    return next;
}

//Create a * subexpression
Node* star(Node* start, char reg, int cur){
    Node* nodes[3];
    for(int i = 0; i < 3; i++){
        nodes[i] = new Node(cur+i);
    }
    start->add_epsilon(nodes[0]);
    start->add_epsilon(nodes[2]);
    nodes[1]->add_epsilon(nodes[0]);
    nodes[1]->add_epsilon(nodes[2]);
    nodes[0]->nodes[reg] = nodes[1];
    return nodes[2];
}

//Create a + subexpression
Node* plus(Node* start, char reg,int cur){
    Node* nodes [3];
    for(int i = 0; i < 3; i++){
        nodes[i] = new Node(cur+i);
    } 
    start->add_epsilon(nodes[0]);
    nodes[0]->nodes[reg] = nodes[1];
    nodes[1]->add_epsilon(nodes[0]);
    nodes[1]->add_epsilon(nodes[2]);
    return nodes[2];
}

//Create a normal subexpression
Node* standard(Node* start, char reg1,int cur){
    Node* next = new Node(cur);
    start->nodes[reg1] = next;
    return next;
}

/*Creates subexpressions of regex containing alphabet, ?, *, +
Takes a startnode, a regular expression string and the current node number
returns the end node of the subexpression
*/
Node* create_subexpr(std::string regex,Node* start_node,int cur_number){
    Node* cur_node = start_node;
    int i;
    int cur = cur_number;
    for(i=0;i<(regex.length()-1);i++){
        switch(regex[i+1]){
            case '?':
                cur_node = optional(cur_node,regex[i],cur);
                cur++;
                i++;
                break;
            case '*':
                cur_node = star(cur_node,regex[i],cur);
                cur += 3;
                i++;
                break;
            case '+':
                cur_node = plus(cur_node,regex[i],cur);
                cur += 3;
                i++;
                break;
            default:
                cur_node = standard(cur_node,regex[i],cur);
                cur++;
                break;
        }
    }
    if(i < regex.length()){
        Node* last = new Node(cur);
        cur_node->nodes[regex[i]] = last;
        cur_node = last;
    }
    return cur_node;
}


int read_until(std::string regex, int start){
    int i = 0;
    for(i = start; i < regex.length(); i++){
        if(regex[i] == '(' || regex[i] == ')'){
            return i-1;
        }
    }
    return i;
}

parse_regex(Node* start_node, std::string regex, int cur){

}

/*
Node* parse_regex(Node* start_node, std::string regex){
    std::map<int,Node*> parentheses;
    int par_count = 0;
    Node* cur_node = start_node;
    std::string reg;
    for(int i = 0; i < regex.length(); i++){
        switch(regex[i]){
            case '(':
                parentheses[par_count] = cur_node;
                par_count++;
                int a; 
                a = read_until(regex,i+1);
                reg = regex.substr(i+1,a-i);
                create_regex(reg,cur_node);
                break;
            case ')':
                
                par_count--;
                return cur_node;
                break;
            default:
                Node* new_node = new Node();
                cur_node->nodes[regex[i]] = new_node;
                break;
        }
    }
    return NULL;
}*/



int main(){
    Node* ne = new Node(0);
    Node* reg = create_regex("abcd+",ne,1);
    std::cout << reg->number;
  return 0;
}