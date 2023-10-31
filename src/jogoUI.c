#include "jogoUI.h"

struct jogoUI j[20];

void comandosJogador(){
    /*O utilizador jogador pode fazer:
● Jogar no mapa: teclas ←, →, ↓ e ↑
● Mudar para modo de escrita de comando: tecla espaço
● Obter a listagem de todos os jogadores ativos no sistema: comando players
● Enviar mensagem a um utilizador especifico: comando msg <nome_utilizador> <mensagem>
● Sair, encerrando o processo jogoUI: comando exit (o motor deve ser avisado)
*/

    char comandos[300];
    char *token;

    do{
        printf("Comando: ");
        fgets(comandos, 300, stdin);
        token = strtok(comandos, " ");

        if(strcmp(token,"exit")==0){
                printf("\nTerminado\n");
                exit(1);
        }
        else if(strcmp(token,"players")==0){
            printf("\nplayers detetado\n");
        }
        else if(strcmp(token,"msg")==0){
            char *name_token = strtok(NULL, " ");
            char *msg_token = strtok(NULL, " ");
            printf("\nA mensagem %s foi enviada para o utilozador %s\n", msg_token, name_token);
        }


    }while(1);


}


int main(int argc, char *argv[]){
    if(argc != 1){
        printf("Login invalido\n");
    }
}