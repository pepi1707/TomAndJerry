#include "Room.h"

bool Room::isPosOkay(const pair<int, int>& pos){
    return pos.first >= 0 && pos.first < m && pos.second >=0 && pos.second < n && !blocked[pos.first][pos.second];
}

void Room::initRoomNodes(){
    roomNodes.build(m, n);
    for(int i = 0; i < m; i ++){
        for(int j = 0; j < n; j ++){
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
    Matrix<int> used(m, n, -1);
    queue<node* > q;
    q.push(roomNodes[jerry]);
    used[jerry] = 1;
    while(!q.empty()){
        
        node* w = q.front(); q.pop();
        if(canPaint[w->pos]){
            w->symbol = 'P';
        }

        for(int i = 0; i < adjacentPosition.size(); i ++){
            pair<int, int> nb_pos = {w->pos.first + adjacentPosition[i].first, w->pos.second + adjacentPosition[i].second};
            if(isPosOkay(nb_pos)){
                node* nb = roomNodes[nb_pos];
                if(used[nb_pos] == used[w->pos] - 1){
                    w->addNext(nb);
                }
                else if(used[nb_pos] == -1){
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

    blocked.build(m, n, 0);
    canPaint.build(m, n, 0);      

    in >> jerry.first >> jerry.second;
    in >> tom.first >> tom.second;

    int k, l;
    in >> k >> l;
    
    while(k--){
        int x, y;
        in >> x >> y;
        string line;
        getline(in, line);
        getline(in, line);
        while(line != "==="){
                
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

void Room::printChosenPath(int idx){
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

PathInfo Room::dfsMaxPaint(pair<int, int> cur, Matrix<int>& used, char prSymbol){
    
    if(cur == jerry){
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
            PathInfo path = dfsMaxPaint(nb, used, symbolFor(cur, nb));
            used[nb] = 0;
            if(path.pathLen >= 0){
                path.turns += isTurn(prSymbol, symbolFor(cur, nb));
                path.pathLen++;
                path.path = symbolFor(cur, nb) + path.path;
                if((path.painted == res.painted && path.turns < res.turns) || path.painted > res.painted){
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

void Room::maxPaint(){
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


void Room::animate(string dronePath){
    pair<int, int> drone = tom;
    Matrix<char> painted(m, n, '.');
    for(int x = 0; x < dronePath.size(); x ++){
        if(dronePath[x] == 'P'){
            painted[drone] = 'P';
        } else if(dronePath[x] == 'S'){
            if(painted[drone] != 'P')
                painted[drone] = '|';
            drone.first ++;
        } else if(dronePath[x] == 'N'){
            if(painted[drone] != 'P')
                painted[drone] = '|';
            drone.first --;
        } else if(dronePath[x] == 'E'){
            if(painted[drone] != 'P')
                painted[drone] = '-';
            drone.second ++;
        } else if(dronePath[x] == 'W'){
            if(painted[drone] != 'P')
                painted[drone] = '-';
            drone.second --;
        }
        ClearScreen();
        for(int i = 0; i < m; i ++){
            for(int j = 0; j < n; j ++){
                pair<int, int> cur({i, j});
                if(cur == drone){
                    std::cout << '*';
                    continue;
                }
                if(cur == tom){
                    std::cout << 'T';
                    continue;
                }
                if(cur == jerry){
                    std::cout << 'J';
                    continue;
                }
                if(blocked[i][j]){
                    std::cout << 'B';
                    continue;
                }
                std::cout << painted[i][j];
            }
            std::cout << "\n";
        }
        usleep(300000);
    }
}