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
    const int expandedWindowHeight = 800;

    window.setSize(Vector2u(expandedWindowWidth, expandedWindowHeight));
    window.setView(View(FloatRect(0, 0, expandedWindowWidth, expandedWindowHeight)));

    int gridRows = 20;
    int gridCols = 20;
    grid = GridFactory::createGrid(settings.getGridType(), gridRows, gridCols);

    float cellSize = min((expandedWindowWidth - 200.f) / gridCols, (expandedWindowHeight - 300.f) / gridRows);
    float gridWidth = cellSize * gridCols;
    float gridHeight = cellSize * gridRows;
    float offsetX = (expandedWindowWidth - gridWidth) / 2;
    float offsetY = 150;

    int selectedState = 1;

    Text stateText("Etat actuel : Vivant", font, 18);
    stateText.setFillColor(Color::Black);
    stateText.setPosition(expandedWindowWidth / 2 - stateText.getLocalBounds().width / 2, 10.f);

    RectangleShape stateButton1(Vector2f(150.f, 30.f));
    stateButton1.setPosition(expandedWindowWidth / 2 - 250.f, 50.f);

    RectangleShape stateButton2(Vector2f(150.f, 30.f));
    stateButton2.setPosition(expandedWindowWidth / 2 - 75.f, 50.f);

    RectangleShape stateButton3(Vector2f(150.f, 30.f));
    stateButton3.setPosition(expandedWindowWidth / 2 + 100.f, 50.f);

    Text stateButtonText1("Vivant", font, 16);
    stateButtonText1.setFillColor(Color::Black);
    stateButtonText1.setPosition(stateButton1.getPosition().x + 10.f, stateButton1.getPosition().y + 5.f);

    Text stateButtonText2("Mort", font, 16);
    stateButtonText2.setFillColor(Color::Black);
    stateButtonText2.setPosition(stateButton2.getPosition().x + 10.f, stateButton2.getPosition().y + 5.f);

    Text stateButtonText3("Obstacle", font, 16);
    stateButtonText3.setFillColor(Color::Black);
    stateButtonText3.setPosition(stateButton3.getPosition().x + 10.f, stateButton3.getPosition().y + 5.f);

    Text rowsText("Nombre de lignes:", font, 20);
    rowsText.setFillColor(Color::Black);
    rowsText.setPosition(50.f, offsetY - 40.f);

    RectangleShape rowsBox(Vector2f(100.f, 30.f));
    rowsBox.setFillColor(Color::White);
    rowsBox.setOutlineColor(Color::Black);
    rowsBox.setOutlineThickness(2.f);
    rowsBox.setPosition(200.f, offsetY - 40.f);

    string rowsInput = to_string(gridRows);
    Text rowsValue(rowsInput, font, 18);
    rowsValue.setFillColor(Color::Black);
    rowsValue.setPosition(210.f, offsetY - 35.f);

    Text colsText("Nombre de colonnes:", font, 20);
    colsText.setFillColor(Color::Black);
    colsText.setPosition(50.f, offsetY);

    RectangleShape colsBox(Vector2f(100.f, 30.f));
    colsBox.setFillColor(Color::White);
    colsBox.setOutlineColor(Color::Black);
    colsBox.setOutlineThickness(2.f);
    colsBox.setPosition(200.f, offsetY);

    string colsInput = to_string(gridCols);
    Text colsValue(colsInput, font, 18);
    colsValue.setFillColor(Color::Black);
    colsValue.setPosition(210.f, offsetY + 5.f);

    auto updateStateButtons = [&]() {
        stateButton1.setFillColor(selectedState == 1 ? Color::Green : Color(200, 200, 200));
        stateButton2.setFillColor(selectedState == 2 ? Color::Red : Color(200, 200, 200));
        stateButton3.setFillColor(selectedState == 3 ? Color::Blue : Color(200, 200, 200));
        };

    updateStateButtons();

    bool isDragging = false;
    bool editingRows = false;
    bool editingCols = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            if (event.type == Event::MouseButtonPressed) {
                if (rowsBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    editingRows = true;
                    editingCols = false;
                }
                else if (colsBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    editingCols = true;
                    editingRows = false;
                }
                else {
                    editingRows = false;
                    editingCols = false;
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

                if (event.mouseButton.button == Mouse::Left) {
                    int x = (event.mouseButton.y - offsetY) / cellSize;
                    int y = (event.mouseButton.x - offsetX) / cellSize;

                    if (x >= 0 && y >= 0 && x < gridRows && y < gridCols) {
                        grid->setCell(x, y, selectedState == 1 ? "Standard" : (selectedState == 3 ? "Obstacle" : "Standard"));
                        grid->getCell(x, y)->setAlive(selectedState == 1);
                        isDragging = true;
                    }
                }
            }

            if (event.type == Event::TextEntered) {
                if (isdigit(event.text.unicode)) {
                    if (editingRows && rowsInput.size() < 3) {
                        rowsInput += static_cast<char>(event.text.unicode);
                        rowsValue.setString(rowsInput);
                    }
                    else if (editingCols && colsInput.size() < 3) {
                        colsInput += static_cast<char>(event.text.unicode);
                        colsValue.setString(colsInput);
                    }
                }
                else if (event.text.unicode == '\b') {
                    if (editingRows && !rowsInput.empty()) {
                        rowsInput.pop_back();
                        rowsValue.setString(rowsInput);
                    }
                    else if (editingCols && !colsInput.empty()) {
                        colsInput.pop_back();
                        colsValue.setString(colsInput);
                    }
                }
            }

            if (event.type == Event::KeyReleased) {
                try {
                    int newRows = stoi(rowsInput);
                    int newCols = stoi(colsInput);

                    if (newRows > 0 && newCols > 0 && (newRows != gridRows || newCols != gridCols)) {
                        gridRows = newRows;
                        gridCols = newCols;
                        grid = GridFactory::createGrid(settings.getGridType(), gridRows, gridCols);

                        cellSize = min((expandedWindowWidth - 200.f) / gridCols, (expandedWindowHeight - 300.f) / gridRows);
                        gridWidth = cellSize * gridCols;
                        gridHeight = cellSize * gridRows;
                        offsetX = (expandedWindowWidth - gridWidth) / 2;
                        offsetY = 150;
                    }
                }
                catch (...) {
                    // Ignore invalid inputs
                }
            }
        }

        window.clear(Color::White);

        for (int i = 0; i < gridRows; ++i) {
            for (int j = 0; j < gridCols; ++j) {
                RectangleShape cell(Vector2f(cellSize, cellSize));
                cell.setPosition(offsetX + j * cellSize, offsetY + i * cellSize);

                Cell* cellObject = grid->getCell(i, j);
                if (cellObject && cellObject->isAlive()) {
                    cell.setFillColor(Color::Green);
                }
                else if (dynamic_cast<ObstacleCell*>(cellObject)) {
                    cell.setFillColor(Color::Blue);
                }
                else {
                    cell.setFillColor(Color::White);
                }

                cell.setOutlineColor(Color::Black);
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
        window.draw(rowsText);
        window.draw(rowsBox);
        window.draw(rowsValue);
        window.draw(colsText);
        window.draw(colsBox);
        window.draw(colsValue);

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