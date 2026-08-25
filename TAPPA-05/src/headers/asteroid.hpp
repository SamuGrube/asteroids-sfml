#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>

class Asteroid {
public:
    Asteroid(sf::Vector2f position, float radius);

    void update(sf::Time deltaTime, float gameWidth, float gameHeight);
    void draw(sf::RenderWindow& window) const;

private:
    void handleScreenWrapping(float gameWidth, float gameHeight);

    sf::ConvexShape m_shape;
    sf::Vector2f    m_velocity;
    float           m_rotationSpeed;
    float           m_radius;
};