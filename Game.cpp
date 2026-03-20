#include "Game.h"
#include <cstdio>   // printf

// ─────────────────────────────────────────────
//  Constructor / Destructor
// ─────────────────────────────────────────────

Game::Game() : state(STATE_PLAYING), score(0), randSeed(12345) {
    board        = new Board();
    currentPiece = new Piece();
}

Game::~Game() {
    delete board;
    delete currentPiece;
    board        = nullptr;
    currentPiece = nullptr;
}

// ─────────────────────────────────────────────
//  Generador LCG simple (sin rand() de <cstdlib>)
//  Evita depender de STL y cumple el requisito
// ─────────────────────────────────────────────

int Game::nextRand() {
    // Parámetros clásicos de LCG (Numerical Recipes)
    randSeed = randSeed * 1664525u + 1013904223u;
    int val = (int)((randSeed >> 16) & 0x7FFF);
    return val % PIECE_COUNT;
}

// ─────────────────────────────────────────────
//  Inicio de partida
// ─────────────────────────────────────────────

void Game::start() {
    board->clear();
    score = 0;
    state = STATE_PLAYING;
    spawnPiece();
}

void Game::spawnPiece() {
    int typeIdx = nextRand();
    currentPiece->init((PieceType)typeIdx);

    // Posición inicial: fila 0, centrada horizontalmente
    int startCol = (BOARD_WIDTH / 2) - (currentPiece->getWidth() / 2);
    currentPiece->posRow = 0;
    currentPiece->posCol = startCol;

    // Si ya colisiona al aparecer → Game Over
    if (collides(0, startCol, currentPiece->getRotation())) {
        state = STATE_GAMEOVER;
    }
}

// ─────────────────────────────────────────────
//  Detección de colisiones con bits
// ─────────────────────────────────────────────

bool Game::collides(int row, int col, int rot) const {
    // Guardamos rotación actual y la cambiamos temporalmente
    // NOTA: para no modificar el estado de la pieza, usamos una pieza temporal
    // Creamos pieza temporal en pila (sin new) para eficiencia
    Piece temp;
    temp.init(currentPiece->getType());

    // Rotar hasta la rotación deseada
    for (int r = 0; r < rot; ++r) {
        temp.rotate();
    }

    int h = temp.getHeight();

    for (int pr = 0; pr < h; ++pr) {
        int boardRow = row + pr;

        // Fuera del tablero por arriba (permitido, la pieza está emergiendo)
        if (boardRow < 0) continue;

        // Fuera del tablero por abajo
        if (boardRow >= BOARD_HEIGHT) return true;

        // Obtener bits de la fila de la pieza y desplazarlos a la posición col
        uint16_t pieceBits = temp.getRowBits(pr);

        // Desplazar los bits al offset de columna
        // La pieza tiene bits alineados a la izquierda (bit 15 = col 0 de pieza)
        // Si posCol > 0, desplazamos a la derecha
        uint16_t shifted;
        if (col >= 0) {
            shifted = pieceBits >> col;
        } else {
            shifted = pieceBits << (-col);
        }

        // Verificar que no se salga del borde derecho
        // Si algún bit de la pieza quedó cortado al desplazar, hay colisión
        // Para detectarlo: reconstruimos desde la derecha
        if (col >= 0) {
            // Los bits que se "cayeron" a la derecha serían los de menos valor
            // Verificación: los bits originales desplazados de vuelta deben coincidir
            uint16_t restored = shifted << col;
            if (restored != pieceBits) return true;  // bits cortados = colisión con borde
        } else {
            uint16_t restored = shifted >> (-col);
            if (restored != pieceBits) return true;
        }

        // Verificar colisión con celdas ocupadas del tablero
        if ((board->getRow(boardRow) & shifted) != 0) return true;
    }

    return false;
}

// ─────────────────────────────────────────────
//  Movimiento y rotación
// ─────────────────────────────────────────────

bool Game::tryMove(int dRow, int dCol) {
    int newRow = currentPiece->posRow + dRow;
    int newCol = currentPiece->posCol + dCol;

    if (!collides(newRow, newCol, currentPiece->getRotation())) {
        currentPiece->posRow = newRow;
        currentPiece->posCol = newCol;
        return true;
    }
    return false;
}

