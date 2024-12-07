#include "CLI.hpp"

CLI::CLI() : settings(100, "Standard") {}

void CLI::runConsoleMode() {
    cout << "=== Mode Console ===\n";

    try {
        string filePath = FileHandler::openFileDialog();
        while (filePath.empty()) {
            cout << "Choix invalide. Veuillez reessayer";
            string filePath = FileHandler::openFileDialog();
        }
        cout << "Fichier choisi : " << filePath << endl;
        cout << "1. Standard\n";
        cout << "2. Toroidal\n";
        cout << "Choisissez le type de grille : ";
        int gridChoice;
        cin >> gridChoice;
        while (gridChoice != 1 && gridChoice != 2) {
            cout << "Choix invalide. Veuillez entrer 1 pour Standard ou 2 pour Toroidal : ";
            cin >> gridChoice;
        }

        string gridType = (gridChoice == 1) ? "Standard" : "Toroidal";
        settings.setGridType(gridType);

        auto grid = FileHandler::loadGridFromFile(filePath, settings);
        cout << "Grille de type " << gridType << " creee avec succes.\n";

        LifeAlgorithm algorithm(grid.get());
        unordered_set<string> previousStates;
        bool simulationEnded = false;

        for (int iteration = 0; iteration < settings.getMaxIterations(); ++iteration) {
            cout << "Iteration " << iteration + 1 << "...\n";

            string currentHash = algorithm.hashGrid(*grid);
            if (previousStates.find(currentHash) != previousStates.end()) {
                cout << "Simulation terminee : grille en boucle apres " << iteration + 1 << " iterations.\n";
                simulationEnded = true;
                break;
            }

            previousStates.insert(currentHash);

            auto toggledCells = algorithm.computeCellsToToggle();
            if (toggledCells.empty()) {
                cout << "Simulation stabilisee apres " << iteration + 1 << " iterations.\n";
                simulationEnded = true;
                break;
            }

            algorithm.toggleCells(toggledCells);
            grid->printGridCLI();
        }

        if (!simulationEnded) {
            cout << "Simulation terminee : limite d'iterations atteinte.\n";
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
    } catch (const exception& e) {
        cerr << "Erreur : " << e.what() << endl;
    }
}
