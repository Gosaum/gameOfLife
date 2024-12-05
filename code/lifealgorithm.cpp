#include "lifealgorithm.hpp"

void LifeAlgorithm::iterateGrid(Grid& grid) {
    vector<Cell*> toToggle = grid.computeCellsToToggle();
    grid.toggleCells(toToggle);
}