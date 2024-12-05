#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <string>
#include <fstream>
#include <stdexcept>
#include "Grid.hpp"

class FileHandler {
public:
    static std::string openFileDialog();
    static std::string saveFileDialog();
    static std::unique_ptr<Grid> loadGridFromFile(const std::string& path, const std::string& type);
    static void saveGridToFile(const Grid& grid, const std::string& path);
};

#endif