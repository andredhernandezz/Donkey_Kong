#ifndef FUNCOES_INIMIGOS_H
#define FUNCOES_INIMIGOS_H

#include "raylib.h"
#include <stdio.h>
#include "funcoes_mapa.h"
#include "constantes.h"

typedef struct {
    int linha; // coordenada y
    int coluna; // coordenada x
    int direcao; // para que lado esta caminhando (+ direita / - esquerda)
}INIMIGO;

// posicao inicial do inimigos
void encontrarInimigos(char mapa[LINHAS][COLUNAS], INIMIGO inimigos[], int *total) {
    *total = 0; // ponteiro que registra o total de inimigos
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (mapa[i][j] == 'E') { // percorre toda a matriz e onde encontra 'E'...

                inimigos[*total].linha = i; // define os campos de um inimigo no vetor inimigos
                inimigos[*total].coluna = j;
                inimigos[*total].direcao = 1;

                mapa[i][j] = '.'; // substitui o 'E' por '.' para facilitar as funcoes de movimentacao

                (*total)++; // acrescenta 1 ao total de inimigos
            }
        }
    }
}

// movimenta os inimigos
void movimentaInimigos(char mapa[LINHAS][COLUNAS], INIMIGO inimigos[], int total, int *contador){
    int novaColuna; // coluna atual + ou - 1 (depende da direcao)

    (*contador)++; // timer para desacelerar o movimento dos inimigos
    if (*contador >= 12){ // depois de 12 repeticoes do looping do jogo...

        // definicao dinamica da posicao dos inimigos
        for (int i = 0; i < total; i++) {
            int linha = inimigos[i].linha;
            int coluna = inimigos[i].coluna;

            // verifica chao
            if (linha < LINHAS - 1 && (mapa[linha + 1][coluna] == 'B' || mapa[linha + 1][coluna] == 'Z' || mapa[linha + 1][coluna] == 'H')) {

                novaColuna = coluna + inimigos[i].direcao;

                // verifica colisao ou borda
                if (novaColuna <= 0 || novaColuna >= COLUNAS - 1 ||
                    mapa[linha][novaColuna] == 'B' || mapa[linha][novaColuna] == 'Z' ||
                    !temChao(mapa[linha+1][novaColuna])) { // caso haja parede ou falta de chao na novaColuna,...

                    inimigos[i].direcao *= -1; // inverte direcao

                } else {
                    inimigos[i].coluna = novaColuna; // se nao houver buraco ou parede, segue em frente
                }
            }
        }
        *contador = 0; // reseta o timer
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

// desenha os inimigos na tela (sobrepoe o mapa)
void desenharInimigos(INIMIGO inimigos[], int total, Texture2D dir, Texture2D esq) {
    for (int i = 0; i < total; i++) {
        int x = inimigos[i].coluna * TILE_SIZE; // coordenada x do inimigo em pixels
        int y = inimigos[i].linha * TILE_SIZE; // coordenada y do inimigo em pixels

        if (inimigos[i].direcao == 1)
            DrawTexture(dir, x, y, WHITE); // desenha ele olhando para a direita
        else
            DrawTexture(esq, x, y, WHITE); // desenha ele olhando para a esquerda
    }
}

#endif
