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

    static int contadorMovX = 0;
    contadorMovX++;

    // clique responde na hora
    if (IsKeyPressed(KEY_RIGHT)) {
        p->coluna++;
        p->direcao = 1;
        contadorMovX = 0;
        return;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        p->coluna--;
        p->direcao = -1;
        contadorMovX = 0;
        return;
    }

    // so avança o contador quando estiver segurando a tecla
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) {
        contadorMovX++;
    } else {
        contadorMovX = 0; // zera o contador, caso a tecla seja solta
    }

    // segurar usa delay
    if (contadorMovX >= 16) {

        if (IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT)) {
            p->coluna++;
            p->direcao = 1;
        }

        else if (IsKeyPressed(KEY_LEFT)  || IsKeyDown(KEY_LEFT)) {
            p->coluna--;
            p->direcao = -1;
        }

        contadorMovX = 0;
    }
}

// funcionamento das escadas
void moverVertical(char mapa[LINHAS][COLUNAS], PLAYER *p) {

    static int contadorMovY=0;
    contadorMovY++;

    // clique responde na hora
    if (IsKeyPressed(KEY_UP) && (mapa[p->linha][p->coluna] == 'S' || mapa[p->linha][p->coluna] == 'H')) {
        p->linha--;
        contadorMovY=0;
        return;
    }
    if (IsKeyPressed(KEY_DOWN) && (mapa[p->linha][p->coluna] == 'D' || mapa[p->linha][p->coluna] == 'H')) {
        p->linha++;
        contadorMovY=0;
        return;
    }

    // so avança o contador quando estiver segurando a tecla
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)) &&
        (mapa[p->linha][p->coluna] == 'S' || mapa[p->linha][p->coluna] == 'H' || mapa[p->linha][p->coluna] == 'D')) {
        contadorMovY++;
    } else {
        contadorMovY = 0; // zera o contador, caso a tecla seja solta
    }

    if (contadorMovY > 20){

        // subir
        if (IsKeyPressed(KEY_UP) || IsKeyDown(KEY_UP)
            && (mapa[p->linha][p->coluna] == 'S' || mapa[p->linha][p->coluna] == 'H')) {
            if (mapa[p->linha][p->coluna] == 'H' ||
                mapa[p->linha][p->coluna] == 'S') {

                (p->linha)--;
            }
        }

        // descer
        else if (IsKeyPressed(KEY_DOWN)  || IsKeyDown(KEY_DOWN)
                 && (mapa[p->linha][p->coluna] == 'D' || mapa[p->linha][p->coluna] == 'H')) {
            if (mapa[p->linha][p->coluna] == 'H' ||
                     mapa[p->linha][p->coluna] == 'D') {

                (p->linha)++;
            }
        }
        contadorMovY=0;
    }
}

void pular(char mapa[LINHAS][COLUNAS],PLAYER *p){
    if (IsKeyPressed(KEY_SPACE)){
        if (mapa[p->linha + 1][p->coluna] == 'Z' || mapa[p->linha + 1][p->coluna] == 'B' || mapa[p->linha + 1][p->coluna] == 'H'){

            p->velY = -2;
        }
    }

}

// implementação da gravidade
void aplicarGravidade(char mapa[LINHAS][COLUNAS], PLAYER *p) {

    p->contadorGravidade++;

    if (p->contadorGravidade >= 7) {

        // SUBINDO
        if (p->velY < 0) {
            if (mapa[p->linha - 1][p->coluna] == '.') {
                p->linha--;
            } else {
                p->velY = 0; // bateu no teto
            }
        }

        // CAINDO
        else {
            if (mapa[p->linha + 1][p->coluna] == '.' ||mapa[p->linha + 1][p->coluna] == 'D' ||
                mapa[p->linha + 1][p->coluna] == 'P' ||mapa[p->linha + 1][p->coluna] == 'E' ) {

                p->linha++;
            } else {
                p->velY = 0; // está no chão
            }
        }

        // GRAVIDADE
        if (p->velY < 3)
            p->velY++;

        p->contadorGravidade = 0;
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
