#include "Grid.hpp"

#define DEFAULT_ROWS 20
#define DEFAULT_COLS 20

void Grid::updateAliveCells(){
    
    /*
    met à jour l'attribut aliveCells en recherchant linéairement toutes les cellules vivantes dans la grille
    */
    aliveCells.clear();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            if ((*grid[i][j]).isAlive()==true){ //si la cellule est vivante
                aliveCells.push_back(grid[i][j]); //ajoute son pointeur
            }
        }
    }
}

Grid::Grid():n(DEFAULT_ROWS),p(DEFAULT_COLS){

    /*
    constructeur par défaut : instancie un objet avec une grille morte de taille par défaut
    */

    grid = new Cell**[n]; //créé un tableau de n pointeurs de pointeurs de Cell
    for (int i = 0; i<n; ++i) { //pour chaque élément
        grid[i] = new Cell*[p]; //affecte élément à un tableau de p pointeurs de Cell
        for (int j=0; j<p; ++j) {
            grid[i][j] = CellFactory::createCell("0", i, j); //créé une cellule pour chaque élément de la grille
        }
    }
};

Grid::Grid(int rows, int cols):n(rows),p(cols){

    /*
    constructeur 1 : instancie un objet avec une grille morte de taille n * p
    */

    grid = new Cell**[rows];
    for (int i = 0; i < rows; ++i) {
        grid[i] = new Cell*[cols];
        for (int j=0; j < cols; ++j) {
            grid[i][j] = CellFactory::createCell("0", i, j);
        }
    }
};

Grid::Grid(int rows, int cols): n(rows), p(cols) {

    /*
    constructeur 1 : instancie un objet avec une grille morte de taille n * p
    */
   
    grid.resize(n, vector<std::unique_ptr<Cell>>(p));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            grid[i][j] = CellFactory::createCell("0", i, j);
        }
    }
}


Grid::Grid(int rows, int cols, const vector<string>& data) : n(rows), p(cols) {
    
    /*
    constructeur 2 : instancie un objet avec une grille de taille n * p et avec un jeu de donnée
    ---
    utilise un vecteur qui contient à l'index [n+p] le type (en string) de la cellule [n][p]
    si le vecteur ne contient pas assez d'éléments, le reste du tableau est rempli par une cellule classique morte
    si le vecteur a trop d'éléments, les éléments en trop sont ignorés
    ---
    note : le cas où le type de la cellule est inconnu (e.g. "hello world!" aulieu de "0" ou "1" ou "O" ...)
    est traité dans la classe CellFactory : une cellule classique morte est instanciée
    */

    int dataIndex = 0;
    int dataSize = data.size();
    grid = new Cell**[rows];
    for (int i = 0; i < rows; ++i) {
        grid[i] = new Cell*[cols];
        for (int j = 0; j < cols; ++j) {
            string cellType = (dataIndex < dataSize) ? data.at(dataIndex++) : "0";
            grid[i][j] = CellFactory::createCell(cellType, i, j);
        }
    }

    updateAliveCells();
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

void Grid::printGrid() const {
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            if (cell) {
                if (dynamic_cast<ObstacleCell*>(cell.get())) {
                    std::cout << "\033[34m⬛ "; // Bleu pour les obstacles
                } else if (cell->isAlive()) {
                    std::cout << "\033[32m⬛ "; // Vert pour les vivants
                } else {
                    std::cout << "\033[30m⬛ "; // Noir pour les morts
                }
            } else {
                std::cout << "\033[30m⬛ "; // Noir pour les cellules nulles
            }
        }
        std::cout << "\033[0m\n"; // Réinitialisation de la couleur après chaque ligne
    }
    std::cout << "\033[0m\n"; // Réinitialisation de la couleur
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

string Grid::getGridSignature() const {

    vector<Cell*> sortedAliveCells = aliveCells;
    //trie aliveCells
    sort(sortedAliveCells.begin(), sortedAliveCells.end(), [](Cell* a, Cell* b) {
        if ((*a).getX() == (*b).getX()) {
            return (*a).getY() < (*b).getY();
        }
        return (*a).getX() < (*b).getX();
    });

    //écrit la signature
    string signature = to_string(n) + to_string(p);
    for(Cell* aliveCell : sortedAliveCells){
        signature += to_string((*aliveCell).getX()) + to_string((*aliveCell).getY());
    }
    return signature;
}