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

#include "Grid.hpp"

class LifeAlgorithm {
private:
    Grid* grid;

public:
    LifeAlgorithm(Grid* grid);

    vector<Cell*> computeCellsToToggle();
    void toggleCells(const vector<Cell*>& cellsToToggle);
    string hashGrid(const Grid& grid);
    void simulateWithLoopDetection(Grid* grid, int maxIterations);
};