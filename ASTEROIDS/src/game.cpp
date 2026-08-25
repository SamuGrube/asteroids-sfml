#include "headers/game.hpp"
#include <iostream>
#include <algorithm>
#include <random>

// Helper per calcolare la distanza euclidea tra due punti
float getDistance(sf::Vector2f a, sf::Vector2f b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

// Helper per centrare l'origine di qualsiasi sf::Text
auto centerTextOrigin = [](sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });
};

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

// Costruttore: inizializza la finestra e prepara la navicella
Game::Game() : m_window(sf::VideoMode({1024, 768}), "Asteroids", sf::Style::Default),
               m_uiText(m_font) { //Finestra completa di titolo, pulsante di chiusura e ridimensionabile
    m_window.setFramerateLimit(240);

    //Impostiamo la vista logica di gioco a 1024x768
    m_view.setSize({GAME_WIDTH, GAME_HEIGHT});
    m_view.setCenter({GAME_WIDTH / 2.f, GAME_HEIGHT / 2.f});
    m_window.setView(m_view);

    //Caricamento del font
    bool fontLoaded = m_font.openFromFile("../../assets/arial.ttf");

    if (!fontLoaded) {
        std::cerr << "ERRORE CRITICO: Nessun font trovato! Impossibile avviare la GUI.\n";
        std::exit(EXIT_FAILURE);
    }

    m_uiText.setFont(m_font);
    m_uiText.setFillColor(sf::Color::White);

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

    //Caricamento Suoni. 
    if (!m_shootBuffer.loadFromFile("../../assets/shoot.wav") && !m_shootBuffer.loadFromFile("shoot.wav")) {
        std::cout << "[Audio] Avviso: shoot.wav non trovato!\n";
    }
    if (!m_explosionBuffer.loadFromFile("../../assets/explosion.wav") && !m_explosionBuffer.loadFromFile("explosion.wav")) {
        std::cout << "[Audio] Avviso: explosion.wav non trovato!\n";
    }
    if (!m_hitBuffer.loadFromFile("../../assets/hit.wav") && !m_hitBuffer.loadFromFile("hit.wav")) {
        std::cout << "[Audio] Avviso: hit.wav non trovato!\n";
    }
    if (!m_gameOverBuffer.loadFromFile("../../assets/gameover.wav") && !m_gameOverBuffer.loadFromFile("gameover.wav")) {
        std::cout << "[Audio] Avviso: gameover.wav non trovato!\n";
    }
    if (!m_beat1Buffer.loadFromFile("../../assets/beat1.wav") && !m_beat1Buffer.loadFromFile("beat1.wav")) {
        std::cout << "[Audio] Avviso: beat1.wav non trovato!\n";
    }
    if (!m_beat2Buffer.loadFromFile("../../assets/beat2.wav") && !m_beat2Buffer.loadFromFile("beat2.wav")) {
        std::cout << "[Audio] Avviso: beat2.wav non trovato!\n";
    }

    m_shootSound.setVolume(20.f);
    m_explosionSound.setVolume(30.f);
    m_hitSound.setVolume(30.f);
    m_gameOverSound.setVolume(100.f);
    m_beat1Sound.setVolume(100.f);
    m_beat2Sound.setVolume(100.f);

    resetGame(); // Inizializza il gioco con vite e asteroidi
    m_state = GameState::MainMenu;
}

