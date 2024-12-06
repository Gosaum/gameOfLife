#include "gol.hpp"

#include "gridfactory.hpp"
#include "lifealgorithm.hpp"

#include <iostream> //pour le hash
using namespace std;

#define T_MAX 3

//for debugging
/*
void printCells(vector<Cell*> vect) {
     cout << "aliveCells" << endl;
    for (Cell* e : vect){
        cout << (*e).isAlive() << "(" << (*e).getX() << "," <<  (*e).getY() << ")" << endl;
    }
}

void printGrid(Grid *grid){
    Cell*** g = (*grid).getGrid();
    for (int i = 0; i<(*grid).getN(); ++i) {
        string row = "";
        for (int j=0; j<(*grid).getP(); ++j) {
            row += to_string((*g[i][j]).isAlive());
        }
    cout << row << endl;
    }
}
*/
void GOL::run(){

    vector<string> data = {
        "0","0","0","0","0","0","0",
        "0","0","0","0","0","0","0",
        "0","1","1","1","0","0","0",
        "0","0","0","0","0","0","0",
        "0","0","0","0","0","0","1",
    };
    int maxIteration = 10;
    Grid* foo = GridFactory::createGrid("std",5,7,data);
    LifeAlgorithm fooUtils(foo);

    Cell*** g = (*foo).getGrid();
        for (int i = 0; i<(*foo).getN(); ++i) {
        string row = "";
        for (int j=0; j<(*foo).getP(); ++j) {
            row += to_string((*g[i][j]).isAlive());
        }
        cout << row << endl;
        };

    for(int i=1;i<=maxIteration;i++){
        cout << "ITERATION N°" << i << endl;
        fooUtils.iterateGrid();
        
        Cell*** g = (*foo).getGrid();
        for (int i = 0; i<(*foo).getN(); ++i) {
        string row = "";
        for (int j=0; j<(*foo).getP(); ++j) {
            row += to_string((*g[i][j]).isAlive());
        }
        cout << row << endl;
        };

        if(fooUtils.isGridStable(T_MAX)){
            cout << "grille stable" << endl;
            return;
        }
    }
    cout << "fin" << endl;
}