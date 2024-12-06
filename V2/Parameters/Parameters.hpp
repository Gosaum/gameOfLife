#pragma once

#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid.hpp"

class FileHandler {
public:
    static std::string openFileDialog();
    static std::string saveFileDialog();
    static std::unique_ptr<Grid> loadGridFromFile(const std::string& path, const std::string& type);
    static void saveGridToFile(const Grid& grid, const std::string& path);
    static void saveSimulationHistory(const string& path,const vector<pair<int, vector<vector<string>>>>& simulationHistory);
};

class ErrorHandler {
public:
    static void showError(const std::string& message);
    static void showError(const std::string& message, sf::RenderWindow* window);
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