bool checkPolygonCollision(const std::vector<sf::Vector2f>& polyA, const std::vector<sf::Vector2f>& polyB) {
    auto checkAxes = [](const std::vector<sf::Vector2f>& p1, const std::vector<sf::Vector2f>& p2) {
        for (std::size_t i = 0; i < p1.size(); ++i) {
            sf::Vector2f pStart = p1[i];
            sf::Vector2f pEnd = p1[(i + 1) % p1.size()];
            
            // Perpendicolare del lato (Asse di proiezione)
            sf::Vector2f edge = pEnd - pStart;
            sf::Vector2f normal{-edge.y, edge.x};

            // Proiezione dei vertici di PolyA sull'asse
            float minA = std::numeric_limits<float>::max(), maxA = -std::numeric_limits<float>::max();
            for (const auto& v : p1) {
                float proj = v.x * normal.x + v.y * normal.y;
                minA = std::min(minA, proj);
                maxA = std::max(maxA, proj);
            }

            // Proiezione dei vertici di PolyB sull'asse
            float minB = std::numeric_limits<float>::max(), maxB = -std::numeric_limits<float>::max();
            for (const auto& v : p2) {
                float proj = v.x * normal.x + v.y * normal.y;
                minB = std::min(minB, proj);
                maxB = std::max(maxB, proj);
            }

            // Se troviamo un'asse su cui le proiezioni non si sovrappongono, NON c'è collisione
            if (maxA < minB || maxB < minA) return false;
        }
        return true;
    };

    // Bisogna controllare gli assi di entrambi i poligoni
    return checkAxes(polyA, polyB) && checkAxes(polyB, polyA);
}

void Game::resetGame() {
    m_lives = 3;
    m_score = 0;
    m_velocity = {0.f, 0.f};
    m_wave = 1;
    m_speedMultiplier = 1.0f;
    m_ship.setPosition({GAME_WIDTH / 2.f, GAME_HEIGHT / 2.f});
    m_asteroids.clear();
    m_bullets.clear();
    m_particles.clear();
    m_invulnerabilityTimer = m_maxInvulnerabilityTime;
    spawnAsteroids(5, m_speedMultiplier);
}

void Game::spawnAsteroids(std::size_t count, float speedMultiplier) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution xDist(0.f, GAME_WIDTH);
    std::uniform_real_distribution yDist(0.f, GAME_HEIGHT);

    for(std::size_t i = 0; i < count; ++i){
        sf::Vector2f pos;

        // Assicuriamoci che l'asteroide non venga generato troppo vicino alla posizione corrente della navicella
        do {
            pos = sf::Vector2f(xDist(gen), yDist(gen));
        } while (std::hypot(pos.x - m_ship.getPosition().x, pos.y - m_ship.getPosition().y) < 150.f);

        m_asteroids.emplace_back(pos, 40.f, speedMultiplier); // Aggiungiamo un nuovo asteroide con raggio 40
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

        if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if(m_state == GameState::MainMenu && keyPressed->code == sf::Keyboard::Key::Enter) {
                m_state = GameState::Playing;
            }else if(m_state == GameState::GameOver && keyPressed->code == sf::Keyboard::Key::R) {
                resetGame();
                m_state = GameState::Playing;
            }
        }
    }
}

