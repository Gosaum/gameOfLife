#include "GUI.hpp"

GUI::GUI() : window(sf::VideoMode(800, 600), "Game of Life - Mode GUI", sf::Style::Titlebar | sf::Style::Close) {
    if (!font.loadFromFile("arial.ttf")) {
        throw runtime_error("Impossible de charger la police Arial.");
    }
}

void GUI::runGUI() {
    renderBegin();
}

void GUI::renderBegin() {
    window.requestFocus();
    while (window.isOpen()) {
        Text title("Bienvenue dans Game of Life", font, 30);
        title.setFillColor(Color::Black);
        title.setPosition((window.getSize().x - title.getLocalBounds().width) / 2, 50.f);

        RectangleShape fileButton(Vector2f(400.f, 50.f));
        fileButton.setPosition((window.getSize().x - fileButton.getSize().x) / 2, 150.f);
        fileButton.setFillColor(Color::White);
        fileButton.setOutlineColor(Color::Black);
        fileButton.setOutlineThickness(2.f);

        Text fileButtonText("Charger une grille existante", font, 20);
        fileButtonText.setFillColor(Color::Black);
        fileButtonText.setPosition(fileButton.getPosition().x + 20.f, fileButton.getPosition().y + 10.f);

        RectangleShape createButton(Vector2f(400.f, 50.f));
        createButton.setPosition((window.getSize().x - createButton.getSize().x) / 2, 250.f);
        createButton.setFillColor(Color::White);
        createButton.setOutlineColor(Color::Black);
        createButton.setOutlineThickness(2.f);

        Text createButtonText("Creer une grille personnalisee", font, 20);
        createButtonText.setFillColor(Color::Black);
        createButtonText.setPosition(createButton.getPosition().x + 20.f, createButton.getPosition().y + 10.f);

        window.clear(Color::White);
        window.draw(title);
        window.draw(fileButton);
        window.draw(fileButtonText);
        window.draw(createButton);
        window.draw(createButtonText);
        window.display();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            if (event.type == Event::MouseButtonPressed) {
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
                    catch (const exception& e) {
                        ErrorHandler::showError(e.what(), &window);
                    }
                }

                if (createButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    renderCustomGrid();
                    renderConfigMenu();
                    renderGrid(*grid);
                }
            }
        }
    }
}

