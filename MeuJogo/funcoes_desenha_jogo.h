#ifndef FUNCOES_DESENHA_JOGO_H
#define FUNCOES_DESENHA_JOGO_H

#include "raylib.h"
#include <stdio.h>
#include "constantes.h"

// desenha o jogo
void desenharMapa(char mapa[LINHAS][COLUNAS]) {

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {

            int x = j * TILE_SIZE;
            int y = i * TILE_SIZE;

            switch (mapa[i][j]) {

                case 'Z':
                    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, RED);
                    break;
                case 'B':
                    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, DARKBLUE);
                    break;
                case 'E':
                    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, YELLOW);
                    break;

                case 'S':
                case 'D':
                case 'H':
                    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, SKYBLUE);
                    break;

                case 'F':
                    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, GREEN);
                    break;

                default:

                    break;
            }
        }
    }
}

// desenha o player
void desenharPlayer(int linha, int coluna) {
    int x = coluna * TILE_SIZE;
    int y = linha * TILE_SIZE;

    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, BEIGE);
}

// desenha os inimigos na tela
void desenharInimigos(int inimigoLinha[], int inimigoColuna[], int total) {

    for (int i = 0; i < total; i++) {

        int x = inimigoColuna[i] * TILE_SIZE;
        int y = inimigoLinha[i] * TILE_SIZE;

        DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, YELLOW);
    }
}

#endif