// Aggiornamento della logica
void Game::update(sf::Time deltaTime) {
    float dt = deltaTime.asSeconds(); //Tempo trascorso dall'ultimo frame in secondi

    // --- LOGICA BATTITO CARDIACO (Basata su onde e tempo) ---
    // 1. Il tempo di pausa diminuisce all'aumentare dell'ondata (es. da 0.9s iniziali fino a un minimo di 0.3s)
    float baseInterval = std::max(0.3f, 0.9f - (static_cast<float>(m_wave - 1) * 0.1f));

    // 2. Il battito accelera leggermente anche durante la stessa ondata man mano che il timer avanza
    float beatDelay = baseInterval;

    // 3. Aggiorna il timer
    m_beatTimer -= deltaTime;
    if (m_beatTimer <= sf::Time::Zero) {
        if (m_playBeat1) {
            m_beat1Sound.play();
        } else {
            m_beat2Sound.play();
        }
        m_playBeat1 = !m_playBeat1;
        m_beatTimer = sf::seconds(beatDelay);
    }

    // Gestione dell'invulnerabilità della navicella dopo la collisione
    if(m_state != GameState::Playing) return;

    if(m_invulnerabilityTimer > sf::Time::Zero){
        m_invulnerabilityTimer -= deltaTime;
        if(m_invulnerabilityTimer < sf::Time::Zero) m_invulnerabilityTimer = sf::Time::Zero;
    }

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

        //Creazione di particelle per l'effetto scia della navicella
        sf::Vector2f exhaustPos = m_ship.getTransform().transformPoint({0.f, 15.f}); // Punto di uscita del getto (dietro la navicella)
        sf::Vector2f exhaustVel = -direction * 120.f + sf::Vector2f((std::rand() % 40 - 20), (std::rand() % 40 - 20)); // Velocità casuale per le particelle
        m_particles.emplace_back(exhaustPos, exhaustVel, sf::seconds(0.25f), sf::Color::White);
    }

    // 3. APPLICAZIONE DELL'ATTRITO E SPOSTAMENTO
    m_velocity *= std::pow(m_drag, dt * 60.f); // Attrito indipendente dal framerate
    m_ship.move(m_velocity * dt); // Spostamento: p = p + v * dt

    // 4. GESTIONE DELLO SCREEN WRAPPING
    handleScreenWrapping();

    // 5. AGGIORNAMENTO DEGLI ASTEROIDI
    for(auto& asteroid : m_asteroids){
        asteroid.update(deltaTime, GAME_WIDTH, GAME_HEIGHT);
    }

    //Collisioni tra asteroidi
    for (std::size_t i = 0; i < m_asteroids.size(); ++i) {
        for (std::size_t j = i + 1; j < m_asteroids.size(); ++j) {
            Asteroid& a1 = m_asteroids[i];
            Asteroid& a2 = m_asteroids[j];

            if (a1.isDead() || a2.isDead()) continue;

            float dist = getDistance(a1.getPosition(), a2.getPosition());
            float minDist = a1.getRadius() + a2.getRadius();

            if (dist < minDist) {
                //Evita divisioni per zero se spawnati nell'esatto identico pixel
                if (dist == 0.f) dist = 0.1f;

                // A) RISOLUZIONE COMPENETRAZIONE (Overlap)
                // Di quanti pixel si stanno sormontando?
                float overlap = minDist - dist;
                // Vettore normale della collisione (direzione da a1 a a2)
                sf::Vector2f normal = (a2.getPosition() - a1.getPosition()) / dist;

                // Spingiamo a1 indietro e a2 in avanti di metà sormonto per ciascuno
                a1.setPosition(a1.getPosition() - normal * (overlap / 2.f));
                a2.setPosition(a2.getPosition() + normal * (overlap / 2.f));

                // B) DEVIAZIONE CASUALE
                a1.deflect();
                a2.deflect();
            }
        }
    }

    // 6. GESTIONE DELLO SPARO DEI PROIETTILI
    m_fireTimer += deltaTime; //Aggiorniamo il timer del cooldown dello sparo
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && m_fireTimer >= m_fireCooldown){
        sf::Vector2f nosePosition = m_ship.getTransform().transformPoint({0.f, -20.f});

        m_bullets.emplace_back(nosePosition, m_ship.getRotation().asDegrees()); // Creiamo un nuovo proiettile
        m_fireTimer = sf::Time::Zero; // Resettiamo il timer del cooldown

        m_shootSound.play();
    }
    for(auto& bullet : m_bullets){
        bullet.update(deltaTime, GAME_WIDTH, GAME_HEIGHT);
        if(bullet.getMiss()) m_score -= 10;
    }

    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(), [](const Bullet& b) {
        return b.isDead();
    }), m_bullets.end());

    // 7. GESTIONE DELLE COLLISIONI TRA PROIETTILI E ASTEROIDI
    std::vector<Asteroid> newAsteroids; // Contenitore per i nuovi asteroidi generati dalla distruzione di quelli vecchi
    for(auto& bullet : m_bullets){
        if(bullet.isDead()) continue;

        for(auto& asteroid : m_asteroids){
            if(asteroid.isDead()) continue;

            if(getDistance(bullet.getPosition(), asteroid.getPosition()) <= asteroid.getRadius()){
                bullet.setDead(true);
                asteroid.setDead(true);

                // Creiamo un'esplosione di particelle alla posizione dell'asteroide
                createExplosion(asteroid.getPosition(), 15);
                m_explosionSound.play();

                //Gestione suddivisione asteroidi
                if(asteroid.getRadius() > 20.f){
                    m_score += 10;
                    float newRadius = asteroid.getRadius() /2.f;
                    newAsteroids.emplace_back(asteroid.getPosition(), newRadius, m_speedMultiplier);
                    newAsteroids.emplace_back(asteroid.getPosition(), newRadius, m_speedMultiplier);
                }else if(asteroid.getRadius() > 10.f){
                    m_score += 50;
                    float newRadius = asteroid.getRadius() /2.f;
                    newAsteroids.emplace_back(asteroid.getPosition(), newRadius, m_speedMultiplier);
                    newAsteroids.emplace_back(asteroid.getPosition(), newRadius, m_speedMultiplier);
                }else m_score += 100;
                break;
            }
        }
    }

    // Inserisce i nuovi frammenti nel vettore principale
    m_asteroids.insert(m_asteroids.end(), newAsteroids.begin(), newAsteroids.end());

    // 8. COLLISIONI TRA NAVICELLA E ASTEROIDI
    if(m_invulnerabilityTimer <= sf::Time::Zero) { // Solo se la navicella non è invulnerabile
        float shipRadius = 17.f; // Raggio approssimativo della navicella
        for(auto& asteroid : m_asteroids){
            if(asteroid.isDead()) continue;

            if(getDistance(m_ship.getPosition(), asteroid.getPosition()) < (asteroid.getRadius() + shipRadius)){
                if(checkPolygonCollision(getShipGlobalVertices(), asteroid.getGlobalVertices())){
                    m_lives--;
                    if(m_lives <= 0){
                        m_state = GameState::GameOver;
                        m_gameOverSound.play();
                    }else {
                        m_hitSound.play();
                        createExplosion(m_ship.getPosition(), 20);
                        //Respawn della navicella al centro dello schermo
                        m_ship.setPosition({GAME_WIDTH / 2.f, GAME_HEIGHT / 2.f});
                        m_velocity = {0.f, 0.f}; // Resettiamo la velocità
                        m_invulnerabilityTimer = m_maxInvulnerabilityTime; // Imposta l'invulnerabilità della navicella
                    }
                    break;
                }
            }
        }
    }

    // 9. RIMOZIONE DEGLI ASTEROIDI MORTI
    m_asteroids.erase(std::remove_if(m_asteroids.begin(), m_asteroids.end(), [](const Asteroid& a) {
        return a.isDead();
    }), m_asteroids.end());

    if(m_asteroids.empty() && m_state == GameState::Playing){
        m_wave++;
        m_speedMultiplier += 0.2f; // Aumentiamo la velocità degli asteroidi ad ogni onda successiva
        std::size_t asteroidCount = 4 + m_wave; // Aumentiamo il numero di asteroidi ad ogni onda successiva
        spawnAsteroids(asteroidCount, m_speedMultiplier); // Genera nuovi asteroidi con il moltiplicatore di velocità aggiornato
    }

    // 10. AGGIORNAMENTO DELLE PARTICELLE
    for(auto& particle : m_particles){
        particle.update(deltaTime);
    }
    m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(), [](const Particle& p) {
        return p.isDead();
    }), m_particles.end());
}

