#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>

class Particle {
public:
    Particle(sf::Vector2f position, sf::Vector2f velocity, sf::Time lifetime, sf::Color color = sf::Color::White);

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool isDead() const { return m_lifetime <= sf::Time::Zero; }

private:
    sf::CircleShape m_shape;
    sf::Vector2f    m_velocity;
    sf::Time        m_lifetime;
    sf::Time        m_maxLifetime;
    sf::Color       m_color;
};