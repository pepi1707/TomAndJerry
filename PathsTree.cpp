#include "PathsTree.h"

PathsTree::PathsTree(){
    root = nullptr;
}

PathsTree::~PathsTree(){
    clearTreeNodes(root);   
}

void PathsTree::clearTreeNodes(const node* const cur) const{
    if(cur == nullptr)
        return;
    for(node* nb : cur->next){
        clearTreeNodes(nb);
    }
    delete cur;
}

void PathsTree::dfsBuild(node* const curPathNode, const node* const curRoomNode, const pair<int, int>& jerry) const{
    if(curRoomNode->symbol == 'P' && curPathNode->symbol != 'P'){
        node* toPaint = new node('P', curPathNode->pos);
        curPathNode->addNext(toPaint);
        dfsBuild(toPaint, curRoomNode, jerry);
        curPathNode->cntLeaves = toPaint->cntLeaves;
        return;
    }
    if(curRoomNode->pos == jerry){
        curPathNode->cntLeaves = 1;
        return;
    }
    for(node* nbPoint : curRoomNode->next){
        node* newPath = new node(symbolFor(curRoomNode->pos, nbPoint->pos), nbPoint->pos);
        curPathNode->addNext(newPath);
        dfsBuild(newPath, nbPoint, jerry);
        curPathNode->cntLeaves += newPath->cntLeaves;
    } 
}

void PathsTree::dfsPrint(const node* const cur, ofstream& out, int idx) const{
    out << "\t" << (long)cur << "[label=\"" << cur->symbol;
    if(cur->next.size() == 0){
        out << ", idx: " << idx; 
    }
    out << "\"];\n";
    for(int i = 0; i < cur->next.size(); i ++){
        node *nb = cur->next[i];
        out << "\t" << (long)cur << " -> " << (long)nb << ";\n";
        dfsPrint(nb, out, idx);
        idx -= nb->cntLeaves;
        
    }

}

PathInfo PathsTree::dfsFindChosenPath(const node* const cur, int idx, const char& prevSymbol) const{
    
    for(int i = 0; i < cur->next.size(); i ++){
        if(idx >= cur->next[i]->cntLeaves){
            idx -= cur->next[i]->cntLeaves;
            continue;
        }
        node* nb = cur->next[i];
        PathInfo curPath, restPath = dfsFindChosenPath(nb, idx, nb->symbol == 'P' ? cur->symbol : nb->symbol);
        curPath.path = nb->symbol + restPath.path;
        curPath.turns = isTurn(prevSymbol, nb->symbol) + restPath.turns;
        curPath.pathLen = (nb->symbol != 'P') + restPath.pathLen;
        curPath.painted = (nb->symbol == 'P') + restPath.painted;

        return curPath;
    }
    return PathInfo();
}

PathInfo PathsTree::dfsCommonPath(const node* const cur, int idx1, int idx2, const char& prevSymbol) const{

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
        PathInfo curPath, restPath = dfsCommonPath(nb, idx1, idx2, nb->symbol == 'P' ? cur->symbol : nb->symbol);
        curPath.path = nb->symbol + restPath.path;
        curPath.turns = isTurn(prevSymbol, nb->symbol) + restPath.turns;
        curPath.pathLen = (nb->symbol != 'P') + restPath.pathLen;
        curPath.painted = (nb->symbol == 'P') + restPath.painted;

        return curPath;
    }
    return PathInfo();
}

void PathsTree::build(const node* const tom, const pair<int,int>& jerry){
    root = new node('T', tom->pos);
    dfsBuild(root, tom, jerry);
}

void PathsTree::print(ofstream& out, int idx) const{
    dfsPrint(root, out, idx);
}

PathInfo PathsTree::findChosenPath(int idx, const char& prevSymbol) const{
    return dfsFindChosenPath(root, idx, prevSymbol);
}

PathInfo PathsTree::commonPath(int idx1, int idx2, const char& prevSymbol) const{
    return dfsCommonPath(root, idx1, idx2, prevSymbol);
}

int PathsTree::numberOfLeaves() const{
    return root->cntLeaves;
}

char symbolFor(pair<int, int> from, pair<int, int> to){
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

bool isTurn(const char& c1, const char& c2){
    return ((c1 == 'N' || c1 == 'S') && (c2 == 'E' || c2 == 'W')) ||
            ((c1 == 'E' || c1 == 'W') && (c2 == 'N' || c2 == 'S'));
}

