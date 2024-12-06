#include "Parameters.hpp"
#include <windows.h>
#include <commdlg.h>

std::string FileHandler::openFileDialog() {
    char filename[MAX_PATH] = { 0 };
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return std::string(filename);
    }
    return "";
}

std::string FileHandler::saveFileDialog() {
    char filename[MAX_PATH] = { 0 };
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn)) {
        return std::string(filename);
    }
    return "";
}

std::unique_ptr<Grid> FileHandler::loadGridFromFile(const std::string& path, const std::string& type) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + path);
    }

    int rows, cols;
    file >> rows >> cols;

    auto grid = GridFactory::createGrid(type, rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int state;
            file >> state;
            if (state == 1) {
                grid->setCell(i, j, "Standard");
                grid->getCell(i, j)->setAlive(true);
            }
            else if (state == 2) {
                grid->setCell(i, j, "Obstacle");
            }
            else {
                grid->setCell(i, j, "Standard");
            }
        }
    }

    file.close();
    return grid;
}

void FileHandler::saveGridToFile(const Grid& grid, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier pour la sauvegarde : " + path);
    }

    file << grid.getN() << " " << grid.getP() << "\n";
    for (int i = 0; i < grid.getN(); ++i) {
        for (int j = 0; j < grid.getP(); ++j) {
            Cell* cell = grid.getCell(i, j);
            if (dynamic_cast<ObstacleCell*>(cell)) {
                file << "2 ";
            }
            else if (cell->isAlive()) {
                file << "1 ";
            }
            else {
                file << "0 ";
            }
        }
        file << "\n";
    }

    file.close();
}

static void FileHandler::saveSimulationHistory(
    const string& path,
    const vector<pair<int, vector<vector<string>>>>& simulationHistory
) {
    ofstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier pour la sauvegarde : " + path);
    }

    for (const auto& iterationData : simulationHistory) {
        int iteration = iterationData.first;
        const auto& gridState = iterationData.second;

        file << "Iteration: " << iteration << "\n";
        for (const auto& row : gridState) {
            for (const auto& cell : row) {
                file << cell << " ";
            }
            file << "\n";
        }
        file << "\n";
    }

    file.close();
}

void ErrorHandler::showError(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
}

void ErrorHandler::showError(const std::string& message, sf::RenderWindow* window) {
    if (!window) {
        showError(message);
        return;
    }

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error: Unable to load font to display the error message." << std::endl;
        return;
    }

    sf::Text errorText;
    errorText.setFont(font);
    errorText.setString("Error: " + message);
    errorText.setCharacterSize(20);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(10.f, 10.f);

    window->clear(sf::Color::White);
    window->draw(errorText);
    window->display();

    sf::Event event;
    while (window->waitEvent(event)) {
        if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed) {
            window->close();
            return;
        }
    }
}

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
