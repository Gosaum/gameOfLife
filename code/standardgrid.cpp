#include "standardgrid.hpp"

StandardGrid::StandardGrid():Grid(){};

StandardGrid::StandardGrid(int n, int p):Grid(n,p){};

StandardGrid::StandardGrid(int n, int p,const vector<string>& data):Grid(n,p,data){};