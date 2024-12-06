#pragma once //include guard

#include "grid.hpp"

class ToroidalGrid : public Grid{
    
    public :

    ToroidalGrid(); //default grid
    ToroidalGrid(int n, int p);
    ToroidalGrid(int n, int p,const vector<string>& data);

    vector<Cell*> mooreNeighborhood(const Cell *cell) const override;

};