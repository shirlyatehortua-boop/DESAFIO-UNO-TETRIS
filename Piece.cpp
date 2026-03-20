#include "Piece.h"

// ─────────────────────────────────────────────
//  Constructor / Destructor
// ─────────────────────────────────────────────

Piece::Piece() : type(PIECE_I), color(COLOR_LIGHT_BLUE), rotation(0),
                 shapes(nullptr), posRow(0), posCol(0) {
    for (int r = 0; r < MAX_ROTATIONS; ++r) {
        heights[r] = 0;
        widths[r]  = 0;
    }
}

Piece::~Piece() {
    freeShapes();
}

void Piece::freeShapes() {
    if (shapes != nullptr) {
        for (int r = 0; r < MAX_ROTATIONS; ++r) {
            delete[] shapes[r];
            shapes[r] = nullptr;
        }
        delete[] shapes;
        shapes = nullptr;
    }
}

// ─────────────────────────────────────────────
//  Inicialización
// ─────────────────────────────────────────────

void Piece::init(PieceType t) {
    freeShapes();

    type     = t;
    color    = (PieceColor)t;   // color = índice de tipo
    rotation = 0;

    // Reservar memoria dinámica para las 4 rotaciones, cada una con 4 filas
    shapes = new uint16_t*[MAX_ROTATIONS];
    for (int r = 0; r < MAX_ROTATIONS; ++r) {
        shapes[r] = new uint16_t[MAX_PIECE_ROWS];
        for (int row = 0; row < MAX_PIECE_ROWS; ++row) {
            shapes[r][row] = 0x0000;
        }
    }

    buildShapes();
}

void Piece::buildShapes() {
    switch (type) {
        case PIECE_I: buildI(); break;
        case PIECE_O: buildO(); break;
        case PIECE_T: buildT(); break;
        case PIECE_S: buildS(); break;
        case PIECE_Z: buildZ(); break;
        case PIECE_J: buildJ(); break;
        case PIECE_L: buildL(); break;
        default: break;
    }
}

// ─────────────────────────────────────────────
//  Definición de formas por bits
//
//  Convención: bits alineados a la IZQUIERDA del uint16_t
//  Bit 15 = columna 0 de la pieza, bit 14 = columna 1, etc.
//
//  Ejemplo pieza I horizontal (1 fila, 4 cols):
//  1111 0000 0000 0000 = 0xF000
// ─────────────────────────────────────────────

void Piece::buildI() {
    // Rotación 0: horizontal  ████  (1x4)
    shapes[0][0] = 0xF000;  // 1111 0000 0000 0000
    heights[0] = 1; widths[0] = 4;

    // Rotación 1: vertical  (4x1)
    shapes[1][0] = 0x8000;  // 1000...
    shapes[1][1] = 0x8000;
    shapes[1][2] = 0x8000;
    shapes[1][3] = 0x8000;
    heights[1] = 4; widths[1] = 1;

    // Rotación 2 = igual a 0 (simétrica)
    shapes[2][0] = 0xF000;
    heights[2] = 1; widths[2] = 4;

    // Rotación 3 = igual a 1
    shapes[3][0] = 0x8000;
    shapes[3][1] = 0x8000;
    shapes[3][2] = 0x8000;
    shapes[3][3] = 0x8000;
    heights[3] = 4; widths[3] = 1;
}

void Piece::buildO() {
    // Todas las rotaciones iguales: ██ (2x2)
    //  1100 0000 0000 0000 = 0xC000
    for (int r = 0; r < MAX_ROTATIONS; ++r) {
        shapes[r][0] = 0xC000;
        shapes[r][1] = 0xC000;
        heights[r] = 2; widths[r] = 2;
    }
}

void Piece::buildT() {
    // Rotación 0:  ███   (3 cols fila 0, centro fila 1)
    //              .█.
    shapes[0][0] = 0xE000;  // 1110 0000...
    shapes[0][1] = 0x4000;  // 0100 0000...
    heights[0] = 2; widths[0] = 3;

    // Rotación 1:  █.
    //              ██
    //              █.
    shapes[1][0] = 0x8000;  // 1000...
    shapes[1][1] = 0xC000;  // 1100...
    shapes[1][2] = 0x8000;  // 1000...
    heights[1] = 3; widths[1] = 2;

    // Rotación 2:  .█.
    //              ███
    shapes[2][0] = 0x4000;  // 0100...
    shapes[2][1] = 0xE000;  // 1110...
    heights[2] = 2; widths[2] = 3;

    // Rotación 3:  .█
    //              ██
    //              .█
    shapes[3][0] = 0x4000;  // 0100...
    shapes[3][1] = 0xC000;  // 1100...
    shapes[3][2] = 0x4000;  // 0100...
    heights[3] = 3; widths[3] = 2;
}

