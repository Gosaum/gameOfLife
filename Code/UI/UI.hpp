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
#include "Parameters.hpp"
#include "LifeAlgorithm.hpp"

class UI {
private:
    GameSettings settings;

public:
    UI();
    int renderBegin();
    void runSimulation(unique_ptr<Grid>& grid);
};