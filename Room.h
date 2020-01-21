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
#include <cassert>


class Room{

    int n, m;
    std::pair<int, int> jerry;
    std::pair<int, int> tom;

    Matrix<int> blocked;
    Matrix<int> canPaint;
    Matrix<node*> roomNodes;

    std::vector<std::pair<int, int>> adjacentPosition = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    PathsTree pathsTree;

    bool isRealPos(const std::pair<int, int>& pos) const;

    bool isPosOkay(const std::pair<int, int>& pos) const;

    void initRoomNodes();

    void clearRoomNodes();

    void makeRoomEdges();

    PathInfo dfsMaxPaint(std::pair<int, int>, Matrix<int>&, char) const;

    void animate(string) const;

public:

    Room();

    ~Room();

    void read(const char file[256]);

    void buildPathTree();

    void printPathTree(const char file[]) const;

    void printChosenPath(int idx) const;

    void twoDronesMostPaint() const;

    void maxPaint() const;
    
};

#endif
