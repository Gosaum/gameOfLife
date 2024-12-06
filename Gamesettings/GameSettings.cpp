#include "GameSettings.hpp"

GameSettings::GameSettings(int maxIterations, const std::string& gridType) : maxIterations(maxIterations), gridType(gridType) {}

int GameSettings::getMaxIterations() const {
    return maxIterations;
}

void GameSettings::setMaxIterations(int iterations) {
    maxIterations = iterations;
}

const std::string& GameSettings::getGridType() const {
    return gridType;
}

void GameSettings::setGridType(const std::string& type) {
    gridType = type;
}