#ifndef CELL_H
#define CELL_H
#include"islands.h"

struct Cell{
    int count =0; //Кол-во мостов в этой клетке
    char type = 'E';
    Island* island = nullptr;
};

#endif // CELL_H
