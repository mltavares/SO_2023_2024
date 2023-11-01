#include "jogoUI.h"

void comandosJogador(){
    /*O utilizador jogador pode fazer:
● Jogar no mapa: teclas ←, →, ↓ e ↑
● Mudar para modo de escrita de comando: tecla espaço
● Obter a listagem de todos os jogadores ativos no sistema: comando players
● Enviar mensagem a um utilizador especifico: comando msg <nome_utilizador> <mensagem>
● Sair, encerrando o processo jogoUI: comando exit (o motor deve ser avisado)
*/

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

        if(strcmp(token,"exit")==0){
                printf("\nexit detetado\n");
                exit(1);
        }
        else if(strcmp(token,"players")==0){
            printf("\nplayers detetado\n");
        }
        else if(strcmp(token,"msg")==0){
            char *name_token = strtok(NULL, " ");
            char *msg_token = strtok(NULL, " ");
            printf("\nA mensagem %s foi enviada para o utilizador %s\n", msg_token, name_token);
        }
        else {
            printf("\nComando invalido\n");
        }


    }while(1);


}


int main(int argc, char *argv[]){
    if(argc != 1){
        printf("Login invalido\n");
    }
    else{
        comandosJogador();
    }
}