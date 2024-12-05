#include "gridfactory.hpp"

Grid* GridFactory::createGrid(const string& gridType) {
    
    if (gridType == "std" || gridType == "standard" || gridType == "Standard") {
        return new StandardGrid ();
    }
    if (gridType == "tor" || gridType == "toroidal" || gridType == "Toroidal") {
        return new ToroidalGrid();
    }
    else {
        return new StandardGrid(); //si le type est inconnu, créé par défaut une grille classique
    }
}

Grid* GridFactory::createGrid(const string& gridType, int n, int p) {
    
    if (gridType == "std" || gridType == "standard" || gridType == "Standard") {
        return new StandardGrid (n,p);
    }
    if (gridType == "tor" || gridType == "toroidal" || gridType == "Toroidal") {
        return new ToroidalGrid(n,p);
    }
    else {
        return new StandardGrid(n,p); //si le type est inconnu, créé par défaut une grille classique
    }
}

Grid* GridFactory::createGrid(const string& gridType, int n, int p, const vector<string>& data) {
    
    if (gridType == "std" || gridType == "standard" || gridType == "Standard") {
        return new StandardGrid (n,p,data);
    }
    if (gridType == "tor" || gridType == "toroidal" || gridType == "Toroidal") {
        return new ToroidalGrid(n,p,data);
    }
    else {
        return new StandardGrid(n,p,data); //si le type est inconnu, créé par défaut une grille classique
    }
}