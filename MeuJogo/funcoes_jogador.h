#ifndef FUNCOES_JOGADOR_H
#define FUNCOES_JOGADOR_H

#include "raylib.h"
#include <stdio.h>
#include "constantes.h"

typedef struct{
    int linha; // coordenada y
    int coluna; // coordenada x
    int velY; // velocidade vertical (usada para calculos de pulo e queda)
    int contadorGravidade; // contador de frames para calculo de gravidade
    int direcao; // caminhando para direita ou esquerda
} PLAYER;

// posicao inicial do player
void encontrarPlayer(char mapa[LINHAS][COLUNAS], PLAYER *p) {

    // percorre o mapa procurando o 'P'
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (mapa[i][j] == 'P') {
                // definicao dos campos do jogador
                p->linha = i;
                p->coluna = j;
            }
        }
    }
    p->direcao = 1; // inicializa olhando para a direita
    p->velY = 0; // nao esta caindo nem subindo
    p->contadorGravidade = 0; // zera o controle de gravidade
}

// movimentacao horizonatal do jogador
void moverHorizontal(char mapa[LINHAS][COLUNAS], PLAYER *p) {

    static int contadorMovX = 0; // controlador da velocidade X
    contadorMovX++;

    // clique responde na hora
    if (IsKeyPressed(KEY_RIGHT)) {
        p->coluna++; // jogador move-se para a direita
        p->direcao = 1; // jogador esta virado para a direita
        contadorMovX = 0;
        return;
    }

    if (IsKeyPressed(KEY_LEFT)) {
        p->coluna--; // jogador move-se para a esquerda
        p->direcao = -1; // jogador esta virado para a esquerda
        contadorMovX = 0;
        return;
    }

    // so incrementa o contador quando estiver segurando a tecla
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) {
        contadorMovX++;
    } else {
        contadorMovX = 0; // zera o contador, caso a tecla seja solta
    }

    // movimento continuo com delay
    if (contadorMovX >= 16) { // depois de 16 frames segurando a tecla, se movimenta

        if (IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT)) {
            p->coluna++;
            p->direcao = 1;
        }

        else if (IsKeyPressed(KEY_LEFT)  || IsKeyDown(KEY_LEFT)) {
            p->coluna--;
            p->direcao = -1;
        }

        contadorMovX = 0; // zera o contador
    }
}

// funcionamento das escadas
void moverVertical(char mapa[LINHAS][COLUNAS], PLAYER *p) {

    static int contadorMovY=0; // controlador da velocidade Y
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

    // so incrementa o contador quando estiver segurando a tecla
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)) &&
        (mapa[p->linha][p->coluna] == 'S' || mapa[p->linha][p->coluna] == 'H' || mapa[p->linha][p->coluna] == 'D')) {
        contadorMovY++; // somente movimenta verticalmente nas escadas
    } else {
        contadorMovY = 0; // caso a tecla seja solta, zera o contador
    }

    if (contadorMovY > 20){ // apos 20 frames segurando a tecla, se movimenta

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
        contadorMovY=0; // zera o contador
    }
}

// funcao de pulo
void pular(char mapa[LINHAS][COLUNAS],PLAYER *p){
    if (IsKeyPressed(KEY_SPACE)){
        // so pula se tiver em cima de algum tipo de chao
        if (mapa[p->linha + 1][p->coluna] == 'Z' || mapa[p->linha + 1][p->coluna] == 'B'
            || mapa[p->linha + 1][p->coluna] == 'H'){

            p->velY = -2; // velocidade negativa faz subir
        }
    }

}

// implementação da gravidade
void aplicarGravidade(char mapa[LINHAS][COLUNAS], PLAYER *p) {

    p->contadorGravidade++;

    // so atualiza a cada 7 frames
    if (p->contadorGravidade >= 7) {

        // dinamica do pulo
        if (p->velY < 0) {

            if (mapa[p->linha - 1][p->coluna] == '.') {
                p->linha--; // so pula se nao ouver nada em cima
            }
            else {
                p->velY = 0; // bateu no teto, para de subir
            }
        }

        // dinamica de queda
        else {
            // se nao tem nada embaixo, cai
            if (mapa[p->linha + 1][p->coluna] == '.' ||mapa[p->linha + 1][p->coluna] == 'D' ||
                mapa[p->linha + 1][p->coluna] == 'P' ||mapa[p->linha + 1][p->coluna] == 'E' ) {

                p->linha++;
            }
            // encostou no chao, para de cair
            else {
                p->velY = 0;
            }
        }

        // gravidade
        // aumenta velY ("puxa" para baixo)
        if (p->velY < 3)
            p->velY++;

        p->contadorGravidade = 0; // reseta o contador
    }
}

// impede que o player atravesse as bordas do mapa
void limitarBordas(PLAYER *p) {
    if (p->coluna < 1) p->coluna = 1;

    if (p->coluna >= COLUNAS - 1) p->coluna = COLUNAS - 2;
}

// desenha o player
void desenharPlayer(PLAYER p, Texture2D dir, Texture2D esq) {

    if (p.direcao == 1) // se direcao positiva, desenha olhando para a direita
        DrawTexture(dir, p.coluna * TILE_SIZE, p.linha * TILE_SIZE, WHITE);
    else // caso contrario, desenha olhando pra esquerda
        DrawTexture(esq, p.coluna * TILE_SIZE, p.linha * TILE_SIZE, WHITE);

}

#endif
