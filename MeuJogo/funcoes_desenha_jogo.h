#ifndef FUNCOES_DESENHA_JOGO_H
#define FUNCOES_DESENHA_JOGO_H

#include "raylib.h"
#include <stdio.h>
#include "constantes.h"


// desenha o jogo
void desenharMapa(char mapa[LINHAS][COLUNAS], Texture2D estrutura, Texture2D escada, Texture2D inimigo) {

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
                    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, GREEN);
                    break;

                default:

                    break;
            }
        }
    }
}

// desenha o player
void desenharPlayer(int linha, int coluna, Texture2D sprite) {
    DrawTexture(sprite, coluna * TILE_SIZE, linha * TILE_SIZE, WHITE);
}

// desenha os inimigos na tela
void desenharInimigos(int inimigoLinha[], int inimigoColuna[], int total, Texture2D sprite) {
    for (int i = 0; i < total; i++) {
        int x = inimigoColuna[i] * TILE_SIZE;
        int y = inimigoLinha[i] * TILE_SIZE;
        DrawTexture(sprite, x, y, WHITE);
    }
}

#endif
