#include "Cell.hpp"

StandardCell::StandardCell(int x, int y) : Cell(x, y) {}

void StandardCell::toggleAlive() {
    alive = !alive;
}

bool StandardCell::computeNextState(int livingNeighbors) {
    if (alive) {
        return (livingNeighbors == 2 || livingNeighbors == 3);
    }
    return (livingNeighbors == 3);
}

ObstacleCell::ObstacleCell(int x, int y) : Cell(x, y) {
    alive = false;
}

void ObstacleCell::toggleAlive() {}

bool ObstacleCell::computeNextState(int livingNeighbors) {
    return false;
}

unique_ptr<Cell> CellFactory::createCell(const string& type, int x, int y) {
    if (type == "Standard") {
        return make_unique<StandardCell>(x, y);
    } else if (type == "Obstacle") {
        return make_unique<ObstacleCell>(x, y);
    } else {
        throw invalid_argument("Unknown cell type: " + type);
    }
}