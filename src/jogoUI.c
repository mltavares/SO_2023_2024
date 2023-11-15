#include "jogoUI.h"

void comandosJogador(WINDOW *janelaBaixo) {
    char comandos[50];
    char *token;

    //wclear(janelaBaixo);
    //wmove(janelaBaixo, 1, 1); 
    //scrollok(janelaBaixo, TRUE); 

    //echo();

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

    mvwprintw(janela, 0, 1, "JOGADOR: %s\n", nomeJogador); // imprime na primeira linha da janela
    wrefresh(janela); // atualiza a janela para exibir a mensagem
}


void desenhaMapa(WINDOW *janela, int tipo)
{

    // quando temos o scroll ativo, não deveremos ter a borda desenhada na janela para não termos o problema escrever em cima das bordas
    if (tipo == 1)
    {
        scrollok(janela, TRUE); // liga o scroll na "janela".
        //wprintw(janela, "\n #> ");
    }
    else 
    {
        keypad(janela, TRUE); // para ligar as teclas de direção (aplicar à janela)
        wclear(janela);// limpa a janela
        wborder(janela, '|', '|', '-', '-', '+', '+', '+', '+'); // Desenha uma borda. Nota importante: tudo o que escreverem, devem ter em conta a posição da borda
    }
    refresh(); // necessário para atualizar a janela
    wrefresh(janela); // necessário para atualizar a janela
}

void trataTeclado(WINDOW *janelaTopo, WINDOW *janelaBaixo) 
{
    keypad(janelaTopo, TRUE);       // para ligar as teclas de direção (aplicar à janelaTopo)
    wmove(janelaTopo, 1, 1);        // posiciona o cursor,visualmente, na posicao 1,1 da janelaTopo
    //nota a posição é relativa à janelaTopo e não ao ecrã.
    int tecla = wgetch(janelaTopo); // MUITO importante: o input é feito sobre a janela em questão, neste caso na janelaTopo
    char comando[100];             // string que vai armazenar o comando

    while (tecla != 113) // trata as tecla até introduzirem a letra q. O código asci de q é 113
    {

        if (tecla == KEY_UP) // quando o utilizador introduz a seta para cima
        {
            desenhaMapa(janelaTopo, 2); // atualiza toda a janela
            mvwprintw(janelaTopo, 1, 1, "Estou a carregar na tecla UP na posição 1,1 "); // escreve a palavra UP na posição  1,1. 
            //Nota: não escreve na posição 0,0 porque está lá a borda da janela que foi criada anteriormente
            wrefresh(janelaTopo);// // necessário para atualizar a janelaTopo. Nota: é apenas esta janela que pretendemos atualizar
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
        else if (tecla == ' ') // trata a tecla espaço
        {  // a tecla espaço ativa a janela inferior e tem o scroll ativo
          //  o wprintw e o wgetstr devem ser utilizados em janelas que tem o scroll ativo.
            desenhaMapa(janelaTopo, 2);
            mvwprintw(janelaTopo, 1, 1, "Carreguei na tecla ESPACO");
            wrefresh(janelaTopo);
            echo();                         // habilita a maneira como o utilizador visualiza o que escreve
                                             // ou seja volta a ligar o echo para se ver o que se está a escrever
            //wprintw(janelaBaixo, "\n #> "); // utilizada para imprimir. 
                                            //nota como a janelaBaixo tem o scroll ativo, ele vai imprimindo linha a linha
            //wgetstr(janelaBaixo, comando);  // para receber do teclado uma string na "janelaBaixo" para a variavel comando
            //wprintw(janelaBaixo, "\n [%s] ", comando);
            comandosJogador(janelaBaixo); // função que trata os comandos do jogador
            noecho(); //voltar a desabilitar o que o utilizador escreve
            wrefresh(janelaBaixo); //sempre que se escreve numa janela, tem de se fazer refresh
           
        }
        wmove(janelaTopo, 1, 1); // posiciona o cursor (visualmente) na posicao 1,1 da janelaTopo
        tecla = wgetch(janelaTopo); //espera que o utilizador introduza um inteiro. Importante e como já referido anteriormente introduzir a janela onde queremos receber o input
    }
}


int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Login invalido\n");
    }
    else{
        // Inicializa assuntos de ncurses
        // -> mostra que tipo de inicialização pode ser necessária
        initscr(); // Obrigatorio e sempre a primeira operação de ncurses
        raw();  // desativa o buffer de input, cada tecla é lida imediatamente
        noecho();  // desliga o echo no ecrã, para voltar ativar devem utilizar a função echo();
        keypad(stdscr, TRUE);  // habilita as teclas  especiais, exemplo -> <-
        attrset(A_DIM);  //altera o brilho no print das janelas
        //as duas linhas seguintes são utilizadas para imprimir no ecrã, não utilizando nenhuma janela
        //as coordenas são relativas ao ecrã                                                               
        mvprintw(1, 10, "[ Up,Down,Right e Left comandos janela de cima ]");  // mensagem fora da janela, na linha 1, coluna 10 do ecrã
        mvprintw(2, 10, "[ space - muda para o foco da janela de baixo / q - sair ]"); // mensagem fora da janela, na linha 2, coluna 10 do ecrã
        WINDOW *janelaTopo = newwin(22, 82, 3, 1);  // Criar janela para a matriz de jogo, tendo os parametro numero de linhas,numero de colunas, posição onde começa a janela  e posição onde termina
        WINDOW *janelaBaixo = newwin(15, 82, 26, 1);  
        desenhaMapa(janelaTopo, 2);  // função exemplo que desenha o janela no ecrã
        desenhaMapa(janelaBaixo, 1);  // função exemplo que desenha o janela no ecrã
        utilizadorAutenticado(janelaBaixo, argv[1]); 
        trataTeclado(janelaTopo, janelaBaixo); // função exemplo que trata o teclado
        wclear(janelaTopo); // função que limpa o ecrã
        wrefresh(janelaTopo);  // função que faz atualização o ecrã com as operações realizadas anteriormente
        delwin(janelaTopo);  // apaga a janela.
        wclear(janelaBaixo); // função que limpa o ecrã
        wrefresh(janelaBaixo); // função que faz atualiza o ecrã com as operações realizadas anteriormente
        delwin(janelaBaixo);  // apaga a janela.
        endwin();  // encerra a utilização do ncurses. Muito importante senão o terminal fica inconsistente (idem se sair por outras vias)
        return 0;
    }
}
