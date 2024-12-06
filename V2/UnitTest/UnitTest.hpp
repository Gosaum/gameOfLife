#pragma once

#include "Grid.hpp"
#include "LifeAlgorithm.hpp"
#include <stdexcept>
#include <iostream>
#include <functional>

using namespace std;

class UnitTest {
public:
    UnitTest() : nombreTests(0), nombreReussis(0) {}

    void runUnitTests();

private:
    struct Test {
        string nom;
        function<void()> fonction;
    };

    vector<Test> tests;
    mutable int nombreTests;
    mutable int nombreReussis;
    void ajouterTest(const string& nomTest, const function<void()>& fonctionTest);
    void executerTests() const;
    void verifierCondition(const string& nomTest, bool condition) const;
};