#ifndef FUNCOES_FASE_H
#define FUNCOES_FASE_H

#include "raylib.h"
#include <stdio.h>
#include "constantes.h"

// funcao que verifica se o player passou de fase
int passouFase(int playerLinha){
    if (playerLinha == 0 && IsKeyPressed(KEY_UP)) {
        return 1;
    }

    return 0;
}

// funcao que verifica se o player voltou pra fase anterior
int voltouFase(int playerLinha){
    if (playerLinha == 28 && IsKeyPressed(KEY_DOWN)){
        return 1;
    }

    return 0;
}

// funcao que verifica se o jogador chegou ao fim do jogo
int vitoria(char mapa[LINHAS][COLUNAS], int playerLinha, int playerColuna){
    if (mapa[playerLinha][playerColuna] == 'F'){
        return 1;
    }
    return 0;
}

#endif
