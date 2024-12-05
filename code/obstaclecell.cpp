#include "obstaclecell.hpp"

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