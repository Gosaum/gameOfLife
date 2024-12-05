#include "cellfactory.hpp"

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