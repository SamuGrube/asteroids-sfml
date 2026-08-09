#pragma once //Viene incluso una sola volta per file di compilazione
#include <SFML/Graphics.hpp>
#include "headers/asteroid.hpp"

class Game {
public:
    Game();      // Costruttore: inizializza la finestra e la navicella
    void run();  // Fa partire il ciclo principale

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    // Variabili membro (iniziano con m_ per distinguerle dalle variabili locali)
    sf::RenderWindow m_window;
    sf::ConvexShape  m_ship;
    sf::View m_view; // Vista logica di gioco (1024x768) per gestire il ridimensionamento della finestra

    const float GAME_WIDTH = 1024.f;
    const float GAME_HEIGHT = 768.f;

    void updateViewport(unsigned int width, unsigned int height);

    // --- FISICA TAPPA 02 ---
    sf::Vector2f m_velocity{0.f, 0.f}; // Vettore velocità corrente (px/s)
    float m_rotationSpeed = 200.f; // Gradi al secondo
    float m_acceleration = 300.f; // Pixel al secondo^2
    float m_drag = 0.985f; //Attrito per rallentare gradualmente la navicella quando non si preme alcun tasto

    // --- NUOVO METODO TAPPA 03 ---
    void handleScreenWrapping();

    // --- NUOVO METODO TAPPA 04 ---
    void spawnAsteroids(std::size_t count);

    std::vector m_asteroids; // Contenitore per gli asteroidi
};