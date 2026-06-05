#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

#define LARGURA_BOTAO 300
#define ALTURA_BOTAO 80
#define BOTAO_X 225

#define POS_Y_NOVO_JOGO 225
#define POS_Y_RANKING   325
#define POS_Y_SAIR      425

typedef struct {
    Rectangle rect;
    Texture2D texture;
} Botao;

#endif

