#include "ErrorHandler.hpp"

void ErrorHandler::showError(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
}

void ErrorHandler::showError(const std::string& message, sf::RenderWindow* window) {
    if (!window) {
        showError(message);
        return;
    }

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error: Unable to load font to display the error message." << std::endl;
        return;
    }

    sf::Text errorText;
    errorText.setFont(font);
    errorText.setString("Error: " + message);
    errorText.setCharacterSize(20);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(10.f, 10.f);

    window->clear(sf::Color::White);
    window->draw(errorText);
    window->display();

    sf::Event event;
    while (window->waitEvent(event)) {
        if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed) {
            window->close();
            return;
        }
    }
}