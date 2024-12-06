#ifndef UI_HPP
#define UI_HPP

#include "GameSettings.hpp"
#include "Grid.hpp"
#include "FileHandler.hpp"
#include "ErrorHandler.hpp"
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

#endif