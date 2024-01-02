#include "jogoUI.h"
#include "comuns.h"

#define LINHAS 16
#define COLUNAS 40
#define PIPE_PATH "/tmp/client_fifo_"
#define JOGADOR_CHAR '@'

char mapa[LINHAS][COLUNAS];

WINDOW *janelaBaixo;
WINDOW *janelaTopo;

void criarClienteFIFO() {
    char fifoName[64];
    sprintf(fifoName, "%s%d", PIPE_PATH, getpid());

    if (mkfifo(fifoName, 0666) == -1) {
        perror("Erro ao criar FIFO do cliente");
        exit(1);
    }
}


void lerMapaDoPipe(char mapa[LINHAS][COLUNAS], int *posX, int *posY) {
    char fifoName[64];
    sprintf(fifoName, "%s%d", PIPE_PATH, getpid());
    int fd = open(fifoName, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir o FIFO do cliente");
        exit(1);
    }

    // Ler o mapa
    for (int i = 0; i < LINHAS; i++) {
        read(fd, mapa[i], COLUNAS);
    }

     // Ler posição inicial
    char posBuffer[10];
    read(fd, posBuffer, sizeof(posBuffer));  // Ler coordenada X
    *posX = atoi(posBuffer);
    read(fd, posBuffer, sizeof(posBuffer));  // Ler coordenada Y
    *posY = atoi(posBuffer);

    close(fd);
}

void exibirMensagem(WINDOW *janelaMensagens, const char *mensagem) {
    wprintw(janelaMensagens, "%s\n", mensagem);
    wrefresh(janelaMensagens);
}

void comandoPlayers(KeyboardHandlerToken *token) {
    for(int i = 0; i < token->players->nPlayers; ++i) {
        mvwprintw(janelaBaixo, i + 1, 1, "Nome e Icone: %s | %c\0\n", token->players->array[i].name, token->players->array[i].icone);
    }
}

void comandosJogador(WINDOW *janelaBaixo) {
    char comandos[50];
    char *token;

    wprintw(janelaBaixo, "\nComando: ");
    wrefresh(janelaBaixo);

    wgetnstr(janelaBaixo, comandos, sizeof(comandos) - 1);

    noecho();

    token = strtok(comandos, " ");

    if (token != NULL) {
        if (strcmp(token, "exit") == 0) {
            wprintw(janelaBaixo, "Saindo...");
        } else if (strcmp(token, "players") == 0) {
            KeyboardHandlerToken keyboardToken;
            comandoPlayers(&keyboardToken);
        } else if (strcmp(token, "msg") == 0) {
            char *name_token = strtok(NULL, " ");
            char *msg_token = strtok(NULL, "\n");
            if (name_token && msg_token) {
                wprintw(janelaBaixo, "Enviando mensagem para %s: %s", name_token, msg_token);
            } else {
                wprintw(janelaBaixo, "Comando 'msg' inválido. Verifique os parâmetros.");
            }
        } else {
            wprintw(janelaBaixo, "Comando '%s' inválido.", token);
        }
    } else {
        wprintw(janelaBaixo, "Nenhum comando foi inserido.");
    }
    wrefresh(janelaBaixo);
}

void utilizadorAutenticado(WINDOW *janela, const char *nomeJogador) {
    mvwprintw(janela, 0, 1, "JOGADOR: %s\n", nomeJogador);
    wrefresh(janela);
}

void desenhaMapa(WINDOW *janela, int tipo, int jogadorX, int jogadorY) {
    if (tipo == 1) {
        scrollok(janela, TRUE);
    } else {
        int maxLinhas, maxColunas;
        getmaxyx(janela, maxLinhas, maxColunas);

        wclear(janela);
        box(janela, 0, 0);

        for (int i = 0; i < LINHAS && i < maxLinhas - 2; i++) {
            for (int j = 0; j < COLUNAS && j < maxColunas - 2; j++) {
                if (i == jogadorY && j == jogadorX) {
                    mvwaddch(janela, i + 1, j + 1, JOGADOR_CHAR);
                } else {
                    mvwaddch(janela, i + 1, j + 1, mapa[i][j]);
                }
            }
        }
    }
    wrefresh(janela);
}

