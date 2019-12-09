#include "Room.h"

bool Room::isPosOkay(pair<int, int> pos){
    return pos.first >= 0 && pos.first < m && pos.second >=0 && pos.second < n && !blocked[pos.first][pos.second];
}

char Room::symbolFor(pair<int, int> from, pair<int, int> to){
    if(from.first == to.first){
        if(from.second < to.second){
            return 'E';
        }
        else{
            return 'W';
        }
    }
    if(from.first < to.first){
        return 'S';
    }
    else{
        return 'N';
    }
    return 'x';
}

int Room::isTurn(const char& c1, const char& c2){
    return ((c1 == 'N' || c1 == 'S') && (c2 == 'E' || c2 == 'W')) ||
            ((c1 == 'E' || c1 == 'W') && (c2 == 'N' || c2 == 'S'));
}

void Room::initRoomNodes(){
    roomNodes = new node**[m];
    for(int i = 0; i < m; i ++){
        roomNodes[i] = new node*[n];
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
        delete[] roomNodes[i];
    }
    delete[] roomNodes;
}

void Room::clearPathNodes(node* cur){
    for(int i = 0; i < cur->next.size(); i++){
        clearPathNodes(cur->next[i]);
    }
    delete cur;
}

void Room::makeRoomEdges(){
    initRoomNodes();
    int **used = new int*[m];
    for(int i = 0; i < m; i ++){
        used[i] = new int[n];
        for(int j = 0; j < n; j ++){
            used[i][j] = -1;
        }
    }
    queue<node* > q;
    q.push(roomNodes[jerry.first][jerry.second]);
    used[jerry.first][jerry.second] = 1;
    while(!q.empty()){
        
        node* w = q.front(); q.pop();

        pair<int, int> nb_pos = w->pos;
        
        nb_pos.first ++;
        if(isPosOkay(nb_pos)){
            node* nb = roomNodes[nb_pos.first][nb_pos.second];
            if(used[nb_pos.first][nb_pos.second] == used[w->pos.first][w->pos.second] - 1){
                w->addNext(nb);
            }
            else if(used[nb_pos.first][nb_pos.second] == -1){
                used[nb_pos.first][nb_pos.second] = used[w->pos.first][w->pos.second] + 1;
                q.push(nb);
            }
        }

        nb_pos = w->pos;
        nb_pos.first --;
        if(isPosOkay(nb_pos)){
            node* nb = roomNodes[nb_pos.first][nb_pos.second];
            if(used[nb_pos.first][nb_pos.second] == used[w->pos.first][w->pos.second] - 1){
                w->addNext(nb);
            }
            else if(used[nb_pos.first][nb_pos.second] == -1){
                used[nb_pos.first][nb_pos.second] = used[w->pos.first][w->pos.second] + 1;
                q.push(nb);
            }
        }

        nb_pos = w->pos;
        nb_pos.second ++;
        if(isPosOkay(nb_pos)){
            node* nb = roomNodes[nb_pos.first][nb_pos.second];
            if(used[nb_pos.first][nb_pos.second] == used[w->pos.first][w->pos.second] - 1){
                w->addNext(nb);
            }
            else if(used[nb_pos.first][nb_pos.second] == -1){
                used[nb_pos.first][nb_pos.second] = used[w->pos.first][w->pos.second] + 1;
                q.push(nb);
            }
        }
        
        nb_pos = w->pos;
        nb_pos.second --;
        if(isPosOkay(nb_pos)){
            node* nb = roomNodes[nb_pos.first][nb_pos.second];
            if(used[nb_pos.first][nb_pos.second] == used[w->pos.first][w->pos.second] - 1){
                w->addNext(nb);
            }
            else if(used[nb_pos.first][nb_pos.second] == -1){
                used[nb_pos.first][nb_pos.second] = used[w->pos.first][w->pos.second] + 1;
                q.push(nb);
            }
        }
    }

}

