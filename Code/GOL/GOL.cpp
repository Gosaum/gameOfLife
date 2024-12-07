#include "GOL.hpp"

void GOL::run() {

    UI ui;
    int choice;
    bool valid = false;

    try {
        while (!valid) {
            choice = ui.renderBegin();

            if (choice == 1) {
                CLI cli;
                cli.runConsoleMode();
                valid  = true;
            }
            else if (choice == 2) {
                GUI gui;
                gui.runGUI();
                valid  = true;
            }
            else if (choice == 3) {
                cout << "Au revoir !" << endl;
                valid  = true;
            }
            else if (choice == 4) {
                cout << "Mode Test Unitaire selectionne.\n";
                UnitTest test;
                test.run();
            }
            else {
                ErrorHandler::showError("Option invalide. Veuillez redemarrer.");
            }
        }
    }
    catch (const exception& e) {
        ErrorHandler::showError(e.what());
    }

    cout << endl << "Appuyez sur ENTREE pour quitter.";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}