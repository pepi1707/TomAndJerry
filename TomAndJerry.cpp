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

/// most paint answer if too slow
/// SSESSPSSSWWWPNNNNNNNPNNEEESSEEEESPENNNESPSSSWPSESSSSWWWPNNE

int main(){

    Room room;
    char txt[] = "bigger-room.txt";
    room.read(txt);
    room.buildPathTree();
    char output[] = "bigGraph.dot";
    room.printPathTree(output);
    room.printChosenPath(1);
    room.printChosenPath(12);
    room.printChosenPath(18);
    room.twoDronesMostPaint();
    room.maxPaint();
   
    return 0;    
}