void Room::dfsBuildPathTree(node* curPath, node* curPoint){
    if(canPaint[curPath->pos.first][curPath->pos.second] && curPath->symbol != 'P'){
        node* toPaint = new node('P', curPath->pos);
        curPath->addNext(toPaint);
        dfsBuildPathTree(toPaint, curPoint);
        curPath->cntLeaves = toPaint->cntLeaves;
        return;
    }
    if(curPoint->pos == jerry){
        curPath->cntLeaves = 1;
        return;
    }
    for(int i = 0; i < curPoint->next.size(); i ++){
        node* nbPoint = curPoint->next[i];
        node* newPath = new node(symbolFor(curPath->pos, nbPoint->pos), nbPoint->pos);
        curPath->addNext(newPath);
        dfsBuildPathTree(newPath, nbPoint);
        curPath->cntLeaves += newPath->cntLeaves;
    }   
}

void Room::dfsPrintPathTree(const node* cur, ofstream& out, int idx){
    out << "\t" << (long)cur << "[label=\"" << cur->symbol;
    if(cur->next.size() == 0){
        out << ", idx: " << idx; 
    }
    out << "\"];\n";
    for(int i = 0; i < cur->next.size(); i ++){
        node *nb = cur->next[i];
        out << "\t" << (long)cur << " -> " << (long)nb << ";\n";
        if(i != 0)
            dfsPrintPathTree(nb, out, idx + cur->next[i-1]->cntLeaves);
        else
            dfsPrintPathTree(nb, out, idx);
    }
}

PathInfo Room::realPrintChosenPath(node *cur, int idx, char prevSymbol){
    
    for(int i = 0; i < cur->next.size(); i ++){
        if(idx >= cur->next[i]->cntLeaves){
            idx -= cur->next[i]->cntLeaves;
            continue;
        }
        node* nb = cur->next[i];
        PathInfo curPath, restPath = realPrintChosenPath(nb, idx, nb->symbol == 'P' ? cur->symbol : nb->symbol);
        curPath.path = nb->symbol + restPath.path;
        curPath.turns = isTurn(prevSymbol, nb->symbol) + restPath.turns;
        curPath.pathLen = (nb->symbol != 'P') + restPath.pathLen;
        curPath.painted = (nb->symbol == 'P') + restPath.painted;

        return curPath;
    }
    return PathInfo();
}

PathInfo Room::commonPath(node *cur, int idx1, int idx2, char prevSymbol){

    for(int i = 0; i < cur->next.size(); i ++){
        if(idx2 >= cur->next[i]->cntLeaves){
            idx1 -= cur->next[i]->cntLeaves;
            idx2 -= cur->next[i]->cntLeaves;
            continue;
        }
        if(idx1 < 0){
            return PathInfo();
        }
        node* nb = cur->next[i];
        PathInfo curPath, restPath = commonPath(nb, idx1, idx2, nb->symbol == 'P' ? cur->symbol : nb->symbol);
        curPath.path = nb->symbol + restPath.path;
        curPath.turns = isTurn(prevSymbol, nb->symbol) + restPath.turns;
        curPath.pathLen = (nb->symbol != 'P') + restPath.pathLen;
        curPath.painted = (nb->symbol == 'P') + restPath.painted;

        return curPath;
    }
    return PathInfo();
}  

Room::Room(){
    blocked = nullptr;
    canPaint = nullptr;
}

Room::~Room(){
    for(int i = 0; i < m; i ++){
        delete[] blocked[i];
        delete[] canPaint[i];
    }
    delete[] blocked;
    delete[] canPaint;
    clearRoomNodes();
    clearPathNodes(root);
}

void Room::read(char file[256]){
    
    ifstream in(file);

    in >> m >> n;

    blocked = new bool*[m];
    canPaint = new bool*[m];

    for(int i = 0; i < m; i ++){
        blocked[i] = new bool[n];
        canPaint[i] = new bool[n];
        for(int j = 0; j < n; j++){
            blocked[i][j] = canPaint[i][j] = false;
        }
    }        

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
    makeRoomEdges();
    root = new node('T', tom);
    dfsBuildPathTree(root, roomNodes[tom.first][tom.second]);
}

