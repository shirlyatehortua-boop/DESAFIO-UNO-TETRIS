#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Piece.h"
#include <stdint.h>

// ─────────────────────────────────────────────
//  Estados del juego
// ─────────────────────────────────────────────
enum GameState {
    STATE_PLAYING  = 0,
    STATE_GAMEOVER = 1
};

// ─────────────────────────────────────────────
//  Motor del juego
// ─────────────────────────────────────────────
class Game {
public:
    Game();
    ~Game();

    // Inicia una nueva partida
    void start();

    // Procesa una acción del usuario
    // 'a' = izquierda, 'd' = derecha, 's' = bajar, 'w' = rotar, 'q' = salir
    void processInput(char input);

    // Imprime el estado actual del tablero con la pieza activa
    void render() const;

    // Devuelve true si el juego terminó
    bool isGameOver() const;

    // Puntaje actual
    int getScore() const;

private:
    Board*     board;
    Piece*     currentPiece;
    GameState  state;
    int        score;

    // Genera una nueva pieza aleatoria centrada en el tablero
    void spawnPiece();

    // Intenta mover la pieza en (dRow, dCol)
    // Devuelve true si el movimiento fue válido
    bool tryMove(int dRow, int dCol);

    // Intenta rotar la pieza
    // Devuelve true si la rotación fue válida
    bool tryRotate();

    // Fija la pieza en el tablero (cuando no puede bajar más)
    void lockPiece();

    // Verifica colisión entre la pieza (en posición dada) y el tablero
    // row/col = posición propuesta para la esquina superior-izquierda
    bool collides(int row, int col, int rotation) const;

    // Construye máscara visual de la pieza activa sobre el tablero
    // pieceMask debe ser un arreglo de BOARD_HEIGHT uint16_t en ceros
    void buildPieceMask(uint16_t* pieceMask) const;

    // Suma puntos por filas eliminadas
    // 1 fila=100, 2=300, 3=500, 4=800 (sistema clásico)
    void addScore(int linesCleared);

    // Semilla aleatoria simple (sin <cstdlib> STL — usamos LCG manual)
    uint32_t randSeed;
    int nextRand();   // devuelve valor entre 0 y PIECE_COUNT-1
};

#endif // GAME_H
