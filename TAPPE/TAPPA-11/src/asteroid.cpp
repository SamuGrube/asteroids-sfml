#include "headers/asteroid.hpp"

Asteroid::Asteroid(sf::Vector2f position, float radius, float speedMultiplier)
    : m_radius(radius),
      m_isDead(false)
{
    // Generatore di numeri casuali
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    std::uniform_real_distribution angleDist(0.f, 2.f * 3.14159f);
    std::uniform_real_distribution speedDist(40.f, 120.f);
    std::uniform_real_distribution rotDist(-60.f, 60.f);
    std::uniform_real_distribution radiusJitter(0.75f, 1.25f);

    // Imposta velocità e rotazione casuali
    float angle = angleDist(gen);
    float speed = speedDist(gen) * speedMultiplier;
    m_velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
    m_rotationSpeed = rotDist(gen) * speedMultiplier;

    // Generazione del poligono irregolare (8 vertici)
    std::size_t points = 8;
    m_shape.setPointCount(points);

    for (std::size_t i = 0; i < points; ++i) {
        float a = (static_cast<float>(i) / static_cast<float>(points)) * 2.f * 3.14159f;
        float r = m_radius * radiusJitter(gen);
        m_shape.setPoint(i, sf::Vector2f(std::cos(a) * r, std::sin(a) * r));
    }

    // Stile retrò
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setOutlineThickness(2.f);
    m_shape.setPosition(position);
}

void Asteroid::update(sf::Time deltaTime, float gameWidth, float gameHeight) {
    float dt = deltaTime.asSeconds();
    
    // Movimento e rotazione
    m_shape.move(m_velocity * dt);
    m_shape.rotate(sf::degrees(m_rotationSpeed * dt));

    handleScreenWrapping(gameWidth, gameHeight);
}

void Asteroid::handleScreenWrapping(float gameWidth, float gameHeight) {
    sf::Vector2f pos = m_shape.getPosition();

    if (pos.x < -m_radius) pos.x = gameWidth + m_radius;
    else if (pos.x > gameWidth + m_radius) pos.x = -m_radius;

    if (pos.y < -m_radius) pos.y = gameHeight + m_radius;
    else if (pos.y > gameHeight + m_radius) pos.y = -m_radius;

    m_shape.setPosition(pos);
}

void Asteroid::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}