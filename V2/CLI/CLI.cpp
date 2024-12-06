#include "CLI.hpp"

CLI::CLI() : settings(100, "Standard") {}

void CLI::runConsoleMode() {
    std::cout << "=== Mode Console ===\n";

    try {
        vector<pair<int, vector<vector<string>>>> iterationData;
        std::string filePath = FileHandler::openFileDialog();
        if (filePath.empty()) {
            std::cout << "Aucun fichier selectionne. Abandon.\n";
            return;
        }

        auto grid = FileHandler::loadGridFromFile(filePath, settings.getGridType());

        int maxIterations;
        std::cout << "Nombre maximum d'iterations (1-10000) : ";
        std::cin >> maxIterations;
        settings.setMaxIterations(maxIterations);

        std::cout << "Grille torique ? (Y/N) : ";
        char toroidalChoice;
        std::cin >> toroidalChoice;
        settings.setGridType((toroidalChoice == 'Y' || toroidalChoice == 'y') ? "Toroidal" : "Standard");

        LifeAlgorithm algorithm(grid.get());
        for (int iteration = 0; iteration < settings.getMaxIterations(); ++iteration) {
            std::cout << "Iteration " << iteration + 1 << "...\n";

            auto toggledCells = algorithm.computeCellsToToggle();
            if (LifeAlgorithm(grid).isGridStable()) {
                std::cout << "Simulation stabilisee apres " << iteration + 1 << " iterations.\n";
                break;
            }

            if (LifeAlgorithm(grid).isGridLooping(4)) {
                std::cout << "Simulation stabilisee apres " << iteration + 1 << " iterations.\n";
                break;
            }

            algorithm.toggleCells(toggledCells);

            vector<vector<string>> gridState(grid->getN(), vector<string>(grid->getP()));
            for (int x = 0; x < grid->getN(); ++x) {
                for (int y = 0; y < grid->getP(); ++y) {
                    auto cell = grid->getCell(x, y);
                    if (dynamic_cast<ObstacleCell*>(cell)) {
                        gridState[x][y] = "O";
                    }
                    else if (cell->isAlive()) {
                        gridState[x][y] = "1";
                    }
                    else {
                        gridState[x][y] = "0";
                    }
                }
            }

            iterationData.emplace_back(iteration + 1, gridState);
            grid->printGrid();
        }

        cout << "Voulez-vous sauvegarder toutes les iterations ? (Y/N) : ";
        char saveChoice;
        cin >> saveChoice;
        if (saveChoice == 'Y' || saveChoice == 'y') {
            string savePath = FileHandler::saveFileDialog();
            if (!savePath.empty()) {
                FileHandler::saveSimulationHistory(savePath, iterationData);
                cout << "Toutes les iterations sauvegardees dans " << savePath << ".\n";
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
}