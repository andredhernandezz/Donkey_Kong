#ifndef PLACAR_H
#define PLACAR_H

#include "raylib.h"
#include <stdio.h>
#include "constantes.h"

typedef struct tipo_placar {
    char nome[20]; // nome do player
    int time; // ou score, dependendo da sua escolha
} TIPO_PLACAR;

// salva resultado da jogada no "placar.bin"
void salvarResultadoNoPlacar(const char *nome, int tempo)
{
    TIPO_PLACAR placar[11];
    int qtd = 0; // quantidade de registros

    // tenta ler o arquivo que possa existir
    FILE *arq = fopen("placar.bin", "rb");
    if (arq != NULL)
    {
        // le ate 10 registros do arquivo e salva no vetor "placar"
        qtd = fread(placar, sizeof(TIPO_PLACAR), 10, arq);
        fclose(arq);
    }

    // adiciona o jogador na primeira posicao livre do array
    TextCopy(placar[qtd].nome, nome);
    placar[qtd].time = tempo;
    qtd++; // adiciona 1 a quantidade de jogadores

    // ordena o ranking por menor tempo usando bubble sort
    for (int i=0; i < qtd; i++){
        for (int j = i + 1; j < qtd; j++){
            if (placar[i].time > placar[j].time){
                TIPO_PLACAR aux = placar[i];
                placar[i]=placar[j];
                placar[j] = aux;
            }
        }
    }

    // mantem registrados apenas os 10 primeiros colocados
    if (qtd > 10){
        qtd = 10;
    }
    // salva os dados no arquivo
    arq = fopen("placar.bin", "wb");
    if (arq != NULL){
        fwrite(placar, sizeof(TIPO_PLACAR), qtd, arq);
        fclose(arq);
    }
}
// funcao que limpa o placar
void limparPlacar(void) {
    remove("placar.bin"); // exclui o arquivo
}

// funcao para veririfcar se o tempo do jogador esta no top 10 do ranking
int deveEntrarNoRanking(int tempo){
    TIPO_PLACAR placar[10];
    int qtd = 0;

    // tenta ler o ranking existente
    FILE *arq = fopen("placar.bin", "rb");
    if (arq != NULL){
        qtd = fread(placar, sizeof(TIPO_PLACAR), 10, arq);
        fclose(arq);
    }
    // se nao tiver 10 jogadores registrados, deve entrar.
    if (qtd < 10){
        return 1;
    }
    // expressao que verifica se o tempo foi menor do que o tempo do ultimo colocado
    return tempo < placar[qtd-1].time;  // eh possivel usar [qtd-1], pois o vetor ja esta ordenado
}


#endif
