#include "UI.hpp"

UI::UI() : settings(100, "Standard") {}

int UI::renderBegin() {
    int choice;
    bool valid = false;
    while (!valid) {
        cout << "=== Game of Life ===\n";
        cout << "1. Mode Console\n";
        cout << "2. Mode Graphique (GUI)\n";
        cout << "3. Quitter\n";
        cout << "Choisissez une option : ";
        cin >> choice;
        if (choice == 1 || choice == 2 || choice == 3 || choice == 4) {
            valid = true;
        }
    }
    return choice;
}

void UI::runSimulation(unique_ptr<Grid>& grid) {
    if (!grid) {
        cerr << "Aucune grille chargee. Simulation impossible.\n";
        return;
    }

    LifeAlgorithm algorithm(grid.get());
    for (int i = 0; i < settings.getMaxIterations(); ++i) {
        cout << "Iteration " << i + 1 << "...\n";
        auto toggledCells = algorithm.computeCellsToToggle();
        if (toggledCells.empty()) {
            cout << "Simulation stabilisee apres " << i + 1 << " iterations.\n";
            break;
        }
        algorithm.toggleCells(toggledCells);
        grid->printGrid();
    }

    cout << "Simulation terminee.\n";
}