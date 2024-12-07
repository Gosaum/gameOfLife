#include "UnitTest.hpp"

void UnitTest::run() {
    std::cout << "Debut des tests unitaires...\n";

    try {
        // Création d'une grille 3x3 en mémoire
        GameSettings settings(100, "Standard");
        auto grid = GridFactory::createGrid(settings.getGridType(), 3, 3);

        // Initialisation des cellules dans la grille
        grid->setCell(0, 0, "Standard");
        grid->getCell(0, 0)->setAlive(true);

        grid->setCell(0, 1, "Standard");

        grid->setCell(0, 2, "Standard");
        grid->getCell(0, 2)->setAlive(true);

        grid->setCell(1, 0, "Standard");
        grid->getCell(1, 0)->setAlive(true);

        grid->setCell(1, 1, "Standard");
        grid->getCell(1, 1)->setAlive(true);

        grid->setCell(1, 2, "Standard");

        grid->setCell(2, 0, "Standard");

        grid->setCell(2, 1, "Standard");
        grid->getCell(2, 1)->setAlive(true);

        grid->setCell(2, 2, "Standard");

        // Afficher la grille initiale
        std::cout << "Grille initiale :\n";
        grid->printGridCLI();

        // Création de l'algorithme
        LifeAlgorithm algorithm(grid.get());

        // Vérification du hash initial
        std::string initialHash = algorithm.hashGrid(*grid);
        assertEqual(!initialHash.empty(), "Le hash initial de la grille ne doit pas etre vide.");

        // Effectuer une itération de simulation
        auto toggledCells = algorithm.computeCellsToToggle();
        assertEqual(!toggledCells.empty(), "Il doit y avoir des cellules a basculer pour cette grille.");

        algorithm.toggleCells(toggledCells);

        // Vérification que le hash de la grille change après une itération
        std::string newHash = algorithm.hashGrid(*grid);
        assertEqual(initialHash != newHash, "Le hash de la grille doit changer apres une iteration.");

        // Afficher la grille après une itération
        std::cout << "Grille apres une iteration :\n";
        grid->printGridCLI();

        std::cout << "Test de simulation d'une iteration termine avec succes.\n";
        std::cout << "Tous les tests ont reussi !\n";
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << "\n";
        exit(1);
    }
}

void UnitTest::assertEqual(bool condition, const std::string& message) {
    if (condition) {
        std::cout << "[SUCCESS] " << message << "\n";
    } else {
        std::cerr << "[FAIL] " << message << "\n";
        exit(1);
    }
}