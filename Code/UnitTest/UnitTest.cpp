#include "UnitTest.hpp"

UnitTest::UnitTest() : totalTests(0), successfulTests(0) {}

void UnitTest::runUnitTests() {
    // Test 1 : Voisinage de Moore dans une grille Standard
    addTest("StandardGrid - Interaction du voisinage Moore", []() {
        StandardGrid grid(5, 5);

        // Configuration initiale des cellules
        grid.setCell(2, 2, "Standard");
        grid.setCell(2, 3, "Standard");
        grid.setCell(2, 4, "Standard");

        grid.getCell(2, 2)->toggleAlive();
        grid.getCell(2, 3)->toggleAlive();
        grid.getCell(2, 4)->toggleAlive();

        // Calcul des voisins
        auto neighbors = grid.mooreNeighborhood(grid.getCell(2, 3));

        int aliveCount = 0;
        for (Cell* neighbor : neighbors) {
            if (neighbor && neighbor->isAlive()) {
                ++aliveCount;
            }
        }

        cout << "Nombre de voisins vivants pour la cellule (2, 3) : " << aliveCount << "\n";
        if (aliveCount != 2) {
            throw runtime_error("Moore neighborhood count is incorrect for StandardGrid.");
        }
    });

    // Test 2 : Interaction avec les bordures dans une grille Toroidal
    addTest("ToroidalGrid - Interaction avec les bordures", []() {
        ToroidalGrid grid(3, 3);

        // Configuration initiale des cellules sur les bordures
        grid.setCell(0, 0, "Standard");
        grid.setCell(0, 2, "Standard");
        grid.setCell(2, 0, "Standard");

        grid.getCell(0, 0)->toggleAlive();
        grid.getCell(0, 2)->toggleAlive();
        grid.getCell(2, 0)->toggleAlive();

        // Calcul des voisins
        auto neighbors = grid.mooreNeighborhood(grid.getCell(0, 0));

        int aliveCount = 0;
        for (Cell* neighbor : neighbors) {
            if (neighbor && neighbor->isAlive()) {
                ++aliveCount;
            }
        }

        cout << "Nombre de voisins vivants pour la cellule (0, 0) dans ToroidalGrid : " << aliveCount << "\n";
        if (aliveCount != 2) {
            throw runtime_error("Toroidal neighborhood count is incorrect.");
        }
    });

    // Test 3 : Algorithme de la vie avec un motif stable (Block) dans une grille Standard
    addTest("LifeAlgorithm - Motif stable", []() {
        StandardGrid grid(4, 4);

        // Configuration du motif stable (Block)
        grid.setCell(1, 1, "Standard");
        grid.setCell(1, 2, "Standard");
        grid.setCell(2, 1, "Standard");
        grid.setCell(2, 2, "Standard");

        grid.getCell(1, 1)->toggleAlive();
        grid.getCell(1, 2)->toggleAlive();
        grid.getCell(2, 1)->toggleAlive();
        grid.getCell(2, 2)->toggleAlive();

        // Simulation de l'algorithme
        LifeAlgorithm algorithm(&grid);

        auto toggledCells = algorithm.computeCellsToToggle();
        if (!toggledCells.empty()) {
            throw runtime_error("Stable pattern (Block) should not change.");
        }
    });

    // Test 4 : Algorithme de la vie avec un oscillateur (Blinker) dans une grille Standard
    addTest("LifeAlgorithm - Oscillateur (Blinker)", []() {
        StandardGrid grid(5, 5);

        // Configuration de l'oscillateur
        grid.setCell(2, 1, "Standard");
        grid.setCell(2, 2, "Standard");
        grid.setCell(2, 3, "Standard");

        grid.getCell(2, 1)->toggleAlive();
        grid.getCell(2, 2)->toggleAlive();
        grid.getCell(2, 3)->toggleAlive();

        LifeAlgorithm algorithm(&grid);

        // Affichage de l'état initial de la grille
        cout << "Etat initial de la grille (Blinker) :\n";
        grid.printGridCLI();

        // Premiere iteration
        auto toggledCells = algorithm.computeCellsToToggle();
        cout << "Cellules a basculer lors de la premiere iteration : " << toggledCells.size() << "\n";
        for (Cell* cell : toggledCells) {
            if (cell) {
                cout << "Cellule (" << cell->getX() << ", " << cell->getY() << ") va changer d'etat.\n";
            } else {
                throw runtime_error("Une cellule invalide a ete detectee lors de la premiere iteration.");
            }
        }

        algorithm.toggleCells(toggledCells);
        cout << "Etat de la grille apres la premiere iteration :\n";
        grid.printGridCLI();

        // Deuxieme iteration
        toggledCells = algorithm.computeCellsToToggle();
        cout << "Cellules a basculer lors de la deuxieme iteration : " << toggledCells.size() << "\n";
        for (Cell* cell : toggledCells) {
            if (cell) {
                cout << "Cellule (" << cell->getX() << ", " << cell->getY() << ") va changer d'etat.\n";
            } else {
                throw runtime_error("Une cellule invalide a ete detectee lors de la deuxieme iteration.");
            }
        }

        algorithm.toggleCells(toggledCells);
        cout << "Etat de la grille apres la deuxieme iteration :\n";
        grid.printGridCLI();

        // Validation de l'etat final
        if (!grid.getCell(2, 1)->isAlive() || !grid.getCell(2, 2)->isAlive() || !grid.getCell(2, 3)->isAlive()) {
            throw runtime_error("Oscillator (Blinker) second phase is incorrect.");
        }
    });

    // Execution de tous les tests
    executeTests();
}

void UnitTest::addTest(const string& testName, const function<void()>& testFunction) {
    tests.push_back({testName, testFunction});
}

void UnitTest::executeTests() const {
    for (const auto& test : tests) {
        cout << "[EN COURS] " << test.name << "\n";
        try {
            test.function();
            verifyCondition(test.name, true);
        } catch (const exception& e) {
            verifyCondition(test.name, false);
            cerr << "[ERREUR] " << e.what() << "\n";
        }
    }

    cout << "\n[RESULTATS] " << successfulTests << " / " << totalTests << " tests reussis.\n";
}

void UnitTest::verifyCondition(const string& testName, bool condition) const {
    ++totalTests;
    if (condition) {
        ++successfulTests;
        cout << "[REUSSI] " << testName << "\n";
    } else {
        cerr << "[ECHEC] " << testName << "\n";
    }
}