#include "gamefield.h"
#include "cell.h"
#include "islands.h"
#include <QTime>
#include <qDebug>
#include <random>
#include<ctime>

GameField::GameField() {}

void GameField::addIsland(Island* island){
    islands.push_back(island);
    field[island->x][island->y].island = island;
    field[island->x][island->y].type = 'I';
}

bool GameField::checkIslandsNear(int x, int y){
    for(int i = x-1; i<=x+1; i++){
        for(int j = y-1; j<=y+1; j++){
            if(i<0 || i>=width||j<0||j>=height){
                continue;
            }
            if(field[i][j].type == 'I'){
                return false;
            }
        }
    }
    return true;
}

bool GameField::addToBot(std::default_random_engine e, Island* origin)
{
    if(origin->bot!=nullptr){
        return false;
    }

    std::uniform_int_distribution<int> randomY(origin->y+1, height-1);
    int newY = randomY(e);
    if(field[origin->x][newY].type!='E' || !checkIslandsNear(origin->x, newY)){
        return false;
    }


    for(int j =origin->y+1; j<newY; j++){
        if(field[origin->x][j].type != 'E'){
            return false;
        }
    }
    for(int j =origin->y+1; j<newY; j++){
        field[origin->x][j].type='V';
    }

    Island* temp = new Island(origin->x, newY);
    temp->top = origin;
    origin->bot=temp;
    addIsland(temp);
    return true;
}

bool GameField::addToRight(std::default_random_engine e, Island* origin){
    if(origin->right!=nullptr){
        return false;
    }

    std::uniform_int_distribution<int> randomX(origin->x+1, width-1);
    int newX = randomX(e);
    if(field[newX][origin->y].type != 'E' || !checkIslandsNear(newX, origin->y)){
        return false;
    }

    for(int i = origin->x+1; i<newX; i++){
        if(field[i][origin->y].type!='E'){
            return false;
        }
    }
    for(int i = origin->x+1; i<newX; i++){
        field[i][origin->y].type= 'H';
    }

    Island* temp = new Island(newX, origin->y);
    temp->left = origin;
    origin->right = temp;
    addIsland(temp);
    return true;
}

bool GameField::addToLeft(std::default_random_engine e, Island* origin){
    if(origin->left != nullptr){
        return false;
    }

    std::uniform_int_distribution<int> randomX(0, origin->x-2);
    int newX = randomX(e);
    if(field[newX][origin->y].type != 'E' || !checkIslandsNear(newX, origin->y)){
        return false;
    }

    for(int i = newX+1; i<origin->x;i++){
        if(field[i][origin->y].type != 'E'){
            return false;
        }
    }
    for(int i = newX+1; i<origin->x;i++){
        field[i][origin->y].type = 'H';
    }

    Island* temp = new Island(newX, origin->y);
    temp->right = origin;
    origin->left = temp;
    addIsland(temp);
    return true;
}

bool GameField::addToTop(std::default_random_engine e, Island* origin){
    if(origin->top != nullptr){
        return false;
    }

    std::uniform_int_distribution<int> randomY(0, origin->y-2);
    int newY = randomY(e);
    if(field[origin->x][newY].type !='E' || !checkIslandsNear(origin->x, newY)){
        return false;
    }

    for(int j =newY+1; j<origin->y; j++){
        if(field[origin->x][j].type !='E'){
            return false;
        }
    }
    for(int j =newY+1; j<origin->y; j++){
        field[origin->x][j].type ='V';
    }

    Island* temp = new Island(origin->x, newY);
    temp->bot = origin;
    origin->top = temp;
    addIsland(temp);
    return true;
}

