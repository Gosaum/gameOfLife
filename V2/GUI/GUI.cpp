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
    iterationsBox.setPosition(400.f, 100.f);

    string iterationsInput = to_string(settings.getMaxIterations());
    Text iterationsValue(iterationsInput, font, 20);
    iterationsValue.setFillColor(Color::Black);
    iterationsValue.setPosition(iterationsBox.getPosition().x + iterationsBox.getSize().x / 3 + 0.f, 105.f);

    Text toroidalText("Mode toroidal:", font, 20);
    toroidalText.setFillColor(Color::Black);
    toroidalText.setPosition(50.f, 200.f);

    RectangleShape toroidalButton(Vector2f(150.f, 40.f));
    toroidalButton.setFillColor(settings.getGridType() == "Toroidal" ? Color::Green : Color::Red);
    toroidalButton.setOutlineColor(Color::Black);
    toroidalButton.setOutlineThickness(2.f);
    toroidalButton.setPosition(400.f, 200.f);

    Text toroidalState(settings.getGridType() == "Toroidal" ? "ON" : "OFF", font, 20);
    toroidalState.setFillColor(Color::Black);
    toroidalState.setPosition(toroidalButton.getPosition().x + toroidalButton.getSize().x / 3 + 0.f, 205.f);

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

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Enter:
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
                    break;
                default:
                    break;
                }
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
    rowsBox.setPosition(250.f, 100.f);

    string rowsInput = to_string(gridRows);
    Text rowsValue(rowsInput, font, 18);
    rowsValue.setFillColor(Color::Black);
    rowsValue.setPosition(rowsBox.getPosition().x + rowsBox.getSize().x / 3, 105.f);

    Text colsText("Nombre de colonnes:", font, 20);
    colsText.setFillColor(Color::Black);
    colsText.setPosition(50.f, 150.f);

    RectangleShape colsBox(Vector2f(100.f, 30.f));
    colsBox.setFillColor(Color::White);
    colsBox.setOutlineColor(Color::Black);
    colsBox.setOutlineThickness(2.f);
    colsBox.setPosition(250.f, 150.f);

    string colsInput = to_string(gridCols);
    Text colsValue(colsInput, font, 18);
    colsValue.setFillColor(Color::Black);
    colsValue.setPosition(colsBox.getPosition().x + colsBox.getSize().x / 3, 155.f);

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
    const int windowWidth = 1000;
    const int windowHeight = 800;

    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Game of Life");
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Erreur: Impossible de charger la police.");
    }

    bool isPaused = true;
    bool simulationEnded = false;
    float simulationSpeed = 1.0f;
    int iteration = 0;
    int selectedState = 1; // 1 = Vivant, 2 = Mort, 3 = Obstacle
    const int maxIterations = 10000;

    const int gridRows = grid.getN();
    const int gridCols = grid.getP();

    float cellSize = std::min((windowWidth - 200.f) / gridCols, (windowHeight - 300.f) / gridRows);
    float gridWidth = cellSize * gridCols;
    float gridHeight = cellSize * gridRows;
    float offsetX = (windowWidth - gridWidth) / 2;
    float offsetY = 100;

    RectangleShape pauseButton(Vector2f(50.f, 50.f));
    pauseButton.setFillColor(Color::Red);
    pauseButton.setOutlineColor(Color::Black);
    pauseButton.setOutlineThickness(2.f);
    pauseButton.setPosition(offsetX + gridWidth + 20.f, offsetY);

    Text speedText("Temps (s):", font, 18);
    speedText.setFillColor(Color::Black);
    speedText.setPosition(10.f, windowHeight - 130.f);

    RectangleShape speedBox(Vector2f(100.f, 30.f));
    speedBox.setFillColor(Color::White);
    speedBox.setOutlineColor(Color::Black);
    speedBox.setOutlineThickness(2.f);
    speedBox.setPosition(150.f, windowHeight - 135.f);

    Text speedValueText(std::to_string(simulationSpeed), font, 18);
    speedValueText.setFillColor(Color::Black);
    speedValueText.setPosition(155.f, windowHeight - 130.f);

    RectangleShape speedUpButton(Vector2f(30.f, 30.f));
    speedUpButton.setFillColor(Color::White);
    speedUpButton.setOutlineColor(Color::Black);
    speedUpButton.setOutlineThickness(2.f);
    speedUpButton.setPosition(270.f, windowHeight - 135.f);

    Text speedUpText("+", font, 18);
    speedUpText.setFillColor(Color::Black);
    speedUpText.setPosition(280.f, windowHeight - 130.f);

    RectangleShape speedDownButton(Vector2f(30.f, 30.f));
    speedDownButton.setFillColor(Color::White);
    speedDownButton.setOutlineColor(Color::Black);
    speedDownButton.setOutlineThickness(2.f);
    speedDownButton.setPosition(310.f, windowHeight - 135.f);

    Text speedDownText("-", font, 18);
    speedDownText.setFillColor(Color::Black);
    speedDownText.setPosition(320.f, windowHeight - 130.f);

    Text iterationText("Iteration : 0", font, 18);
    iterationText.setFillColor(Color::Black);
    iterationText.setPosition(10.f, 60.f);

    Text endReasonText("", font, 18);
    endReasonText.setFillColor(Color::Red);
    endReasonText.setPosition(windowWidth / 2 - 130.f, windowHeight - 150.f);

    RectangleShape saveButton(Vector2f(200.f, 50.f));
    saveButton.setFillColor(Color::White);
    saveButton.setOutlineColor(Color::Black);
    saveButton.setOutlineThickness(1.f);
    saveButton.setPosition(windowWidth / 2 - saveButton.getSize().x / 3, windowHeight - 50.f);

    Text saveButtonText("Sauvegarder", font, 18);
    saveButtonText.setFillColor(Color::Black);
    saveButtonText.setPosition(saveButton.getPosition().x + 25.f, saveButton.getPosition().y + 10.f);

    RectangleShape stateButton1(Vector2f(150.f, 30.f));
    RectangleShape stateButton2(Vector2f(150.f, 30.f));
    RectangleShape stateButton3(Vector2f(150.f, 30.f));

    stateButton1.setPosition(windowWidth / 2 - 250.f, 50.f);
    stateButton2.setPosition(windowWidth / 2 - 75.f, 50.f);
    stateButton3.setPosition(windowWidth / 2 + 100.f, 50.f);

    Text stateButtonText1("Vivant", font, 16);
    Text stateButtonText2("Mort", font, 16);
    Text stateButtonText3("Obstacle", font, 16);

    stateButtonText1.setPosition(stateButton1.getPosition().x + 10.f, stateButton1.getPosition().y + 5.f);
    stateButtonText2.setPosition(stateButton2.getPosition().x + 10.f, stateButton2.getPosition().y + 5.f);
    stateButtonText3.setPosition(stateButton3.getPosition().x + 10.f, stateButton3.getPosition().y + 5.f);

    Text selectedStateText("Selection : Vivant", font, 18);
    selectedStateText.setFillColor(Color::Black);
    selectedStateText.setPosition(10.f, 20.f);

    auto updateStateButtons = [&]() {
        stateButton1.setFillColor(selectedState == 1 ? Color::Green : Color(200, 200, 200));
        stateButton2.setFillColor(selectedState == 2 ? Color::Red : Color(200, 200, 200));
        stateButton3.setFillColor(selectedState == 3 ? Color::Blue : Color(200, 200, 200));
        if (selectedState == 1)
            selectedStateText.setString("Selection : Vivant");
        else if (selectedState == 2)
            selectedStateText.setString("Selection : Mort");
        else
            selectedStateText.setString("Selection : Obstacle");
        };

    updateStateButtons();

    auto lastUpdateTime = chrono::steady_clock::now();
    bool isDragging = false;
    vector<pair<int, vector<vector<string>>>> iterationData;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            if (saveButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                string filePath = FileHandler::saveFileDialog();
                if (!filePath.empty()) {
                    FileHandler::saveSimulationHistory(filePath, iterationData);
                }
            }

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Space) {
                    isPaused = !isPaused;
                    pauseButton.setFillColor(isPaused ? Color::Red : Color(200, 200, 200));
                }
                else if (event.key.code == Keyboard::Up) {
                    simulationSpeed = std::max(simulationSpeed - 0.1f, 0.01f);
                    speedValueText.setString(std::to_string(simulationSpeed).substr(0, 4));
                }
                else if (event.key.code == Keyboard::Down) {
                    simulationSpeed = std::min(simulationSpeed + 0.1f, 10.0f);
                    speedValueText.setString(std::to_string(simulationSpeed).substr(0, 4));
                }
                else if (event.key.code == Keyboard::S) {
                    string filePath = FileHandler::saveFileDialog();
                    if (!filePath.empty()) {
                        FileHandler::saveSimulationHistory(filePath, iterationData);
                    }
                }
                else if (event.key.code == Keyboard::Num1) {
                    selectedState = 1;
                    updateStateButtons();
                }
                else if (event.key.code == Keyboard::Num2) {
                    selectedState = 2;
                    updateStateButtons();
                }
                else if (event.key.code == Keyboard::Num3) {
                    selectedState = 3;
                    updateStateButtons();
                }
            }

            if (event.type == Event::MouseButtonPressed) {
                if (speedUpButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    simulationSpeed = std::min(simulationSpeed + 0.1f, 10.0f);
                    speedValueText.setString(std::to_string(simulationSpeed).substr(0, 4));
                }
                else if (speedDownButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    simulationSpeed = std::max(simulationSpeed - 0.1f, 0.01f);
                    speedValueText.setString(std::to_string(simulationSpeed).substr(0, 4));
                }
                else if (stateButton1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectedState = 1;
                    updateStateButtons();
                }
                else if (stateButton2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectedState = 2;
                    updateStateButtons();
                }
                else if (stateButton3.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectedState = 3;
                    updateStateButtons();
                }
                else if (event.mouseButton.button == Mouse::Left) {
                    int x = (event.mouseButton.y - offsetY) / cellSize;
                    int y = (event.mouseButton.x - offsetX) / cellSize;

                    if (x >= 0 && y >= 0 && x < gridRows && y < gridCols) {
                        grid.setCell(x, y, selectedState == 1 ? "Standard" : (selectedState == 3 ? "Obstacle" : "Standard"));
                        grid.getCell(x, y)->setAlive(selectedState == 1);
                        isDragging = true;
                    }
                }
            }

            if (event.type == Event::MouseMoved && isDragging) {
                int x = (event.mouseMove.y - offsetY) / cellSize;
                int y = (event.mouseMove.x - offsetX) / cellSize;

                if (x >= 0 && y >= 0 && x < gridRows && y < gridCols) {
                    grid.setCell(x, y, selectedState == 1 ? "Standard" : (selectedState == 3 ? "Obstacle" : "Standard"));
                    grid.getCell(x, y)->setAlive(selectedState == 1);
                }
            }

            if (event.type == Event::MouseButtonReleased && isDragging) {
                isDragging = false;
            }
        }

        if (!isPaused && !simulationEnded) {
            auto currentTime = chrono::steady_clock::now();
            auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(currentTime - lastUpdateTime).count();

            if (elapsedTime >= simulationSpeed * 1000) {
                ++iteration;
                iterationText.setString("Iteration : " + to_string(iteration));

                if (iteration >= maxIterations) {
                    simulationEnded = true;
                    isPaused = true;
                    endReasonText.setString("Simulation terminee : limite d'iterations atteinte.");
                }
                else {
                    auto toggledCells = LifeAlgorithm(&grid).computeCellsToToggle();
                    if (toggledCells.empty()) {
                        simulationEnded = true;
                        endReasonText.setString("Simulation terminee : grille stable.");
                    }
                    else {
                        LifeAlgorithm(&grid).toggleCells(toggledCells);
                    }
                }
                lastUpdateTime = currentTime;

                vector<vector<string>> currentGridState(gridRows, vector<string>(gridCols));
                for (int i = 0; i < gridRows; ++i) {
                    for (int j = 0; j < gridCols; ++j) {
                        Cell* cell = grid.getCell(i, j);
                        if (dynamic_cast<ObstacleCell*>(cell)) {
                            currentGridState[i][j] = "O";
                        }
                        else if (cell->isAlive()) {
                            currentGridState[i][j] = "1";
                        }
                        else {
                            currentGridState[i][j] = "0";
                        }
                    }
                }
                iterationData.emplace_back(iteration, currentGridState);
            }
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

        window.draw(pauseButton);
        window.draw(speedText);
        window.draw(speedBox);
        window.draw(speedValueText);
        window.draw(speedUpButton);
        window.draw(speedUpText);
        window.draw(speedDownButton);
        window.draw(speedDownText);
        window.draw(iterationText);
        window.draw(endReasonText);
        window.draw(saveButton);
        window.draw(saveButtonText);
        window.draw(stateButton1);
        window.draw(stateButtonText1);
        window.draw(stateButton2);
        window.draw(stateButtonText2);
        window.draw(stateButton3);
        window.draw(stateButtonText3);
        window.draw(selectedStateText);

        window.display();
    }
}