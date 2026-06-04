#include "raylib.h"
#include "button.h"
#include <stdio.h>
#include <stdlib.h>
#include "funcoes_desenha_jogo.h"
#include "funcoes_fase.h"
#include "funcoes_inimigos.h"
#include "funcoes_mapa.h"
#include "funcoes_jogador.h"
#include "constantes.h"
#include "placar.h"


typedef enum
{
    TELA_MENU,
    TELA_JOGO,
    TELA_PAUSA,
    TELA_RANKING,
    TELA_INPUT_NOME
} EstadoJogo;

int main(void)
{
    //inicializa a janela com tamanho 750x750 com o titulo "Donkey Kong - INF"
    InitWindow(COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, "Donkey Kong - INF");
    //determina os frames per second da tela, nesse caso 60
    SetTargetFPS(60);
    // coloca o jogo em tela cheia

    // CARREGAMENTO DAS MUSICAS SALVAS NA PASTA AUDIO
    InitAudioDevice();
    Music musicaMenu  = LoadMusicStream("audio/menu.mp3");
    Music musicaFase1 = LoadMusicStream("audio/fase1.mp3");
    Music musicaFase2 = LoadMusicStream("audio/fase2.mp3");
    Music musicaFase3 = LoadMusicStream("audio/fase3.mp3");
    PlayMusicStream(musicaMenu);

    // CARREGAMENTO DAS IMAGENS SALVAS NA PASTA GRAPHICS
    Texture2D background = LoadTexture("graphics/capa_menu.png");
    Texture2D spriteMarioDir = LoadTexture("graphics/mario-dir.png");
    Texture2D spriteMarioEsq = LoadTexture("graphics/mario-esq.png");
    Texture2D spriteEstrutura = LoadTexture("graphics/estrutura.png");
    Texture2D spriteEscada = LoadTexture("graphics/escada.png");
    Texture2D spriteInimigoDir = LoadTexture("graphics/enemy-dir.png");
    Texture2D spriteInimigoEsq = LoadTexture("graphics/enemy-esq.png");
    Texture2D spritePortaBaixo = LoadTexture("graphics/porta-baixo.png");
    Texture2D spritePortaCima = LoadTexture("graphics/porta-cima.png");


    // PARA EXIBIR O TEMPO
    float tempoDeJogo = 0.0f;
    char nomeJogador[20] = "\0";
    int letrasContadas = 0;

    // INICIALIZACAO DAS VARIAVEIS NECESSARIAS PARA O JOGO
    char mapa1[LINHAS][COLUNAS]; // matriz para armazenar o mapa 1
    char mapa2[LINHAS][COLUNAS]; // matriz para armazenar o mapa 2
    char mapa3[LINHAS][COLUNAS]; // matriz para armazenar o mapa 3
    char (*mapaAtual)[COLUNAS]; // mapa da fase atual
    mapaAtual = mapa1;

    PLAYER player;
    int gameOver = 0; // flag que indica fim de jogo ou nao
    int timerGameOver = 0; // timer de duracao da tela de game over
    int ganhou=0; // flag que indica se o player ganhou
    int timerGanhou = 0; // timer de duracao da tela de vitoria

    int fase = 1; // fase atual
    int trocandoFase = 0; // flag que indica se a fase esta trocando
    int timerTrocaFase = 0; // temporizador que define a duracao da tela de troca de fase
    int proximaFase = 0; // indicador de qual sera a proxima fase
    int faseAnterior = 0; // indicador de qual era a fase anterior

    INIMIGO inimigos[MAX_INIMIGOS]; // vetor que armazena inimigos
    int totalInimigos=0; // contador de quantos inimigos tem
    int conta_frames_inimigo = 0; // auxiliar para calcular a velocidade dos inimigos.



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

        // CLICAR F1 PARA COLOCAR EM TELA CHEIA
        if (IsKeyPressed(KEY_F1))
        {
            ToggleFullscreen();
        }
        switch (telaAtual)
        {

        case TELA_MENU:
            if (CheckCollisionPointRec(mousePos, btnNovo.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // zera tudo ao iniciar novo jogo
                tempoDeJogo = 0.0f;
                fase = 1;
                gameOver = 0;
                ganhou = 0;
                timerGameOver = 0;
                timerGanhou = 0;
                trocandoFase = 0;
                timerTrocaFase = 0;
                player.contadorGravidade = 0;
                conta_frames_inimigo = 0;

                // carrega mapa e encontra player/inimigos
                carregarMapa(mapa1, "mapa1.txt");
                mapaAtual = mapa1;
                encontrarPlayer(mapaAtual, &player);
                encontrarInimigos(mapaAtual, inimigos, &totalInimigos);

                StopMusicStream(musicaMenu);
                PlayMusicStream(musicaFase1);

                telaAtual = TELA_JOGO;
            }
            if (CheckCollisionPointRec(mousePos, btnRanking.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                telaAtual = TELA_RANKING;
            if (CheckCollisionPointRec(mousePos, btnSair.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                UnloadTexture(spriteMarioDir);
                UnloadTexture(spriteMarioEsq);
                UnloadTexture(background);
                UnloadTexture(btnNovo.texture);
                UnloadTexture(btnRanking.texture);
                UnloadTexture(btnSair.texture);
                UnloadTexture(spriteEscada);
                UnloadTexture(spriteEstrutura);
                UnloadTexture(spriteInimigoDir);
                UnloadTexture(spriteInimigoEsq);
                return 0;
            }
            break;

        case TELA_JOGO:
            // CONTROLE DE TEMPO
            if (!gameOver && !ganhou && !trocandoFase)
            {
                tempoDeJogo += GetFrameTime();
            }
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
                    StopMusicStream(musicaFase3);
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
                    StopMusicStream(musicaFase3);
                    PlayMusicStream(musicaMenu);
                    nomeJogador[0] = '\0';
                    letrasContadas = 0;
                    // Vai para a tela de registro
                    telaAtual = TELA_INPUT_NOME;

                }
                break;
            }

            // troca de fase (animacao de carregamento)
            if (trocandoFase)
            {
                timerTrocaFase++;
                if (timerTrocaFase > FPS * 0.6f)
                {
                    faseAnterior = fase;
                    fase = proximaFase;
                    if (fase == 1)
                    {
                        carregarMapa(mapa1, "mapa1.txt");
                        mapaAtual = mapa1;
                        player.linha = 1;
                        player.coluna = 16;
                        StopMusicStream(musicaFase2);
                        PlayMusicStream(musicaFase1);
                    }
                    else if (fase == 2)
                    {
                        carregarMapa(mapa2, "mapa2.txt");
                        mapaAtual = mapa2;
                        if (faseAnterior == 1)
                            encontrarPlayer(mapaAtual, &player);
                        else if (faseAnterior == 3)
                        {
                            player.linha = 2;
                            player.coluna = 14;
                        }
                        StopMusicStream(musicaFase1);  // ← adiciona
                        PlayMusicStream(musicaFase2);
                    }
                    else if (fase == 3)
                    {
                        carregarMapa(mapa3, "mapa3.txt");
                        mapaAtual = mapa3;
                        encontrarPlayer(mapaAtual, &player);
                        StopMusicStream(musicaFase2);
                        PlayMusicStream(musicaFase3);
                    }
                    encontrarInimigos(mapaAtual, inimigos, &totalInimigos);
                    player.contadorGravidade = 0;
                    conta_frames_inimigo   = 0;
                    trocandoFase = 0;

                }
                break;
            }

            // movimentação normal
            moverHorizontal(mapaAtual, &player);
            moverVertical(mapaAtual, &player);
            pular(mapaAtual, &player);
            aplicarGravidade(mapaAtual, &player);
            movimentaInimigos(mapaAtual, inimigos, totalInimigos, &conta_frames_inimigo);
            limitarBordas(&player);

            // troca de fase pelas bordas
            if (fase == 1 && passouFase(player.linha))
            {
                proximaFase = 2;
                trocandoFase = 1;
                timerTrocaFase = 0;
            }
            if (fase == 2 && voltouFase(player.linha))
            {
                proximaFase = 1;
                trocandoFase = 1;
                timerTrocaFase = 0;
            }
            if (fase == 2 && passouFase(player.linha))
            {
                proximaFase = 3;
                trocandoFase = 1;
                timerTrocaFase = 0;
            }
            if (fase == 3 && voltouFase(player.linha))
            {
                proximaFase = 2;
                trocandoFase = 1;
                timerTrocaFase = 0;
            }

            // colisao e vitoria
            if (colisaoInimigo(player.linha, player.coluna, inimigos, totalInimigos))
                gameOver = 1;
            if (vitoria(mapaAtual, player.linha, player.coluna))
                ganhou = 1;
            break;

        case TELA_PAUSA:
            // TODO: lógica dos botões de pausa (Continuar / Menu / Sair)
            if (IsKeyPressed(KEY_TAB)) telaAtual = TELA_JOGO;
            if (IsKeyPressed(KEY_M))   telaAtual = TELA_MENU;
            if (IsKeyPressed(KEY_S))
            {
                UnloadTexture(spriteMarioDir);
                UnloadTexture(spriteMarioEsq);
                UnloadTexture(background);
                UnloadTexture(btnNovo.texture);
                UnloadTexture(btnRanking.texture);
                UnloadTexture(btnSair.texture);
                UnloadTexture(spriteEscada);
                UnloadTexture(spriteEstrutura);
                UnloadTexture(spriteInimigoDir);
                UnloadTexture(spriteInimigoEsq);
                UnloadMusicStream(musicaMenu);
                UnloadMusicStream(musicaFase1);
                UnloadMusicStream(musicaFase2);
                UnloadMusicStream(musicaFase3);
                CloseAudioDevice();
                return 0;
            }
            break;

        case TELA_RANKING:
            if(!(IsMusicStreamPlaying(musicaMenu)))
                    PlayMusicStream(musicaMenu);

            if (IsKeyPressed(KEY_M)){
                telaAtual = TELA_MENU;
            }
            break;

        case TELA_INPUT_NOME:
            // Captura de texto pela Raylib
            int chave = GetCharPressed();
            while (chave > 0)
            {
                if ((chave >= 32) && (chave <= 125) && (letrasContadas < 19))
                {
                    nomeJogador[letrasContadas] = (char)chave;
                    nomeJogador[letrasContadas + 1] = '\0';
                    letrasContadas++;
                }
                chave = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letrasContadas--;
                if (letrasContadas < 0) letrasContadas = 0;
                nomeJogador[letrasContadas] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER) && letrasContadas > 0)
            {
                // Aqui chamamos a função para salvar no placar
                salvarResultadoNoPlacar(nomeJogador, (int)tempoDeJogo);

                StopMusicStream(musicaFase1);
                StopMusicStream(musicaFase2);
                StopMusicStream(musicaFase3);
                PlayMusicStream(musicaMenu);
                telaAtual = TELA_RANKING;
            }
            break;



        }

        if (telaAtual == TELA_MENU || telaAtual == TELA_RANKING)
        {
            UpdateMusicStream(musicaMenu);
        }
        else if (fase == 1)
        {
            UpdateMusicStream(musicaFase1);
        }
        else if (fase == 2)
        {
            UpdateMusicStream(musicaFase2);
        }
        else if (fase == 3)
        {
            UpdateMusicStream(musicaFase3);
        }


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
            desenharMapa(mapaAtual, spriteEstrutura, spriteEscada, spriteInimigoDir, spritePortaBaixo, spritePortaCima);
            desenharPlayer(player, spriteMarioDir, spriteMarioEsq);
            desenharInimigos(inimigos, totalInimigos, spriteInimigoDir, spriteInimigoEsq);

            // TEXTO DA FASE ATUAL
            DrawText(TextFormat("FASE: %d", fase), 30, 6, 20, WHITE);

            // TEXTO PARA O TEMPO DECORRIDO JOGANDO
            DrawText(TextFormat("TEMPO: %.1fs", tempoDeJogo), GetScreenWidth() - 180, 6, 20, RED);

            if (trocandoFase)
            {
                DrawRectangle(0, 0, COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, DARKBLUE);
                DrawText("  ...", COLUNAS * TILE_SIZE / 2 - 120, LINHAS * TILE_SIZE / 2, 40, WHITE);
            }
            if (gameOver)
            {
                DrawRectangle(0, 0, COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, BLACK);
                DrawText("GAME OVER", COLUNAS * TILE_SIZE / 2 - 120, LINHAS * TILE_SIZE / 2, 40, RED);
            }
            if (ganhou)
            {
                ClearBackground(BLACK);
                DrawRectangle(0, 0, COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, BLUE);
                DrawText("VITÓRIA!", COLUNAS * TILE_SIZE / 2 - 140, 200, 60, GOLD);
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
            TIPO_PLACAR placar[10];
            int qtd = 0;
            FILE *arq = fopen("placar.bin", "rb");
            if (arq != NULL){
                qtd = fread(placar, sizeof(TIPO_PLACAR), 10, arq);
                fclose(arq);
            }
            DrawText("RANKING", 280, 50, 40, GOLD);
            for (int i=0; i < qtd; i++){
                char linha[50];
                sprintf(linha, "%d. %s - %ds", i+1, placar[i].nome, placar[i].time);
                DrawText(linha, 150, 120 + i * 40, 25, WHITE);
            }
            DrawText("M - Voltar ao menu", 200, 550, 20, LIGHTGRAY);
            DrawText("L - Limpar Ranking", 200, 600, 20, LIGHTGRAY);
            if (IsKeyPressed(KEY_L)){
                limparPlacar();
                qtd = 0;
            }


            break;
        case TELA_INPUT_NOME:
            ClearBackground(BLACK);
            DrawText("VITÓRIA! INSIRA SEU NOME:", COLUNAS * TILE_SIZE / 2 - 220, 200, 30, GOLD);
            DrawRectangle(COLUNAS * TILE_SIZE / 2 - 200, 300, 400, 50, LIGHTGRAY);
            DrawRectangleLines(COLUNAS * TILE_SIZE / 2 - 200, 300, 400, 50, GRAY);
            // Mostra o nome que está sendo digitado em tempo real
            DrawText(nomeJogador, COLUNAS * TILE_SIZE / 2 - 180, 312, 26, DARKGRAY);

            DrawText("Pressione ENTER para salvar", COLUNAS * TILE_SIZE / 2 - 150, 400, 20, RAYWHITE);
            break;
        }
        EndDrawing();
    }

    UnloadTexture(spriteMarioDir);
    UnloadTexture(spriteMarioEsq);
    UnloadTexture(background);
    UnloadTexture(btnNovo.texture);
    UnloadTexture(btnRanking.texture);
    UnloadTexture(btnSair.texture);
    UnloadTexture(spriteEscada);
    UnloadTexture(spriteEstrutura);
    UnloadTexture(spriteInimigoDir);
    UnloadTexture(spriteInimigoEsq);
    UnloadMusicStream(musicaMenu);
    UnloadMusicStream(musicaFase1);
    UnloadMusicStream(musicaFase2);
    UnloadMusicStream(musicaFase3);
    CloseAudioDevice();
    CloseWindow();
    return 0;

}

