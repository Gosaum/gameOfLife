#include "lifealgorithm.hpp"

void LifeAlgorithm::iterateGrid(Grid grid) {
    grid.toggleCells(grid.computeCellsToToggle());
}