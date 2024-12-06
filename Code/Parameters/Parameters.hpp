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

#include "Grid.hpp"


class ErrorHandler {
public:
    static void showError(const std::string& message);
    static void showError(const std::string& message, RenderWindow* window);
};

class FileHandler {
public:
    static std::string openFileDialog();
    static std::string saveFileDialog();
    static std::unique_ptr<Grid> loadGridFromFile(const std::string& path, const std::string& type);
    static void saveGridToFile(const Grid& grid, const std::string& path);
    static void saveSimulationHistory(
        const std::string& path,
        const std::vector<std::pair<int, std::vector<std::vector<std::string>>>>& simulationHistory
    );
};

class GameSettings {
private:
    int maxIterations;
    std::string gridType;

public:
    GameSettings(int maxIterations = 100, const std::string& gridType = "Standard");

    int getMaxIterations() const;
    void setMaxIterations(int iterations);

    const std::string& getGridType() const;
    void setGridType(const std::string& type);
};