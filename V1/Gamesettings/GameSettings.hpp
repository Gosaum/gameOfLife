#ifndef GAMESETTINGS_HPP
#define GAMESETTINGS_HPP

#include <string>

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

#endif