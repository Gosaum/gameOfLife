#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <unordered_set>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <limits>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <commdlg.h>
#include <cassert>
#include <chrono>
#include <sstream>

using namespace std;
using namespace sf;

#include "Cell.hpp"

class Grid {
protected:
    int n, p;
    vector<vector<unique_ptr<Cell>>> cells;

public:
    Grid(int rows, int cols);
    virtual ~Grid() = default;

    int getN() const;
    int getP() const;

    void setCell(int x, int y, const string& type);
    Cell* getCell(int x, int y) const;
    void updateAliveCells(const vector<Cell*>& toggledCells);
    virtual vector<Cell*> mooreNeighborhood(Cell* cell) const = 0;
    void printGrid() const;
    void printGridCLI() const;
    vector<vector<int>> getGridState() const;
};

class StandardGrid : public Grid {
public:
    StandardGrid(int rows, int cols);
    vector<Cell*> mooreNeighborhood(Cell* cell) const override;
};

class ToroidalGrid : public Grid {
public:
    ToroidalGrid(int rows, int cols);
    vector<Cell*> mooreNeighborhood(Cell* cell) const override;
};

class GridFactory {
public:
    static unique_ptr<Grid> createGrid(const string& type, int rows, int cols);
};