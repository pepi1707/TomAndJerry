#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <queue>

#include "Room.h"

using namespace std;

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