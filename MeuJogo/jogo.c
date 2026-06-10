#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "button.h"
#include "funcoes_desenha_jogo.h"
#include "funcoes_fase.h"
#include "funcoes_inimigos.h"
#include "funcoes_mapa.h"
#include "funcoes_jogador.h"
#include "constantes.h"
#include "placar.h"

// definicao de estados da tela com EstadoJogo para utilizacao nos switch case na sequencia (melhor uso em vez de atribuir inteiros e usar numeros no switch, pois é nitido qual tela está pelo nome)
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

    // inicializa a fonte utilizada na definicao das teclas no menu
    Font fonte = LoadFontEx("C:/Windows/Fonts/arial.ttf", 16, NULL, 0);

    // CARREGAMENTO DAS MUSICAS SALVAS NA PASTA AUDIO
    InitAudioDevice(); // funcao necessaria da raylib para inicializar audio
    Music musicaMenu  = LoadMusicStream("audio/menu.mp3"); // musica do menu e ranking
    Music musicaFase1 = LoadMusicStream("audio/fase1.mp3"); // musica da fase 1
    Music musicaFase2 = LoadMusicStream("audio/fase2.mp3"); // musica da fase 2
    Music musicaFase3 = LoadMusicStream("audio/fase3.mp3"); // musica da fase 3
    PlayMusicStream(musicaMenu); // inicializa o jogo com a musica do menu e ranking

    // CARREGAMENTO DAS IMAGENS SALVAS NA PASTA GRAPHICS
    Texture2D background = LoadTexture("graphics/capa_menu.png"); // imagem de fundo do menu
    Texture2D spriteMarioDir = LoadTexture("graphics/mario-dir.png"); // imagem mario olhando para direita
    Texture2D spriteMarioEsq = LoadTexture("graphics/mario-esq.png"); // imagem mario olhando para esquerda
    Texture2D spriteEstrutura = LoadTexture("graphics/estrutura.png"); // imagem para as estruturas (chao)
    Texture2D spriteEscada = LoadTexture("graphics/escada.png"); // imagem para as escadas
    Texture2D spriteInimigoDir = LoadTexture("graphics/enemy-dir.png"); // imagem para os inimigos (olhando para direita)
    Texture2D spriteInimigoEsq = LoadTexture("graphics/enemy-esq.png"); // imagem para os inimigos (olhando para esquerda)
    Texture2D spritePortaBaixo = LoadTexture("graphics/porta-baixo.png"); // imagem da parte de baixo da porta (de vitoria)
    Texture2D spritePortaCima = LoadTexture("graphics/porta-cima.png"); // imagem da parte de cima da porta (de vitoria)

    float tempoDeJogo = 0.0f; // inicializacao do tempo de jogo
    char nomeJogador[20] = "\0"; // inicializacao do vetor de nome para o jogador inserir pós-jogo
    int letrasContadas = 0; // quantidade de letras do nomeJogador para input de nome em TELA_INPUT

    // INICIALIZACAO DAS VARIAVEIS NECESSARIAS PARA O JOGO
    char mapa1[LINHAS][COLUNAS]; // matriz para armazenar o mapa 1
    char mapa2[LINHAS][COLUNAS]; // matriz para armazenar o mapa 2
    char mapa3[LINHAS][COLUNAS]; // matriz para armazenar o mapa 3
    char (*mapaAtual)[COLUNAS]; // mapa da fase atual
    mapaAtual = mapa1; //mapaAtual inicializa com o mapa1 (fase1)

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

    // Declaracao dos botoes do menu
    Botao btnNovo;
    Botao btnRanking;
    Botao btnSair;

    btnNovo.rect = (Rectangle){BOTAO_X, POS_Y_NOVO_JOGO, LARGURA_BOTAO, ALTURA_BOTAO}; // usa-se compound literal para inicializar o retangulo da estrutura do bota (em vezes de declarar a variavel separada)
    btnNovo.texture = LoadTexture("graphics/novo_jogo_menu.png"); // carrega a imagem para aparecer no lugar do botao de novo jogo

    btnRanking.rect = (Rectangle){BOTAO_X, POS_Y_RANKING, LARGURA_BOTAO, ALTURA_BOTAO}; // usa-se compound literal para inicializar o retangulo da estrutura do bota (em vezes de declarar a variavel separada)
    btnRanking.texture = LoadTexture("graphics/ranking_menu.png"); // carrega a imagem para aparecer no lugar do botao de ranking

    btnSair.rect = (Rectangle){BOTAO_X, POS_Y_SAIR, LARGURA_BOTAO, ALTURA_BOTAO}; // usa-se compound literal para inicializar o retangulo da estrutura do bota (em vezes de declarar a variavel separada)
    btnSair.texture = LoadTexture("graphics/sair_menu.png"); // carrega a imagem para aparecer no lugar do botao de sair

    // definindo em qual tela o jogo comeca (menu):
    EstadoJogo telaAtual = TELA_MENU;

    // enquanto ESC nao for pressionada ou nao for clicado no x para fechar a janela, continuara executando o programa em looping
    while (!WindowShouldClose())
    {
        Vector2 mousePos = GetMousePosition(); // inicializa mousePos com as posicoes x e y do ponteiro do mouse

        //definindo a logica de atualizacao:

        // CLICAR F1 PARA COLOCAR EM TELA CHEIA
        if (IsKeyPressed(KEY_F1))
        {
            ToggleFullscreen(); // funcao para colocar o jogo em tela cheia
        }
        switch (telaAtual) // SWITCH CASE PARA A PARTE LÓGICA DO JOGO (com os estados de tela definidos no typedef enum no inicio do codigo)
        {

        case TELA_MENU:
            if (CheckCollisionPointRec(mousePos, btnNovo.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) // funcoes que verificam se o usuario clica no bota de Novo Jogo
            {
                // zera tudo ao iniciar novo jogo (a inicializados no inicio do codigo)
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
                mapaAtual = mapa1; // mapa atual agora é o mapa 1
                encontrarPlayer(mapaAtual, &player); // funcao para definir a posicao do jogador
                encontrarInimigos(mapaAtual, inimigos, &totalInimigos); // funcao para definir a posicao dos inimigos

                StopMusicStream(musicaMenu); // para a musica do menu
                PlayMusicStream(musicaFase1); // comeca a musica da fase 1

                telaAtual = TELA_JOGO; // a tela atual agora é a tela jogo
            }
            if (CheckCollisionPointRec(mousePos, btnRanking.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) // se o usuario clicar dentro do espaco do botao de Ranking, muda a tela para a tela do ranking
                telaAtual = TELA_RANKING;
            if (CheckCollisionPointRec(mousePos, btnSair.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) //caso o usuario clicar no botao de sair, libera da RAM cada imagem que foi carregada no LoadTexture (inicio do codigo)
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

        case TELA_JOGO:
            // CONTROLE DE TEMPO
            if (!gameOver && !ganhou && !trocandoFase) // se o jogador nao morreu, nao venceu e nao esta na animacao de troca de fase, incrementa o tempo de jogo (para timer)
            {
                tempoDeJogo += GetFrameTime(); // GetFrameTime retorna o tempo em segundos que o ultimo frame demorou para renderizar (0.0167 a 60 FPS), logo 0.0167 * 60 = 1s, aumenta o tempo de jogo em 1 segundo.
            }
            // pausa
            if (IsKeyPressed(KEY_TAB)) // caso o usuario pressione a tecla 'TAB', muda a tela atual para tela de pausa
            {
                telaAtual = TELA_PAUSA;
                break;
            }
            // tela de game over (só conta o timer, depois volta ao menu)
            if (gameOver)
            {
                timerGameOver++; // incrementa 1 a cada frame
                if (timerGameOver > FPS * 3) // equivale a 3 segundos, ou seja, quando passar de 3 segundos carregando, começa a tocar a musica do menu e muda a tela pra tela do menu
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
                timerGanhou++; //incrementa 1 a cada frame
                if (timerGanhou > FPS * 3) // quando passar 3 segundos, comeca a tocar a musica do menu e vai pra tela de input do nome (para salvar a pontuacao no ranking)
                {

                    StopMusicStream(musicaFase1);
                    StopMusicStream(musicaFase2);
                    StopMusicStream(musicaFase3);
                    PlayMusicStream(musicaMenu);
                    if (deveEntrarNoRanking((int) tempoDeJogo)){
                        nomeJogador[0] = '\0';
                        letrasContadas = 0;
                        // Vai para a tela de registro
                        telaAtual = TELA_INPUT_NOME;
                    }
                    else {
                        telaAtual = TELA_RANKING;
                    }

                }
                break;
            }

            // troca de fase (animacao de carregamento)
            if (trocandoFase)
            {
                timerTrocaFase++;
                if (timerTrocaFase > FPS * 0.6f) // delay de 0.6 segundos
                {
                    faseAnterior = fase;
                    fase = proximaFase;
                    if (fase == 1) // se a fase for a 1 executa os comandos
                    {
                        carregarMapa(mapa1, "mapa1.txt"); // carrega o mapa da fase 1
                        mapaAtual = mapa1;
                        player.linha = 1; // define a linha inicial do jogador ao entrar na fase 1
                        player.coluna = 16; // define a coluna inicial do jogador ao entrar na fase 1
                        StopMusicStream(musicaFase2);
                        PlayMusicStream(musicaFase1); // comeca a tocar a musica da fase 1
                    }
                    else if (fase == 2) // se a fase for a 2 executa os comandos
                    {
                        carregarMapa(mapa2, "mapa2.txt"); // carrega o mapa da fase 2
                        mapaAtual = mapa2;
                        if (faseAnterior == 1)
                            encontrarPlayer(mapaAtual, &player);
                        else if (faseAnterior == 3)
                        {
                            player.linha = 2;
                            player.coluna = 14;
                        }
                        StopMusicStream(musicaFase1);
                        PlayMusicStream(musicaFase2); // comeca a tocar a musica da fase 2
                    }
                    else if (fase == 3) // se a fase for a 3 executa os comandos
                    {
                        carregarMapa(mapa3, "mapa3.txt"); // carrega o mapa da fase 3
                        mapaAtual = mapa3;
                        encontrarPlayer(mapaAtual, &player);
                        StopMusicStream(musicaFase2);
                        PlayMusicStream(musicaFase3); // comeca a tocar a musica da fase 3
                    }
                    encontrarInimigos(mapaAtual, inimigos, &totalInimigos); // recarrega os inimigos
                    player.contadorGravidade = 0; // zera a gravidade
                    conta_frames_inimigo = 0; // zera o contador de frames dos inimigos
                    trocandoFase = 0; // desliga a flag de troca de fase

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
            // lógica dos botões de pausa (Continuar / Menu / Sair)
            if (IsKeyPressed(KEY_TAB)) telaAtual = TELA_JOGO; // se o usuario clica a tecla 'TAB' retorna para a tela do jogo
            if (IsKeyPressed(KEY_M))   telaAtual = TELA_MENU; // se o usuario clica a tecla 'M' volta para a tela do menu
            if (IsKeyPressed(KEY_S)) // se o usuario clica a tecla 'S', encerra o jogo e libera a memoria de todas as imagens e audios carregados
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
            if(!(IsMusicStreamPlaying(musicaMenu))) // se nao estiver tocando a musica do menu na tela do ranking, toca
                PlayMusicStream(musicaMenu); // comeca a tocar a musica do menu

            if (IsKeyPressed(KEY_M)) // caso a tecla 'M' seja pressionada
            {
                telaAtual = TELA_MENU; // sai da tela do ranking e vai pra tela do menu
            }
            break;

        case TELA_INPUT_NOME:
            // Captura de texto pela Raylib
            int chave = GetCharPressed(); //GetCharPressed() retorna o codigo ASCII da tecla digitada ou 0 se nenhuma for pressionada
            while (chave > 0) // enquanto for maior que 0
            {
                if ((chave >= 32) && (chave <= 125) && (letrasContadas < 19)) // se o que o usuario digitar teclas do intervalo indicado e a quantidade total de letras for menor que 19
                {
                    nomeJogador[letrasContadas] = (char)chave; // altera o vetor nomeJogador (com o nome do jogador) com o caractere digitado no teclado
                    nomeJogador[letrasContadas + 1] = '\0'; // adiciona o '\0' um caractere depois do digitado pelo usuario (para garantir que o último caractere sempre será '\0')
                    letrasContadas++;
                }
                chave = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) // caso o usuario pressione a tecla backspace
            {
                letrasContadas--; // decrementa a variavel que conta a quantidade de letras
                if (letrasContadas < 0) letrasContadas = 0; // para caso o usuario aperte backspace antes de digitar uma letra, assim o contador nunca ficará negativo
                nomeJogador[letrasContadas] = '\0'; // termina a string com '\0'
            }

            if (IsKeyPressed(KEY_ENTER) && letrasContadas > 0) // caso o usuario clicar 'ENTER' e o numero de caracteres seja superior a 0 (isto é, usuario terminou de digitar seu nome)
            {
                salvarResultadoNoPlacar(nomeJogador, (int)tempoDeJogo); // a funcao para salvar o nome e o resultado (pontuacao) do jogador é chamada
                // para de tocar quaisquer musicas que estejam tocando
                StopMusicStream(musicaFase1);
                StopMusicStream(musicaFase2);
                StopMusicStream(musicaFase3);
                PlayMusicStream(musicaMenu);
                telaAtual = TELA_RANKING; // nova tela agora é do ranking
            }
            break;
        }

        // Raylib exige que a funcao UpdateMusicStream() seja chamada a cada frame para que a musica continue tocando, caso contrario, ela para
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
        BeginDrawing(); // Funcao para iniciar o desenho do frame atual
        ClearBackground(BLACK); // funcao para deixar o fundo preto
        //
        switch (telaAtual){ // SWITCH CASE PARA A PARTE VISUAL DO JOGO (com os estados de tela definidos no typedef enum no inicio do codigo

        case TELA_MENU:

            DrawTexture(background, 0, 0, WHITE); // desenha a imagem de fundo na posicao 0,0
            // desenho dos textos indicando as teclas e suas respectivas funcoes
            DrawTextEx(fonte, "SETAS up/down/left/dir",    (Vector2){50, 72},  16, 1, WHITE);
            DrawTextEx(fonte, "TAB     Pausar",    (Vector2){50, 94},  16, 1, WHITE);
            DrawTextEx(fonte, "ESC     Fechar",    (Vector2){50, 116}, 16, 1, WHITE);
            DrawTextEx(fonte, "F1      Tela Cheia",(Vector2){50, 138}, 16, 1, WHITE);
            DrawTextEx(fonte, "ESPACO  Pular",(Vector2){50, 160}, 16, 1, WHITE);
            // desenho dos botoes e aplica o efeito de hover (caso o mouse passe por cima da area indicada, muda a cor)
            DrawTexture(btnNovo.texture, btnNovo.rect.x, btnNovo.rect.y,
                        CheckCollisionPointRec(mousePos, btnNovo.rect) ? GRAY : WHITE);
            DrawTexture(btnRanking.texture, btnRanking.rect.x, btnRanking.rect.y,
                        CheckCollisionPointRec(mousePos, btnRanking.rect) ? GRAY : WHITE);
            DrawTexture(btnSair.texture, btnSair.rect.x, btnSair.rect.y,
                        CheckCollisionPointRec(mousePos, btnSair.rect) ? GRAY : WHITE);
            break;

        case TELA_JOGO:
            ClearBackground(BLACK);
            desenharMapa(mapaAtual, spriteEstrutura, spriteEscada, spriteInimigoDir, spritePortaBaixo, spritePortaCima); // desenha o mapa atual e carrega todas as sprites a serem usadas
            desenharPlayer(player, spriteMarioDir, spriteMarioEsq); // desenha o jogador e carrega as sprites do jogador (Mario)
            desenharInimigos(inimigos, totalInimigos, spriteInimigoDir, spriteInimigoEsq); // desenha os inimigos e carrega as sprites dos inimigos
            // TEXTO DA FASE ATUAL
            DrawText(TextFormat("FASE: %d", fase), 30, 6, 20, WHITE);
            // TEXTO PARA O TEMPO DECORRIDO JOGANDO
            DrawText(TextFormat("TEMPO: %.1fs", tempoDeJogo), GetScreenWidth() - 180, 6, 20, RED);
            if (trocandoFase){
                DrawRectangle(0, 0, COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, DARKBLUE);
                DrawText("  ...", COLUNAS * TILE_SIZE / 2 - 120, LINHAS * TILE_SIZE / 2, 40, WHITE);
            }
            if (gameOver){
                DrawRectangle(0, 0, COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, BLACK);
                DrawText("GAME OVER", COLUNAS * TILE_SIZE / 2 - 120, LINHAS * TILE_SIZE / 2, 40, RED);
            }
            if (ganhou){
                ClearBackground(BLACK);
                DrawRectangle(0, 0, COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, BLUE);
                DrawText("VITÓRIA!", COLUNAS * TILE_SIZE / 2 - 140, 200, 60, GOLD);
            }
            break;

        case TELA_PAUSA:
            // desenhar botões de pausa
            ClearBackground(BLACK);
            DrawText("PAUSADO", 275, 150, 40, WHITE);
            DrawText("TAB - Continuar\nM - Menu\nS - Sair\n", COLUNAS * TILE_SIZE / 2 - 160, LINHAS * TILE_SIZE / 2 + 20, 20, LIGHTGRAY);
            break;

        case TELA_RANKING:
            // exibir placar.bin
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
    // libera da memoria RAM todas as imagens e audios utilizados
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
    // fecha a janela
    CloseWindow();
    return 0;
}