void GUI::renderConfigMenu() {
    window.requestFocus();
    Text title("Configuration de la simulation", font, 30);
    title.setFillColor(Color::Black);
    title.setPosition(50.f, 20.f);

    Text iterationsText("Nombre d'iterations (1-10000):", font, 20);
    iterationsText.setFillColor(Color::Black);
    iterationsText.setPosition(50.f, 100.f);

    RectangleShape iterationsBox(Vector2f(150.f, 40.f));
    iterationsBox.setFillColor(Color::White);
    iterationsBox.setOutlineColor(Color::Black);
    iterationsBox.setOutlineThickness(2.f);
    iterationsBox.setPosition(300.f, 100.f);

    string iterationsInput = to_string(settings.getMaxIterations());
    Text iterationsValue(iterationsInput, font, 20);
    iterationsValue.setFillColor(Color::Black);
    iterationsValue.setPosition(310.f, 105.f);

    Text toroidalText("Mode toroidal:", font, 20);
    toroidalText.setFillColor(Color::Black);
    toroidalText.setPosition(50.f, 200.f);

    RectangleShape toroidalButton(Vector2f(150.f, 40.f));
    toroidalButton.setFillColor(settings.getGridType() == "Toroidal" ? Color::Green : Color::Red);
    toroidalButton.setOutlineColor(Color::Black);
    toroidalButton.setOutlineThickness(2.f);
    toroidalButton.setPosition(300.f, 200.f);

    Text toroidalState(settings.getGridType() == "Toroidal" ? "ON" : "OFF", font, 20);
    toroidalState.setFillColor(Color::Black);
    toroidalState.setPosition(350.f, 205.f);

    RectangleShape confirmButton(Vector2f(150.f, 50.f));
    confirmButton.setFillColor(Color::White);
    confirmButton.setOutlineColor(Color::Black);
    confirmButton.setOutlineThickness(2.f);
    confirmButton.setPosition(200.f, 300.f);

    Text confirmText("Confirmer", font, 20);
    confirmText.setFillColor(Color::Black);
    confirmText.setPosition(confirmButton.getPosition().x + 20.f, confirmButton.getPosition().y + 10.f);

    bool editingIterations = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            if (event.type == Event::MouseButtonPressed) {
                if (iterationsBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    editingIterations = true;
                }
                else {
                    editingIterations = false;
                }

                if (toroidalButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (settings.getGridType() == "Toroidal") {
                        settings.setGridType("Standard");
                        toroidalButton.setFillColor(Color::Red);
                        toroidalState.setString("OFF");
                    }
                    else {
                        settings.setGridType("Toroidal");
                        toroidalButton.setFillColor(Color::Green);
                        toroidalState.setString("ON");
                    }
                }

                if (confirmButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    try {
                        int iterations = stoi(iterationsInput);
                        if (iterations < 1 || iterations > 10000) {
                            throw invalid_argument("Iterations must be between 1 and 10000.");
                        }
                        settings.setMaxIterations(iterations);
                        return;
                    }
                    catch (...) {
                        ErrorHandler::showError("Entrez un nombre valide entre 1 et 10000.", &window);
                    }
                }
            }

            if (editingIterations && event.type == Event::TextEntered) {
                if (isdigit(event.text.unicode)) {
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

        window.clear(Color::White);
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
    window.requestFocus();
    const int expandedWindowWidth = 1000;
    const int expandedWindowHeight = 800;

    window.setSize(Vector2u(expandedWindowWidth, expandedWindowHeight));
    window.setView(View(FloatRect(0, 0, expandedWindowWidth, expandedWindowHeight)));

    int gridRows = 20;
    int gridCols = 20;
    grid = GridFactory::createGrid(settings.getGridType(), gridRows, gridCols);

    for (int i = 0; i < gridRows; ++i) {
        for (int j = 0; j < gridCols; ++j) {
            grid->setCell(i, j, "Standard");
        }
    }

    float cellSize = min((expandedWindowWidth - 200.f) / gridCols, (expandedWindowHeight - 400.f) / gridRows);
    float gridWidth = cellSize * gridCols;
    float gridHeight = cellSize * gridRows;
    float offsetX = (expandedWindowWidth - gridWidth) / 2;
    float offsetY = 200;

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
    rowsText.setPosition(50.f, 100.f);

    RectangleShape rowsBox(Vector2f(100.f, 30.f));
    rowsBox.setFillColor(Color::White);
    rowsBox.setOutlineColor(Color::Black);
    rowsBox.setOutlineThickness(2.f);
    rowsBox.setPosition(200.f, 100.f);

    string rowsInput = to_string(gridRows);
    Text rowsValue(rowsInput, font, 18);
    rowsValue.setFillColor(Color::Black);
    rowsValue.setPosition(210.f, 105.f);

    Text colsText("Nombre de colonnes:", font, 20);
    colsText.setFillColor(Color::Black);
    colsText.setPosition(50.f, 150.f);

    RectangleShape colsBox(Vector2f(100.f, 30.f));
    colsBox.setFillColor(Color::White);
    colsBox.setOutlineColor(Color::Black);
    colsBox.setOutlineThickness(2.f);
    colsBox.setPosition(200.f, 150.f);

    string colsInput = to_string(gridCols);
    Text colsValue(colsInput, font, 18);
    colsValue.setFillColor(Color::Black);
    colsValue.setPosition(210.f, 155.f);

    RectangleShape saveButton(Vector2f(200.f, 50.f));
    saveButton.setFillColor(Color::White);
    saveButton.setOutlineThickness(1.f);
    saveButton.setOutlineColor(Color::Black);
    saveButton.setPosition(expandedWindowWidth / 2 - saveButton.getSize().x / 2, gridHeight + offsetY + 60.f);

    Text saveButtonText("Sauvegarder Grille", font, 18);
    saveButtonText.setFillColor(Color::Black);
    saveButtonText.setPosition(saveButton.getPosition().x + 20.f, saveButton.getPosition().y + 10.f);

    RectangleShape launchButton(Vector2f(200.f, 50.f));
    launchButton.setFillColor(Color::White);
    launchButton.setOutlineThickness(1.f);
    launchButton.setOutlineColor(Color::Black);
    launchButton.setPosition(expandedWindowWidth / 2 - launchButton.getSize().x / 2, gridHeight + offsetY + 120.f);

    Text launchButtonText("Lancer Simulation", font, 18);
    launchButtonText.setFillColor(Color::Black);
    launchButtonText.setPosition(launchButton.getPosition().x + 20.f, launchButton.getPosition().y + 10.f);

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

                if (saveButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    string filePath = FileHandler::saveFileDialog();
                    if (!filePath.empty()) {
                        FileHandler::saveGridToFile(*grid, filePath);
                    }
                }

                if (launchButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    return;
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

            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    isDragging = false;
                }
            }

            if (event.type == Event::MouseMoved && isDragging) {
                int x = (event.mouseMove.y - offsetY) / cellSize;
                int y = (event.mouseMove.x - offsetX) / cellSize;

                if (x >= 0 && y >= 0 && x < gridRows && y < gridCols) {
                    grid->setCell(x, y, selectedState == 1 ? "Standard" : (selectedState == 3 ? "Obstacle" : "Standard"));
                    grid->getCell(x, y)->setAlive(selectedState == 1);
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

                try {
                    int rows = stoi(rowsInput);
                    int cols = stoi(colsInput);
                    if (rows > 0 && cols > 0) {
                        gridRows = rows;
                        gridCols = cols;
                        grid = GridFactory::createGrid(settings.getGridType(), gridRows, gridCols);

                        cellSize = min((expandedWindowWidth - 200.f) / gridCols, (expandedWindowHeight - 400.f) / gridRows);
                        gridWidth = cellSize * gridCols;
                        gridHeight = cellSize * gridRows;
                        offsetX = (expandedWindowWidth - gridWidth) / 2;
                        offsetY = 200;
                    }
                }
                catch (...) {}
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
        window.draw(saveButton);
        window.draw(saveButtonText);
        window.draw(launchButton);
        window.draw(launchButtonText);

        window.display();
    }
}

void GUI::renderGrid(Grid& grid) {
    window.requestFocus();
    const int expandedWindowWidth = 1000;
    const int expandedWindowHeight = 800;

    int gridRows = grid.getN();
    int gridCols = grid.getP();

    float cellSize = min((expandedWindowWidth - 200.f) / gridCols, (expandedWindowHeight - 400.f) / gridRows);
    float gridWidth = cellSize * gridCols;
    float gridHeight = cellSize * gridRows;
    float offsetX = (expandedWindowWidth - gridWidth) / 2;
    float offsetY = 150;

    window.setSize(Vector2u(expandedWindowWidth, expandedWindowHeight));
    window.setView(View(FloatRect(0, 0, expandedWindowWidth, expandedWindowHeight)));

    Text pauseText("Simulation en pause", font, 20);
    pauseText.setFillColor(Color::Red);
    pauseText.setPosition(10.f, 10.f);

    Text iterationText("Iteration : 0", font, 20);
    iterationText.setFillColor(Color::Black);
    iterationText.setPosition(10.f, 40.f);

    Text endReasonText("", font, 20);
    endReasonText.setFillColor(Color::Black);
    endReasonText.setPosition(10.f, 70.f);

    Text currentCellType("Type cellule : Vivant", font, 20);
    currentCellType.setFillColor(Color::Black);
    currentCellType.setPosition(10.f, 100.f);

    RectangleShape pauseButton(Vector2f(150.f, 40.f));
    pauseButton.setFillColor(Color::White);
    pauseButton.setOutlineColor(Color::Black);
    pauseButton.setOutlineThickness(2.f);
    pauseButton.setPosition(expandedWindowWidth - 200.f, 10.f);

    Text pauseButtonText("Pause", font, 20);
    pauseButtonText.setFillColor(Color::Black);
    pauseButtonText.setPosition(pauseButton.getPosition().x + 10.f, pauseButton.getPosition().y + 5.f);

    RectangleShape saveButton(Vector2f(150.f, 40.f));
    saveButton.setFillColor(Color::White);
    saveButton.setOutlineColor(Color::Black);
    saveButton.setOutlineThickness(2.f);
    saveButton.setPosition(expandedWindowWidth - 200.f, 60.f);

    Text saveButtonText("Sauvegarder", font, 20);
    saveButtonText.setFillColor(Color::Black);
    saveButtonText.setPosition(saveButton.getPosition().x + 10.f, saveButton.getPosition().y + 5.f);

    RectangleShape cellTypeButton(Vector2f(150.f, 40.f));
    cellTypeButton.setFillColor(Color::White);
    cellTypeButton.setOutlineColor(Color::Black);
    cellTypeButton.setOutlineThickness(2.f);
    cellTypeButton.setPosition(expandedWindowWidth - 200.f, 110.f);

    Text cellTypeButtonText("Changer Type", font, 20);
    cellTypeButtonText.setFillColor(Color::Black);
    cellTypeButtonText.setPosition(cellTypeButton.getPosition().x + 10.f, cellTypeButton.getPosition().y + 5.f);

    Text speedText("Vitesse (s):", font, 20);
    speedText.setFillColor(Color::Black);
    speedText.setPosition(10.f, expandedWindowHeight - 80.f);

    RectangleShape slider(Vector2f(200.f, 10.f));
    slider.setFillColor(Color::Black);
    slider.setPosition(150.f, expandedWindowHeight - 70.f);

    CircleShape sliderKnob(10.f);
    sliderKnob.setFillColor(Color::Red);
    sliderKnob.setPosition(slider.getPosition().x + 190.f, slider.getPosition().y - 5.f);

    float sliderValue = 10.f;
    bool isPaused = true;
    bool draggingSlider = false;
    int iteration = 0;
    int selectedCellType = 1;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            if (event.type == Event::MouseButtonPressed) {
                if (pauseButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    isPaused = !isPaused;
                }

                if (saveButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    string filePath = FileHandler::saveFileDialog();
                    if (!filePath.empty()) {
                        FileHandler::saveGridToFile(grid, filePath);
                    }
                }

                if (cellTypeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectedCellType = (selectedCellType % 3) + 1;
                    if (selectedCellType == 1) currentCellType.setString("Type cellule : Vivant");
                    if (selectedCellType == 2) currentCellType.setString("Type cellule : Mort");
                    if (selectedCellType == 3) currentCellType.setString("Type cellule : Obstacle");
                }

                if (sliderKnob.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    draggingSlider = true;
                }
            }

            if (event.type == Event::MouseButtonReleased) {
                draggingSlider = false;
            }

            if (event.type == Event::MouseMoved && draggingSlider) {
                float x = event.mouseMove.x;
                if (x >= slider.getPosition().x && x <= slider.getPosition().x + slider.getSize().x) {
                    sliderKnob.setPosition(x - sliderKnob.getRadius(), sliderKnob.getPosition().y);
                    sliderValue = 10.f - ((x - slider.getPosition().x) / slider.getSize().x * 10.f);
                }
            }
        }

        if (!isPaused) {
            ++iteration;
            auto toggledCells = LifeAlgorithm(&grid).computeCellsToToggle();
            if (toggledCells.empty()) {
                isPaused = true;
                endReasonText.setString("Fin : Stabilisee");
            }
            else {
                LifeAlgorithm(&grid).toggleCells(toggledCells);
            }

            iterationText.setString("Iteration : " + to_string(iteration));
        }

        window.clear(Color::White);

        for (int i = 0; i < gridRows; ++i) {
            for (int j = 0; j < gridCols; ++j) {
                RectangleShape cell(Vector2f(cellSize, cellSize));
                cell.setPosition(offsetX + j * cellSize, offsetY + i * cellSize);

                Cell* cellObject = grid.getCell(i, j);
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

        if (isPaused) {
            window.draw(pauseText);
        }

        window.draw(iterationText);
        window.draw(endReasonText);
        window.draw(currentCellType);
        window.draw(pauseButton);
        window.draw(pauseButtonText);
        window.draw(saveButton);
        window.draw(saveButtonText);
        window.draw(cellTypeButton);
        window.draw(cellTypeButtonText);
        window.draw(speedText);
        window.draw(slider);
        window.draw(sliderKnob);

        window.display();

        sf::sleep(sf::seconds(sliderValue));
    }
}
