#pragma once

#include "Grid.hpp"
#include "Parameters.hpp"
#include "LifeAlgorithm.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class UI {
private:
    GameSettings settings;

public:
    UI();
    int renderBegin();
    void runSimulation(unique_ptr<Grid>& grid);
};