bool Game::tryRotate() {
    int nextRot = (currentPiece->getRotation() + 1) % MAX_ROTATIONS;

    if (!collides(currentPiece->posRow, currentPiece->posCol, nextRot)) {
        currentPiece->rotate();
        return true;
    }
    return false;
}

// ─────────────────────────────────────────────
//  Fijar pieza en el tablero
// ─────────────────────────────────────────────

void Game::lockPiece() {
    int h = currentPiece->getHeight();

    for (int pr = 0; pr < h; ++pr) {
        int boardRow = currentPiece->posRow + pr;
        if (boardRow < 0 || boardRow >= BOARD_HEIGHT) continue;

        uint16_t pieceBits = currentPiece->getRowBits(pr);
        int col = currentPiece->posCol;

        uint16_t shifted;
        if (col >= 0) {
            shifted = pieceBits >> col;
        } else {
            shifted = pieceBits << (-col);
        }

        board->mergeRow(boardRow, shifted);
    }

    // Eliminar filas completas y sumar puntaje
    int cleared = board->clearFullRows();
    addScore(cleared);

    // Generar nueva pieza
    spawnPiece();
}

// ─────────────────────────────────────────────
//  Procesamiento de entrada del usuario
// ─────────────────────────────────────────────

void Game::processInput(char input) {
    if (state == STATE_GAMEOVER) return;

    switch (input) {
        case 'a': case 'A':
            tryMove(0, -1);
            break;

        case 'd': case 'D':
            tryMove(0, +1);
            break;

        case 's': case 'S':
            // Bajar una posición; si no puede → fijar pieza
            if (!tryMove(+1, 0)) {
                lockPiece();
            }
            break;

        case 'w': case 'W':
            tryRotate();
            break;

        case 'q': case 'Q':
            state = STATE_GAMEOVER;
            break;

        default:
            break;
    }
}

// ─────────────────────────────────────────────
//  Puntaje
// ─────────────────────────────────────────────

void Game::addScore(int linesCleared) {
    // Sistema clásico de puntuación
    int points = 0;
    switch (linesCleared) {
        case 1: points = 100; break;
        case 2: points = 300; break;
        case 3: points = 500; break;
        case 4: points = 800; break;  // Tetris!
        default: points = 0;  break;
    }
    score += points;
}

int Game::getScore() const {
    return score;
}

bool Game::isGameOver() const {
    return state == STATE_GAMEOVER;
}

// ─────────────────────────────────────────────
//  Construcción de máscara visual de la pieza
// ─────────────────────────────────────────────

void Game::buildPieceMask(uint16_t* pieceMask) const {
    int h = currentPiece->getHeight();

    for (int pr = 0; pr < h; ++pr) {
        int boardRow = currentPiece->posRow + pr;
        if (boardRow < 0 || boardRow >= BOARD_HEIGHT) continue;

        uint16_t pieceBits = currentPiece->getRowBits(pr);
        int col = currentPiece->posCol;

        uint16_t shifted;
        if (col >= 0) {
            shifted = pieceBits >> col;
        } else {
            shifted = pieceBits << (-col);
        }

        pieceMask[boardRow] |= shifted;
    }
}

// ─────────────────────────────────────────────
//  Renderizado en consola
// ─────────────────────────────────────────────

void Game::render() const {
    // Construir máscara de la pieza activa
    uint16_t* pieceMask = new uint16_t[BOARD_HEIGHT];
    for (int r = 0; r < BOARD_HEIGHT; ++r) pieceMask[r] = 0;

    if (state == STATE_PLAYING) {
        buildPieceMask(pieceMask);
    }

    board->print(pieceMask);
    delete[] pieceMask;

    // Información de estado
    printf("Puntaje: %d\n", score);

    if (state == STATE_GAMEOVER) {
        printf("\n*** GAME OVER ***\n");
    } else {
        printf("Accion: [A]Izq [D]Der [S]Bajar [W]Rotar [Q]Salir: ");
    }
}
