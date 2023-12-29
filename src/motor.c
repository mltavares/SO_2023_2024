#include "motor.h"

#define MAX_MSG_LEN 1024
#define NLIN 16
#define NCOL 40
#define LOCK_FILE "/tmp/motor.lock"
#define PIPE_PATH "/tmp/mapa_pipe"

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
    int fd = open(PIPE_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Erro ao abrir o pipe");
        return;
    }

    for (int i = 0; i < LINHAS; i++) {
        write(fd, jogo->maze[i], COLUNAS);
    }

    close(fd);
}


void comandosMotor(){
    char comandos[50];
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
            printf("users detetado\n");
        }
        else if(strcmp(token,"kick")==0){
            char *name_token = strtok(NULL, " ");
            if(name_token == NULL){
                printf("Utilizador invalido\n");
            }
            else{
                printf("O utilizador %s foi kickado\n", name_token);
            }
        }
        else if(strcmp(token,"bots")==0){
            printf("bots detetado\n");
        }
        else if(strcmp(token,"bmov")==0){
            printf("bmov detetado\n");
        }
        else if(strcmp(token,"rbm")==0){
            printf("rbm detetado\n");
        }
        else if(strcmp(token,"begin")==0){
            printf("begin detetado\n");
        }
        else if(strcmp(token,"end")==0){
            printf("end detetado\n");
            remove(LOCK_FILE);
            exit(1);
        }
        else if(strcmp(token,"test_bot")==0){
            lancarBot();
        }
        /*else if(strcmp(token,"mapa")==0){
            exibirMapa();
        }*/
        else{
            printf("Comando invalido\n");
        }
    }

    }while(1);

}

int main(int argc, char *argv[]){
    int lock_fd = open(LOCK_FILE, O_CREAT | O_EXCL, 0644);
    if (lock_fd == -1) {
        perror("Uma instância do motor já está em execução.");
        return 1;
    }

    mkfifo(PIPE_PATH, 0666);

    Jogo jogo;
    carregarMapa(&jogo, "mapa.txt");
    enviarMapa(&jogo);

    VariaveisAmbiente();
    comandosMotor();
    close(lock_fd);
    remove(LOCK_FILE);
    unlink(PIPE_PATH);
    return 0;
}

