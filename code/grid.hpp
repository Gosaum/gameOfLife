#pragma once //include guard

#include <vector>
using namespace std;

#include "cellfactory.hpp"

class Grid{

    protected : 
    
    Cell ***grid;
    int n;
    int p;
    vector<Cell*> aliveCells;

    Grid();
    Grid(int n, int p);
    Grid(int n, int p, const vector<string>& data);

    public :

    Cell*** getGrid() const;
    int getN() const;
    int getP() const;
    vector<Cell*> getAliveCells() const;

    void updateAliveCells();
    void updateAliveCells(const vector<Cell*>& toggledCells);
    void toggleCells(const vector<Cell*>& cellsToToggle);

    virtual vector<Cell*> mooreNeighborhood(const Cell *cell) const;
    virtual vector<Cell*> computeCellsToToggle() const; 
};