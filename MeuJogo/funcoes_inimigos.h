#ifndef FUNCOES_INIMIGOS_H
#define FUNCOES_INIMIGOS_H

#include "raylib.h"
#include <stdio.h>
#include "funcoes_mapa.h"
#include "constantes.h"

typedef struct {
    int linha;
    int coluna;
    int direcao;
}INIMIGO;

// posicao do inimigos
void encontrarInimigos(char mapa[LINHAS][COLUNAS], INIMIGO inimigos[], int *total) {

    *total = 0;

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {

            if (mapa[i][j] == 'E') {

                inimigos[*total].linha = i;
                inimigos[*total].coluna = j;
                inimigos[*total].direcao = 1;

                mapa[i][j] = '.';

                (*total)++;
            }
        }
    }
}

// movimenta os inimigos
void movimentaInimigos(char mapa[LINHAS][COLUNAS], INIMIGO inimigos[], int total, int *contador){

    (*contador)++; // timer para desacelerar o movimento dos inimigos
    if (*contador >= 12){

        // definicao dinamica da posicao dos inimigos
        for (int i = 0; i < total; i++) {
            int linha = inimigos[i].linha;
            int coluna = inimigos[i].coluna;

            // verifica chão
            if (linha < LINHAS - 1 && (mapa[linha + 1][coluna] == 'B' || mapa[linha + 1][coluna] == 'Z' || mapa[linha + 1][coluna] == 'H')) {

                int novaColuna = coluna + inimigos[i].direcao;

                // verifica colisão ou borda
                if (novaColuna <= 0 || novaColuna >= COLUNAS - 1 ||
                    mapa[linha][novaColuna] == 'B' || mapa[linha][novaColuna] == 'Z' ||
                    !temChao(mapa[linha+1][novaColuna])) {

                    inimigos[i].direcao *= -1;

                } else {
                    inimigos[i].coluna = novaColuna;
                }
            }
        }
        *contador = 0;
    }
}

// funcao que verifica se houve colisao com algum inimigo
int colisaoInimigo(int playerLinha, int playerColuna, INIMIGO inimigos[], int total) {
    for (int i = 0; i < total; i++) {
        if (playerLinha == inimigos[i].linha && playerColuna == inimigos[i].coluna) {
            return 1; // player e inimigo na mesma posicao (houve colisao)
        }
    }
    return 0; // nao houve colisão
}

// desenha os inimigos na tela
void desenharInimigos(INIMIGO inimigos[], int total, Texture2D dir, Texture2D esq) {
    for (int i = 0; i < total; i++) {
        int x = inimigos[i].coluna * TILE_SIZE;
        int y = inimigos[i].linha * TILE_SIZE;

        if (inimigos[i].direcao == 1)
            DrawTexture(dir, x, y, WHITE);
        else
            DrawTexture(esq, x, y, WHITE);
    }
}

#endif
