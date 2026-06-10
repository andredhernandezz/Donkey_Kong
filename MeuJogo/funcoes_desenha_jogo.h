#ifndef FUNCOES_DESENHA_JOGO_H
#define FUNCOES_DESENHA_JOGO_H

#include "raylib.h"
#include <stdio.h>
#include "constantes.h"


// desenha as estruturas estatica do jogo
void desenharMapa(char mapa[LINHAS][COLUNAS], Texture2D estrutura, Texture2D escada, Texture2D inimigo, Texture2D portaBaixo, Texture2D portaCima) {

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {

            int x = j * TILE_SIZE; // coordenadas do desenho em x
            int y = i * TILE_SIZE; // coordenadas do desenho em y

            // diferentes casos da matriz definidos no mapa.txt
            switch (mapa[i][j]) {
                case 'Z': // plataforma
                    DrawTexture(estrutura, x, y, WHITE);
                    break;
                case 'B': // bordas
                    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, DARKBLUE);
                    break;

                case 'S': // inicio de escada
                case 'D': // topo de escada
                case 'H': // meio de escada
                    DrawTexture(escada, x, y, WHITE);
                    break;

                case 'F': // objetivo (parte de baixo)
                    DrawTexture(portaBaixo, x, y, WHITE);
                    break;
                case 'f': // objetivo (parte de cima)
                    DrawTexture(portaCima, x, y, WHITE);
                    break;

                default:

                    break;
            }
        }
    }
}



#endif
