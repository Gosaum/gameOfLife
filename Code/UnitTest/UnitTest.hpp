#pragma once

#include "Grid.hpp"
#include "LifeAlgorithm.hpp"
#include <stdexcept>
#include <iostream>
#include <functional>

using namespace std;

class UnitTest {
public:
    UnitTest();

    void runUnitTests();

private:
    struct Test {
        string name;
        function<void()> function;
    };

    vector<Test> tests;
    mutable int totalTests;
    mutable int successfulTests;

    void addTest(const string& testName, const function<void()>& testFunction);
    void executeTests() const;
    void verifyCondition(const string& testName, bool condition) const;
};