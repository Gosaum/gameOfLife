#include "cell.hpp"

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