#include "raylib.h"
#include "button.h"
#include <stdio.h>
#include <stdlib.h>
#include "funcoes_desenha_jogo.h"
#include "funcoes_fase.h"
#include "funcoes_inimigos.h"
#include "funcoes_mapa.h"
#include "funcoes_movimentacao_jogador.h"
#include "constantes.h"

typedef enum
{
    TELA_MENU,
    TELA_JOGO,
    TELA_PAUSA,
    TELA_RANKING,
} EstadoJogo;

int main(void)
{
    //inicializa a janela com tamanho 750x750 com o titulo "Donkey Kong - INF"
    InitWindow(COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, "Donkey Kong - INF");
    //determina os frames per second da tela, nesse caso 60
    SetTargetFPS(60);

    InitAudioDevice();
    Music musicaMenu  = LoadMusicStream("audio/menu.mp3");
    Music musicaFase1 = LoadMusicStream("audio/fase1.mp3");
    Music musicaFase2 = LoadMusicStream("audio/fase2.mp3");
    PlayMusicStream(musicaMenu);

    // carregando a imagem da capa do menu como a variavel background
    Texture2D background = LoadTexture("graphics/capa_menu.png");

    // TESTE

    // APENAS TEXTE MARIO
    Texture2D spriteMario = LoadTexture("graphics/mario.png");

    Texture2D spriteEstrutura = LoadTexture("graphics/estrutura.png");

    Texture2D spriteEscada = LoadTexture("graphics/escada.png");

    Texture2D spriteInimigo = LoadTexture("graphics/enemy.png");




    // TESTE
    char mapa1[LINHAS][COLUNAS]; // matriz para carregar o mapa 1
    char mapa2[LINHAS][COLUNAS]; // matriz para carregar o mapa 2
    char (*mapaAtual)[COLUNAS]; // mapa da fase atual
    mapaAtual = mapa1;

    int playerLinha = 0, playerColuna = 0; // indices da posicao do player
    int conta_frames_gravidade = 0; // auxiliar para calcular a velocidade da grav.
    int conta_frames_inimigo = 0; // auxiliar para calcular a velocidade dos inimigos.
    int gameOver = 0; // flag que indica fim de jogo ou nao
    int timerGameOver = 0; // timer de duracao da tela de game over
    int ganhou=0; // flag que indica se o player ganhou
    int timerGanhou = 0; // timer de duracao da tela de vitoria

    int fase = 1;
    int trocandoFase = 0;
    int timerTrocaFase = 0;
    int proximaFase = 0;

    int inimigoLinha[MAX_INIMIGOS]; // define a linha em que o inimigo se encontra
    int inimigoColuna[MAX_INIMIGOS]; // define a coluna em que o inimigo se encontra
    int inimigoDirecao[MAX_INIMIGOS]; // define se o inimigo vai para a direita ou esquerda
    int totalInimigos;



//  Botoes do menu
    Botao btnNovo;
    btnNovo.rect = (Rectangle)
    {
        BOTAO_X, POS_Y_NOVO_JOGO, LARGURA_BOTAO, ALTURA_BOTAO
    };
    btnNovo.texture = LoadTexture("graphics/novo_jogo_menu.png");
    btnNovo.ativo = true;

    Botao btnRanking;
    btnRanking.rect = (Rectangle)
    {
        BOTAO_X, POS_Y_RANKING, LARGURA_BOTAO, ALTURA_BOTAO
    };
    btnRanking.texture = LoadTexture("graphics/ranking_menu.png");
    btnRanking.ativo = true;

    Botao btnSair;
    btnSair.rect = (Rectangle)
    {
        BOTAO_X, POS_Y_SAIR, LARGURA_BOTAO, ALTURA_BOTAO
    };
    btnSair.texture = LoadTexture("graphics/sair_menu.png");
    btnSair.ativo = true;

    // definindo em qual tela o jogo comeca (menu):
    EstadoJogo telaAtual = TELA_MENU;

    // enquanto ESC nao for pressionada ou nao for clicado no x para fechar a janela, continuara executando o programa em looping
    while (!WindowShouldClose())
    {
        Vector2 mousePos = GetMousePosition();

        //definindo a logica de atualizacao:

        switch (telaAtual)
        {

        case TELA_MENU:
            if (CheckCollisionPointRec(mousePos, btnNovo.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // zera tudo ao iniciar novo jogo
                fase = 1;
                gameOver = 0;
                ganhou = 0;
                timerGameOver = 0;
                timerGanhou = 0;
                trocandoFase = 0;
                timerTrocaFase = 0;
                conta_frames_gravidade = 0;
                conta_frames_inimigo = 0;

                // carrega mapa e encontra player/inimigos
                carregarMapa(mapa1, "mapa1.txt");
                mapaAtual = mapa1;
                encontrarPlayer(mapaAtual, &playerLinha, &playerColuna);
                encontrarInimigos(mapaAtual, inimigoLinha, inimigoColuna, inimigoDirecao, &totalInimigos);

                StopMusicStream(musicaMenu);
                PlayMusicStream(musicaFase1);

                telaAtual = TELA_JOGO;
            }
            if (CheckCollisionPointRec(mousePos, btnRanking.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                telaAtual = TELA_RANKING;
            if (CheckCollisionPointRec(mousePos, btnSair.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                UnloadTexture(spriteMario);
                UnloadTexture(background);
                UnloadTexture(btnNovo.texture);
                UnloadTexture(btnRanking.texture);
                UnloadTexture(btnSair.texture);
                UnloadTexture(spriteEscada);
                UnloadTexture(spriteEstrutura);
                UnloadTexture(spriteInimigo);
                return 0;
            }
            break;

        case TELA_JOGO:
            // pausa
            if (IsKeyPressed(KEY_TAB))
            {
                telaAtual = TELA_PAUSA;
                break;
            }

            // tela de game over (só conta o timer, depois volta ao menu)
            if (gameOver)
            {
                timerGameOver++;
                if (timerGameOver > FPS * 3)
                {
                    StopMusicStream(musicaFase1);
                    StopMusicStream(musicaFase2);
                    PlayMusicStream(musicaMenu);
                    telaAtual = TELA_MENU;
                }
                break;
            }
            // tela de vitoria (só conta o timer, depois volta ao menu)
            if (ganhou)
            {
                timerGanhou++;
                if (timerGanhou > FPS * 3)
                {
                    StopMusicStream(musicaFase1);
                    StopMusicStream(musicaFase2);
                    PlayMusicStream(musicaMenu);
                    telaAtual = TELA_MENU;
                }
                break;
            }

            // troca de fase (animacao de carregamento)
            if (trocandoFase)
            {
                timerTrocaFase++;
                if (timerTrocaFase > FPS * 0.6f)
                {
                    fase = proximaFase;
                    if (fase == 1)
                    {
                        carregarMapa(mapa1, "mapa1.txt");
                        mapaAtual = mapa1;
                        playerLinha = 1;
                        playerColuna = 16;
                        StopMusicStream(musicaFase2);
                        PlayMusicStream(musicaFase1);
                    }
                    else if (fase == 2) {
                        carregarMapa(mapa2, "mapa2.txt");
                        mapaAtual = mapa2;
                        encontrarPlayer(mapaAtual, &playerLinha, &playerColuna);
                        StopMusicStream(musicaFase1);  // ← adiciona
                        PlayMusicStream(musicaFase2);
                    }
                    encontrarInimigos(mapaAtual, inimigoLinha, inimigoColuna, inimigoDirecao, &totalInimigos);
                    conta_frames_gravidade = 0;
                    conta_frames_inimigo   = 0;
                    trocandoFase = 0;
                }
                break;
            }

            // movimentação normal
            moverHorizontal(mapaAtual, &playerLinha, &playerColuna);
            moverVertical(mapaAtual, &playerLinha, playerColuna);
            aplicarGravidade(mapaAtual, &playerLinha, playerColuna, &conta_frames_gravidade);
            movimentaInimigos(mapaAtual, inimigoLinha, inimigoColuna, inimigoDirecao, totalInimigos, &conta_frames_inimigo);
            limitarBordas(&playerColuna);

            // troca de fase pelas bordas
            if (fase == 1 && passouFase(playerLinha))
            {
                proximaFase = 2;
                trocandoFase = 1;
                timerTrocaFase = 0;
            }
            if (fase == 2 && voltouFase(playerLinha))
            {
                proximaFase = 1;
                trocandoFase = 1;
                timerTrocaFase = 0;
            }

            // colisao e vitoria
            if (colisaoInimigo(playerLinha, playerColuna, inimigoLinha, inimigoColuna, totalInimigos))
                gameOver = 1;
            if (vitoria(mapaAtual, playerLinha, playerColuna))
                ganhou = 1;
            break;

        case TELA_PAUSA:
            // TODO: lógica dos botões de pausa (Continuar / Menu / Sair)
            if (IsKeyPressed(KEY_TAB)) telaAtual = TELA_JOGO;
            if (IsKeyPressed(KEY_M))   telaAtual = TELA_MENU;
            if (IsKeyPressed(KEY_S))
            {
                UnloadTexture(spriteMario);
                UnloadTexture(background);
                UnloadTexture(btnNovo.texture);
                UnloadTexture(btnRanking.texture);
                UnloadTexture(btnSair.texture);
                return 0;
            }
            break;

        case TELA_RANKING:
            // TODO: exibir placar.bin
            if (IsKeyPressed(KEY_M)) telaAtual = TELA_MENU;
            break;
        }

        if (telaAtual == TELA_MENU)
            UpdateMusicStream(musicaMenu);
        else if (fase == 1)
            UpdateMusicStream(musicaFase1);
        else if (fase == 2)
            UpdateMusicStream(musicaFase2);


        //iniciar desenho
        BeginDrawing();
        ClearBackground(BLACK);

        switch (telaAtual)
        {

        case TELA_MENU:
            DrawTexture(background, 0, 0, WHITE);
            DrawTexture(btnNovo.texture, btnNovo.rect.x, btnNovo.rect.y,
                        CheckCollisionPointRec(mousePos, btnNovo.rect) ? GRAY : WHITE);
            DrawTexture(btnRanking.texture, btnRanking.rect.x, btnRanking.rect.y,
                        CheckCollisionPointRec(mousePos, btnRanking.rect) ? GRAY : WHITE);
            DrawTexture(btnSair.texture, btnSair.rect.x, btnSair.rect.y,
                        CheckCollisionPointRec(mousePos, btnSair.rect) ? GRAY : WHITE);
            break;

        case TELA_JOGO:
            ClearBackground(BLACK);
            desenharMapa(mapaAtual, spriteEstrutura, spriteEscada, spriteInimigo);
            desenharPlayer(playerLinha, playerColuna, spriteMario);
            desenharInimigos(inimigoLinha, inimigoColuna, totalInimigos, spriteInimigo);

            if (trocandoFase)
            {
                DrawRectangle(0, 0, COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, DARKBLUE);
                DrawText("Carregando...", COLUNAS * TILE_SIZE / 2 - 120, LINHAS * TILE_SIZE / 2, 40, WHITE);
            }
            if (gameOver)
            {
                DrawRectangle(0, 0, COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, BLACK);
                DrawText("GAME OVER", COLUNAS * TILE_SIZE / 2 - 120, LINHAS * TILE_SIZE / 2, 40, RED);
            }
            if (ganhou)
            {
                DrawRectangle(0, 0, COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, BLUE);
                DrawText("VOCE GANHOU!", COLUNAS * TILE_SIZE / 2 - 120, LINHAS * TILE_SIZE / 2, 40, WHITE);
            }
            break;

        case TELA_PAUSA:
            // TODO: desenhar botões de pausa
            ClearBackground(BLACK);
            DrawText("PAUSADO", 275, 150, 40, WHITE);
            DrawText("TAB - Continuar\nM - Menu\nS - Sair\n", COLUNAS * TILE_SIZE / 2 - 160, LINHAS * TILE_SIZE / 2 + 20, 20, LIGHTGRAY);
            break;

        case TELA_RANKING:
            // TODO: exibir placar.bin
            DrawText("RANKING", 100, 200, 40, GOLD);
            DrawText("(em breve)", 100, 260, 20, LIGHTGRAY);
            DrawText("M - Voltar ao menu", 100, 350, 40, RAYWHITE);
            break;
        }
        EndDrawing();
    }

    UnloadTexture(spriteMario);
    UnloadTexture(background);
    UnloadTexture(btnNovo.texture);
    UnloadTexture(btnRanking.texture);
    UnloadTexture(btnSair.texture);
    UnloadTexture(spriteEscada);
    UnloadTexture(spriteEstrutura);
    UnloadTexture(spriteInimigo);
    UnloadMusicStream(musicaMenu);
    UnloadMusicStream(musicaFase1);
    UnloadMusicStream(musicaFase2);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
