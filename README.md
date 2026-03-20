# Tetris en C++ con Qt — Desafío I Informática II UdeA

## Descripción
Implementación del juego Tetris por consola usando operaciones a nivel de bits,
memoria dinámica y el framework Qt, como parte del Desafío I del curso
Informática II (Semestre 2026-1, Universidad de Antioquia).

## ¿Por qué este tablero?

Originalmente quería usar el tablero clásico de 10 columnas por 18 filas, que es
el que recuerdo del Tetris de mi infancia. Sin embargo, el requisito del desafío
establece que el ancho debe ser estrictamente un múltiplo de 8.

Por eso elegí 16 columnas x 18 filas. Es el múltiplo de 8 más cercano a 10,
y además da más espacio horizontal al jugador para mover y rotar las piezas.

## ¿Cómo funciona la lógica de bits?

El tablero se representa fila por fila. Cada fila es un uint16_t (2 bytes = 16 bits).
Cada bit representa una celda: 0 = vacía, 1 = ocupada.

Fila vacía:   0000 0000 0000 0000  = 0x0000
Fila llena:   1111 1111 1111 1111  = 0xFFFF  (se elimina)

Para detectar colisiones se usa el operador & (AND) entre los bits de la pieza
y los bits del tablero. Si el resultado es distinto de cero, hay colisión.

## Las piezas y sus colores

Colores asignados según el orden favorito de mi mamá:

I - Linea 1x4    - Azul claro
O - Cuadrado 2x2 - Rojo
T - Forma T      - Naranja
S - Forma S      - Amarillo
Z - Forma Z      - Verde
J - Forma J      - Morado
L - Forma L      - Azul oscuro

## Controles

A = Mover izquierda
D = Mover derecha
S = Bajar
W = Rotar
Q = Salir

## Puntaje

1 linea  = 100 puntos
2 lineas = 300 puntos
3 lineas = 500 puntos
4 lineas = 800 puntos (Tetris!)

## Compilación

1. Abrir tetris.pro en Qt Creator
2. Seleccionar kit Desktop
3. Presionar Ctrl+R