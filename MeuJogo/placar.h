#ifndef PLACAR_H
#define PLACAR_H

#include "raylib.h"
#include <stdio.h>
#include "constantes.h"

typedef struct tipo_placar {
    char nome[20];
    int time; // ou score, dependendo da sua escolha
} TIPO_PLACAR;


void salvarResultadoNoPlacar(const char *nome, int tempo)
{
    TIPO_PLACAR placar[11];
    int qtd = 0;

    // TENTA LER O ARQUIVO QUE POSSA EXISTIR
    FILE *arq = fopen("placar.bin", "rb");
    if (arq != NULL)
    {
        qtd = fread(placar, sizeof(TIPO_PLACAR), 10, arq);
        fclose(arq);
    }

    // ADICIONA O JOGADOR NO FINAL DO ARRAY
    TextCopy(placar[qtd].nome, nome);
    placar[qtd].time = tempo;
    qtd++;

    // ORDENA O RANKING (COM BUBBLE SORT)

    for (int i=0; i < qtd; i++){
        for (int j = i + 1; j < qtd; j++){
            if (placar[i].time > placar[j].time){
                TIPO_PLACAR aux = placar[i];
                placar[i]=placar[j];
                placar[j] = aux;
            }
        }
    }

    if (qtd > 10){
        qtd = 10;
    }
    // ATUALIZA OS DADOS ATUALIZADOS NO ARQUIVO
    arq = fopen("placar.bin", "wb");
    if (arq != NULL){
        fwrite(placar, sizeof(TIPO_PLACAR), qtd, arq);
        fclose(arq);
    }
}
// FUNCAO PARA LIMPAR O PLACAR
void limparPlacar(void) {
    remove("placar.bin");
}

// FUNCAO PARA VERIFICAR SE O TEMPO DO JOGADOR ESTA NO TOP 10 DO RANKING
int deveEntrarNoRanking(int tempo){
    TIPO_PLACAR placar[10];
    int qtd = 0;
    FILE *arq = fopen("placar.bin", "rb");
    if (arq != NULL){
        qtd = fread(placar, sizeof(TIPO_PLACAR), 10, arq);
        fclose(arq);
    }

    if (qtd < 10){
        return 1;
    }
    return tempo < placar[qtd-1].time;


}


#endif
