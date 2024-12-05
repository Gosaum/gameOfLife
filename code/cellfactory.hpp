#pragma once

#include <string>
using namespace std;

#include "cell.hpp"
#include "standardcell.hpp"
#include "obstaclecell.hpp"

class CellFactory {

    public:

    static Cell* createCell(const string& cellType, int x, int y);
};