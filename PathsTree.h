#ifndef PATHSTREE_H
#define PATHSTREE_H

#include "node.h"
#include "PathInfo.h"
#include <fstream>

class PathsTree{

    node* root; 

    void clearTreeNodes(const node* const cur) const;

    void dfsBuild(node* const curPathNode, const node* const curRoomNode, const pair<int,int>& jerry) const;

    void dfsPrint(const node* const cur, ofstream& out, int idx) const;

    PathInfo dfsFindChosenPath(const node* const cur, int idx, const char& prevSymbol) const;

    PathInfo dfsCommonPath(const node* const cur, int idx1, int idx2, const char& prevSymbol) const;

public:

    PathsTree();

    ~PathsTree();

    void build(const node* const tom, const pair<int, int>& jerry);

    void print(ofstream& out, int idx) const;

    PathInfo findChosenPath(int idx, const char& prevSymbol) const;

    PathInfo commonPath(int idx1, int idx2, const char& prevSymbol) const;

    int numberOfLeaves() const;


};


char symbolFor(pair<int, int> from, pair<int, int> to);

bool isTurn(const char& c1, const char& c2);


#endif