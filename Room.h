#ifndef ROOM_H
#define ROOM_H

#include "node.h"
#include "PathInfo.h"
#include "Matrix.h"
#include "PathsTree.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <queue>


class Room{

    int n, m;
    pair<int, int> jerry;
    pair<int, int> tom;

    Matrix<int> blocked;
    Matrix<int> canPaint;
    Matrix<node*> roomNodes;

    std::vector<pair<int, int>> adjacentPosition = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    PathsTree pathsTree;

    bool isPosOkay(const pair<int, int>& pos);

    void initRoomNodes();

    void clearRoomNodes();

    void makeRoomEdges();

    PathInfo dfsMaxPaint(pair<int, int>, Matrix<int>&, char);

    void animate(string);

public:

    Room();

    ~Room();

    void read(const char file[256]);

    void buildPathTree();

    void printPathTree(const char file[]) const;

    void printChosenPath(int idx);

    void twoDronesMostPaint() const;

    void maxPaint();
    
};

#endif
