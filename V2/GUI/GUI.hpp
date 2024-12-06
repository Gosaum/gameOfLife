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

#include "Parameters.hpp"
#include "LifeAlgorithm.hpp"
#include "Grid.hpp"

class GUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    unique_ptr<Grid> grid;
    GameSettings settings;

    void renderBegin();
    void renderConfigMenu();
    void renderCustomGrid();
    void renderGrid(Grid& grid);

public:
    GUI();
    void runGUI();
};