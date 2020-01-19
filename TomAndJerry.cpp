#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <queue>

#include "Room.h"


/// compile with this command in Unix systems:
/// g++ TomAndJerry.cpp Room.cpp PathsTree.cpp -lcurses

/// command for dotty to pdf
/// dot -Tpdf bigGraph.dot -o bigGraph.pdf

int main(){

    Room room;
    char txt[] = "room.txt";
    room.read(txt);
    room.buildPathTree();
    char output[] = "graph.dot";
    room.printPathTree(output);
    room.printChosenPath(1);
    room.printChosenPath(12);
    room.printChosenPath(18);
    room.twoDronesMostPaint();
    room.maxPaint();
   
    return 0;    
}