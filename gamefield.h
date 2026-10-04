#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include"islands.h"
#include <random>
#include <map>
#include <vector>
#include <unordered_set>
#include"cell.h"

class GameField
{
public:
    GameField();
    int width, height = 0;
    std::vector<Island*> islands;
    Cell** field = nullptr;

    void addIsland(Island* island);
    bool startNewGame();
    void addBridge(Island* start, Island* finish);
    bool checkWinCondition();
    // void deleteBridge(Island* start, Island* finish);
    void removeBridgesConectedTo(Island* island);
    ~GameField();
private:
    bool addToRight(std::default_random_engine e, Island* origin);
    bool addToBot(std::default_random_engine e, Island* origin);
    bool addToLeft(std::default_random_engine e, Island* origin);
    bool addToTop(std::default_random_engine e, Island* origin);
    bool checkIslandsNear(int x, int y);
    std::unordered_set<Island*> visited;
    void DFS(Island* island);
};

#endif // GAMEFIELD_H
