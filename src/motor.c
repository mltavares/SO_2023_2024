#include "motor.h"
#include "jogoUI.h"

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
            //lancar um bot
            printf("A lancar bot...\n");
        }
        else{
            printf("Comando invalido\n");
        }



    }while(1);

}

int main(int argc, char *argv[]){
    comandosMotor();
    return 0;
}

