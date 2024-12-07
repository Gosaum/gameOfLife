#pragma once
#include <iostream>
#include <unordered_set>
#include "LifeAlgorithm.hpp"
#include "Grid.hpp"
#include "Parameters.hpp"

class UnitTest {
public:
    static void run();
private:
    static void assertEqual(bool condition, const std::string& message);
};