void Room::printPathTree(const char file[]){
    ofstream out(file);
    out << "digraph {\n";
    dfsPrintPathTree(root, out, 0);
    out << "}";
    out.close();
}

void Room::printChosenPath(int idx){
    PathInfo pathInfo = realPrintChosenPath(root, idx, 'T');
    animate(pathInfo.path);
    cout << "=========\nINFO FOR PATH NUMBER " << idx << ":\n";
    cout << "commands: " << pathInfo.path << endl;
    cout << "length of path: " << pathInfo.pathLen << "\n";
    cout << "painted blocks: " << pathInfo.painted << "\n";
    cout << "turns: " << pathInfo.turns << "\n";
}

void Room::twoDronesMostPaint(){

    int leaves = root->cntLeaves;
    int maxPaint = -1;
    int minTurns = 1e9;
    pair<pair<PathInfo, PathInfo>, PathInfo> ans;

    for(int i = 0; i < leaves; i ++){
        PathInfo path1 = realPrintChosenPath(root, i, 'T');
        for(int j = i + 1; j < leaves; j ++){
            PathInfo path2 = realPrintChosenPath(root, j, 'T');
            PathInfo path3 = commonPath(root, i, j, 'T');
            cout << path3.path << endl;
            int curPaint = path1.painted + path2.painted - path3.painted;
            int curTurns = path1.turns + path2.turns;
            if(curPaint == maxPaint){
                if(curTurns < minTurns){
                    minTurns = curTurns;
                    ans = {{path1, path2}, path3};
                }
            }
            if(curPaint > maxPaint){
                maxPaint = curPaint;
                minTurns = curTurns;
                ans = {{path1, path2}, path3};
            }

        }
    }

    cout << "======\nTWO DRONES CAN PAINT MAXIMUM:\n";
    cout << maxPaint << " blocks\n";
    cout << "with the minimum of " << minTurns << " turns amongst all\n";
    cout << "Drone commands:\n";
    cout << "First drone: " << ans.first.first.path << endl;
    cout << "Second drone: ";
    int i;
    for(i = 0; i < ans.second.path.size(); i++){
        if(ans.first.second.path[i] == 'P'){
            continue;
        }
        cout << ans.first.second.path[i];
    }
    for(; i < ans.first.second.path.size(); i ++){
        cout << ans.first.second.path[i];
    }
    cout << endl;
}

