#pragma once
#include <SFML/Graphics.hpp>
#include <numbers>
#include <cmath>

class Asteroid {
public:
    Asteroid(sf::Vector2f position, float radius, float speedMultiplier = 1.0f);

    void update(sf::Time deltaTime, float gameWidth, float gameHeight);
    void draw(sf::RenderWindow& window) const;
    void setDead(bool dead) { m_isDead = dead; }
    void setPosition(sf::Vector2f newPos);
    void deflect();
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    float getRadius() const { return m_radius; }
    bool isDead() const { return m_isDead; }
    std::vector<sf::Vector2f> getGlobalVertices() const; // Metodo per ottenere i vertici globali dell'asteroide
private:
    void handleScreenWrapping(float gameWidth, float gameHeight);

    sf::ConvexShape m_shape;
    sf::Vector2f    m_velocity;
    float           m_rotationSpeed;
    float           m_radius;
    bool            m_isDead;
};