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

string LifeAlgorithm::hashGrid(const Grid& grid) {
    ostringstream oss;
    for (int i = 0; i < grid.getN(); ++i) {
        for (int j = 0; j < grid.getP(); ++j) {
            Cell* cell = grid.getCell(i, j);
            if (dynamic_cast<ObstacleCell*>(cell)) {
                oss << "O";
            } else {
                oss << (cell->isAlive() ? "1" : "0");
            }
        }
    }
    return oss.str();
}

void LifeAlgorithm::simulateWithLoopDetection(Grid* grid, int maxIterations) {
    unordered_set<string> previousStates;
    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        string currentHash = hashGrid(*grid);
        if (previousStates.find(currentHash) != previousStates.end()) {
            cout << "La simulation boucle apres " << iteration + 1 << " iterations.\n";
            break;
        }
        previousStates.insert(currentHash);

        auto toggledCells = computeCellsToToggle();
        if (toggledCells.empty()) {
            cout << "Simulation stabilisee apres " << iteration + 1 << " iterations.\n";
            break;
        }
        toggleCells(toggledCells);
    }
}