#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>

class Asteroid {
public:
    Asteroid(sf::Vector2f position, float radius);

    void update(sf::Time deltaTime, float gameWidth, float gameHeight);
    void draw(sf::RenderWindow& window) const;

    // --- NUOVI METODI TAPPA 06 ---
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    float getRadius() const { return m_radius; }
    bool isDead() const { return m_isDead; }
    void setDead(bool dead) { m_isDead = dead; }

private:
    void handleScreenWrapping(float gameWidth, float gameHeight);

    sf::ConvexShape m_shape;
    sf::Vector2f    m_velocity;
    float           m_rotationSpeed;
    float           m_radius;
    bool            m_isDead;
};