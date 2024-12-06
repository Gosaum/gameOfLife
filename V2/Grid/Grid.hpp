#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "Cell.hpp"

class Grid {
protected:
    int n, p;
    std::vector<std::vector<std::unique_ptr<Cell>>> cells;

public:
    Grid(int rows, int cols);
    virtual ~Grid() = default;

    int getN() const;
    int getP() const;

    void setCell(int x, int y, const std::string& type);
    Cell* getCell(int x, int y) const;
    void updateAliveCells(const std::vector<Cell*>& toggledCells);
    virtual std::vector<Cell*> mooreNeighborhood(Cell* cell) const = 0;
    void printGrid() const;
};

class StandardGrid : public Grid {
public:
    StandardGrid(int rows, int cols);
    std::vector<Cell*> mooreNeighborhood(Cell* cell) const override;
};

class ToroidalGrid : public Grid {
public:
    ToroidalGrid(int rows, int cols);
    std::vector<Cell*> mooreNeighborhood(Cell* cell) const override;
};

class GridFactory {
public:
    static std::unique_ptr<Grid> createGrid(const std::string& type, int rows, int cols);
};