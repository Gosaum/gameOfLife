#pragma once

#include "Grid.hpp"

class LifeAlgorithm{
    
    private :

    Grid& grid;
    vector<string> signatureHistory;

    public :

    LifeAlgorithm(Grid& grid);
    LifeAlgorithm(Grid *grid);

    void iterateGrid();
    bool isGridStable();
    bool isGridStable(int Tmax);
};