void Piece::buildS() {
    // Rotación 0:  .██
    //              ██.
    shapes[0][0] = 0x6000;  // 0110 0000...
    shapes[0][1] = 0xC000;  // 1100 0000...
    heights[0] = 2; widths[0] = 3;

    // Rotación 1:  █.
    //              ██
    //              .█
    shapes[1][0] = 0x8000;  // 1000...
    shapes[1][1] = 0xC000;  // 1100...
    shapes[1][2] = 0x4000;  // 0100...
    heights[1] = 3; widths[1] = 2;

    // Rotación 2 = igual a 0
    shapes[2][0] = 0x6000;
    shapes[2][1] = 0xC000;
    heights[2] = 2; widths[2] = 3;

    // Rotación 3 = igual a 1
    shapes[3][0] = 0x8000;
    shapes[3][1] = 0xC000;
    shapes[3][2] = 0x4000;
    heights[3] = 3; widths[3] = 2;
}

void Piece::buildZ() {
    // Rotación 0:  ██.
    //              .██
    shapes[0][0] = 0xC000;  // 1100 0000...
    shapes[0][1] = 0x6000;  // 0110 0000...
    heights[0] = 2; widths[0] = 3;

    // Rotación 1:  .█
    //              ██
    //              █.
    shapes[1][0] = 0x4000;
    shapes[1][1] = 0xC000;
    shapes[1][2] = 0x8000;
    heights[1] = 3; widths[1] = 2;

    // Rotación 2 = 0
    shapes[2][0] = 0xC000;
    shapes[2][1] = 0x6000;
    heights[2] = 2; widths[2] = 3;

    // Rotación 3 = 1
    shapes[3][0] = 0x4000;
    shapes[3][1] = 0xC000;
    shapes[3][2] = 0x8000;
    heights[3] = 3; widths[3] = 2;
}

void Piece::buildJ() {
    // Rotación 0:  █..
    //              ███
    shapes[0][0] = 0x8000;  // 1000...
    shapes[0][1] = 0xE000;  // 1110...
    heights[0] = 2; widths[0] = 3;

    // Rotación 1:  ██
    //              █.
    //              █.
    shapes[1][0] = 0xC000;
    shapes[1][1] = 0x8000;
    shapes[1][2] = 0x8000;
    heights[1] = 3; widths[1] = 2;

    // Rotación 2:  ███
    //              ..█
    shapes[2][0] = 0xE000;
    shapes[2][1] = 0x2000;  // 0010...
    heights[2] = 2; widths[2] = 3;

    // Rotación 3:  .█
    //              .█
    //              ██
    shapes[3][0] = 0x4000;
    shapes[3][1] = 0x4000;
    shapes[3][2] = 0xC000;
    heights[3] = 3; widths[3] = 2;
}

void Piece::buildL() {
    // Rotación 0:  ..█
    //              ███
    shapes[0][0] = 0x2000;  // 0010...
    shapes[0][1] = 0xE000;  // 1110...
    heights[0] = 2; widths[0] = 3;

    // Rotación 1:  █.
    //              █.
    //              ██
    shapes[1][0] = 0x8000;
    shapes[1][1] = 0x8000;
    shapes[1][2] = 0xC000;
    heights[1] = 3; widths[1] = 2;

    // Rotación 2:  ███
    //              █..
    shapes[2][0] = 0xE000;
    shapes[2][1] = 0x8000;
    heights[2] = 2; widths[2] = 3;

    // Rotación 3:  ██
    //              .█
    //              .█
    shapes[3][0] = 0xC000;
    shapes[3][1] = 0x4000;
    shapes[3][2] = 0x4000;
    heights[3] = 3; widths[3] = 2;
}

// ─────────────────────────────────────────────
//  Rotación
// ─────────────────────────────────────────────

void Piece::rotate() {
    rotation = (rotation + 1) % MAX_ROTATIONS;
}

// ─────────────────────────────────────────────
//  Getters
// ─────────────────────────────────────────────

PieceType  Piece::getType()     const { return type; }
PieceColor Piece::getColor()    const { return color; }
int        Piece::getRotation() const { return rotation; }
int        Piece::getHeight()   const { return heights[rotation]; }
int        Piece::getWidth()    const { return widths[rotation]; }

uint16_t Piece::getRowBits(int r) const {
    if (r < 0 || r >= MAX_PIECE_ROWS) return 0;
    return shapes[rotation][r];
}
