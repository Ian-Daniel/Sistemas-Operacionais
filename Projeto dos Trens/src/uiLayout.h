#ifndef LAYOUTCONSTANTS_H
#define LAYOUTCONSTANTS_H

#include <QPoint>

namespace Layout {
    const int Y_TOP = 150;   // Linha Superior;
    const int Y_MID = 270;   // Linha do Meio (O novo centro da tela);
    const int Y_BOT = 390;   // Linha Inferior.

    const int X_TIP_LEFT  = 110;  // Ponta Esquerda;
    const int X_COL_LEFT  = 310;  // Coluna Vertical Esquerda;
    const int X_COL_RIGHT = 490;  // Coluna Vertical Direita;
    const int X_TIP_RIGHT = 690;  // Ponta Direita.

    // Ponta Esquerda:
    const QPoint P_TipLeft(X_TIP_LEFT, Y_MID);

    // Coluna Esquerda:
    const QPoint P_TopLeft(X_COL_LEFT, Y_TOP);
    const QPoint P_MidLeft(X_COL_LEFT, Y_MID);
    const QPoint P_BotLeft(X_COL_LEFT, Y_BOT);

    // Coluna Direita:
    const QPoint P_TopRight(X_COL_RIGHT, Y_TOP);
    const QPoint P_MidRight(X_COL_RIGHT, Y_MID);
    const QPoint P_BotRight(X_COL_RIGHT, Y_BOT);

    // Ponta Direita:
    const QPoint P_TipRight(X_TIP_RIGHT, Y_MID);

    const int Step = 2;
}

#endif