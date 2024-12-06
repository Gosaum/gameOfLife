#pragma once
#include <vector>
#include <iostream>
#include "Grid.hpp"

using namespace std;

class LifeAlgorithm {
private:
    Grid* grid;

public:
    LifeAlgorithm(Grid* grid);

    vector<Cell*> computeCellsToToggle();
    void toggleCells(const vector<Cell*>& cellsToToggle);
};