# Asteroids - SFML C++23

Progetto sviluppato in C++23 e SFML 3.1.0 diviso in tappe incrementali.

---

**Requisiti di Sistema**
* **CMake**: versione 3.28 o superiore
* **Compilatore C++23**: GCC 13+, Clang 16+ o MSVC 2022+
* **Dipendenze**: Gestite automaticamente tramite CMake (FetchContent)

---

**1. Compilazione del Progetto (Build)**

È disponibile un unico comando di build che configura e compila simultaneamente tutti gli eseguibili dedicati alle varie tappe del progetto.

dalla radice del progetto:

```bash
# 1. Configurazione del progetto CMake
cmake -B build

# 2. Compilazione di tutte le tappe insieme
cmake --build build
```

**Esecuzione degli eseguibili**
* **Linux/MacOS**: ./build/bin/tappa01 -> tappa + numero tappa
* **Windows**: .\build\bin\Release\tappa13.exe

**Schema dei Comandi e Interfaccia Utente**
L'interazione con l'applicazione avviene esclusivamente tramite tastiera. Le funzinalità sono state introdotte in modo progressivo attraverso le varie tappe di sviluppo:
* **Navigazione e Stati di Gioco**
* INVIO (Enter): Avvia il gioco dalla schermata del Menu Principale (Tappa-08)
* R: Ricomincia una nuova partita dalla scherata di Game Over (Tappa-08)
* Chiusura Finestra/ALT + F4: Chiude l'applicazione (Disponibile in tutte le tappe)
* **Controllo Navicella e Movimento**
* Freccia Sinistra / A: Ruota la navicella in senso antiorario (Tappa-02)
* Freccia Destra / D: Ruota la navicella in senso orario (Tappa-02)
* Freccia Su / W: Attiva la spinta dei motori per accellerare nella direzione di puntamento della navicella (Tappa-02)