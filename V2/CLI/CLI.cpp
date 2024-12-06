#include "CLI.hpp"

CLI::CLI() : settings(100, "Standard") {}

void CLI::runConsoleMode() {
    std::cout << "=== Mode Console ===\n";

    try {
        // Chargement du fichier
        std::string filePath = FileHandler::openFileDialog();
        if (filePath.empty()) {
            std::cout << "Aucun fichier selectionne. Abandon.\n";
            return;
        }

        vector<pair<int, vector<vector<std::string>>>> iterationData;
        auto grid = FileHandler::loadGridFromFile(filePath, settings.getGridType());

        // Configuration des paramètres
        int maxIterations;
        std::cout << "Nombre maximum d'iterations (1-10000) : ";
        std::cin >> maxIterations;
        settings.setMaxIterations(maxIterations);

        std::cout << "Grille torique ? (Y/N) : ";
        char toroidalChoice;
        std::cin >> toroidalChoice;
        settings.setGridType((toroidalChoice == 'Y' || toroidalChoice == 'y') ? "Toroidal" : "Standard");

        // Simulation avec détection de boucle
        LifeAlgorithm algorithm(grid.get());
        std::unordered_set<std::string> previousStates;
        bool simulationEnded = false;

        for (int iteration = 0; iteration < settings.getMaxIterations(); ++iteration) {
            std::cout << "Iteration " << iteration + 1 << "...\n";

            // Calcul du hash pour l'état actuel de la grille
            std::string currentHash = algorithm.hashGrid(*grid);
            if (previousStates.find(currentHash) != previousStates.end()) {
                std::cout << "Simulation terminee : grille en boucle apres " << iteration + 1 << " iterations.\n";
                simulationEnded = true;
                break;
            }

            previousStates.insert(currentHash);

            // Sauvegarde de l'état actuel
            vector<vector<std::string>> currentState(grid->getN(), vector<std::string>(grid->getP()));
            for (int i = 0; i < grid->getN(); ++i) {
                for (int j = 0; j < grid->getP(); ++j) {
                    Cell* cell = grid->getCell(i, j);
                    currentState[i][j] = (cell && cell->isAlive()) ? "1" : "0";
                }
            }
            iterationData.emplace_back(iteration + 1, currentState);

            // Calcul des cellules à basculer
            auto toggledCells = algorithm.computeCellsToToggle();
            if (toggledCells.empty()) {
                std::cout << "Simulation stabilisee apres " << iteration + 1 << " iterations.\n";
                simulationEnded = true;
                break;
            }

            // Application des modifications
            algorithm.toggleCells(toggledCells);

            // Affichage de la grille
            grid->printGridCLI();
        }

        if (!simulationEnded) {
            std::cout << "Simulation terminee : limite d'iterations atteinte.\n";
        }

        // Sauvegarde des itérations
        std::cout << "Voulez-vous sauvegarder toutes les iterations ? (Y/N) : ";
        char saveChoice;
        std::cin >> saveChoice;
        if (saveChoice == 'Y' || saveChoice == 'y') {
            std::string savePath = FileHandler::saveFileDialog();
            if (!savePath.empty()) {
                FileHandler::saveSimulationHistory(savePath, iterationData);
                std::cout << "Toutes les iterations sauvegardees dans " << savePath << ".\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
}