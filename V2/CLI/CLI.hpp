#pragma once

#include "Parameters.hpp"
#include "LifeAlgorithm.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class CLI {
private:
    GameSettings settings;
    FileHandler fileHandler;

public:
    CLI();
    void runConsoleMode();
};