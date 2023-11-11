#include "motor.h"

#define MAX_MSG_LEN 1024
#define NLIN 16
#define NCOL 40


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

void lancarBot() {
    printf("Lançar bot\n"); 
    char string[100][MAX_MSG_LEN];
    int descriptor[2];
    
    if(pipe(descriptor) == -1) {
        perror("Erro ao criar pipe");
        return;
    }

    int id = fork();

    if(id == 0) { // Filho
        close(STDOUT_FILENO);
        dup(descriptor[1]);
        close(descriptor[0]);
        close(descriptor[1]);
        
        execl("./bot", "bot", "1", "10", (char *) NULL); // intervalo=1 e duracao=10
        perror("Erro ao lançar bot");
        exit(1);
    } else { // Pai
        int i = 0;
        time_t start_time = time(NULL);
        int timeout = 10; // timeout = 10 segundos 

        while(1) {
            if(difftime(time(NULL), start_time) > timeout) {
                break;
            }
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

    }
}

void exibirMapa() {
    FILE *mapFile;
    char linha[NCOL + 1];

    mapFile = fopen("mapa.txt", "r");

    if (mapFile == NULL) {
        perror("Erro ao abrir o arquivo de mapa");
        return;
    }

    while (fgets(linha, sizeof(linha), mapFile) != NULL) {
        printf("%s", linha);
    }

    fclose(mapFile);
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
            exit(1);
        }
        else if(strcmp(token,"test_bot")==0){
            lancarBot();
        }
        else if(strcmp(token,"mapa")==0){
            exibirMapa();
        }
        else{
            printf("Comando invalido\n");
        }


    }while(1);

}

int main(int argc, char *argv[]){
    VariaveisAmbiente();
    comandosMotor();
    return 0;
}

