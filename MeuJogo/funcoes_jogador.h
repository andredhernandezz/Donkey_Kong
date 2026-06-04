#ifndef FUNCOES_JOGADOR_H
#define FUNCOES_JOGADOR_H

#include "raylib.h"
#include <stdio.h>
#include "constantes.h"


typedef struct{
    int linha;
    int coluna;
    int velY;
    int contadorGravidade;
    int direcao;
} PLAYER;

// posicao do player
void encontrarPlayer(char mapa[LINHAS][COLUNAS], PLAYER *p) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (mapa[i][j] == 'P') {
                p->linha = i;
                p->coluna = j;
            }
        }
    }

    p->direcao = 1;
    p->velY = 0;
    p->contadorGravidade = 0;
}

// movimentacao horizonatal do jogador
void moverHorizontal(char mapa[LINHAS][COLUNAS], PLAYER *p) {

    if (IsKeyPressed(KEY_RIGHT)) {
        if (p->linha < LINHAS - 1 &&
            (mapa[p->linha + 1][p->coluna] == 'Z' ||
             mapa[p->linha + 1][p->coluna] == 'B' ||
             mapa[p->linha][p->coluna] == 'D' ||
             mapa[p->linha][p->coluna] == 'H')) {

            p->coluna++;
            p->direcao = 1;
        }
    }

    if (IsKeyPressed(KEY_LEFT)) {
        if (p->linha < LINHAS - 1 &&
            (mapa[p->linha + 1][p->coluna] == 'Z' ||
             mapa[p->linha + 1][p->coluna] == 'B' ||
             mapa[p->linha][p->coluna] == 'D' ||
             mapa[p->linha][p->coluna] == 'H')) {

            p->coluna--;
            p->direcao = -1;
        }
    }
}

// funcioamento das escadas
void moverVertical(char mapa[LINHAS][COLUNAS], PLAYER *p) {

    // subir
    if (IsKeyPressed(KEY_UP)) {
        if (mapa[p->linha][p->coluna] == 'H' ||
            mapa[p->linha][p->coluna] == 'S') {

            (p->linha)--;
        }
    }

    // descer
    if (IsKeyPressed(KEY_DOWN)) {
        if (mapa[p->linha][p->coluna] == 'H' ||
            mapa[p->linha][p->coluna] == 'D') {

            (p->linha)++;
        }
    }
}

// implementação da gravidade
void aplicarGravidade(char mapa[LINHAS][COLUNAS], PLAYER *p) {

    (p->contadorGravidade)++; // conta os ciclos do loop

    if (p->contadorGravidade >= 7) { // apos 7 ciclos (frames) um movimento é feito
        if (mapa[p->linha + 1][p->coluna] == '.' || mapa[p->linha + 1][p->coluna] == 'D' ||
            mapa[p->linha + 1][p->coluna] == 'P' || mapa[p->linha + 1][p->coluna] == 'E') {
            (p->linha)++;
        }
        p->contadorGravidade = 0; // recomeça o contador
    }
}

// impede que o player atravesse as bordas do mapa
void limitarBordas(PLAYER *p) {
    if (p->coluna < 1) p->coluna = 1;

    if (p->coluna >= COLUNAS - 1) p->coluna = COLUNAS - 2;
}

// desenha o player
void desenharPlayer(PLAYER p, Texture2D dir, Texture2D esq) {

    if (p.direcao == 1)
        DrawTexture(dir, p.coluna * TILE_SIZE, p.linha * TILE_SIZE, WHITE);
    else
        DrawTexture(esq, p.coluna * TILE_SIZE, p.linha * TILE_SIZE, WHITE);

}

#endif
