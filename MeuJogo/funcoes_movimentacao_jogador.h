#ifndef FUNCOES_MOVIMENTACAO_JOGADOR_H
#define FUNCOES_MOVIMENTACAO_JOGADOR_H

#include "raylib.h"
#include <stdio.h>
#include "constantes.h"

// posicao do player
void encontrarPlayer(char mapa[LINHAS][COLUNAS], int *linha, int *coluna) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (mapa[i][j] == 'P') {
                *linha = i;
                *coluna = j;
            }
        }
    }
}

// movimentacao horizonatal do jogador
void moverHorizontal(char mapa[LINHAS][COLUNAS], int *linha, int *coluna) {

    if (IsKeyPressed(KEY_RIGHT)) {
        if (*linha < LINHAS - 1 &&
            (mapa[*linha + 1][*coluna] == 'Z' ||
             mapa[*linha + 1][*coluna] == 'B' ||
             mapa[*linha][*coluna] == 'D' ||
             mapa[*linha][*coluna] == 'H')) {

            (*coluna)++;
        }
    }

    if (IsKeyPressed(KEY_LEFT)) {
        if (*linha < LINHAS - 1 &&
            (mapa[*linha + 1][*coluna] == 'Z' ||
             mapa[*linha + 1][*coluna] == 'B' ||
             mapa[*linha][*coluna] == 'D' ||
             mapa[*linha][*coluna] == 'H')) {

            (*coluna)--;
        }
    }
}

// funcioamento das escadas
void moverVertical(char mapa[LINHAS][COLUNAS], int *linha, int coluna) {

    // subir
    if (IsKeyPressed(KEY_UP)) {
        if (mapa[*linha][coluna] == 'H' ||
            mapa[*linha][coluna] == 'S') {

            (*linha)--;
        }
    }

    // descer
    if (IsKeyPressed(KEY_DOWN)) {
        if (mapa[*linha][coluna] == 'H' ||
            mapa[*linha][coluna] == 'D') {

            (*linha)++;
        }
    }
}

// implementação da gravidade
void aplicarGravidade(char mapa[LINHAS][COLUNAS], int *linha, int coluna, int *contador) {

    (*contador)++; // conta os ciclos do loop

    if (*contador >= 7) { // apos 7 ciclos (frames) um movimento é feito
        if (mapa[*linha + 1][coluna] == '.' || mapa[*linha + 1][coluna] == 'P' || mapa[*linha + 1][coluna] == 'E') {
            (*linha)++;
        }
        *contador = 0; // recomeça o contador
    }
}

// impede que o player atravesse as bordas do mapa
void limitarBordas(int *coluna) {
    if (*coluna < 1) *coluna = 1;
    if (*coluna >= COLUNAS - 1) *coluna = COLUNAS - 2;
}

#endif

