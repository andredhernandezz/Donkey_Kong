#ifndef FUNCOES_INIMIGOS_H
#define FUNCOES_INIMIGOS_H

#include "raylib.h"
#include <stdio.h>
#include "funcoes_mapa.h"
#include "constantes.h"


// posicao do inimigos
void encontrarInimigos(char mapa[LINHAS][COLUNAS],
                      int inimigoLinha[], int inimigoColuna[],
                      int inimigoDirecao[], int *total) {

    *total = 0;

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {

            if (mapa[i][j] == 'E') {

                inimigoLinha[*total] = i;
                inimigoColuna[*total] = j;
                inimigoDirecao[*total] = 1;

                mapa[i][j] = '.';

                (*total)++;
            }
        }
    }
}

// movimenta os inimigos
void movimentaInimigos(char mapa[LINHAS][COLUNAS],int inimigoLinha[], int inimigoColuna[],int inimigoDirecao[], int total, int *contador){

    (*contador)++; // timer para desacelerar o movimento dos inimigos
    if (*contador >= 12){

        // definicao dinamica da posicao dos inimigos
        for (int i = 0; i < total; i++) {
            int linha = inimigoLinha[i];
            int coluna = inimigoColuna[i];

            // verifica chão
            if (linha < LINHAS - 1 && (mapa[linha + 1][coluna] == 'B' || mapa[linha + 1][coluna] == 'Z' || mapa[linha + 1][coluna] == 'H')) {

                int novaColuna = coluna + inimigoDirecao[i];

                // verifica colisão ou borda
                if (novaColuna <= 0 || novaColuna >= COLUNAS - 1 ||
                    mapa[linha][novaColuna] == 'B' || mapa[linha][novaColuna] == 'Z' ||
                    !temChao(mapa[linha+1][novaColuna])) {

                    inimigoDirecao[i] *= -1;

                } else {
                    inimigoColuna[i] = novaColuna;
                }
            }
        }
        *contador = 0;
    }
}

// funcao que verifica se houve colisao com algum inimigo
int colisaoInimigo(int playerLinha, int playerColuna, int inimigoLinha[], int inimigoColuna[], int total) {
    for (int i = 0; i < total; i++) {
        if (playerLinha == inimigoLinha[i] && playerColuna == inimigoColuna[i]) {
            return 1; // player e inimigo na mesma posicao (houve colisao)
        }
    }
    return 0; // nao houve colisão
}

#endif
