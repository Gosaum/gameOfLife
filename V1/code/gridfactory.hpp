#pragma once

#include <string>
using namespace std;

#include "grid.hpp"
#include "standardgrid.hpp"
#include "toroidalgrid.hpp"

class GridFactory {

    public:

    static Grid* createGrid(const string& gridType);
    static Grid* createGrid(const string& gridType, int n, int p);
    static Grid* createGrid(const string& gridType, int n, int p, const vector<string>& data);
};