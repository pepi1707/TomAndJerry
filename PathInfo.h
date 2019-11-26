#ifndef PATHINFO_H
#define PATHINFO_H

#include <iostream>
#include <cstring>

using namespace std;

struct PathInfo{
    
    string path;
    int pathLen;
    int painted;
    int turns;

    PathInfo(){
        path = "";
        pathLen = painted = turns = 0;
    }

    PathInfo(string _path, int _pathLen, int _painted, int _turns) : path(_path),  pathLen(_pathLen), painted(_painted), turns(_turns){

    }
};

#endif