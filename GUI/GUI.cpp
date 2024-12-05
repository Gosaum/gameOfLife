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
    sf::Text title("Configuration de la simulation", font, 30);
    title.setFillColor(sf::Color::Black);
    title.setPosition(50.f, 20.f);

    sf::Text iterationsText("Nombre d'iterations (1-10000):", font, 20);
    iterationsText.setFillColor(sf::Color::Black);
    iterationsText.setPosition(50.f, 100.f);

    sf::RectangleShape iterationsBox(sf::Vector2f(150.f, 40.f));
    iterationsBox.setFillColor(sf::Color::White);
    iterationsBox.setOutlineColor(sf::Color::Black);
    iterationsBox.setOutlineThickness(2.f);
    iterationsBox.setPosition(300.f, 100.f);

    std::string iterationsInput = std::to_string(settings.getMaxIterations());
    sf::Text iterationsValue(iterationsInput, font, 20);
    iterationsValue.setFillColor(sf::Color::Black);
    iterationsValue.setPosition(310.f, 105.f);

    sf::Text toroidalText("Mode toroidal:", font, 20);
    toroidalText.setFillColor(sf::Color::Black);
    toroidalText.setPosition(50.f, 200.f);

    sf::RectangleShape toroidalButton(sf::Vector2f(150.f, 40.f));
    toroidalButton.setFillColor(settings.getGridType() == "Toroidal" ? sf::Color::Green : sf::Color::Red);
    toroidalButton.setOutlineColor(sf::Color::Black);
    toroidalButton.setOutlineThickness(2.f);
    toroidalButton.setPosition(300.f, 200.f);

    sf::Text toroidalState(settings.getGridType() == "Toroidal" ? "ON" : "OFF", font, 20);
    toroidalState.setFillColor(sf::Color::Black);
    toroidalState.setPosition(350.f, 205.f);

    sf::RectangleShape confirmButton(sf::Vector2f(150.f, 50.f));
    confirmButton.setFillColor(sf::Color::White);
    confirmButton.setOutlineColor(sf::Color::Black);
    confirmButton.setOutlineThickness(2.f);
    confirmButton.setPosition(200.f, 300.f);

    sf::Text confirmText("Confirmer", font, 20);
    confirmText.setFillColor(sf::Color::Black);
    confirmText.setPosition(confirmButton.getPosition().x + 20.f, confirmButton.getPosition().y + 10.f);

    bool editingIterations = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (iterationsBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    editingIterations = true;
                }
                else {
                    editingIterations = false;
                }

                if (toroidalButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (settings.getGridType() == "Toroidal") {
                        settings.setGridType("Standard");
                        toroidalButton.setFillColor(sf::Color::Red);
                        toroidalState.setString("OFF");
                    }
                    else {
                        settings.setGridType("Toroidal");
                        toroidalButton.setFillColor(sf::Color::Green);
                        toroidalState.setString("ON");
                    }
                }

                if (confirmButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    try {
                        int iterations = std::stoi(iterationsInput);
                        if (iterations < 1 || iterations > 10000) {
                            throw std::invalid_argument("Iterations must be between 1 and 10000.");
                        }
                        settings.setMaxIterations(iterations);
                        return;
                    }
                    catch (...) {
                        ErrorHandler::showError("Entrez un nombre valide entre 1 et 10000.", &window);
                    }
                }
            }

            if (editingIterations && event.type == sf::Event::TextEntered) {
                if (std::isdigit(event.text.unicode)) {
                    if (iterationsInput.size() < 5) {
                        iterationsInput += static_cast<char>(event.text.unicode);
                        iterationsValue.setString(iterationsInput);
                    }
                }
                else if (event.text.unicode == '\b' && !iterationsInput.empty()) {
                    iterationsInput.pop_back();
                    iterationsValue.setString(iterationsInput);
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(title);
        window.draw(iterationsText);
        window.draw(iterationsBox);
        window.draw(iterationsValue);
        window.draw(toroidalText);
        window.draw(toroidalButton);
        window.draw(toroidalState);
        window.draw(confirmButton);
        window.draw(confirmText);
        window.display();
    }
}

