#include "Grid.hpp"

Grid::Grid(int rows, int cols) : n(rows), p(cols), cells(rows) {
    for (auto& row : cells) {
        row.resize(cols);
    }
}

int Grid::getN() const {
    return n;
}

int Grid::getP() const {
    return p;
}

void Grid::setCell(int x, int y, const std::string& type) {
    cells[x][y] = CellFactory::createCell(type, x, y);
}

Cell* Grid::getCell(int x, int y) const {
    return cells[x][y].get();
}

void Grid::updateAliveCells(const std::vector<Cell*>& toggledCells) {
    for (auto cell : toggledCells) {
        if (cell) {
            cell->toggleAlive();
        }
    }
}

void Grid::printGrid() const {
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            std::cout << (cell && cell->isAlive() ? "1 " : "0 ");
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

StandardGrid::StandardGrid(int rows, int cols) : Grid(rows, cols) {}

std::vector<Cell*> StandardGrid::mooreNeighborhood(Cell* cell) const {
    std::vector<Cell*> neighbors;
    int x = cell->getX();
    int y = cell->getY();

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < n && ny >= 0 && ny < p) {
                neighbors.push_back(cells[nx][ny].get());
            }
        }
    }
    return neighbors;
}

ToroidalGrid::ToroidalGrid(int rows, int cols) : Grid(rows, cols) {}

std::vector<Cell*> ToroidalGrid::mooreNeighborhood(Cell* cell) const {
    std::vector<Cell*> neighbors;
    int x = cell->getX();
    int y = cell->getY();

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + n) % n;
            int ny = (y + dy + p) % p;
            neighbors.push_back(cells[nx][ny].get());
        }
    }
    return neighbors;
}

std::unique_ptr<Grid> GridFactory::createGrid(const std::string& type, int rows, int cols) {
    if (type == "Standard") {
        return std::make_unique<StandardGrid>(rows, cols);
    } else if (type == "Toroidal") {
        return std::make_unique<ToroidalGrid>(rows, cols);
    } else {
        throw std::invalid_argument("Unknown grid type: " + type);
    }
}