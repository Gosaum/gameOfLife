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

class CLI {
private:
    GameSettings settings;
    FileHandler fileHandler;
    vector<pair<int, vector<vector<std::string>>>> iterationData;

public:
    CLI();
    void runConsoleMode();
};