#include "Parameters.hpp"

// Classe ErrorHandler
void ErrorHandler::showError(const string& message) {
    cerr << "Error: " << message << endl;
}

void ErrorHandler::showError(const string& message, RenderWindow* window) {
    if (!window) {
        showError(message);
        return;
    }

    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Error: Unable to load font to display the error message." << endl;
        return;
    }

    Text errorText;
    errorText.setFont(font);
    errorText.setString("Error: " + message);
    errorText.setCharacterSize(20);
    errorText.setFillColor(Color::Red);
    errorText.setPosition(10.f, 10.f);

    window->clear(Color::White);
    window->draw(errorText);
    window->display();

    Event event;
    while (window->waitEvent(event)) {
        if (event.type == Event::Closed || event.type == Event::KeyPressed) {
            window->close();
            return;
        }
    }
}

// Classe GameSettings
GameSettings::GameSettings(int maxIterations, const string& gridType)
    : maxIterations(maxIterations), gridType(gridType) {}

int GameSettings::getMaxIterations() const {
    return maxIterations;
}

void GameSettings::setMaxIterations(int iterations) {
    maxIterations = iterations;
}

const string& GameSettings::getGridType() const {
    return gridType;
}

void GameSettings::setGridType(const string& type) {
    gridType = type;
}

// Classe FileHandler
string FileHandler::openFileDialog() {
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
        return string(filename);
    }
    return "";
}

string FileHandler::saveFileDialog() {
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
        return string(filename);
    }
    return "";
}

std::unique_ptr<Grid> FileHandler::loadGridFromFile(const std::string& path, const GameSettings& settings) {
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier : " + path);
    }

    int rows, cols;
    file >> rows >> cols;

    auto grid = GridFactory::createGrid(settings.getGridType(), rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int state;
            file >> state;
            if (state == 1) {
                grid->setCell(i, j, "Standard");
                grid->getCell(i, j)->setAlive(true);
            } else if (state == 2) {
                grid->setCell(i, j, "Obstacle");
            } else {
                grid->setCell(i, j, "Standard");
            }
        }
    }

    file.close();
    return grid;
}

void FileHandler::saveGridToFile(const Grid& grid, const string& path) {
    ofstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier pour la sauvegarde : " + path);
    }

    file << grid.getN() << " " << grid.getP() << "\n";
    for (int i = 0; i < grid.getN(); ++i) {
        for (int j = 0; j < grid.getP(); ++j) {
            Cell* cell = grid.getCell(i, j);
            if (dynamic_cast<ObstacleCell*>(cell)) {
                file << "2 ";
            } else if (cell->isAlive()) {
                file << "1 ";
            } else {
                file << "0 ";
            }
        }
        file << "\n";
    }

    file.close();
}

void FileHandler::saveSimulationHistory(
    const string& path,
    const vector<vector<vector<int>>>& iterationData
) {
    ofstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier pour la sauvegarde : " + path);
    }

    for (size_t iteration = 0; iteration < iterationData.size(); ++iteration) {
        file << "Iteration: " << iteration + 1 << "\n";
        const auto& gridState = iterationData[iteration];

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
