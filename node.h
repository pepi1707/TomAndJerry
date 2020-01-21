#ifndef NODE_H
#define NODE_H

#include <vector>

struct node{

    char symbol;
    std::pair<int, int> pos;
    std::vector<node*> next;
    int cntLeaves;

    node(){
        cntLeaves = 0;
    }

    node(const char _symbol, const std::pair<int, int> _pos){
        symbol = _symbol;
        pos = _pos;
        cntLeaves = 0;
    }

    void addNext(node* _next){
        next.push_back(_next);
    }

};

#endif