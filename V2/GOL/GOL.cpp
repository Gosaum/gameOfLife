#include "GOL.hpp"

void GOL::run(){
    try {
        UI ui;
        int choice = ui.renderBegin();

        if (choice == 1) {
            CLI cli;
            cli.runConsoleMode();
        }
        else if (choice == 2) {
            GUI gui;
            gui.runGUI();
        }
        else if (choice == 3) {
            cout << "Au revoir !" << endl;
        }
        else {
            ErrorHandler::showError("Option invalide. Veuillez redemarrer.");
        }
    }
    catch (const exception& e) {
        ErrorHandler::showError(e.what());
    }

    cout << endl << "Appuyez sur ENTREE pour quitter.";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return ;
}