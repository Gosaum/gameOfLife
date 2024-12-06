#pragma once //include guard

#include <vector>
#include <string>
using namespace std;

#include "Cell.hpp"

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
    string getGridSignature() const;

    void updateAliveCells();
    void updateAliveCells(const vector<Cell*>& toggledCells);
    void toggleCells(const vector<Cell*>& cellsToToggle);

    virtual vector<Cell*> mooreNeighborhood(const Cell *cell) const;
    virtual vector<Cell*> computeCellsToToggle() const; 
};

class StandardGrid : public Grid{
    
    public :

    StandardGrid(); //default grid
    StandardGrid(int n, int p);
    StandardGrid(int n, int p,const vector<string>& data);

};

class ToroidalGrid : public Grid{
    
    public :

    ToroidalGrid(); //default grid
    ToroidalGrid(int n, int p);
    ToroidalGrid(int n, int p,const vector<string>& data);

    vector<Cell*> mooreNeighborhood(const Cell *cell) const override;
};

class GridFactory {

    public:

    static Grid* createGrid(const string& gridType);
    static Grid* createGrid(const string& gridType, int n, int p);
    static Grid* createGrid(const string& gridType, int n, int p, const vector<string>& data);
};