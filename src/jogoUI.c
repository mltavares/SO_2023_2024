#include "jogoUI.h"

#define LINHAS 16
#define COLUNAS 40
#define PIPE_PATH "/tmp/client_fifo_"

char mapa[LINHAS][COLUNAS];

void criarClienteFIFO() {
    char fifoName[64];
    sprintf(fifoName, "%s%d", PIPE_PATH, getpid());

    if (mkfifo(fifoName, 0666) == -1) {
        perror("Erro ao criar FIFO do cliente");
        exit(1);
    }
}

void lerMapaDoPipe(char mapa[LINHAS][COLUNAS]) {
    char fifoName[64];
    sprintf(fifoName, "%s%d", PIPE_PATH, getpid());
    int fd = open(fifoName, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir o FIFO do cliente");
        exit(1);
    }

    for (int i = 0; i < LINHAS; i++) {
        read(fd, mapa[i], COLUNAS);
    }

    close(fd);
}

void exibirMensagem(WINDOW *janelaMensagens, const char *mensagem) {
    wprintw(janelaMensagens, "%s\n", mensagem);
    wrefresh(janelaMensagens);
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
            wprintw(janelaBaixo, "Listando jogadores...");
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

void desenhaMapa(WINDOW *janela, int tipo) {
    if (tipo == 1) {
        scrollok(janela, TRUE);
    } else {
        int maxLinhas, maxColunas;
        getmaxyx(janela, maxLinhas, maxColunas);

        wclear(janela);
        box(janela, 0, 0);

        for (int i = 0; i < LINHAS && i < maxLinhas - 2; i++) {
            mvwprintw(janela, i + 1, 1, "%.*s", maxColunas - 2, mapa[i]);
        }
    }
    refresh();
    wrefresh(janela);
}

void trataTeclado(WINDOW *janelaTopo, WINDOW *janelaBaixo) {
    keypad(janelaTopo, TRUE);
    wmove(janelaTopo, 1, 1);

    int tecla = wgetch(janelaTopo);
    char comando[100];

    while (tecla != 113) {
        if (tecla == KEY_UP) {
            desenhaMapa(janelaTopo, 2);
            wrefresh(janelaTopo);
        } else if (tecla == KEY_RIGHT) {
            desenhaMapa(janelaTopo, 2);
            wrefresh(janelaTopo);
        } else if (tecla == KEY_LEFT) {
            desenhaMapa(janelaTopo, 2);
            wrefresh(janelaTopo);
        } else if (tecla == KEY_DOWN) {
            desenhaMapa(janelaTopo, 2);
            wrefresh(janelaTopo);
        } else if (tecla == ' ') {
            desenhaMapa(janelaTopo, 2);
            wrefresh(janelaTopo);
            echo();
            comandosJogador(janelaBaixo);
            noecho();
            wrefresh(janelaBaixo);
        }
        wmove(janelaTopo, 1, 1);
        tecla = wgetch(janelaTopo);
    }
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
    lerMapaDoPipe(mapa);
    desenhaMapa(janelaTopo, 2);

    scrollok(janelaMensagens, TRUE);
    box(janelaMensagens, 0, 0);
    desenhaMapa(janelaBaixo, 1);
    desenhaMapa(janelaMensagens, 1);
    utilizadorAutenticado(janelaBaixo, argv[1]);
    trataTeclado(janelaTopo, janelaBaixo);
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