bool GameField::startNewGame(){
    int startTime = clock();
    if (field != nullptr) {
        for (int i = 0; i < width; i++) {
            delete[] field[i];
        }
        delete[] field;
        field = nullptr;
    }

    for(auto land : islands){
        delete land;
        islands.pop_back();
    }

    width = 10;
    height = 10;
    field = new Cell*[width];

    for(int i =0; i<width;i++){
        field[i] = new Cell[height];
    }

    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> randomWidth(0, width-1);
    std::uniform_int_distribution<int> randomHeight(0, height-1);

    Island* firstIsland = new Island(randomWidth(e), randomHeight(e));
    addIsland(firstIsland);

    for(int i = 0; i<9; i++){
        if(clock() - startTime > 1){
            return false;
        }
        // qDebug()<<i;
        std::uniform_int_distribution<int> randIslandId(0, islands.size()-1);
        Island* origin = islands[randIslandId(e)];

        if(origin->x<=1 && (origin->bot == nullptr || origin->top==nullptr || origin->right == nullptr)){
            if(origin->y<=1 && (origin->bot == nullptr || origin->right==nullptr)){
                std::uniform_int_distribution<int> randDirection(0, 1);
                int direction = randDirection(e);

                switch(direction){
                case 0:{
                    if(!addToBot(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                case 1:{
                    if(!addToRight(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                }
            }
            else if(origin->y>=height-2 && (origin->right==nullptr || origin->top == nullptr)){
                std::uniform_int_distribution<int> randDirection(0, 1);
                int direction = randDirection(e);

                switch(direction){
                case 0:{
                    if(!addToRight(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                case 1:{
                    if(!addToTop(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                }
            }
            else if(origin->y > 1 && origin->y <height-2){
                std::uniform_int_distribution<int> randDirection(0, 2);
                int direction  =randDirection(e);
                switch (direction) {
                case 0:
                    if(!addToTop(e, origin)){
                        i--;
                        continue;
                    }
                    break;
                case 1:{
                    if(!addToRight(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                case 2:{
                    if(!addToBot(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                default:
                    break;
                }
            }
            else{
                i--;
                continue;
            }
        }
        else if(origin->x>=width-2 && (origin->left == nullptr || origin->top == nullptr || origin->bot == nullptr)){
            if(origin->y<=1 && (origin->bot == nullptr || origin->left==nullptr)){
                std::uniform_int_distribution<int> randDirection(0, 1);
                int direction = randDirection(e);

                switch(direction){
                case 0:{
                    if(!addToBot(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                case 1:{
                    if(!addToLeft(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                }
            }
            else if(origin->y>=height-2 && (origin->left==nullptr || origin->top == nullptr)){
                std::uniform_int_distribution<int> randDirection(0, 1);
                int direction = randDirection(e);

                switch(direction){
                case 0:{
                    if(!addToLeft(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                case 1:{
                    if(!addToTop(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                }
            }
            else if(origin->y > 1 && origin->y <height-2){
                std::uniform_int_distribution<int> randDirection(0, 2);
                int direction  =randDirection(e);
                switch (direction) {
                case 0:
                    if(!addToTop(e, origin)){
                        i--;
                        continue;
                    }
                    break;
                case 1:{
                    if(!addToLeft(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                case 2:{
                    if(!addToBot(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                default:
                    break;
                }
            }
            else{
                i--;
                continue;
            }
        }
        else if(origin->y<=1 && (origin->bot==nullptr|| origin->left==nullptr||origin->right==nullptr)){
            if(origin->x>1&&origin->x<width-2){
                std::uniform_int_distribution<int> randDirection(0, 2);
                int direction  =randDirection(e);
                switch (direction) {
                case 0:
                    if(!addToBot(e, origin)){
                        i--;
                        continue;
                    }
                    break;
                case 1:{
                    if(!addToLeft(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                case 2:{
                    if(!addToRight(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                }
            }
        }
        else if(origin->y>=height-2 && (origin->top == nullptr || origin->left == nullptr || origin->right == nullptr)){
            if(origin->x>1&&origin->x<width-2){
                std::uniform_int_distribution<int> randDirection(0, 2);
                int direction  =randDirection(e);
                switch (direction) {
                case 0:
                    if(!addToTop(e, origin)){
                        i--;
                        continue;
                    }
                    break;
                case 1:{
                    if(!addToLeft(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                case 2:{
                    if(!addToRight(e, origin)){
                        i--;
                        continue;
                    }
                }
                break;
                }
            }
            else{
                i--;
                continue;
            }
        }
        else if(origin->x>1 && origin->x<width-2 && origin->y>1 && origin->y<height-2 && (origin->bot==nullptr||origin->top==nullptr||origin->left==nullptr||origin->right==nullptr)){
            std::uniform_int_distribution<int> randDirection(0, 3);
            int direction  =randDirection(e);
            switch (direction) {
            case 0:
                if(!addToTop(e, origin)){
                    i--;
                    continue;
                }
                break;
            case 1:{
                if(!addToLeft(e, origin)){
                    i--;
                    continue;
                }
            }
            break;
            case 2:{
                if(!addToRight(e, origin)){
                    i--;
                    continue;
                }
            }
            break;
            case 3:{
                if(!addToBot(e, origin)){
                    i--;
                    continue;
                }
            }
            break;
            }
        }
        else{
            i--;
            continue;
        }
    }

    std::uniform_int_distribution<int> randomBridges(1,2);
    for(Island* island : islands){
        if(island->requiredBot == 0 && island->bot!=nullptr){
            int bridges = randomBridges(e);
            island->required+=bridges;
            island->requiredBot +=bridges;
            island->bot->required += bridges;
            island->bot->requiredTop += bridges;
        }
        if(island->requiredTop == 0 && island->top!=nullptr){
            int bridges = randomBridges(e);
            island->required += bridges;
            island->requiredTop += bridges;
            island->top->required += bridges;
            island->top->requiredBot += bridges;
        }
        if(island->requiredLeft == 0 && island->left!=nullptr){
            int bridges = randomBridges(e);
            island->required += bridges;
            island->requiredLeft += bridges;
            island->left->required += bridges;
            island->left->requiredRight += bridges;
        }
        if(island->requiredRight == 0 && island->right!=nullptr){
            int bridges = randomBridges(e);
            island->required+=bridges;
            island->requiredRight+=bridges;
            island->right->required+=bridges;
            island->right->requiredLeft+=bridges;
        }
    }

    for(int i = 0; i<width;i++){
        for(int j = 0; j<height;j++){
            if(field[i][j].type=='V' || field[i][j].type == 'H'){
                field[i][j].type='E';
            }
        }
    }
    return true;
}

void GameField::addBridge(Island* start, Island* finish){
    if(start->x-finish->x != 0 && start->y-finish->y !=0){
        qDebug()<<"This islands dont have same lines";
        return;
    }
    if(start->current==start->required || finish->current==finish->required){
        qDebug()<<"One island have max bridges";
        return;
    }


    if(start->x-finish->x == 0){
        if(finish->y-start->y >0){
            for(int j = start->y+1; j<finish->y; j++){
                if(field[start->x][j].type !='E' && field[start->x][j].type !='V'){
                    return;
                }
            }
            for(int j = start->y+1; j<finish->y; j++){
                field[start->x][j].type = 'V';
                if(field[start->x][j].count<2)
                    field[start->x][j].count+=1;
                else{
                    return;
                }
            }
        }
        else{
            for(int j = finish->y+1; j<start->y; j++){
                if(field[start->x][j].type !='E' && field[start->x][j].type !='V'){
                    return;
                }
            }
            for(int j = finish->y+1; j<start->y; j++){
                field[start->x][j].type = 'V';
                if(field[start->x][j].count<2)
                    field[start->x][j].count+=1;
                else{
                    return;
                }
            }
        }
    }
    else{
        if(finish->x-start->x >0){
            for(int i = start->x+1; i<finish->x; i++){
                if(field[i][start->y].type !='E' && field[i][start->y].type !='H'){
                    return;
                }
            }
            for(int i = start->x+1; i<finish->x; i++){
                field[i][start->y].type = 'H';
                if(field[i][start->y].count<2)
                    field[i][start->y].count+=1;
                else{
                    return;
                }
            }
        }
        else{
            for(int i = finish->x+1; i<start->x; i++){
                if(field[i][start->y].type !='E' && field[i][start->y].type !='H'){
                    return;
                }
            }
            for(int i = finish->x+1; i<start->x; i++){
                field[i][start->y].type = 'H';
                if(field[i][start->y].count<2)
                    field[i][start->y].count+=1;
                else{
                    return;
                }
            }
        }
    }
    start->current++;
    finish->current++;

    for(auto island : start->neighbors){
        if(island == finish){
            return;
        }
    }

    start->neighbors.push_back(finish);
    finish->neighbors.push_back(start);
}

void GameField::removeBridgesConectedTo(Island* island){
    for(auto neigbor : island->neighbors){
        int dX = neigbor->x-island->x;
        int dY = neigbor->y-island->y;
        if(dX>0){
            int connected = field[island->x+1][island->y].count;
            island->current-=connected;
            neigbor->current-=connected;
            for(int i =island->x+1; i<neigbor->x; i++){
                field[i][island->y].type='E';
                field[i][island->y].count=0;
            }
        }
        else if(dX<0){
            int connected = field[island->x-1][island->y].count;
            island->current-=connected;
            neigbor->current-=connected;
            for(int i =neigbor->x+1; i<island->x; i++){
                field[i][island->y].type='E';
                field[i][island->y].count=0;
            }
        }
        else if(dY>0){
            int connected = field[island->x][island->y+1].count;
            island->current-=connected;
            neigbor->current-=connected;
            for(int j =island->y+1; j<neigbor->y; j++){
                field[island->x][j].type='E';
                field[island->x][j].count=0;
            }
        }
        else if(dY<0){
            int connected = field[island->x][island->y-1].count;
            island->current-=connected;
            neigbor->current-=connected;
            for(int j =neigbor->y+1; j<island->y; j++){
                field[island->x][j].type='E';
                field[island->x][j].count=0;
            }
        }
        neigbor->neighbors.remove(island);
    }
    island->neighbors.clear();
}

void GameField::DFS(Island* island){
    visited.insert(island);

    for (Island* neighbor : island->neighbors) {
        if (!visited.count(neighbor)) {
            DFS(neighbor);
        }
    }
}

bool GameField::checkWinCondition(){
    for(auto island : islands){
        if(island->current<island->required){
            return false;
        }
    }

    visited.clear();
    DFS(islands[0]);
    if(visited.size() == islands.size())
        return true;
    else
        return false;
}

GameField::~GameField(){
    for(int i =0; i<width;i++){
        delete[] field[i];
    }

    delete[] field;

    for(int i = 0; i<islands.size();i++){
        delete islands[i];
    }
}
