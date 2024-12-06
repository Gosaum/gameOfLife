#pragma once
#include <vector>
#include "Grid.hpp"

class LifeAlgorithm {
private:
    Grid* grid;

public:
    LifeAlgorithm(Grid* grid);

    std::vector<Cell*> computeCellsToToggle();
    void toggleCells(const std::vector<Cell*>& cellsToToggle);
};