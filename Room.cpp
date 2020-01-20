#include "Room.h"

bool Room::isRealPos(const pair<int, int>& pos) const{
    return pos.first >= 0 && pos.first < m && pos.second >= 0 && pos.second < n;
}

bool Room::isPosOkay(const pair<int, int>& pos) const{
    return isRealPos(pos) && !blocked[pos];
}

void Room::initRoomNodes(){
    roomNodes.build(m, n);
    for(int i = 0; i < m; i ++){
        for(int j = 0; j < n; j ++){
            //'X' is a character without a meaning
            roomNodes[i][j] = new node('X', {i, j});
        }
    }
}

void Room::clearRoomNodes(){
    for(int i = 0; i < m; i ++){
        for(int j = 0; j < n; j ++){
            delete roomNodes[i][j];
        }
    }
}


void Room::makeRoomEdges(){

    //start a bfs from jerry (destination)
    Matrix<int> used(m, n, -1);
    queue<node* > q;
    q.push(roomNodes[jerry]);
    used[jerry] = 1;
    while(!q.empty()){
        
        node* w = q.front(); q.pop();
        if(canPaint[w->pos]){
            w->symbol = 'P';
        }
        
        //going through adjacent position (neighbours)
        for(int i = 0; i < adjacentPosition.size(); i ++){

            pair<int, int> nb_pos = {w->pos.first + adjacentPosition[i].first, w->pos.second + adjacentPosition[i].second};

            if(isPosOkay(nb_pos)){
                node* nb = roomNodes[nb_pos];
                if(used[nb_pos] == used[w->pos] - 1){
                    //then nb is from the previous level, so w->nb should exist in the graph
                    w->addNext(nb);
                }
                else if(used[nb_pos] == -1){
                    //then the nb vertex is not yet visited, and we should add it to the queue and mark it as visited
                    used[nb_pos] = used[w->pos] + 1;
                    q.push(nb);
                }
            }
        }

    }

}  

Room::Room(){
    
}

Room::~Room(){
    clearRoomNodes();
}

void Room::read(const char file[256]){
    
    ifstream in(file);

    in >> m >> n;

    assert(m >= 0 && n >= 0 && "Sizes of the room are with negative sign");

    blocked.build(m, n, 0);
    canPaint.build(m, n, 0);      

    in >> jerry.first >> jerry.second;
    in >> tom.first >> tom.second;

    assert(isRealPos(jerry) && "Jerry's position is invalid");
    assert(isRealPos(tom) && "Tom's position is invalid");

    int k, l;
    in >> k >> l;

    assert(k >= 0 && "Number of objects is negative");
    assert(l >= 0 && "Number of available positions for painting is negative");
    
    while(k--){
        int x, y;
        in >> x >> y;
        assert(isRealPos({x,y}) && "Object has an invalid starting position");
        string line;
        getline(in, line);
        getline(in, line);
        while(line != "==="){
            assert(isRealPos({x, y + line.size() - 1}) && "Object is placed outside of the room");
            for(int i = 0; i < line.size(); i ++){
                if(line[i] == '1'){
                    blocked[x][y + i] = true;
                }
            }
            x++;
            getline(in, line);
        }

    }
    for(int i = 0; i < l; i ++){
        int x, y;
        in >> x >> y;
        assert(isRealPos({x, y}) && "Available position for painting is out of bounds");
        canPaint[x][y] = true;
    }
    in.close();

}

void Room::buildPathTree(){
    initRoomNodes();
    makeRoomEdges();
    pathsTree.build(roomNodes[tom], jerry);
}

void Room::printPathTree(const char file[]) const{
    ofstream out(file);
    out << "digraph {\n";
    pathsTree.print(out, 0);
    out << "}";
    out.close();
}

void Room::printChosenPath(int idx) const{
    PathInfo pathInfo = pathsTree.findChosenPath(idx, 'T');
    animate(pathInfo.path);
    std::cout << "=========\nINFO FOR PATH NUMBER " << idx << ":\n";
    std::cout << "commands: " << pathInfo.path << "\n";
    std::cout << "length of path: " << pathInfo.pathLen << "\n";
    std::cout << "painted blocks: " << pathInfo.painted << "\n";
    std::cout << "turns: " << pathInfo.turns << "\n";
}

