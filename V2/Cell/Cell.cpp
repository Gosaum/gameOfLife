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

std::unique_ptr<Cell> CellFactory::createCell(const std::string& type, int x, int y) {
    if (type == "Standard") {
        return std::make_unique<StandardCell>(x, y);
    } else if (type == "Obstacle") {
        return std::make_unique<ObstacleCell>(x, y);
    } else {
        throw std::invalid_argument("Unknown cell type: " + type);
    }
}