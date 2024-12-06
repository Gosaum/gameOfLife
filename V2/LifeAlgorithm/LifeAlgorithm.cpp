#include "LifeAlgorithm.hpp"

LifeAlgorithm::LifeAlgorithm(Grid* grid) : grid(grid) {}

vector<Cell*> LifeAlgorithm::computeCellsToToggle() {
    vector<Cell*> toggledCells;
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

void LifeAlgorithm::toggleCells(const vector<Cell*>& cellsToToggle) {
    for (Cell* cell : cellsToToggle) {
        cell->toggleAlive();
    }
}

bool LifeAlgorithm::isGridStable() {
    string currentSignature = grid.getGridSignature();
    if (signatureHistory.size()>1){
        if(signatureHistory[signatureHistory.size() - 1] == currentSignature){
            return true;
        }
    }
    signatureHistory.push_back(currentSignature);
    return false;
}

bool LifeAlgorithm::isGridLooping(int Tmax) {
    string currentSignature = grid.getGridSignature();

    for (int T = Tmax; T >= 1; --T) {

        if (static_cast<int>(signatureHistory.size()) >= T) {
            int previousIndex = static_cast<int>(signatureHistory.size()) - T;
            if (signatureHistory[previousIndex] == currentSignature) {
                return true;
            }
        }
    }
    signatureHistory.push_back(currentSignature);
    return false;
}