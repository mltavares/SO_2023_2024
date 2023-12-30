#include "motor.h"

#define MAX_MSG_LEN 1024
#define NLIN 16
#define NCOL 40
#define MAX_CLIENTES 5
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


void comandosMotor() {
    char comandos[50];
    char *token;

    do {
        printf("\nComando: ");
        fflush(stdout);

        fgets(comandos, 50, stdin);

        size_t len = strlen(comandos);
        if (len > 0 && comandos[len-1] == '\n') {
            comandos[len-1] = '\0';
        }
        token = strtok(comandos, " ");

        if (token != NULL) {
            if (strcmp(token, "users") == 0) {
                printf("users detetado\n");
            }
            else if (strcmp(token, "kick") == 0) {
                char *name_token = strtok(NULL, " ");
                if (name_token == NULL) {
                    printf("Utilizador inválido\n");
                }
                else {
                    printf("O utilizador %s foi kickado\n", name_token);
                }
            }
            else if (strcmp(token, "bots") == 0) {
                printf("bots detetado\n");
            }
            else if (strcmp(token, "bmov") == 0) {
                printf("bmov detetado\n");
            }
            else if (strcmp(token, "rbm") == 0) {
                printf("rbm detetado\n");
            }
            else if (strcmp(token, "begin") == 0) {
                printf("begin detetado\n");
            }
            else if (strcmp(token, "end") == 0) {
                printf("end detetado\n");
                remove(LOCK_FILE);
                exit(1);
            }
            else if (strcmp(token, "test_bot") == 0) {
                lancarBot();
            }
            else {
                printf("Comando inválido\n");
            }
        }
    } while (1);
}


void aguardarConexoes(int periodoInscricao) {
    time_t inicio = time(NULL);
    contadorClientes = 0;

    while (time(NULL) - inicio < periodoInscricao) {
        DIR *d = opendir("/tmp");
        if (d) {
            struct dirent *dir;
            while ((dir = readdir(d)) != NULL) {
                if (strncmp(dir->d_name, "client_fifo_", 12) == 0) {
                    contadorClientes++;
                }
            }
            closedir(d);
        }
        sleep(1); // Para reduzir o uso da CPU
    }
}


int main(int argc, char *argv[]) {
    int lock_fd = open(LOCK_FILE, O_CREAT | O_EXCL, 0644);
    if (lock_fd == -1) {
        perror("Uma instância do motor já está em execução.");
        return 1;
    }

    Jogo jogo;
    carregarMapa(&jogo, "mapa.txt");

    VariaveisAmbiente();

    int periodoInscricao = atoi(getenv("INSCRICAO") ? getenv("INSCRICAO") : "60");

    aguardarConexoes(periodoInscricao);

    if (contadorClientes >= MAX_CLIENTES) {
        enviarMapa(&jogo);
        comandosMotor();
    } else {
        printf("Número insuficiente de jogadores para iniciar o jogo.\n");
    }

    // Código de limpeza
    close(lock_fd);
    remove(LOCK_FILE);

    for (int i = 0; i < contadorClientes; i++) {
        char pipePath[64];
        sprintf(pipePath, "%s%d", PIPE_PATH, i);
        unlink(pipePath);
    }

    return 0;
}