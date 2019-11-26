#ifndef ROOM_H
#define ROOM_H

#include "node.h"
#include "PathInfo.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <queue>

class Room{

    int n, m;
    pair<int, int> jerry;
    pair<int, int> tom;

    bool **blocked;
    bool **canPaint;
    node ***roomNodes;

    node *root;

    bool isPosOkay(pair<int, int> pos);

    char symbolFor(pair<int, int> from, pair<int, int> to);

    int isTurn(const char& c1, const char& c2);

    void initRoomNodes();

    void clearRoomNodes();

    void clearPathNodes(node* cur);

    void makeRoomEdges();

    void dfsBuildPathTree(node* curPath, node* curPoint);

    void dfsPrintPathTree(const node* cur, ofstream& out, int idx);

    PathInfo realPrintChosenPath(node *cur, int idx, char prevSymbol);

    PathInfo commonPath(node *cur, int idx1, int idx2, char prevSymbol);

public:

    Room();

    ~Room();

    void read(char file[256]);

    void buildPathTree();

    void printPathTree(const char file[]);

    void printChosenPath(int idx);

    void twoDronesMostPaint();
    
};

#endif
