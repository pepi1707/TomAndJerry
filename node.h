#ifndef NODE_H
#define NODE_H

#include <vector>

using namespace std;

struct node{

    char symbol;
    pair<int, int> pos;
    vector<node*> next;
    int cntLeaves;

    node(){
        cntLeaves = 0;
    }

    node(const char _symbol, const pair<int, int> _pos){
        symbol = _symbol;
        pos = _pos;
        cntLeaves = 0;
    }

    void addNext(node* _next){
        next.push_back(_next);
    }

};

#endif