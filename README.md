# Tetris en C++ con Qt — Desafío I Informática II UdeA

## Descripción
Implementación de Tetris por consola usando **operaciones a nivel de bits**, **memoria dinámica** y el framework **Qt**, como parte del Desafío I del curso Informática II (Semestre 2026-1, Universidad de Antioquia).

## Estructura del proyecto
```
tetris/
├── tetris.pro   → Archivo de proyecto Qt
├── main.cpp     → Punto de entrada
├── Board.h/.cpp → Tablero (lógica de bits por fila)
├── Piece.h/.cpp → Piezas (7 tetriminos con rotaciones en bits)
└── Game.h/.cpp  → Motor del juego (colisiones, gravedad, puntaje)
```

## Características técnicas
- **Tablero**: 16 × 18, cada fila representada como `uint16_t`
- **Detección de colisiones**: operaciones `&` y `>>` entre bits de pieza y tablero
- **Limpieza de filas**: compactación con punteros sobre arreglo dinámico
- **Rotaciones**: 4 estados por pieza, almacenados en `uint16_t**` (memoria dinámica)
- **Aleatoriedad**: generador LCG propio (sin `rand()` de STL)
- **Sin STL**, sin `string`, sin `struct` no autorizada

## Controles
| Tecla | Acción          |
|-------|-----------------|
| A     | Mover izquierda |
| D     | Mover derecha   |
| S     | Bajar           |
| W     | Rotar           |
| Q     | Salir           |

## Compilación
1. Abrir `tetris.pro` en **Qt Creator**
2. Build → Run (Ctrl+R)

## Puntaje
| Líneas eliminadas | Puntos |
|-------------------|--------|
| 1                 | 100    |
| 2                 | 300    |
| 3                 | 500    |
| 4 (Tetris!)       | 800    |

## Colores de las piezas
Asignados según el orden de colores favoritos 💜

| Pieza | Color       |
|-------|-------------|
| I     | Azul claro  |
| O     | Rojo        |
| T     | Naranja     |
| S     | Amarillo    |
| Z     | Verde       |
| J     | Morado      |
| L     | Azul oscuro |
