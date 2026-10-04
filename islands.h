#ifndef ISLANDS_H
#define ISLANDS_H

#include <list>
#include <vector>


struct Island{
    int x, y; //Координаты
    int required = 0; //Необходимое кол-во мостов
    int current = 0; //Текущее кол-во мостов

    Island* top = nullptr;
    Island* bot = nullptr;
    Island* left = nullptr;
    Island* right = nullptr;

    int requiredTop = 0;
    int requiredRight = 0;
    int requiredLeft = 0;
    int requiredBot = 0;

    std::list<Island*> neighbors;


    Island(int _x, int _y): x(_x), y(_y) {}
};

#endif // ISLANDS_H
