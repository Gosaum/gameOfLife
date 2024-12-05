#pragma once //include guard

#include "grid.hpp"

class StandardGrid : public Grid{
    
    public :

    StandardGrid(); //default grid
    StandardGrid(int n, int p);
    StandardGrid(int n, int p,const vector<string>& data);

};