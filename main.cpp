#include <QCoreApplication>
#include "Game.h"
#include <cstdio>

// ─────────────────────────────────────────────
//  Lectura de un carácter sin eco (Qt/plataforma)
//  En consola Qt usamos QTextStream sobre stdin
// ─────────────────────────────────────────────
#include <QTextStream>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QTextStream in(stdin);
    QTextStream out(stdout);

    out << "+==============================+\n";
    out << "|        T E T R I S           |\n";
    out << "|   Informatica II - UdeA      |\n";
    out << "+==============================+\n";
    out << "Tablero: " << BOARD_WIDTH << " x " << BOARD_HEIGHT << "\n";
    out << "Presiona ENTER después de cada acción.\n\n";
    out.flush();

    Game* game = new Game();
    game->start();
    game->render();

    while (!game->isGameOver()) {
        // Leer línea (el usuario presiona la tecla + Enter)
        QString line = in.readLine();
        if (line.isEmpty()) {
            // Enter solo → bajar automáticamente (gravedad manual)
            game->processInput('s');
        } else {
            QChar ch = line.at(0);
            game->processInput(ch.toLatin1());
        }

        // Limpiar pantalla en Windows
        system("cls");

        game->render();
    }

    delete game;
    return 0;
}
