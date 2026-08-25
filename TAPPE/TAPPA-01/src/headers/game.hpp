#pragma once
#include <SFML/Graphics.hpp>

class Game {
public:
    Game();      // Costruttore: inizializza la finestra e la navicella
    void run();  // Fa partire il ciclo principale

private:
    void processEvents();
    void update(sf::Time deltaTime); // Prepararsi a usare il tempo per la fisica
    void render();

    // Variabili membro (iniziano con m_ per distinguerle dalle variabili locali)
    sf::RenderWindow m_window;
    sf::ConvexShape  m_ship;
    sf::View m_view;

    const float GAME_WIDTH = 1024.f;
    const float GAME_HEIGHT = 768.f;

    void updateViewport(unsigned int width, unsigned int height);
};