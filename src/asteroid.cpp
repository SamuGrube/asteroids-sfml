#include "headers/asteroid.hpp"
#include <random>

// Funzione helper per lo screen wrapping universale
inline void applyScreenWrapping(sf::Vector2f& pos, float width, float height, float margin = 0.f) {
    if (pos.x < -margin) {
        pos.x = width + margin;
    } else if (pos.x > width + margin) {
        pos.x = -margin;
    }

    if (pos.y < -margin) {
        pos.y = height + margin;
    } else if (pos.y > height + margin) {
        pos.y = -margin;
    }
}

Asteroid::Asteroid(sf::Vector2f position, float radius, float speedMultiplier)
    : m_radius(radius),
      m_isDead(false)
{
    // Generatore di numeri casuali
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    std::uniform_real_distribution angleDist(0.f, 2.f * std::numbers::pi_v<float>);
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
        float a = (static_cast<float>(i) / static_cast<float>(points)) * 2.f * std::numbers::pi_v<float>;
        float r = m_radius * radiusJitter(gen);
        m_shape.setPoint(i, sf::Vector2f(std::cos(a) * r, std::sin(a) * r));
    }

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
    applyScreenWrapping(pos, gameWidth, gameHeight, m_radius); // Usa direttamente il raggio
    m_shape.setPosition(pos);
}

void Asteroid::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}

std::vector<sf::Vector2f> Asteroid::getGlobalVertices() const {
    std::vector<sf::Vector2f> globalVerts;
    sf::Transform transform = m_shape.getTransform();
    
    for (std::size_t i = 0; i < m_shape.getPointCount(); ++i) {
        globalVerts.push_back(transform.transformPoint(m_shape.getPoint(i)));
    }
    return globalVerts;
}

void Asteroid::setPosition(sf::Vector2f newPos) {
    m_shape.setPosition(newPos);
}

void Asteroid::deflect() {
    // 1. Calcola la velocità attuale
    float currentSpeed = std::hypot(m_velocity.x, m_velocity.y);
    
    // 2. Calcola l'angolo attuale di movimento in radianti
    float currentAngle = std::atan2(m_velocity.y, m_velocity.x);
    
    // 3. Genera una deviazione casuale (es. tra 45° e 135°)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    // 0.785 è circa 45 gradi, 2.356 è circa 135 gradi in radianti
    std::uniform_real_distribution<float> angleDist(0.785f, 2.356f); 
    std::uniform_int_distribution<int> signDist(0, 1);
    
    // Applica deviazione positiva (senso orario) o negativa (antiorario)
    float randomOffset = angleDist(gen);
    if (signDist(gen) == 0) randomOffset = -randomOffset;
    
    float newAngle = currentAngle + randomOffset;
    
    // 4. Riapplica la nuova direzione, mantenendo la velocità originale
    m_velocity.x = std::cos(newAngle) * currentSpeed;
    m_velocity.y = std::sin(newAngle) * currentSpeed;
}