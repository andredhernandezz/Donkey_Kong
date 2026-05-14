#include "raylib.h"
#include "button.h"

#define LINHAS 30
#define COLUNAS 30
#define TILE_SIZE 25

typedef enum {
    TELA_MENU,
    TELA_JOGO,
    TELA_RANKING
} EstadoJogo;

int main(void)
{
    //inicializa a janela com tamanho 750x750 com o titulo "Donkey Kong - INF"
    InitWindow(COLUNAS * TILE_SIZE, LINHAS * TILE_SIZE, "Donkey Kong - INF");
    //determina os frames per second da tela, nesse caso 60
    SetTargetFPS(60);

    // carregando a imagem da capa do menu como a variavel background
    Texture2D background = LoadTexture("graphics/capa_menu.png");

    Botao btnNovo;
    btnNovo.rect = (Rectangle){ BOTAO_X, POS_Y_NOVO_JOGO, LARGURA_BOTAO, ALTURA_BOTAO };
    btnNovo.texture = LoadTexture("graphics/novo_jogo_menu.png");
    btnNovo.ativo = true;

    Botao btnRanking;
    btnRanking.rect = (Rectangle){ BOTAO_X, POS_Y_RANKING, LARGURA_BOTAO, ALTURA_BOTAO };
    btnRanking.texture = LoadTexture("graphics/ranking_menu.png");
    btnRanking.ativo = true;

    Botao btnSair;
    btnSair.rect = (Rectangle){ BOTAO_X, POS_Y_SAIR, LARGURA_BOTAO, ALTURA_BOTAO };
    btnSair.texture = LoadTexture("graphics/sair_menu.png");
    btnSair.ativo = true;

    // definindo em qual tela o jogo comeca (menu):
    EstadoJogo telaAtual = TELA_MENU;

    // enquanto ESC nao for pressionada ou nao for clicado no x para fechar a janela, continuara executando o programa em looping
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        //definindo a logica de atualizacao:
        bool fecharJogo = false;
        switch (telaAtual){
            case TELA_MENU:
                if (CheckCollisionPointRec(mousePos, btnNovo.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                telaAtual = TELA_JOGO;
                }
                // Lógica do botão Ranking
                if (CheckCollisionPointRec(mousePos, btnRanking.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    telaAtual = TELA_RANKING;
                }
                // Lógica do botão Sair
                if (CheckCollisionPointRec(mousePos, btnSair.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    fecharJogo = true;
                }

                break;

            case TELA_JOGO:
                //
                //
                //
                //
                break;

            case TELA_RANKING:
                //
                //
                //
                //
                break;
            }
        if (fecharJogo) break;

        //iniciar desenho
        BeginDrawing();
            // imagem do desenho fica preta e depois sera substituida pelas imagens do menu do jogo
            ClearBackground(BLACK);

            switch (telaAtual) {
                case TELA_MENU:
                    DrawTexture(background, 0, 0, WHITE);

                    Color cNovo = CheckCollisionPointRec(mousePos, btnNovo.rect) ? GRAY : WHITE;
                    DrawTexture(btnNovo.texture, btnNovo.rect.x, btnNovo.rect.y, cNovo);

                    Color cRank = CheckCollisionPointRec(mousePos, btnRanking.rect) ? GRAY : WHITE;
                    DrawTexture(btnRanking.texture, btnRanking.rect.x, btnRanking.rect.y, cRank);

                    Color cSair = CheckCollisionPointRec(mousePos, btnSair.rect) ? GRAY : WHITE;
                    DrawTexture(btnSair.texture, btnSair.rect.x, btnSair.rect.y, cSair);
                    break;

                case TELA_JOGO:
                    DrawText("VOCE ESTA NO JOGO! ('M' de Menu para voltar)", 100, 300, 20, RAYWHITE);
                    break;

                case TELA_RANKING:
                    DrawText("RANKING TESTE- JOGADOR 1", 100, 300, 30, GOLD);
                    DrawText("Pressione  a tecla 'M' de Menu para voltar", 100, 350, 20, RAYWHITE);
                    break;
            }
        if (IsKeyPressed(KEY_M)) {
            telaAtual = TELA_MENU;
        }
        EndDrawing();



    }

    UnloadTexture(background);
    UnloadTexture(btnNovo.texture);
    UnloadTexture(btnRanking.texture);
    UnloadTexture(btnSair.texture);


    // fecha a janela aberta
    CloseWindow();
    return 0;
}
