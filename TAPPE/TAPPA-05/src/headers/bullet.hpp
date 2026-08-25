#pragma once
#include <SFML/Graphics.hpp>
#include <cmath> 

class Bullet {
public:
    Bullet(sf::Vector2f position, float rotationAngle);

    void update(sf::Time deltaTime, float gameWidth, float gameHeight);
    void draw(sf::RenderWindow& window) const;
    bool isDead() const { return m_isDead; }

private:
    sf::CircleShape m_shape;
    sf::Vector2f    m_velocity;
    float           m_lifetime{0.f};
    float           m_maxLifetime{1.2f}; // Scompare dopo 1.2 secondi
    bool            m_isDead{false};
};