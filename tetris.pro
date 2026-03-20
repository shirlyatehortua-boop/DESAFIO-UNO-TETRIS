QT       += core
QT       -= gui

TARGET    = tetris
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE  = app

# Estándar C++11 (sin ANSI C)
CONFIG   += c++11

SOURCES  += \
    main.cpp    \
    Board.cpp   \
    Piece.cpp   \
    Game.cpp

HEADERS  += \
    Board.h \
    Piece.h \
    Game.h
