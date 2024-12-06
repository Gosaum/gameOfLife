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

class Cell {
protected:
    bool alive;
    int x, y;

public:
    Cell(int x = 0, int y = 0) : alive(false), x(x), y(y) {}
    virtual ~Cell() {}
    bool isAlive() const { return alive; }
    void setAlive(bool state) { alive = state; }
    int getX() const { return x; }
    int getY() const { return y; }
    virtual void toggleAlive() = 0;
    virtual bool computeNextState(int livingNeighbors) = 0;
};

class StandardCell : public Cell {
public:
    StandardCell(int x, int y);
    void toggleAlive() override;
    bool computeNextState(int livingNeighbors) override;
};

class ObstacleCell : public Cell {
public:
    ObstacleCell(int x, int y);
    void toggleAlive() override;
    bool computeNextState(int livingNeighbors) override;
};

class CellFactory {
public:
    static std::unique_ptr<Cell> createCell(const std::string& type, int x, int y);
};
