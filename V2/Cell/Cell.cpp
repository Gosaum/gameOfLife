#include "Cell.hpp"

Cell::Cell(bool state, int x, int y):alive(state),x(x),y(y){};

bool Cell::isAlive() const {
    
    return alive;
}

int Cell::getX() const {
    
    return x;
}

int Cell::getY() const {
    
    return y;
}

void Cell::setAlive(bool state){
    
    alive = state;
}

void Cell::toggleAlive(){
    
    alive = !alive;
}

bool Cell::computeNextState(int aliveNeighborsCount) const {
    
    if(alive){
        return (aliveNeighborsCount == 2 || aliveNeighborsCount == 3); //la cellule vivante reste vivante si elle a 2 ou 3 cellules voisines vivantes
    }
    else{
        return (aliveNeighborsCount == 3); //la cellule morte naît si elle a 3 cellules voisines vivantes
    }
}

StandardCell::StandardCell(bool state, int x, int y): Cell(state,x,y) {};

ObstacleCell::ObstacleCell(bool state, int x, int y):Cell(state, x,y){};

void ObstacleCell::setAlive(bool state) {
    
    (void)state; //paramètre inutilisé
}

void ObstacleCell::toggleAlive() {
    
}

bool ObstacleCell::computeNextState(int aliveNeighborsCount) const {
    
    (void)aliveNeighborsCount; //paramètre inutilisé
    return alive;
}

Cell* CellFactory::createCell(const string& cellType, int x, int y) {
    
    if (cellType == "1") {
        return new StandardCell(true, x, y);
    }
    if (cellType == "0") {
        return new StandardCell(false, x, y);
    }
    if (cellType == "O" || cellType == "o") {
        return new ObstacleCell(true, x, y);
    }
    if (cellType == "X" || cellType == "x") {
        return new ObstacleCell(false, x, y);
    }
    else {
        return new StandardCell(false, x, y); //si le type est inconnu, créé par défaut une cellule classique morte
    }
}