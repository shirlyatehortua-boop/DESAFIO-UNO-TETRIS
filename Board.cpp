#include "Board.h"
#include <QtGlobal>   // Q_UNUSED si hace falta
#include <cstdio>     // printf

// ─────────────────────────────────────────────
//  Constructor / Destructor
// ─────────────────────────────────────────────

Board::Board() {
    // Memoria dinámica para las filas
    rows = new uint16_t[BOARD_HEIGHT];
    clear();
}

Board::~Board() {
    delete[] rows;
    rows = nullptr;
}

// ─────────────────────────────────────────────
//  Operaciones básicas
// ─────────────────────────────────────────────

void Board::clear() {
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        rows[i] = 0x0000;   // todas las celdas vacías
    }
}

bool Board::isCellOccupied(int row, int col) const {
    // El bit más significativo representa la columna 0 (izquierda)
    // col=0  → bit 15, col=1 → bit 14, ..., col=15 → bit 0
    uint16_t mask = (uint16_t)(1u << (BOARD_WIDTH - 1 - col));
    return (rows[row] & mask) != 0;
}

void Board::setCell(int row, int col) {
    uint16_t mask = (uint16_t)(1u << (BOARD_WIDTH - 1 - col));
    rows[row] |= mask;
}

uint16_t Board::getRow(int row) const {
    return rows[row];
}

void Board::mergeRow(int row, uint16_t mask) {
    rows[row] |= (mask & ROW_MASK);
}

// ─────────────────────────────────────────────
//  Lógica de filas
// ─────────────────────────────────────────────

bool Board::isRowFull(int row) const {
    // Todos los 16 bits en 1 → fila completa
    return (rows[row] & ROW_MASK) == FULL_ROW;
}

int Board::clearFullRows() {
    int cleared = 0;

    // Recorremos de abajo hacia arriba con punteros
    uint16_t* writePtr = rows + (BOARD_HEIGHT - 1);  // puntero a la última fila
    uint16_t* readPtr  = rows + (BOARD_HEIGHT - 1);  // mismo punto de partida

    // Primer paso: contar filas llenas (para desplazar puntero de lectura)
    // Usamos el esquema: compactar hacia abajo eliminando filas llenas
    // Trabajamos de abajo hacia arriba

    // Arreglo temporal en memoria dinámica para compactar
    uint16_t* temp = new uint16_t[BOARD_HEIGHT];
    int writeIdx = BOARD_HEIGHT - 1;

    for (int r = BOARD_HEIGHT - 1; r >= 0; --r) {
        if (!isRowFull(r)) {
            temp[writeIdx] = rows[r];
            --writeIdx;
        } else {
            ++cleared;
        }
    }

    // Rellenar con filas vacías las posiciones superiores sobrantes
    for (int r = writeIdx; r >= 0; --r) {
        temp[r] = 0x0000;
    }

    // Copiar de vuelta al tablero usando punteros
    uint16_t* src = temp;
    uint16_t* dst = rows;
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        *dst = *src;
        ++dst;
        ++src;
    }

    delete[] temp;

    // Silenciar advertencias de variables no usadas en este contexto
    (void)writePtr;
    (void)readPtr;

    return cleared;
}

// ─────────────────────────────────────────────
//  Condición de Game Over
// ─────────────────────────────────────────────

bool Board::isTopOccupied() const {
    // Si la fila 0 (tope) tiene cualquier bit encendido → Game Over
    return (rows[0] & ROW_MASK) != 0;
}

// ─────────────────────────────────────────────
//  Visualización por consola
// ─────────────────────────────────────────────

void Board::print(uint16_t* pieceMask) const {
    // Borde superior
    printf("+");
    for (int c = 0; c < BOARD_WIDTH; ++c) printf("-");
    printf("+\n");

    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        printf("|");
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            uint16_t colMask = (uint16_t)(1u << (BOARD_WIDTH - 1 - c));

            bool boardBit = (rows[r] & colMask) != 0;
            bool pieceBit = (pieceMask != nullptr) && ((pieceMask[r] & colMask) != 0);

            if (pieceBit)       printf("[]");   // celda de la pieza activa
            else if (boardBit)  printf("##");   // celda fija en el tablero
            else                printf(". ");   // vacío
        }
        printf("|\n");
    }

    // Borde inferior
    printf("+");
    for (int c = 0; c < BOARD_WIDTH; ++c) printf("-");
    printf("+\n");
}