void Room::twoDronesMostPaint() const{

    int leaves = pathsTree.numberOfLeaves();
    int maxPaint = -1;
    int minTurns = m * n;
    std::vector<PathInfo> ans(3);

    for(int i = 0; i < leaves; i ++){
        PathInfo path1 = pathsTree.findChosenPath(i, 'T');
        for(int j = i + 1; j < leaves; j ++){
            PathInfo path2 = pathsTree.findChosenPath(j, 'T');
            PathInfo path3 = pathsTree.commonPath(i, j, 'T');
            int curPaint = path1.painted + path2.painted - path3.painted;
            int curTurns = path1.turns + path2.turns;
            if((curPaint == maxPaint && curTurns < minTurns) || curPaint > maxPaint){
                maxPaint = curPaint;
                minTurns = curTurns;
                ans = {path1, path2, path3};
            }

        }
    }

    std::cout << "========\nTWO DRONES CAN PAINT MAXIMUM:\n";
    std::cout << maxPaint << " blocks\n";
    std::cout << "with the minimum of " << minTurns << " turns amongst all\n";
    std::cout << "Drone commands:\n";
    std::cout << "First drone: " << ans[0].path << "\n";
    std::cout << "Second drone: ";
    int i;
    for(i = 0; i < ans[2].path.size(); i++){
        if(ans[1].path[i] == 'P'){
            continue;
        }
        std::cout << ans[1].path[i];
    }
    for(; i < ans[1].path.size(); i ++){
        std::cout << ans[1].path[i];
    }
    std::cout << "\n";
}

PathInfo Room::dfsMaxPaint(pair<int, int> cur, Matrix<int>& used, char prSymbol) const{
    
    if(cur == jerry){
        //then we arrived at destination and should terminate and return an empty path
        return PathInfo();
    }
    PathInfo res;
    res.painted = - m * n;
    res.pathLen = - m * n;
    pair<int, int> nb;
    for(int i = 0; i < adjacentPosition.size(); i ++){
        nb.first = cur.first + adjacentPosition[i].first;
        nb.second = cur.second + adjacentPosition[i].second;
        if(isPosOkay(nb) && !used[nb]){
            used[nb] = 1;
            //so that we don't visit it again
            PathInfo path = dfsMaxPaint(nb, used, symbolFor(cur, nb));
            used[nb] = 0;
            //so that we can visit it again
            if(path.pathLen >= 0){
                //then we have found a path to destination
                path.turns += isTurn(prSymbol, symbolFor(cur, nb));
                //did we make a turn?
                path.pathLen++;
                path.path = symbolFor(cur, nb) + path.path;
                if((path.painted == res.painted && path.turns < res.turns) || path.painted > res.painted){
                    //then this path is the best we have yet found
                    res = path;
                }
            }
        }
    }

    if(canPaint[cur]){
        res.path = 'P' + res.path;
        res.painted ++;
    }
    return res;
    
}

void Room::maxPaint() const{
    Matrix<int> used(m, n, 0);
    used[tom] = 1;
    PathInfo p = dfsMaxPaint(tom, used, 'X');
    animate(p.path);
    std::cout << "you can paint maximum: " << p.painted << " blocks\n";
    std::cout << "with these commands: " << p.path << "\n";
    std::cout << "with minimum of " << p.turns << " turns\n";
}


#include <unistd.h>
#include <term.h>

void ClearScreen()
{
    if (!cur_term)
    {
        int result;
        setupterm( NULL, STDOUT_FILENO, &result );
        if (result <= 0) return;
    }

    putp( tigetstr( "clear" ) );
}


void Room::animate(string dronePath) const{
    pair<int, int> drone = tom;
    std::cout << "okay\n";
    //a matrix for the positions in the room
    Matrix<char> painted(m, n, '.');
    //matrices for the transition between the positions in the room
    Matrix<char> directionsHor(m, n, ' ');
    Matrix<char> directionsVer(m, n, ' ');
    std::cout << "okay\n";
    for(int x = 0; x < dronePath.size(); x ++){
        if(dronePath[x] == 'P'){
            painted[drone] = 'P';
        }
        if(dronePath[x] == 'S'){
            directionsVer[drone] = '|';
            drone.first ++;
        }
        if(dronePath[x] == 'N'){
            drone.first --;
            directionsVer[drone] = '|';
        }
        if(dronePath[x] == 'E'){
            directionsHor[drone] = '-';
            drone.second ++;
        }
        if(dronePath[x] == 'W'){
            drone.second --;
            directionsHor[drone] = '-';
        }
        ClearScreen();
        for(int i = 0; i < m; i ++){
            for(int j = 0; j < n; j ++){
                pair<int, int> cur({i, j});
                if(cur == drone)
                    std::cout << '*';
                else if(cur == tom)
                    std::cout << 'T';
                else if(cur == jerry)
                    std::cout << 'J';
                else if(blocked[i][j])
                    std::cout << 'B';
                else
                    std::cout << painted[i][j];
                std::cout << directionsHor[i][j];
            }
            std::cout << "\n";
            for(int j = 0; j < n; j ++){
                std::cout << directionsVer[i][j] << " ";
            }
            std::cout << "\n";
        }
        usleep(300000);
    }
}