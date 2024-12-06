#include "toroidalgrid.hpp"

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