void Game::handleScreenWrapping() {
    sf::Vector2f pos = m_ship.getPosition();
    applyScreenWrapping(pos, GAME_WIDTH, GAME_HEIGHT, 15.f);
    m_ship.setPosition(pos);
}

// Rendering differenziato in base allo stato del gioco (Menu, Gioco, Game Over)
void Game::render() {
    m_window.clear(sf::Color::Black);

    if (m_state == GameState::MainMenu) {
        m_uiText.setCharacterSize(40);
        m_uiText.setString("ASTEROIDS");
        m_uiText.setPosition({GAME_WIDTH / 2.f - 110.f, 250.f});
        m_window.draw(m_uiText);

        m_uiText.setCharacterSize(20);
        m_uiText.setString("Premi INVIO per iniziare");
        m_uiText.setPosition({GAME_WIDTH / 2.f - 105.f, 350.f});
        m_window.draw(m_uiText);
    }
    else if (m_state == GameState::Playing) {
        // Calcolo dell'effetto lampeggio (visibile ad intervalli di 0.1 secondi)
        bool drawShip = true;
        if (m_invulnerabilityTimer > sf::Time::Zero) {
            int blink = static_cast<int>(m_invulnerabilityTimer.asSeconds() * 10.f);
            if (blink % 2 == 0) {
                drawShip = false; // Nasconde la navicella per un frame per creare il lampeggio
            }
        }

        if (drawShip) {
            m_window.draw(m_ship);
        }

        for (const auto& asteroid : m_asteroids) asteroid.draw(m_window);
        for (const auto& bullet : m_bullets) bullet.draw(m_window);
        for (const auto& particle : m_particles) particle.draw(m_window);

        // Disegno HUD
        m_uiText.setCharacterSize(20);
        m_uiText.setString("SCORE: " + std::to_string(m_score) + "   LIVES: " + std::to_string(m_lives) + "   WAVE: " + std::to_string(m_wave));
        m_uiText.setPosition({20.f, 20.f});
        m_window.draw(m_uiText);
    }
    else if (m_state == GameState::GameOver) {
        // --- SCHERMATA GAME OVER ---
        sf::Text gameOverText(m_font, "GAME OVER", 60);
        gameOverText.setFillColor(sf::Color::White);
        centerTextOrigin(gameOverText);
        gameOverText.setPosition({1024.f / 2.f, 320.f}); // Centro schermo
        m_window.draw(gameOverText);

        sf::Text scoreText(m_font, "PUNTEGGIO: " + std::to_string(m_score), 30);
        scoreText.setFillColor(sf::Color::White);
        centerTextOrigin(scoreText);
        scoreText.setPosition({1024.f / 2.f, 380.f}); // Sotto al titolo
        m_window.draw(scoreText);

        // Istruzione Riavvio
        sf::Text restartText(m_font, "PREMI R PER RICOMINCIARE", 24);
        restartText.setFillColor(sf::Color::White);
        centerTextOrigin(restartText);
        restartText.setPosition({1024.f / 2.f, 420.f}); // Sotto al titolo
        m_window.draw(restartText);
    }

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

std::vector<sf::Vector2f> Game::getShipGlobalVertices() const{
        sf::FloatRect bounds = m_ship.getGlobalBounds();
        return std::vector<sf::Vector2f>({
            {bounds.position.x + bounds.size.x / 2.f, bounds.position.y}, // Prua
            {bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y}, // Lato destro
            {bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y}, // Coda
            {bounds.position.x, bounds.position.y + bounds.size.y} // Lato sinistro
        });
}

void Game::createExplosion(sf::Vector2f position, int count) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    // Esplicitiamo <float> per evitare problemi di scope o deduzione dei tipi
    std::uniform_real_distribution<float> angleDist(0.f, 2.f * std::numbers::pi_v<float>);
    std::uniform_real_distribution<float> speedDist(30.f, 150.f);
    std::uniform_real_distribution<float> lifeDist(0.2f, 0.6f);

    for (int i = 0; i < count; ++i) {
        float angle = angleDist(gen);
        float speed = speedDist(gen);
        sf::Vector2f vel(std::cos(angle) * speed, std::sin(angle) * speed);
        sf::Time lifetime = sf::seconds(lifeDist(gen));

        m_particles.emplace_back(position, vel, lifetime, sf::Color::White);
    }
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