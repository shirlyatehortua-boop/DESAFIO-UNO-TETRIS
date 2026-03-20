#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

// Ancho fijo: 16 columnas (múltiplo de 8, usa uint16_t por fila)
// Alto fijo:  18 filas
#define BOARD_WIDTH  16
#define BOARD_HEIGHT 18

// Cada fila del tablero es un uint16_t
// Bit en posición i = 1 → celda ocupada, 0 → vacía
// La columna 0 es el bit más significativo (izquierda visual)
// Fila completa = 0xFFFF (todos los bits en 1)
#define FULL_ROW     ((uint16_t)0xFFFF)

// Máscara para limitar al ancho real (16 bits = exacto)
#define ROW_MASK     ((uint16_t)0xFFFF)

class Board {
public:
    Board();
    ~Board();

    // Inicializa/limpia el tablero (todo en cero)
    void clear();

    // Lee si una celda está ocupada (col 0=izquierda)
    bool isCellOccupied(int row, int col) const;

    // Fija bits en el tablero (cuando una pieza aterriza)
    void setCell(int row, int col);

    // Devuelve la fila como uint16_t para comparaciones con bits de pieza
    uint16_t getRow(int row) const;

    // Combina (OR) una máscara de bits en una fila
    void mergeRow(int row, uint16_t mask);

    // Verifica si la fila está completamente llena
    bool isRowFull(int row) const;

    // Elimina filas completas y baja las superiores
    // Devuelve la cantidad de filas eliminadas
    int clearFullRows();

    // Verifica si el tablero tiene celdas ocupadas en la fila 0 (game over)
    bool isTopOccupied() const;

    // Imprime el tablero en consola (para debug / interfaz turno a turno)
    // pieceMask[BOARD_HEIGHT]: bits de la pieza activa superpuestos visualmente
    void print(uint16_t* pieceMask) const;

private:
    // Tablero: arreglo dinámico de filas, cada fila es uint16_t
    uint16_t* rows;
};

#endif // BOARD_H
