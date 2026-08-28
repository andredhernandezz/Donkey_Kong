# Donkey Kong - INF

Jogo em C inspirado no clássico *Donkey Kong*, desenvolvido como trabalho final da disciplina **Algoritmos e Programação (INF01202)** no Instituto de Informática da UFRGS, utilizando a biblioteca **Raylib**.

**Autores:** André Delgado Hernandez e Germano Garlet de Bortoli

## Sobre o jogo

O jogador controla o personagem (Mario) em um mapa baseado em plataformas e escadas, precisando desviar de inimigos e alcançar a porta de vitória. O jogo conta com sistema de fases (3 mapas), placar de recordes (ranking) salvo em arquivo binário e trilha sonora própria para cada tela.

## Como executar

1. Certifique-se de ter o **Code::Blocks** com a biblioteca **Raylib** configurada no ambiente.
2. Abra o arquivo `MeuJogo.cbp`, dentro da pasta `MeuJogo/`.
3. Clique em **Build and Run** (`F9`).

> Importante: mantenha todos os arquivos dentro da pasta `MeuJogo/`, incluindo os mapas (`mapa1.txt`, `mapa2.txt`, `mapa3.txt`) e as pastas `audio/` e `graphics/`.

## Controles

| Tecla | Ação |
|---|---|
| `←` / `→` | Movimentação horizontal |
| `↑` / `↓` | Subir / descer escadas |
| `SPACE` | Pular |
| `TAB` | Pausar o jogo |
| `F1` | Alternar tela cheia |
| `ESC` | Fechar o jogo |

## Estrutura do projeto

O jogo é organizado em um arquivo principal, `jogo.c`, e bibliotecas auxiliares (headers), cada uma responsável por um conjunto de funcionalidades:

- **`button.h`** — struct `Botao` e constantes de posicionamento dos botões do menu.
- **`constantes.h`** — dimensões da matriz do mapa, `TILE_SIZE`, `MAX_INIMIGOS`, `FPS`.
- **`funcoes_desenha_jogo.h`** — desenho do mapa na tela a partir da matriz e das sprites.
- **`funcoes_fase.h`** — lógica de transição entre fases e verificação de vitória.
- **`funcoes_inimigos.h`** — struct `INIMIGO` e funções de spawn, movimentação, colisão e desenho dos inimigos.
- **`funcoes_jogador.h`** — lógica do jogador (movimentação, gravidade, escadas).
- **`funcoes_mapa.h`** — carregamento do mapa a partir de arquivo `.txt` e verificação de colisão com o chão.
- **`placar.h`** — struct `TIPO_PLACAR` e funções de leitura/escrita do ranking em `placar.bin`.

Todos os headers usam `#ifndef` / `#define` / `#endif` para evitar inclusões duplicadas.

### Pastas

- `audio/` — músicas do jogo (`.mp3`).
- `graphics/` — sprites utilizadas no jogo.
- `mapa1.txt`, `mapa2.txt`, `mapa3.txt` — as três fases do jogo.

## Estruturas de dados (structs)

- **`PLAYER`** — posição na matriz (`linha`, `coluna`), velocidade vertical, contador de gravidade e direção (para escolha da sprite).
- **`INIMIGO`** — posição na matriz e direção de movimento.
- **`TIPO_PLACAR`** — nome do jogador e tempo de partida; usada para montar o ranking (top 10), ordenado por tempo com bubble sort.
- **`Botao`** — área clicável (`Rectangle`) e textura (`Texture2D`) de um botão do menu.

## Telas do jogo

O jogo é controlado por uma máquina de estados (`enum EstadoJogo`) com dois `switch-case` — um para lógica e outro para desenho:

- **`TELA_MENU`** — botões de Novo Jogo, Ranking e Sair.
- **`TELA_JOGO`** — jogabilidade principal: movimentação, inimigos, contagem de tempo, transição de fases, game over e vitória.
- **`TELA_PAUSA`** — pausa o jogo (`TAB` retoma, `M` volta ao menu, `S` sai).
- **`TELA_RANKING`** — exibe o top 10 de tempos lido de `placar.bin`, com opção de limpar o ranking (`L`).
- **`TELA_INPUT_NOME`** — captura o nome do jogador ao vencer, para salvar no ranking.

