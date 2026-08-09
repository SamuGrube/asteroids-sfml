#include "headers/game.hpp"

// Costruttore: inizializza la finestra e prepara la navicella
Game::Game() : m_window(sf::VideoMode({1024, 768}), "Asteroids - Tappa 02", sf::Style::Default) { //Finestra completa di titolo, pulsante di chiusura e ridimensionabile
    m_window.setFramerateLimit(60);

    //Impostiamo la vista logica di gioco a 1024x768
    m_view.setSize({GAME_WIDTH, GAME_HEIGHT});
    m_view.setCenter({GAME_WIDTH / 2.f, GAME_HEIGHT / 2.f});
    m_window.setView(m_view);

    // Setup della navicella
    m_ship.setPointCount(4);
    m_ship.setPoint(0, sf::Vector2f(0.f, -20.f));
    m_ship.setPoint(1, sf::Vector2f(15.f, 15.f));
    m_ship.setPoint(2, sf::Vector2f(0.f, 10.f));
    m_ship.setPoint(3, sf::Vector2f(-15.f, 15.f));

    m_ship.setFillColor(sf::Color::Transparent);
    m_ship.setOutlineColor(sf::Color::White);
    m_ship.setOutlineThickness(2.f);
    m_ship.setPosition({GAME_WIDTH / 2.f, GAME_HEIGHT / 2.f});
}

// Il ciclo principale del gioco
void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        sf::Time deltaTime = clock.restart(); //In deltaTime c'è il tempo trascorso dall'ultimo frame (utile per la fisica)
        
        processEvents();
        update(deltaTime);
        render();
    }
}

// Gestione dell'input (tastiera/chiusura)
void Game::processEvents() {
    while (const auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }

        if(const auto* resized = event->getIf<sf::Event::Resized>()){
            updateViewport(resized->size.x, resized->size.y);
        }
    }
}

// Aggiornamento della logica (nella Tappa 01 non c'è movimento, quindi è vuoto)
void Game::update(sf::Time deltaTime) {
    float dt = deltaTime.asSeconds(); //Tempo trascorso dall'ultimo frame in secondi

    // 1. ROTAZIONE (Sinistra/Destra)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
        m_ship.rotate(sf::degrees(-m_rotationSpeed * dt)); //Rotazione antioraria
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
        m_ship.rotate(sf::degrees(m_rotationSpeed * dt)); //Rotazione oraria
    }

    // 2. ACCELERAZIONE (Su / W)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
        float radians = m_ship.getRotation().asRadians(); //Angolo di rotazione della navicella in radianti

        // Calcoliamo la direzione verso cui punta la prua (0 gradi in SFML è rivolto in alto con la nostra forma)
        sf::Vector2f direction(std::sin(radians), -std::cos(radians));

        // Incrementiamo la velocità: v = v + a * dt
        m_velocity += direction * m_acceleration * dt;
    }

    // 3. APPLICAZIONE DELL'ATTRITO E SPOSTAMENTO
    m_velocity *= std::pow(m_drag, dt * 60.f); // Attrito indipendente dal framerate
    m_ship.move(m_velocity * dt); // SPostamento: p = p + v * dt

    // 4. GESTIONE DELLO SCREEN WRAPPING
    handleScreenWrapping();
}

void Game::handleScreenWrapping(){
    sf::Vector2f pos = m_ship.getPosition();

    // Se esce a sinistra -> Riappare a destra
    if(pos.x < 0.f){
        pos.x = GAME_WIDTH;
    }
    // Se esce a destra -> Riappare a sinistra
    else if(pos.x > GAME_WIDTH){
        pos.x -= GAME_WIDTH;
    }

    // Se esce in alto -> Riappare in basso
    if(pos.y < 0.f){
        pos.y = GAME_HEIGHT;
    }
    // Se esce in basso -> Riappare in alto
    else if(pos.y > GAME_HEIGHT){
        pos.y -= GAME_HEIGHT;
    }

    m_ship.setPosition(pos);
}

// Disegno a schermo
void Game::render() {
    m_window.clear(sf::Color::Black);
    m_window.draw(m_ship);
    m_window.display();
}

// Aggiorna il viewport della vista logica di gioco in base alle dimensioni della finestra
void Game::updateViewport(unsigned int width, unsigned int height) {
    float windowRatio = static_cast<float>(width) / static_cast<float>(height);
    float targetRatio = GAME_WIDTH / GAME_HEIGHT; // 4.0 / 3.0 = 1.3333

    //Valori percentuali del viewport (da 0.0 a 1.0)
    float viewportWidth = 1.0f;
    float viewportHeight = 1.0f;
    float viewportLeft = 0.0f;
    float viewportTop = 0.0f;

    if (windowRatio > targetRatio) {
        // Finestra troppo larga -> Bande nere a destra e sinistra (Pillarbox)
        viewportWidth = targetRatio / windowRatio;
        viewportLeft = (1.0f - viewportWidth) / 2.0f;
    } else {
        // Finestra troppo alta -> Bande nere sopra e sotto (Letterbox)
        viewportHeight = windowRatio / targetRatio;
        viewportTop = (1.0f - viewportHeight) / 2.0f;
    }

    // Applichiamo il viewport percentuale (da 0.0 a 1.0)
    m_view.setViewport(sf::FloatRect({viewportLeft, viewportTop}, {viewportWidth, viewportHeight}));
    m_window.setView(m_view);
}