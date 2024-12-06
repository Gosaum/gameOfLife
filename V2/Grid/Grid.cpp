#include "Grid.hpp"
#define DEFAULT_N 10
#define DEFAULT_P 10

#include <iostream>
#include <unordered_set>
#include <algorithm>
using namespace std;

Grid::Grid():n(DEFAULT_N),p(DEFAULT_P){

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

Grid::Grid(int n, int p):n(n),p(p){

    /*
    constructeur 1 : instancie un objet avec une grille morte de taille n * p
    */

    grid = new Cell**[n];
    for (int i = 0; i < n; ++i) {
        grid[i] = new Cell*[p];
        for (int j=0; j < p; ++j) {
            grid[i][j] = CellFactory::createCell("0", i, j);
        }
    }
};

Grid::Grid(int n, int p, const vector<string>& data) : n(n), p(p) {
    
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
    grid = new Cell**[n];
    for (int i = 0; i < n; ++i) {
        grid[i] = new Cell*[p];
        for (int j = 0; j < p; ++j) {
            string cellType = (dataIndex < dataSize) ? data.at(dataIndex++) : "0";
            grid[i][j] = CellFactory::createCell(cellType, i, j);
        }
    }

    updateAliveCells();
}

Cell*** Grid::getGrid() const {
    
    return grid;
}

int Grid::getN() const {
    
    return n;
}

int Grid::getP() const {
    
    return p;
}

vector<Cell*> Grid::getAliveCells() const {
    
    return aliveCells;
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

void Grid::updateAliveCells(const vector<Cell*>& toggledCells){

    /*
    met à jour l'attribut aliveCells à partir d'un vecteur de pointeurs vers des cellules qui viennent de changer d'état
    ---
    supprime dans un premier temps toutes les cellules mortes de aliveCells
    ajoute ensuite chaque cellule vivante du vecteur en paramètre à aliveCells
    */
    
    vector<Cell*> updatedAliveCells; //vecteur temporaire pour éviter des problèmes d'indexation
    for (Cell* cell : aliveCells) {
        if ((*cell).isAlive()) { //(une cellule présente dans aliveCells n'est pas forcément vivante après que la grille à changé d'où la vérification)
            updatedAliveCells.push_back(cell); //ajoute les pointeurs vers les cellules vivantes de l'attribut aliveCells au vecteur temporaire
        }
    }
    for (Cell* toggledCell : toggledCells){
        if((*toggledCell).isAlive()==true){
            updatedAliveCells.push_back(toggledCell); //ajoute les pointeurs vers les cellules vivantes de l'argument au vecteur temporaire
            } 
    }
    aliveCells = updatedAliveCells; //écrase l'attribut par le vecteur temporaire
}

void Grid::toggleCells(const vector<Cell*>& cellsToToggle){

    /*
    change d'état les cellules du vecteur cellsToToggle et met à jour l'attribut aliveCells
    */

    for (Cell* cell : cellsToToggle){
        (*cell).toggleAlive();
    }
    updateAliveCells(cellsToToggle);
}

vector<Cell*> Grid::mooreNeighborhood(const Cell* cell) const {

    /*
    renvoie un vecteur qui contient les pointeurs vers les cellules du voisinnage de moore de la cellule pointée en argument
    ---
    méthode par défaut :
    voisinnage de moore d'ordre 1 dans une grille rectangle classique
    */

    int x = (*cell).getX();
    int y = (*cell).getY();
    vector <Cell*> neighbors;
    int maxX = n-1; //-1 parce que l'indexation commence à 0
    int maxY = p-1;
    //pour éviter de sortir de la grille on définit les limites
    int topLimit  = ((x-1) >= 0) ? x-1 : 0;
    int bottomLimit = ((x+1) <= maxX ) ? x+1 : maxX ; 
    int leftLimit  = ((y-1) >= 0) ? y-1 : 0;
    int rightLimit = ((y+1) <= maxY ) ? y+1 : maxY;

    for (int i = topLimit; i <= bottomLimit; ++i) { //pour chaque ligne de x-1 à x+1 sauf limite
        for (int j = leftLimit; j <= rightLimit; ++j) { //pour chaque colonne de y-1 à y+1 sauf limite
            if (!(i == x && j == y)) { //si il s'agit bien d'un voisin et non de la cellule étudiée
                neighbors.push_back(grid[i][j]); //on ajoute le voisin
            }
        }
    }

    return neighbors;
}



vector<Cell*> Grid::computeCellsToToggle() const {
    
    /*
    renvoie un vecteur qui contient les pointeurs vers les cellules de la grile dont l'état est sujet à un changement
    ---
    méthode par défaut :
    vérifie si chaque cellule vivante et chaque cellule voisine de moore d'ordre 1 à une cellule vivante a besoin de changer d'état
    si oui ajoute à un vecteur et renvoie le vecteur
    */

    vector <Cell*> toToggle;

    unordered_set <Cell*> deadNeighbors; //un set désordonné pour éviter les doublons

    for (Cell* aliveCell : aliveCells) {
        int aliveNeighborCount = 0;
        vector <Cell*> neighbors = mooreNeighborhood(aliveCell);
        for (Cell* neighbor : neighbors){
            
            if ((*neighbor).isAlive()){
                aliveNeighborCount += 1;
            }
            else {
                deadNeighbors.insert(neighbor);
            }
        }

        if((*aliveCell).computeNextState(aliveNeighborCount)!=(*aliveCell).isAlive()){
            toToggle.push_back(aliveCell);;
        }
        
    }

    for (Cell* deadNeighbor : deadNeighbors){

        int aliveNeighborCount = 0;
        vector <Cell*> neighbors = mooreNeighborhood(deadNeighbor);
        
        for (Cell* neighbor : neighbors){
            if ((*neighbor).isAlive()){
                aliveNeighborCount += 1;
            }
        }

        if((*deadNeighbor).computeNextState(aliveNeighborCount)!=(*deadNeighbor).isAlive()){
            toToggle.push_back(deadNeighbor);
        }
    }

    return toToggle;
}

StandardGrid::StandardGrid():Grid(){};

StandardGrid::StandardGrid(int n, int p):Grid(n,p){};

StandardGrid::StandardGrid(int n, int p,const vector<string>& data):Grid(n,p,data){};

ToroidalGrid::ToroidalGrid():Grid(){};

ToroidalGrid::ToroidalGrid(int n, int p):Grid(n,p){};

ToroidalGrid::ToroidalGrid(int n, int p,const vector<string>& data):Grid(n,p,data){};

vector<Cell*> ToroidalGrid::mooreNeighborhood(const Cell* cell) const {

    /*
    renvoie un vecteur qui contient les pointeurs vers les cellules du voisinnage de moore de la cellule pointée en argument
    ---
    méthode pour une grille torique :
    voisinnage de moore d'ordre 1 dans une grille torique où le haut rejoint le bas et la droite rejoint la gauche
    */

    int x = cell->getX();
    int y = cell->getY();
    vector<Cell*> neighbors;

    for (int i = (x-1); i <= (x+1); ++i) {      //de la ligne au dessus à la ligne en dessous de la cellule
        for (int j = (y-1); j <= (y+1); ++j) {  //des cases à gauche aux cases à droite de la cellules
            if (!(i == x && j == y)) {    //si il s'agit bien d'un voisin et non de la cellule étudiée
                neighbors.push_back(grid[((i + n) % n)][((j + p) % p)]); //on ajoute le voisin avec modulo pour la propriété torique
            }
        }
    }
    return neighbors;
}

Grid* GridFactory::createGrid(const string& gridType) {
    
    if (gridType == "std" || gridType == "standard" || gridType == "Standard") {
        return new StandardGrid ();
    }
    if (gridType == "tor" || gridType == "toroidal" || gridType == "Toroidal") {
        return new ToroidalGrid();
    }
    else {
        return new StandardGrid(); //si le type est inconnu, créé par défaut une grille classique
    }
}

Grid* GridFactory::createGrid(const string& gridType, int n, int p) {
    
    if (gridType == "std" || gridType == "standard" || gridType == "Standard") {
        return new StandardGrid (n,p);
    }
    if (gridType == "tor" || gridType == "toroidal" || gridType == "Toroidal") {
        return new ToroidalGrid(n,p);
    }
    else {
        return new StandardGrid(n,p); //si le type est inconnu, créé par défaut une grille classique
    }
}

Grid* GridFactory::createGrid(const string& gridType, int n, int p, const vector<string>& data) {
    
    if (gridType == "std" || gridType == "standard" || gridType == "Standard") {
        return new StandardGrid (n,p,data);
    }
    if (gridType == "tor" || gridType == "toroidal" || gridType == "Toroidal") {
        return new ToroidalGrid(n,p,data);
    }
    else {
        return new StandardGrid(n,p,data); //si le type est inconnu, créé par défaut une grille classique
    }
}