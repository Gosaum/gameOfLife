#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>
#include "Cell.hpp"

using namespace std;

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