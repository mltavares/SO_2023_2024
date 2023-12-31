#include "motor.h"

#define MAX_MSG_LEN 1024
#define NLIN 16
#define NCOL 40
#define LOCK_FILE "/tmp/motor.lock"
#define PIPE_PATH "/tmp/client_fifo_"

int contadorClientes = 0;

void VariaveisAmbiente() {
    char *env_INSCRICAO = getenv("INSCRICAO");
    char *env_NPLAYERS = getenv("NPLAYERS");
    char *env_DURACAO = getenv("DURACAO");
    char *env_DECREMENTO = getenv("DECREMENTO");

    printf("INSCRICAO: %s\n", env_INSCRICAO);
    printf("NPLAYERS: %s\n", env_NPLAYERS);
    printf("DURACAO: %s\n", env_DURACAO);
    printf("DECREMENTO: %s\n", env_DECREMENTO);
}

void handle_sigint(int signum) {
    child_exit_status = 1;
}

void lancarBot() {
    printf("Lançar bot\n"); 
    char string[100][MAX_MSG_LEN];
    int descriptor[2];
    child_exit_status = 0;
    
    if (pipe(descriptor) == -1) {
        perror("Erro ao criar pipe");
        return;
    }

    int id = fork();

    if (id == 0) { // Filho
        close(STDOUT_FILENO);
        dup(descriptor[1]);
        close(descriptor[0]);
        close(descriptor[1]);

        execl("./bot", "bot", "1", "10", (char *) NULL); 
        perror("Erro ao lançar bot");
        exit(1);
    } else { // Pai
        close(descriptor[1]);
        signal(SIGINT, handle_sigint);

        int i = 0;

        while (!child_exit_status) {
            ssize_t count = read(descriptor[0], string[i], sizeof(string[i]));

            if (count == -1) {
                perror("Erro na leitura do pipe");
                break; 
            } else if (count == 0) {
                break; 
            }

            printf("RECEBI: %s", string[i]);
            ++i;
        }

        close(descriptor[0]); 

        int status;
        wait(&status);
    }
}

void carregarMapa(Jogo *jogo, const char *nomeArquivo) {
    FILE *mapFile = fopen(nomeArquivo, "r");
    if (mapFile == NULL) {
        perror("Erro ao abrir o arquivo de mapa");
        exit(1);
    }

    char linha[COLUNAS + 1];
    for (int i = 0; i < LINHAS; i++) {
        if (fgets(linha, sizeof(linha), mapFile) == NULL) {
            if (feof(mapFile)) {
                break; 
            } else {
                perror("Erro ao ler o arquivo do mapa");
                fclose(mapFile);
                exit(1);
            }
        }
        strncpy(jogo->maze[i], linha, COLUNAS);
    }

    fclose(mapFile);
}

void enviarMapa(const Jogo *jogo) {
    DIR *d;
    struct dirent *dir;
    d = opendir("/tmp");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(dir->d_name, "client_fifo_", 12) == 0) {
                char fullPath[512];
                snprintf(fullPath, sizeof(fullPath), "/tmp/%s", dir->d_name);
                int fd = open(fullPath, O_WRONLY);
                if (fd != -1) {
                    for (int i = 0; i < LINHAS; i++) {
                        write(fd, jogo->maze[i], COLUNAS);
                    }
                    close(fd);
                }
            }
        }
        closedir(d);
    }
}

