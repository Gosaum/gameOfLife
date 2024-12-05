#ifndef CLI_HPP
#define CLI_HPP

#include "FileHandler.hpp"
#include "GameSettings.hpp"
#include <iostream>

class CLI {
private:
    GameSettings settings;
    FileHandler fileHandler;

public:
    CLI();
    void runConsoleMode();
};

#endif