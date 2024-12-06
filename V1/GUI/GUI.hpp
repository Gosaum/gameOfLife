#ifndef GUI_HPP
#define GUI_HPP

#include "GameSettings.hpp"
#include "FileHandler.hpp"
#include "ErrorHandler.hpp"
#include "Grid.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class GUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    std::unique_ptr<Grid> grid;
    GameSettings settings;

    void renderBegin();
    void renderConfigMenu();
    void renderCustomGrid();
    void renderGrid(Grid& grid);

public:
    GUI();
    void runGUI();
};

#endif