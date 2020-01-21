#include "PathsTree.h"

PathsTree::PathsTree(){
    root = nullptr;
}

PathsTree::~PathsTree(){
    clearTreeNodes(root);   
}

void PathsTree::clearTreeNodes(const node* cur) const{
    if(cur == nullptr)
        return;
    for(node* nb : cur->next){
        clearTreeNodes(nb);
    }
    delete cur;
}

void PathsTree::dfsBuild(node* curPathNode, const node* curRoomNode, const std::pair<int, int>& jerry) const{
    if(curRoomNode->symbol == 'P' && curPathNode->symbol != 'P'){
        //then we should make a paint command but we shouldn't get stuck in an infinite loop and only make one paint node
        node* toPaint = new node('P', curPathNode->pos);
        curPathNode->addNext(toPaint);
        dfsBuild(toPaint, curRoomNode, jerry);
        curPathNode->cntLeaves = toPaint->cntLeaves;
        return;
    }
    if(curRoomNode->pos == jerry){
        //we have arrived at destiantion
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

void PathsTree::dfsPrint(const node* cur, ofstream& out, int idx) const{
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

PathInfo PathsTree::dfsFindChosenPath(const node* cur, int idx, const char& prevSymbol) const{
    
    for(int i = 0; i < cur->next.size(); i ++){
        if(idx >= cur->next[i]->cntLeaves){
            //then the number of paths in this vertex's subtree is smaller than the index of the path we want
            //and we should calculate the number of paths we have skipped
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
    //then we have arrived at a leaf
    return PathInfo();
}

PathInfo PathsTree::dfsFindPathWithoutRepetition(const node* cur1, const node* cur2, int idx1, int idx2, const char& prevSymbol) const{

    const node *next1 = nullptr, *next2 = nullptr;
    int idx1c = idx1, idx2c = idx2;
    //find next for the first path
    for(int i = 0; i < cur1->next.size(); i ++){
        if(idx1c >= cur1->next[i]->cntLeaves){
            idx1c -= cur1->next[i]->cntLeaves;
        }
        else{
            next1 = cur1->next[i];
            break;
        }
    }
    if(!next1){
        //then they will no longer intercept and we should find the rest of the second path
        return dfsFindChosenPath(cur2, idx2, prevSymbol);
    }
    
    //find next for the second path
    for(int i = 0; i < cur2->next.size(); i ++){
        if(idx2c >= cur2->next[i]->cntLeaves){
            idx2c -= cur2->next[i]->cntLeaves;
        }
        else{
            next2 = cur2->next[i];
            break;
        }
    }
    if(!next2){
        //then the second path has come to an end
        return PathInfo();
    }
    PathInfo res;
    if(next1->pos == next2->pos && next1->symbol == 'P' && next2->symbol == 'P'){
        //then the second drone mustn't paint in this position
        //next 1 and 2
        return dfsFindPathWithoutRepetition(next1, next2, idx1c, idx2c, cur2->symbol);
    }
    else if(next1->symbol == 'P'){
        //because they don't intersect for now
        //next 1
        return dfsFindPathWithoutRepetition(next1, cur2, idx1c, idx2, prevSymbol);
    }
    else if(next2->symbol == 'P'){
        //they don't interesect
        //next 2
        res = dfsFindPathWithoutRepetition(cur1, next2, idx1, idx2c, cur2->symbol);
    }
    else{
        //they can both move since they don't intersect or the intersection is not a paint node
        //next 1 and 2
        res = dfsFindPathWithoutRepetition(next1, next2, idx1c, idx2c, next2->symbol);
    }
    //standard operations
    res.turns = isTurn(prevSymbol, next2->symbol) + res.turns;
    res.path = next2->symbol + res.path;
    res.pathLen = (next2->symbol != 'P') + res.pathLen;
    res.painted = (next2->symbol == 'P') + res.painted;
    return res;
    
}

void PathsTree::build(const node* tom, const std::pair<int,int>& jerry){
    root = new node('T', tom->pos);
    dfsBuild(root, tom, jerry);
}

void PathsTree::print(ofstream& out, int idx) const{
    dfsPrint(root, out, idx);
}

PathInfo PathsTree::findChosenPath(int idx) const{
    return dfsFindChosenPath(root, idx, 'T');
}

PathInfo PathsTree::findPathWithoutRepetition(int idx1, int idx2) const{
    return dfsFindPathWithoutRepetition(root, root, idx1, idx2, 'T');
}

int PathsTree::numberOfLeaves() const{
    return root->cntLeaves;
}

char symbolFor(const std::pair<int, int>& from, const std::pair<int, int>& to){
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

