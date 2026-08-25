#include "headers/bullet.hpp" // Adatta il percorso se usi headers/Bullet.hpp

Bullet::Bullet(sf::Vector2f position, float rotationAngle) {
    m_shape.setRadius(2.f);
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOrigin({2.f, 2.f}); // Centriamo il punto di origine del cerchio
    m_shape.setPosition(position);

    float radians = rotationAngle * 3.14159f / 180.f;
    float speed = 600.f; // Velocità del proiettile (px/s)

    m_velocity = sf::Vector2f(std::sin(radians) * speed, -std::cos(radians) * speed);
}

void Bullet::update(sf::Time deltaTime, float gameWidth, float gameHeight) {
    float dt = deltaTime.asSeconds();
    m_shape.move(m_velocity * dt);

    m_lifetime += dt;
    if (m_lifetime >= m_maxLifetime) {
        m_isDead = true;
    }

    // Se esce dallo schermo lo segniamo come "morto"
    sf::Vector2f pos = m_shape.getPosition();
    if (pos.x < 0.f || pos.x > gameWidth || pos.y < 0.f || pos.y > gameHeight) {
        m_isDead = true;
    }
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}