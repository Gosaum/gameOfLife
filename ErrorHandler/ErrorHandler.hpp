#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

class ErrorHandler {
public:
    static void showError(const std::string& message);
    static void showError(const std::string& message, sf::RenderWindow* window);
};

#endif