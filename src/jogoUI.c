#include "jogoUI.h"

void comandosJogador(WINDOW *janelaBaixo) {
    char comandos[50];
    char *token;

    wprintw(janelaBaixo, "\nComando: ");
    wrefresh(janelaBaixo);

    // Lê a entrada do usuário na janelaBaixo
    wgetnstr(janelaBaixo, comandos, sizeof(comandos)-1);

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
                wprintw(janelaBaixo, "Comando 'msg' inválido. Verifique os parametros.");
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


void desenhaMapa(WINDOW *janela, int tipo)
{

    
    if (tipo == 1)
    {
        scrollok(janela, TRUE); 
    }
    else 
    {
        keypad(janela, TRUE); 
        wclear(janela);
        wborder(janela, '|', '|', '-', '-', '+', '+', '+', '+'); 
    }
    refresh(); 
    wrefresh(janela); 
}

void trataTeclado(WINDOW *janelaTopo, WINDOW *janelaBaixo) 
{
    keypad(janelaTopo, TRUE);       
    wmove(janelaTopo, 1, 1);       
   
    int tecla = wgetch(janelaTopo); 
    char comando[100];           

    while (tecla != 113) 
    {

        if (tecla == KEY_UP) 
        {
            desenhaMapa(janelaTopo, 2); 
            mvwprintw(janelaTopo, 1, 1, "Estou a carregar na tecla UP na posição 1,1 "); 
           
            wrefresh(janelaTopo);
        }
        else if (tecla == KEY_RIGHT)
        {
            desenhaMapa(janelaTopo, 2);
            mvwprintw(janelaTopo, 1, 1, "Estou a carregar na tecla RIGHT na posição 1,1");
            wrefresh(janelaTopo);
        }
        else if (tecla == KEY_LEFT)
        {
            desenhaMapa(janelaTopo, 2);
            mvwprintw(janelaTopo, 1, 1, "Estou a carregar na tecla LEFT na posição 1,1");
            wrefresh(janelaTopo);
        }
        else if (tecla == KEY_DOWN)
        {
            desenhaMapa(janelaTopo, 2);
            mvwprintw(janelaTopo, 1, 1, "Estou a carregar na tecla DOWN na posição 1,1");
            wrefresh(janelaTopo);
        }
        else if (tecla == ' ') 
        {  
          
            desenhaMapa(janelaTopo, 2);
            mvwprintw(janelaTopo, 1, 1, "Carreguei na tecla ESPACO");
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


int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Login invalido\n");
    }
    else{
       
        initscr(); 
        raw();  
        noecho();  
        keypad(stdscr, TRUE);  
        attrset(A_DIM);                                                             
        mvprintw(1, 10, "[ Up,Down,Right e Left comandos janela de cima ]");  
        mvprintw(2, 10, "[ space - muda para o foco da janela de baixo / q - sair ]"); 
        WINDOW *janelaTopo = newwin(22, 82, 3, 1);  
        WINDOW *janelaBaixo = newwin(15, 82, 26, 1);  
        desenhaMapa(janelaTopo, 2); 
        desenhaMapa(janelaBaixo, 1);  
        utilizadorAutenticado(janelaBaixo, argv[1]); 
        trataTeclado(janelaTopo, janelaBaixo); 
        wclear(janelaTopo); 
        wrefresh(janelaTopo);  
        delwin(janelaTopo);  
        wclear(janelaBaixo); 
        wrefresh(janelaBaixo); 
        delwin(janelaBaixo);  
        endwin();  
        return 0;
    }
}
