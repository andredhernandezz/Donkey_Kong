#ifndef FUNCOES_MAPA_H
#define FUNCOES_MAPA_H

#include "raylib.h"
#include <stdio.h>
#include "constantes.h"

void carregarMapa(char mapa[LINHAS][COLUNAS], char *nomeArquivo) {

    FILE *f = fopen(nomeArquivo, "r");

    if (f == NULL) {
        printf("Erro ao abrir %s\n", nomeArquivo);
        return;
    }

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            fscanf(f, " %c", &mapa[i][j]);
        }
    }

    fclose(f);
}

// funcao que retorna uma condicao para verificar a existencia ou nao de chao em tal posicao
int temChao(char c) {
    return (c == 'B' || c == 'Z' || c == 'H');
}

#endif // FUNCOES
