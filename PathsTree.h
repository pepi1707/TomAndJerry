#ifndef PATHSTREE_H
#define PATHSTREE_H

#include "node.h"
#include "PathInfo.h"
#include <fstream>

class PathsTree{

    node* root; 

    void clearTreeNodes(const node* cur) const;

    void dfsBuild(node* curPathNode, const node* curRoomNode, const std::pair<int,int>& jerry) const;

    void dfsPrint(const node* cur, ofstream& out, int idx) const;

    PathInfo dfsFindChosenPath(const node* cur, int idx, const char& prevSymbol) const;

    PathInfo dfsFindPathWithoutRepetition(const node* cur1, const node* cur2, int idx1, int idx2, const char& prevSymbol) const;

public:

    PathsTree();

    ~PathsTree();

    void build(const node* tom, const std::pair<int, int>& jerry);

    void print(ofstream& out, int idx) const;

    PathInfo findChosenPath(int idx) const;

    PathInfo findPathWithoutRepetition(int idx1, int idx2) const;

    int numberOfLeaves() const;


};


char symbolFor(const std::pair<int, int>& from, const std::pair<int, int>& to);

bool isTurn(const char& c1, const char& c2);


#endif