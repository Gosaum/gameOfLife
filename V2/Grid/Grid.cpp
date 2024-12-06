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

void Grid::setCell(int x, int y, const string& type) {
    cells[x][y] = CellFactory::createCell(type, x, y);
}

Cell* Grid::getCell(int x, int y) const {
    return cells[x][y].get();
}

void Grid::updateAliveCells(const vector<Cell*>& toggledCells) {
    for (auto cell : toggledCells) {
        if (cell) {
            cell->toggleAlive();
        }
    }
}

void Grid::printGrid() const {
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            cout << (cell && cell->isAlive() ? "1 " : "0 ");
        }
        cout << "\n";
    }
    cout << "\n";
}

void Grid::printGridCLI() const {
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            if (cell) {
                if (dynamic_cast<ObstacleCell*>(cell.get())) {
                    std::cout << "\033[34m⬛ "; // Bleu pour les obstacles
                } else if (cell->isAlive()) {
                    std::cout << "\033[32m⬛ "; // Vert pour les vivants
                } else {
                    std::cout << "\033[37m⬛ "; // Blanc pour les morts
                }
            } else {
                std::cout << "\033[37m⬛ "; // Blanc pour les cellules nulles
            }
        }
        std::cout << "\033[0m\n"; // Réinitialisation de la couleur après chaque ligne
    }
    std::cout << "\033[0m\n"; // Réinitialisation de la couleur après toute la grille
}

StandardGrid::StandardGrid(int rows, int cols) : Grid(rows, cols) {}

vector<Cell*> StandardGrid::mooreNeighborhood(Cell* cell) const {
    vector<Cell*> neighbors;
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

vector<Cell*> ToroidalGrid::mooreNeighborhood(Cell* cell) const {
    vector<Cell*> neighbors;
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

unique_ptr<Grid> GridFactory::createGrid(const string& type, int rows, int cols) {
    if (type == "Standard") {
        return make_unique<StandardGrid>(rows, cols);
    } else if (type == "Toroidal") {
        return make_unique<ToroidalGrid>(rows, cols);
    } else {
        throw invalid_argument("Unknown grid type: " + type);
    }
}