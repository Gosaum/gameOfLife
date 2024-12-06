#include "UnitTest.hpp"
#include <limits>

using namespace std;

void UnitTest::ajouterTest(const string& nomTest, const function<void()>& fonctionTest) {
    tests.push_back({nomTest, fonctionTest});
}

void UnitTest::executerTests() const {
    nombreTests = 0;
    nombreReussis = 0;

    for (const auto& test : tests) {
        cout << "[EN COURS] " << test.nom << "\n";
        try {
            test.fonction();
            verifierCondition(test.nom, true);
        } catch (const exception& e) {
            verifierCondition(test.nom, false);
            cerr << "[ERREUR] Le test a échoué : " << e.what() << "\n";
        }
    }

    cout << "\n[RESULTAT] " << nombreReussis << " / " << nombreTests << " tests réussis.\n";
}

void UnitTest::verifierCondition(const string& nomTest, bool condition) const {
    ++nombreTests;
    if (condition) {
        ++nombreReussis;
        cout << "[REUSSI] " << nomTest << "\n";
    } else {
        cerr << "[ECHEC] " << nomTest << "\n";
    }
}

void UnitTest::runUnitTests() {
    int rows, cols;

    cout << "Entrez le nombre de lignes pour la grille : ";
    cin >> rows;

    cout << "Entrez le nombre de colonnes pour la grille : ";
    cin >> cols;

    cout << "\nAjout des tests pour une grille de " << rows << "x" << cols << ".\n";

    // Test 1 : Création d'une grille StandardGrid
    ajouterTest("Création d'une grille StandardGrid", [rows, cols]() {
        StandardGrid grille(rows, cols);
        if (grille.getN() != rows || grille.getP() != cols) {
            throw runtime_error("Les dimensions de la grille StandardGrid sont incorrectes !");
        }
    });

    // Test 2 : Création d'une grille ToroidalGrid
    ajouterTest("Création d'une grille ToroidalGrid", [rows, cols]() {
        ToroidalGrid grille(rows, cols);
        if (grille.getN() != rows || grille.getP() != cols) {
            throw runtime_error("Les dimensions de la grille ToroidalGrid sont incorrectes !");
        }
    });

    // Test 3 : Vérification des voisins (Moore Neighborhood)
    ajouterTest("Voisinage Moore (StandardGrid)", [rows, cols]() {
        StandardGrid grille(rows, cols);
        int x, y;
        cout << "Entrez les coordonnées x et y pour tester le voisinage : ";
        cin >> x >> y;

        if (x < 0 || x >= rows || y < 0 || y >= cols) {
            throw runtime_error("Coordonnées hors limites !");
        }

        grille.setCell(x, y, "Standard");
        auto voisins = grille.mooreNeighborhood(grille.getCell(x, y));

        cout << "Nombre de voisins trouvés : " << voisins.size() << "\n";
        if (voisins.size() != 8 && x > 0 && x < rows - 1 && y > 0 && y < cols - 1) {
            throw runtime_error("Nombre de voisins incorrect !");
        }
    });

    // Test 4 : Simulation de l'algorithme de la vie
    ajouterTest("Simulation de l'algorithme de la vie (StandardGrid)", [rows, cols]() {
        StandardGrid grille(rows, cols);
        grille.setCell(1, 1, "Standard");
        grille.setCell(1, 2, "Standard");
        grille.setCell(1, 3, "Standard");

        grille.getCell(1, 1)->toggleAlive();
        grille.getCell(1, 2)->toggleAlive();
        grille.getCell(1, 3)->toggleAlive();

        LifeAlgorithm algo(&grille);
        auto cellulesModifiees = algo.computeCellsToToggle();
        algo.toggleCells(cellulesModifiees);

        if (!grille.getCell(0, 2)->isAlive() ||
            !grille.getCell(1, 2)->isAlive() ||
            !grille.getCell(2, 2)->isAlive()) {
            throw runtime_error("L'algorithme a échoué !");
            cout << endl << "Appuyez sur ENTREE pour quitter.";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    });

    cout << "\nLancement des tests...\n";
    executerTests();
    cout << endl << "Appuyez sur ENTREE pour quitter.";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}