#include "headers/particle.hpp"

Particle::Particle(sf::Vector2f position, sf::Vector2f velocity, sf::Time lifetime, sf::Color color)
    : m_velocity(velocity), m_lifetime(lifetime), m_maxLifetime(lifetime), m_color(color)
{
    m_shape.setRadius(1.5f);
    m_shape.setOrigin({1.5f, 1.5f});
    m_shape.setPosition(position);
    m_shape.setFillColor(color);
}

void Particle::update(sf::Time deltaTime) {
    float dt = deltaTime.asSeconds();
    m_shape.move(m_velocity * dt);
    m_lifetime -= deltaTime;

    // Effetto Fade-Out (sfumatura alpha graduale verso la fine della vita)
    float alphaRatio = m_lifetime.asSeconds() / m_maxLifetime.asSeconds();
    if (alphaRatio < 0.f) alphaRatio = 0.f;

    sf::Color current = m_color;
    current.a = static_cast<std::uint8_t>(255.f * alphaRatio);
    m_shape.setFillColor(current);
}

void Particle::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}