#include "UI.hpp"

UI::UI() : settings(100, "Standard") {}

int UI::renderBegin() {
    std::cout << "=== Game of Life ===\n";
    std::cout << "1. Mode Console\n";
    std::cout << "2. Mode Graphique (GUI)\n";
    std::cout << "3. Quitter\n";
    std::cout << "Choisissez une option : ";
    int choice;
    std::cin >> choice;
    return choice;
}

void UI::runSimulation(std::unique_ptr<Grid>& grid) {
    if (!grid) {
        std::cerr << "Aucune grille chargee. Simulation impossible.\n";
        return;
    }

    LifeAlgorithm algorithm(grid.get());
    for (int i = 0; i < settings.getMaxIterations(); ++i) {
        std::cout << "Iteration " << i + 1 << "...\n";
        auto toggledCells = algorithm.computeCellsToToggle();
        if (toggledCells.empty()) {
            std::cout << "Simulation stabilisee apres " << i + 1 << " iterations.\n";
            break;
        }
        algorithm.toggleCells(toggledCells);
        grid->printGrid();
    }

    std::cout << "Simulation terminee.\n";
}