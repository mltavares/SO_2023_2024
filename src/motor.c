#include "motor.h"
#include "jogoUI.h"

void comandosMotor(){
    char comandos[50];
    char *token;

    printf("Comando: ");

    do{
        fgets(comandos, 50, stdin);
        token = strtok(comandos, " ");

        if(strcmp(token, "users")==0){
            printf("\nusers detetado\n");
        }
        if(strcmp(token, "kick")==0){
            char *name_token = strtok(NULL, " ");
            printf("\nO utilizador %s foi kickado\n", name_token);
        }
        if(strcmp(token, "bots")==0){
            printf("\nbots detetado\n");
        }
        if(strcmp(token, "bmov")==0){
            printf("\nbmov detetado\n");
        }
        if(strcmp(token, "rbm")==0){
            printf("\nrbm detetado\n");
        }
        if(strcmp(token, "begin")==0){
            printf("\nbegin detetado\n");
        }
        if(strcmp(token, "end")==0){
            printf("\nend detetado\n");
            exit(1);
        }



    }while(1);

}

