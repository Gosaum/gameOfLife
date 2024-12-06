#include "Parameters.hpp"
#include "Grid.hpp"
#include "LifeAlgorithm.hpp"
#include "CLI.hpp"
#include "GUI.hpp"
#include <iostream>

int main() {
    bool valid = false;
    while (!valid) { 
        std::cout << "=== Game of Life ===" << std::endl;
        std::cout << "1.Interface Graphique\n2.Ligne de Commande\n3.Quitter\nSélectionnez le mode : ";
        int mode;
        std::cin >> mode;

        if (mode == 1) {
            // Mode Interface Graphique
            GUI gui;
            gui.run();
            valid = true;
        } else if (mode == 2) {
            // Mode Ligne de Commande
            CLI cli;
            cli.runConsoleMode();
            valid = true;
        } else if (mode == 3) {
            std::cout << "Quitter l'application. Au revoir!" << std::endl;
            valid = true;
        } else {
            std::cerr << "Mode invalide. Réessayez." << std::endl;
        }
    }
    return 0;
}