#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <queue>

#include "Room.h"

using namespace std;

int main(){

    Room room;
    char txt[] = "room.txt";
    room.read(txt);
    room.buildPathTree();
    char output[] = "graph.dot";
    room.printPathTree(output);
    room.printChosenPath(0);
    room.printChosenPath(1);
    room.printChosenPath(2);
    room.twoDronesMostPaint();
    return 0;    
}