void GUI::renderCustomGrid() {
    const int expandedWindowWidth = 1000;
    const int expandedWindowHeight = 700;

    window.setSize(sf::Vector2u(expandedWindowWidth, expandedWindowHeight));
    window.setView(sf::View(sf::FloatRect(0, 0, expandedWindowWidth, expandedWindowHeight)));

    const int gridRows = settings.getGridType() == "Toroidal" ? 20 : 20;
    const int gridCols = 20;

    auto grid = GridFactory::createGrid(settings.getGridType(), gridRows, gridCols);

    const float cellSize = std::min((expandedWindowWidth - 200.f) / gridCols, (expandedWindowHeight - 300.f) / gridRows);
    const float offsetX = (expandedWindowWidth - gridCols * cellSize) / 2;
    const float offsetY = 100;

    int selectedState = 1;

    sf::Text stateText("Etat actuel : Vivant", font, 18);
    stateText.setFillColor(sf::Color::Black);
    stateText.setPosition(expandedWindowWidth / 2 - stateText.getLocalBounds().width / 2, 10.f);

    sf::RectangleShape stateButton1(sf::Vector2f(150.f, 30.f));
    sf::RectangleShape stateButton2(sf::Vector2f(150.f, 30.f));
    sf::RectangleShape stateButton3(sf::Vector2f(150.f, 30.f));

    stateButton1.setPosition(expandedWindowWidth / 2 - 250.f, 50.f);
    stateButton2.setPosition(expandedWindowWidth / 2 - 75.f, 50.f);
    stateButton3.setPosition(expandedWindowWidth / 2 + 100.f, 50.f);

    sf::Text stateButtonText1("Vivant", font, 16);
    sf::Text stateButtonText2("Mort", font, 16);
    sf::Text stateButtonText3("Obstacle", font, 16);

    stateButtonText1.setPosition(stateButton1.getPosition().x + 10.f, stateButton1.getPosition().y + 5.f);
    stateButtonText2.setPosition(stateButton2.getPosition().x + 10.f, stateButton2.getPosition().y + 5.f);
    stateButtonText3.setPosition(stateButton3.getPosition().x + 10.f, stateButton3.getPosition().y + 5.f);

    auto updateStateButtons = [&]() {
        stateButton1.setFillColor(selectedState == 1 ? sf::Color::Green : sf::Color(200, 200, 200));
        stateButton2.setFillColor(selectedState == 2 ? sf::Color::Red : sf::Color(200, 200, 200));
        stateButton3.setFillColor(selectedState == 3 ? sf::Color::Blue : sf::Color(200, 200, 200));
        };

    updateStateButtons();

    sf::RectangleShape confirmButton(sf::Vector2f(200.f, 50.f));
    confirmButton.setFillColor(sf::Color::White);
    confirmButton.setOutlineThickness(1.f);
    confirmButton.setOutlineColor(sf::Color::Black);
    confirmButton.setPosition(expandedWindowWidth / 2 - confirmButton.getSize().x / 2, gridRows * cellSize + offsetY + 40.f);

    sf::Text confirmText("Lancer Simulation", font, 18);
    confirmText.setFillColor(sf::Color::Black);
    confirmText.setPosition(confirmButton.getPosition().x + 20.f, confirmButton.getPosition().y + 10.f);

    sf::RectangleShape saveButton(sf::Vector2f(200.f, 50.f));
    saveButton.setFillColor(sf::Color::White);
    saveButton.setOutlineThickness(1.f);
    saveButton.setOutlineColor(sf::Color::Black);
    saveButton.setPosition(expandedWindowWidth / 2 - saveButton.getSize().x / 2, gridRows * cellSize + offsetY + 100.f);

    sf::Text saveButtonText("Sauvegarder Grille", font, 18);
    saveButtonText.setFillColor(sf::Color::Black);
    saveButtonText.setPosition(saveButton.getPosition().x + 10.f, saveButton.getPosition().y + 10.f);

    bool isDragging = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = (event.mouseButton.y - offsetY) / cellSize;
                    int y = (event.mouseButton.x - offsetX) / cellSize;
                    if (x >= 0 && y >= 0 && x < gridRows && y < gridCols) {
                        grid->updateAliveCells({ grid->getCell(x, y) });
                        isDragging = true;
                    }

                    if (stateButton1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        selectedState = 1;
                        stateText.setString("Etat actuel : Vivant");
                        updateStateButtons();
                    }
                    if (stateButton2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        selectedState = 2;
                        stateText.setString("Etat actuel : Mort");
                        updateStateButtons();
                    }
                    if (stateButton3.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        selectedState = 3;
                        stateText.setString("Etat actuel : Obstacle");
                        updateStateButtons();
                    }
                    if (confirmButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        return;
                    }
                    if (saveButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        std::string filePath = FileHandler::saveFileDialog();
                        if (!filePath.empty()) {
                            ErrorHandler::showError("Grille sauvegardee avec succes.", &window);
                        }
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isDragging = false;
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                if (isDragging) {
                    int x = (event.mouseMove.y - offsetY) / cellSize;
                    int y = (event.mouseMove.x - offsetX) / cellSize;
                    if (x >= 0 && y >= 0 && x < gridRows && y < gridCols) {
                        grid->updateAliveCells({ grid->getCell(x, y) });
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        for (int i = 0; i < gridRows; ++i) {
            for (int j = 0; j < gridCols; ++j) {
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cell.setPosition(offsetX + j * cellSize, offsetY + i * cellSize);

                auto cellObject = grid->getCell(i, j);
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

        window.draw(stateText);
        window.draw(stateButton1);
        window.draw(stateButtonText1);
        window.draw(stateButton2);
        window.draw(stateButtonText2);
        window.draw(stateButton3);
        window.draw(stateButtonText3);
        window.draw(confirmButton);
        window.draw(confirmText);
        window.draw(saveButton);
        window.draw(saveButtonText);

        window.display();
    }
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