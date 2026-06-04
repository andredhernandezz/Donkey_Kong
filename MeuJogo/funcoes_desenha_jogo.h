#ifndef FUNCOES_DESENHA_JOGO_H
#define FUNCOES_DESENHA_JOGO_H

#include "raylib.h"
#include <stdio.h>
#include "constantes.h"


// desenha o jogo
void desenharMapa(char mapa[LINHAS][COLUNAS], Texture2D estrutura, Texture2D escada, Texture2D inimigo, Texture2D portaBaixo, Texture2D portaCima) {

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {

            int x = j * TILE_SIZE;
            int y = i * TILE_SIZE;

            switch (mapa[i][j]) {

                case 'Z':
                    DrawTexture(estrutura, x, y, WHITE);
                    break;
                case 'B':
                    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, DARKBLUE);
                    break;
                case 'E':
                    DrawTexture(inimigo, x, y, WHITE);
                    break;

                case 'S':
                case 'D':
                case 'H':
                    DrawTexture(escada, x, y, WHITE);
                    break;

                case 'F':
                    DrawTexture(portaBaixo, x, y, WHITE);
                    break;
                case 'f':
                    DrawTexture(portaCima, x, y, WHITE);
                    break;

                default:

                    break;
            }
        }
    }
}



#endif