int generateRandom(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void comandoUsers(KeyboardHandlerToken *token) {
    printf("Lista Users:\n");
    for(int i = 0; i < token->players->nPlayers; ++i) {
        printf("\t<%s>\n", token->players->array[i].name);
    }
}

void comandoBots(KeyboardHandlerToken *token) {
    BotArray *bots = token->bots;

    for(int i = 0; i < bots->nBots; ++i) {
        printf("Intervalo Bot: %d | Duracao Bot: %d\n", bots->bots[i].interval, bots->bots[i].duration);
    }
}


void comandoBMOV(KeyboardHandlerToken *token) {
    BmovArray *bmovs = token->bmovs;
    if(bmovs->nbmovs < MAX_BMOVS) {
        initBmov(token, &bmovs->bmovs[bmovs->nbmovs]);
        bmovs->nbmovs++;
        printf("Bmov adicionada com sucesso\n");
    } else {
        printf("Numero maximo de Bmovs\n");
        return;
    }
}


void initBmov(KeyboardHandlerToken *token, Bmov *bmov) {
    Map *map = token->map;
    int x;
    int y;
    
    do {
        x = generateRandom(0, MAX_WIDTH - 2);
        y = generateRandom(0, MAX_WIDTH - 2);
    } while(map->array[x][y] != ' ');
    
    bmov->x = x;
    bmov->y = y;
}


void comandoRBM(KeyboardHandlerToken *token) {
    BmovArray *bmovs = token->bmovs;

    if(bmovs->nbmovs <= 0) {
        printf("Nao existe bmov para remover\n");
        return;
    }

    for(int i = 0; i < bmovs->nbmovs - 1; ++i) {
        bmovs->bmovs[i] = bmovs->bmovs[i + 1];
    }

    bmovs->nbmovs--;
    printf("Bmov removida\n");
}


void comandoBegin(KeyboardHandlerToken *token) {
    printf("O jogo vai começar\n");
    token->keyboardFeed = 0;
}

void comandosMotor(Jogo *jogo, PlayerArray *players) {
    char comandos[50];
    char name_token[50];
    char *token;

    do{

        printf("\nComando: ");
        fflush(stdout);  

        fgets(comandos, 50, stdin);

        size_t len = strlen(comandos);
        if (len > 0 && comandos[len-1] == '\n') {
            comandos[len-1] = '\0';
        }
        token = strtok(comandos, " ");

                if(token!=NULL){
        if(strcmp(token,"users")==0){
            KeyboardHandlerToken keyboardToken;
            comandoUsers(&keyboardToken);
        }
        else if(strcmp(token,"kick")==0){
                char *name_token = strtok(NULL, " ");
                if (name_token == NULL) {
                    printf("Utilizador inválido\n");
                }
                else {
                    printf("O utilizador %s foi kickado\n", name_token);
                }
            }
        else if(strcmp(token,"bots")==0){
            KeyboardHandlerToken keyboardToken;
            comandoBots(&keyboardToken);
        }
        else if(strcmp(token,"bmov")==0){
            KeyboardHandlerToken keyboardToken;
            comandoBMOV(&keyboardToken);
        }
        else if(strcmp(token,"rbm")==0){
            KeyboardHandlerToken keyboardToken;
            comandoRBM(&keyboardToken);
        }
        else if(strcmp(token,"begin")==0){
            KeyboardHandlerToken keyboardToken;
            comandoBegin(&keyboardToken);
        }
        else if(strcmp(token,"end")==0){
            printf("end detetado\n");
            remove(LOCK_FILE);
            exit(1);
        }
        else if(strcmp(token,"test_bot")==0){
            lancarBot();
        }
        else{
            printf("Comando invalido\n");
        }
    }

     lerComandosDosJogadores(players, jogo);

    }while(1);

}


void aguardarConexoes(int periodoInscricao, int nPlayersMinimo, int nPlayersMaximo) {
    time_t inicio = time(NULL);
    contadorClientes = 0;

    while (time(NULL) - inicio < periodoInscricao) {
        DIR *d = opendir("/tmp");
        if (d) {
            struct dirent *dir;
            int contadorTemp = 0;
            while ((dir = readdir(d)) != NULL) {
                if (strncmp(dir->d_name, "client_fifo_", 12) == 0) {
                    contadorTemp++;
                }
            }
            closedir(d);

            if (contadorTemp > contadorClientes) {
                contadorClientes = contadorTemp;
            }

            if (contadorClientes >= nPlayersMinimo) {
                printf("Número mínimo de jogadores atingido. Iniciando o jogo.\n");
                break; 
            }

            if (contadorClientes >= nPlayersMaximo) {
                printf("Número máximo de jogadores atingido. Iniciando o jogo.\n");
                break; 
            }
        }
        sleep(1);
    }
}

void lerComandosDosJogadores(PlayerArray *players, Jogo *jogo) {
    char comando;
    ssize_t bytesLidos;

    for (int i = 0; i < players->nPlayers; i++) {
        int fd = open(players->array[i].pipe, O_RDONLY | O_NONBLOCK);

        if (fd != -1) {
            while ((bytesLidos = read(fd, &comando, sizeof(comando))) > 0) {
                // Atualizar a posição do jogador com base no comando
                int x = players->array[i].xCoordinate;
                int y = players->array[i].yCoordinate;

                switch (comando) {
                    case 'w': // Mover para cima
                        if (y > 0 && (jogo->maze[y - 1][x] != 'P' && jogo->maze[y - 1][x] != 'B')) {
                            players->array[i].yCoordinate--;
                        }
                        break;
                    case 's': // Mover para baixo
                        if (y < LINHAS - 1 && (jogo->maze[y + 1][x] != 'P' && jogo->maze[y + 1][x] != 'B')) {
                            players->array[i].yCoordinate++;
                        }
                        break;
                    case 'a': // Mover para esquerda
                        if (x > 0 && (jogo->maze[y][x - 1] != 'P' && jogo->maze[y][x - 1] != 'B')) {
                            players->array[i].xCoordinate--;
                        }
                        break;
                    case 'd': // Mover para direita
                        if (x < COLUNAS - 1 && (jogo->maze[y][x + 1] != 'P' && jogo->maze[y][x + 1] != 'B')) {
                            players->array[i].xCoordinate++;
                        }
                        break;
                }
            }

            close(fd);
        }
    }
}


int main(int argc, char *argv[]) {
    int lock_fd = open(LOCK_FILE, O_CREAT | O_EXCL, 0644);
    if (lock_fd == -1) {
        perror("Uma instância do motor já está em execução.");
        return 1;
    }

    Jogo jogo;
    PlayerArray players;
    carregarMapa(&jogo, "mapa.txt");

    VariaveisAmbiente();

    int nPlayersMinimo = atoi(getenv("NPLAYERS") ? getenv("NPLAYERS") : "5");
    int periodoInscricao = atoi(getenv("INSCRICAO") ? getenv("INSCRICAO") : "60");

    aguardarConexoes(periodoInscricao, nPlayersMinimo, MAX_PLAYERS);

    if (contadorClientes >= nPlayersMinimo && contadorClientes <= MAX_PLAYERS) {
        enviarMapa(&jogo);
        comandosMotor(&jogo, &players);
    } else {
        printf("Número insuficiente de jogadores para iniciar o jogo.\n");
    }

    close(lock_fd);
    remove(LOCK_FILE);

    for (int i = 0; i < contadorClientes; i++) {
        char pipePath[64];
        sprintf(pipePath, "%s%d", PIPE_PATH, i);
        unlink(pipePath);
    }

    return 0;
}
