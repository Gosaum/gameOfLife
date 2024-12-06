#include "lifealgorithm.hpp"

//#include <functional>
using namespace std;

LifeAlgorithm::LifeAlgorithm(Grid& grid): grid(grid) {}

LifeAlgorithm::LifeAlgorithm(Grid *grid): grid(*grid) {}

void LifeAlgorithm::iterateGrid() {

    vector<Cell*> toToggle = grid.computeCellsToToggle();
    grid.toggleCells(toToggle);
}

bool LifeAlgorithm::isGridStable() {

    /*
    renvoie true si la grille est la même que la précédente
    */

    string currentSignature = grid.getGridSignature();
    if (signatureHistory.size()>1){
        if(signatureHistory[signatureHistory.size() - 1] == currentSignature){
            return true;
        }
    }
    signatureHistory.push_back(currentSignature);
    return false;
}

bool LifeAlgorithm::isGridStable(int Tmax) {
    
    /*
    renvoie true si la grille est la même que la précédente ou qu'elle est oscillante
    ---
    prend Tmax la période maximale qui est detectée l'algorithme
    vérifie si la grille était la même que il y a Tmax itérations,
    puis vérifie si la grille était la même que il y a Tmax-1 itérations,
    puis Tmax-2 ... jusqu'à vérifier si la grille était la même que la dernière itération
    ne vérifie pas pour une période T si il n'y a pas encore eu T itérations de grille
    */

    string currentSignature = grid.getGridSignature();

    for (int T = Tmax; T >= 1; --T) {

        if (static_cast<int>(signatureHistory.size()) >= T) {
            int previousIndex = static_cast<int>(signatureHistory.size()) - T;
            if (signatureHistory[previousIndex] == currentSignature) {
                return true;
            }
        }
    }
    signatureHistory.push_back(currentSignature);
    return false;
}