#pragma once

#include "Grid.hpp"
#include "Parameters.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

class UI {
private:
    GameSettings settings;

public:
    UI();
    int renderBegin();
    void runSimulation(std::unique_ptr<Grid>& grid);
};