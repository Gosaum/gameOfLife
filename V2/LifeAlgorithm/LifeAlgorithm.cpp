#include "LifeAlgorithm.hpp"

LifeAlgorithm::LifeAlgorithm(Grid* grid) : grid(grid) {}

std::vector<Cell*> LifeAlgorithm::computeCellsToToggle() {
    std::vector<Cell*> toggledCells;
    for (int i = 0; i < grid->getN(); ++i) {
        for (int j = 0; j < grid->getP(); ++j) {
            Cell* cell = grid->getCell(i, j);
            if (!cell) continue;

            int livingNeighbors = 0;
            for (Cell* neighbor : grid->mooreNeighborhood(cell)) {
                if (neighbor && neighbor->isAlive()) {
                    ++livingNeighbors;
                }
            }

            if (cell->isAlive() != cell->computeNextState(livingNeighbors)) {
                toggledCells.push_back(cell);
            }
        }
    }
    return toggledCells;
}

void LifeAlgorithm::toggleCells(const std::vector<Cell*>& cellsToToggle) {
    for (Cell* cell : cellsToToggle) {
        cell->toggleAlive();
    }
}