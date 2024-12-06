#pragma once

#include "Parameters.hpp"
#include "LifeAlgorithm.hpp"
#include "Grid.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>
#include <stdexcept>

using namespace sf;
using namespace std;

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