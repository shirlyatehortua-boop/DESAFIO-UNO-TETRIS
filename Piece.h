#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>

// ─────────────────────────────────────────────
//  Tipos de piezas (7 tetriminos clásicos)
// ─────────────────────────────────────────────
enum PieceType {
    PIECE_I = 0,
    PIECE_O = 1,
    PIECE_T = 2,
    PIECE_S = 3,
    PIECE_Z = 4,
    PIECE_J = 5,
    PIECE_L = 6,
    PIECE_COUNT = 7
};

// Colores asignados según el orden favorito de mamá :)
// Índice coincide con PieceType
// 0=Azul claro, 1=Rojo, 2=Naranja, 3=Amarillo, 4=Verde, 5=Morado, 6=Azul oscuro
enum PieceColor {
    COLOR_LIGHT_BLUE = 0,
    COLOR_RED        = 1,
    COLOR_ORANGE     = 2,
    COLOR_YELLOW     = 3,
    COLOR_GREEN      = 4,
    COLOR_PURPLE     = 5,
    COLOR_DARK_BLUE  = 6
};

// ─────────────────────────────────────────────
//  Representación de pieza
//
//  Cada rotación se guarda como 4 filas de uint16_t
//  (máximo necesario: pieza I vertical = 4 filas x 1 col)
//  El bit más significativo = columna izquierda (igual que el tablero)
//
//  pieceRows[rotación][fila dentro de la pieza] → uint16_t con bits
//  pieceHeight[rotación] → cuántas filas ocupa en esa rotación
//  pieceWidth[rotación]  → cuántas columnas ocupa
// ─────────────────────────────────────────────

#define MAX_ROTATIONS 4
#define MAX_PIECE_ROWS 4

class Piece {
public:
    Piece();
    ~Piece();

    // Inicializa la pieza con un tipo dado
    void init(PieceType type);

    // Rotar 90° en sentido horario
    void rotate();

    // Getters
    PieceType  getType()     const;
    PieceColor getColor()    const;
    int        getRotation() const;
    int        getHeight()   const;   // filas que ocupa la rotación actual
    int        getWidth()    const;   // columnas que ocupa la rotación actual

    // Devuelve la fila 'r' de la pieza en la rotación actual como uint16_t
    // Los bits están alineados a la izquierda (bit 15 = col 0 de la pieza)
    uint16_t getRowBits(int r) const;

    // Posición de la pieza en el tablero
    int posRow;   // fila superior de la pieza en el tablero
    int posCol;   // columna izquierda de la pieza en el tablero

private:
    PieceType  type;
    PieceColor color;
    int        rotation;   // 0..3

    // Datos de forma: [rotación][fila] → bits alineados a la izquierda
    // Se almacenan en memoria dinámica
    uint16_t** shapes;      // shapes[4][4]
    int        heights[MAX_ROTATIONS];
    int        widths[MAX_ROTATIONS];

    // Inicializa las formas según el tipo de pieza
    void buildShapes();

    // Libera la memoria de shapes
    void freeShapes();

    // Helpers para construir cada pieza
    void buildI();
    void buildO();
    void buildT();
    void buildS();
    void buildZ();
    void buildJ();
    void buildL();
};

#endif // PIECE_H
