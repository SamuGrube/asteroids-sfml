#pragma once //Viene incluso una sola volta per file di compilazione
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include "asteroid.hpp"
#include "bullet.hpp"
#include <iostream>

enum class GameState {
    MainMenu,
    Playing,
    GameOver
};

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
    void spawnAsteroids(std::size_t count, float speedMultiplier = 1.0f); // Metodo per generare asteroidi in posizioni casuali

    std::vector<Asteroid> m_asteroids; // Contenitore per gli asteroidi

    // --- PROIETTILI TAPPA 05 ---
    std::vector<Bullet> m_bullets; // Contenitore per i proiettili
    sf::Time m_fireCooldown{sf::seconds(0.2f)}; // Tempo minimo tra due spari consecutivi
    sf::Time m_fireTimer{sf::Time::Zero}; // Timer per gestire il cooldown dello sparo

    // --- VITE E GIOCO TAPPA 07 ---
    int m_lives = 3; // Numero di vite iniziali
    void resetGame(); // Metodo per resettare il gioco dopo la collisione

    // --- NUOVI MEMBRI TAPPA 08 ---
    GameState m_state{GameState::MainMenu};
    int m_score{0};

    sf::Font m_font;
    sf::Text m_uiText;

    // --- NUOVI MEMBRI TAPPA 09 ---
    int m_wave{1}; // Numero dell'onda corrente
    float m_speedMultiplier{1.0f}; // Moltiplicatore di velocità per aumentare la difficoltà

    // --- NUOVI MEMBRI TAPPA 10 ---
    sf::Time m_invulnerabilityTimer{sf::Time::Zero}; // Timer per gestire l'invulnerabilità della navicella dopo la collisione
    const sf::Time m_maxInvulnerabilityTime{sf::seconds(3.f)}; // Durata dell'invulnerabilità
};