void trataTeclado(WINDOW *janelaTopo, WINDOW *janelaBaixo, int *jogadorX, int *jogadorY) {
    char fifoName[64];
    sprintf(fifoName, "%s%d", PIPE_PATH, getpid());
    int fifo_fd = open(fifoName, O_WRONLY);

    if (fifo_fd == -1) {
        perror("Erro ao abrir o FIFO do cliente para escrita");
        return;
    }

    keypad(janelaTopo, TRUE);
    int tecla;
    char comando;

    while ((tecla = wgetch(janelaTopo)) != 'q') { 
        if (tecla == ' ') {
            desenhaMapa(janelaTopo, 2, *jogadorX, *jogadorY);
            wrefresh(janelaTopo);
            echo();
            comandosJogador(janelaBaixo); 
            noecho();
            wrefresh(janelaBaixo);
        } else {
            switch (tecla) {
                case KEY_UP:
                    comando = 'w';
                    break;
                case KEY_DOWN:
                    comando = 's';
                    break;
                case KEY_LEFT:
                    comando = 'a';
                    break;
                case KEY_RIGHT:
                    comando = 'd';
                    break;
                default:
                    comando = '\0';
                    break;
            }
        }

        if (comando != '\0') {
            write(fifo_fd, &comando, sizeof(comando));

            desenhaMapa(janelaTopo, 2, *jogadorX, *jogadorY);
            wrefresh(janelaTopo);
        }
    }

    close(fifo_fd);
}


void *escutaAtualizacoes(void *arg) {
    char fifoName[64];
    sprintf(fifoName, "%s%d", PIPE_PATH, getpid());
    int fd = open(fifoName, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir FIFO para leitura");
        exit(1);
    }

    char buffer[COLUNAS + 1];  // +1 para o caractere nulo
    int jogadorX = -1;  // Inicialmente, posição do jogador desconhecida
    int jogadorY = -1;

    while (1) {
        for (int i = 0; i < LINHAS; i++) {
            ssize_t bytes = read(fd, buffer, COLUNAS);
            if (bytes > 0) {
                buffer[bytes] = '\0';

                // Verificar se a linha contém a representação do jogador
                char *pos = strchr(buffer, JOGADOR_CHAR);
                if (pos != NULL) {
                    // Calcular as coordenadas do jogador
                    jogadorX = pos - buffer;
                    jogadorY = i;

                    // Atualizar a posição do jogador na UI
                    mvwaddch(janelaTopo, jogadorY + 1, jogadorX + 1, JOGADOR_CHAR);
                }

                // Atualizar a linha do mapa na UI
                mvwprintw(janelaTopo, i + 1, 1, "%s", buffer);
            }
        }

        // Atualizar a janela após processar todas as linhas
        wrefresh(janelaTopo);
    }
    close(fd);
    return NULL;
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Login inválido\n");
        return 1;
    }

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    attrset(A_DIM);
    WINDOW *janelaTopo = newwin(LINHAS + 2, COLUNAS + 1, 3, 10);
    WINDOW *janelaBaixo = newwin(15, 82, 26, 1);
    WINDOW *janelaMensagens = newwin(22, 30, 3, 84);

    criarClienteFIFO();
    int posX, posY;
    lerMapaDoPipe(mapa, &posX, &posY);

    desenhaMapa(janelaTopo, 2, posX, posY);
    wmove(janelaTopo, posY + 1, posX + 1);

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, escutaAtualizacoes, NULL) != 0) {
        perror("Falha ao criar thread de escuta");
        return 1;
    }

    scrollok(janelaMensagens, TRUE);
    box(janelaMensagens, 0, 0);
    desenhaMapa(janelaBaixo, 1, 0, 0);
    desenhaMapa(janelaMensagens, 1, 0, 0);
    utilizadorAutenticado(janelaBaixo, argv[1]);
    trataTeclado(janelaTopo, janelaBaixo, &posX, &posY);
    wclear(janelaTopo);
    wrefresh(janelaTopo);
    delwin(janelaTopo);
    wclear(janelaBaixo);
    wrefresh(janelaBaixo);
    delwin(janelaBaixo);
    wclear(janelaMensagens);
    wrefresh(janelaMensagens);
    wclear(janelaMensagens);
    endwin();
    char fifoName[64];
    sprintf(fifoName, "%s%d", PIPE_PATH, getpid());
    unlink(fifoName);
    return 0;
}


