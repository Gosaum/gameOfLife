#include "CLI.hpp"

CLI::CLI() : settings(100, "Standard") {}

void CLI::runConsoleMode() {
    std::cout << "=== Mode Console ===\n";

    try {
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
            if (toggledCells.empty()) {
                std::cout << "Simulation stabilisee apres " << iteration + 1 << " iterations.\n";
                break;
            }

            algorithm.toggleCells(toggledCells);
            grid->printGrid();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
}