PathInfo Room::dfsMaxPaint(pair<int, int> cur, std::vector<std::vector<int> >& used, char prSymbol){
    
    if(cur == jerry){
        return PathInfo();
    }
    PathInfo res;
    res.painted = - m * n;
    res.pathLen = - m * n;
    pair<int, int> nb;
    
    nb = cur;
    nb.first ++;
    if(isPosOkay(nb) && !used[nb.first][nb.second]){
        used[nb.first][nb.second] = 1;
        PathInfo path = dfsMaxPaint(nb, used, symbolFor(cur, nb));
        used[nb.first][nb.second] = 0;
        if(path.pathLen >= 0){
            path.turns += isTurn(prSymbol, symbolFor(cur, nb));
            path.pathLen++;
            path.path = symbolFor(cur, nb) + path.path;
            if(path.painted == res.painted){
                if(path.turns < res.turns){
                    res = path;
                }
            }
            if(path.painted > res.painted){
                res = path;
            }
        }
    }

    nb = cur;
    nb.first --;
    if(isPosOkay(nb) && !used[nb.first][nb.second]){
        used[nb.first][nb.second] = 1;
        PathInfo path = dfsMaxPaint(nb, used, symbolFor(cur, nb));
        used[nb.first][nb.second] = 0;
        if(path.pathLen >= 0){
            path.turns += isTurn(prSymbol, symbolFor(cur, nb));
            path.pathLen++;
            path.path = symbolFor(cur, nb) + path.path;
            if(path.painted == res.painted){
                if(path.turns < res.turns){
                    res = path;
                }
            }
            if(path.painted > res.painted){
                res = path;
            }
        }
    }
    
    nb = cur;
    nb.second ++;
    if(isPosOkay(nb) && !used[nb.first][nb.second]){
        used[nb.first][nb.second] = 1;
        PathInfo path = dfsMaxPaint(nb, used, symbolFor(cur, nb));
        used[nb.first][nb.second] = 0;
        if(path.pathLen >= 0){
            path.turns += isTurn(prSymbol, symbolFor(cur, nb));
            path.pathLen++;
            path.path = symbolFor(cur, nb) + path.path;
            if(path.painted == res.painted){
                if(path.turns < res.turns){
                    res = path;
                }
            }
            if(path.painted > res.painted){
                res = path;
            }
        }
    }
    
    nb = cur;
    nb.second --;
    if(isPosOkay(nb) && !used[nb.first][nb.second]){
        used[nb.first][nb.second] = 1;
        PathInfo path = dfsMaxPaint(nb, used, symbolFor(cur, nb));
        used[nb.first][nb.second] = 0;
        if(path.pathLen >= 0){
            path.turns += isTurn(prSymbol, symbolFor(cur, nb));
            path.pathLen++;
            path.path = symbolFor(cur, nb) + path.path;
            if(path.painted == res.painted){
                if(path.turns < res.turns){
                    res = path;
                }
            }
            if(path.painted > res.painted){
                res = path;
            }
        }
    }
    if(canPaint[cur.first][cur.second]){
        res.path = 'P' + res.path;
        res.painted ++;
    }
    return res;
    
}

void Room::maxPaint(){
    std:vector<std::vector<int> > used;
    used.assign(m, {});
    for(int i = 0 ; i < m ; i ++){
        used[i].assign(n, 0);
    }
    used[tom.first][tom.second] = 1;
    PathInfo p = dfsMaxPaint(tom, used, 'X');
    cout << "you can paint maximum: " << p.painted << " blocks\n";
    cout << "with these commands: " << p.path << "\n";
    cout << "with minimum of " << p.turns << " turns\n";
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
    std::vector<std::vector<char> > painted;
    painted.assign(m, {});
    for(int i = 0; i < m; i ++){
        painted[i].assign(n, '.');
    }
    for(int x = 0; x < dronePath.size(); x ++){
        if(dronePath[x] == 'P'){
            painted[drone.first][drone.second] = 'P';
        } else if(dronePath[x] == 'S'){
            if(painted[drone.first][drone.second] != 'P')
                painted[drone.first][drone.second] = '|';
            drone.first ++;
        } else if(dronePath[x] == 'N'){
            if(painted[drone.first][drone.second] != 'P')
                painted[drone.first][drone.second] = '|';
            drone.first --;
        } else if(dronePath[x] == 'E'){
            if(painted[drone.first][drone.second] != 'P')
                painted[drone.first][drone.second] = '-';
            drone.second ++;
        } else if(dronePath[x] == 'W'){
            if(painted[drone.first][drone.second] != 'P')
                painted[drone.first][drone.second] = '-';
            drone.second --;
        }
        ClearScreen();
        for(int i = 0; i < m; i ++){
            for(int j = 0; j < n; j ++){
                pair<int, int> cur({i, j});
                if(cur == drone){
                    cout << '*';
                    continue;
                }
                if(cur == tom){
                    cout << 'T';
                    continue;
                }
                if(cur == jerry){
                    cout << 'J';
                    continue;
                }
                if(blocked[i][j]){
                    cout << 'B';
                    continue;
                }
                cout << painted[i][j];
            }
            cout << endl;
        }
        usleep(1000000);
    }
}