#pragma once

#include "Parameters.hpp"
#include <string>
#include <iostream>
#include <fstream>

class CLI {
private:
    GameSettings settings;
    FileHandler fileHandler;

public:
    CLI();
    void runConsoleMode();
};