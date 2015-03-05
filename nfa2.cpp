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
        void add_node(char,Node*);
        bool accepting;
        char next;
        Node* next_node;
        Node* epsilon[2];
};

Node::Node(int cur){
    number = cur;
    next = '\0';
    accepting = false;
}

void Node::add_epsilon(Node* node){
    epsilon[0] = node;
}

void Node::add_node(char reg,Node* node){
    next = reg;
    next_node = node;
}

//Create a ? subexpression
Node* optional(Node* start, char reg, int cur){
    Node* next = new Node(cur);
    start->next = reg;
    start->next_node = next;
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
    start->epsilon[1] = nodes[2];
    nodes[1]->add_epsilon(nodes[2]);
    nodes[0]->next = reg;
    nodes[0]->next_node = nodes[1]; 
    return nodes[2];
}

//Create a + subexpression
Node* plus(Node* start, char reg,int cur){
    Node* nodes [3];
    for(int i = 0; i < 3; i++){
        nodes[i] = new Node(cur+i);
    } 
    start->add_epsilon(nodes[0]);
    nodes[0]->next = reg;
    nodes[0]->next_node = nodes[1];
    nodes[1]->epsilon[1] = nodes[0];
    nodes[1]->add_epsilon(nodes[2]);
    return nodes[2];
}

//Create a normal subexpression
Node* standard(Node* start, char reg1,int cur){
    Node* next = new Node(cur);
    start->next = reg1;
    start->next_node = next;
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
        cur_node->next = regex[i];
        cur_node->next_node = last;
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


Node* parse_regex(std::string regex){
    Node* parentheses[10][2];
    int par_count = 0;
    Node* init_node = new Node(0);
    Node* start_node = init_node;
    Node* temp_node;
    Node* end_node;
    int cur = 1;
    std::string sub_reg;
    for(int i = 0; i < regex.length(); i++){
        if(regex[i] == '('){
                temp_node = new Node(cur);
                cur++;
                start_node->add_epsilon(temp_node);
                
                parentheses[par_count][0] = start_node;  
                
                int a;
                a = read_until(regex,i+1);
                sub_reg = regex.substr(i+1,a-i);
                end_node = create_subexpr(sub_reg,temp_node,cur);
                
                parentheses[par_count][1] = end_node; 
                cur = end_node->number+1;
                
                start_node = end_node;
                i = a;
                par_count++;
        } if(regex[i] == ')'){
            par_count--;
            switch(regex[i+1]){
                case '?':
                    break;
                case '+':
                    temp_node = parentheses[par_count][0]->epsilon[0];
                    end_node->epsilon[1] = temp_node;
                    i++;
                    break;
                case '*':

                    break;
                default:
                    break;
            }
        }
    }
    temp_node = new Node(cur);
    end_node->add_epsilon(temp_node);
    temp_node->accepting = true;

    return init_node;
}

void printNFA(Node* start_node){
    Node* next = start_node;
    std::cout << "start in 0 \n";
    while(!next->accepting){
        if(next->next == '\0'){
            std::cout << "epsilon to " << next->epsilon[0]->number << "\n";
            if(next->epsilon[1] != 0){
            std::cout << "epsilon to " << next->epsilon[1]->number << "\n";
            }
            next = next->epsilon[0];
        }
        else{
            std::cout << next->next << " to " << next->next_node->number << "\n";
            next = next->next_node;
        }
    }   
}


int main(){
    //std::cout << read_until("aasss(sasas",0);
    Node* reg = parse_regex("(12(34)+)+");
    printNFA(reg);
  return 0;
}