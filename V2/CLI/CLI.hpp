#pragma once

#include "Parameters.hpp"
#include <string>

class CLI {
private:
    GameSettings settings;
    FileHandler fileHandler;

public:
    CLI();
    void runConsoleMode();
};