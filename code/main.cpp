#include <iostream>
using namespace std;

#include "lifealgorithm.hpp"
#include "standardgrid.hpp"
#include "toroidalgrid.hpp"

//for debugging
void printCells(vector<Cell*> vect) {
     cout << "aliveCells" << endl;
    for (Cell* e : vect){
        cout << (*e).isAlive() << "(" << (*e).getX() << "," <<  (*e).getY() << ")" << endl;
    }
}

void printGrid(Grid grid){
    Cell*** g = grid.getGrid();
    for (int i = 0; i<grid.getN(); ++i) {
        string row = "";
        for (int j=0; j<grid.getP(); ++j) {
            row += to_string((*g[i][j]).isAlive());
        }
    cout << row << endl;
    }
}

int main(){
    
    /*BATTERIE1
    vector<string> data = {"0","0","1","0","d","1"};
    StandardGrid foo(3,3,data);
    foo.updateAliveCells();
    foo.updateAliveCells(foo.getAliveCells());
    */
    /*BATTERIE2*/
    vector<string> data = {
        "0","0","0","0",
        "1","0","0","0",
        "1","0","0","0",
        "1","0","0","0"};
    ToroidalGrid foo(4,4,data);
    foo.updateAliveCells();
    cout << "initialisation" << endl;
    printGrid(foo);
    LifeAlgorithm::iterateGrid(foo);
    cout << "iteration 1" << endl;
    printGrid(foo);
    LifeAlgorithm::iterateGrid(foo);
    cout << "iteration 2" << endl;
    printGrid(foo);
    LifeAlgorithm::iterateGrid(foo);
    cout << "iteration 3" << endl;
    printGrid(foo);
    cout << foo.getGridSignature() << endl;

    return 0;
    }