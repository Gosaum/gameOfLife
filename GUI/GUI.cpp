#include "GUI.hpp"

GUI::GUI() : window(sf::VideoMode(800, 600), "Game of Life"), settings(10000, "Standard") {
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Impossible de charger la police Arial.");
    }
}

void GUI::runGUI() {
    renderBegin();
}

void GUI::renderBegin() {
    while (window.isOpen()) {
        sf::Text title("Bienvenue dans Game of Life", font, 30);
        title.setFillColor(sf::Color::Black);
        title.setPosition((window.getSize().x - title.getLocalBounds().width) / 2, 50.f);

        sf::RectangleShape fileButton(sf::Vector2f(400.f, 50.f));
        fileButton.setPosition((window.getSize().x - fileButton.getSize().x) / 2, 150.f);
        fileButton.setFillColor(sf::Color::White);
        fileButton.setOutlineColor(sf::Color::Black);
        fileButton.setOutlineThickness(2.f);

        sf::Text fileButtonText("Charger une grille existante", font, 20);
        fileButtonText.setFillColor(sf::Color::Black);
        fileButtonText.setPosition(fileButton.getPosition().x + 20.f, fileButton.getPosition().y + 10.f);

        sf::RectangleShape createButton(sf::Vector2f(400.f, 50.f));
        createButton.setPosition((window.getSize().x - createButton.getSize().x) / 2, 250.f);
        createButton.setFillColor(sf::Color::White);
        createButton.setOutlineColor(sf::Color::Black);
        createButton.setOutlineThickness(2.f);

        sf::Text createButtonText("Creer une grille personnalisee", font, 20);
        createButtonText.setFillColor(sf::Color::Black);
        createButtonText.setPosition(createButton.getPosition().x + 20.f, createButton.getPosition().y + 10.f);

        window.clear(sf::Color::White);
        window.draw(title);
        window.draw(fileButton);
        window.draw(fileButtonText);
        window.draw(createButton);
        window.draw(createButtonText);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (fileButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    try {
                        auto path = FileHandler::openFileDialog();
                        if (!path.empty()) {
                            grid = FileHandler::loadGridFromFile(path, settings.getGridType());
                            renderConfigMenu();
                            renderGrid(*grid);
                        }
                        else {
                            ErrorHandler::showError("Aucun fichier selectionne.", &window);
                        }
                    }
                    catch (const std::exception& e) {
                        ErrorHandler::showError(e.what(), &window);
                    }
                }

                if (createButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    renderCustomGrid();
                }
            }
        }
    }
}

void GUI::renderConfigMenu() {

}

void GUI::renderCustomGrid() {

}

void GUI::renderGrid(Grid& grid) {
    const int gridRows = grid.getN();
    const int gridCols = grid.getP();

    const float cellSize = std::min((window.getSize().x - 200.f) / gridCols, (window.getSize().y - 300.f) / gridRows);
    const float offsetX = (window.getSize().x - gridCols * cellSize) / 2;
    const float offsetY = 100;

    sf::Text pauseText("Simulation en pause (Appuyez sur ESPACE pour continuer)", font, 18);
    pauseText.setFillColor(sf::Color::Red);
    pauseText.setPosition(10.f, 20.f);

    sf::Text iterationText("Iteration : 0", font, 18);
    iterationText.setFillColor(sf::Color::Black);
    iterationText.setPosition(10.f, 60.f);

    bool isPaused = true;
    int iteration = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    isPaused = !isPaused;
                }
            }
        }

        if (!isPaused) {
            ++iteration;
            auto toggledCells = LifeAlgorithm(&grid).computeCellsToToggle();
            if (toggledCells.empty()) {
                isPaused = true;
                ErrorHandler::showError("Simulation stabilisee.", &window);
            }
            else {
                LifeAlgorithm(&grid).toggleCells(toggledCells);
            }
        }

        iterationText.setString("Iteration : " + std::to_string(iteration));

        window.clear(sf::Color::White);

        for (int i = 0; i < gridRows; ++i) {
            for (int j = 0; j < gridCols; ++j) {
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cell.setPosition(offsetX + j * cellSize, offsetY + i * cellSize);

                Cell* cellObject = grid.getCell(i, j);
                if (cellObject->isAlive()) {
                    cell.setFillColor(sf::Color::Green);
                }
                else {
                    cell.setFillColor(sf::Color::White);
                }

                cell.setOutlineColor(sf::Color::Black);
                cell.setOutlineThickness(1.f);
                window.draw(cell);
            }
        }

        if (isPaused) {
            window.draw(pauseText);
        }
        window.draw(iterationText);
        window.display();
    }
}