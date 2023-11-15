#ifndef JOGOUI_H
#define JOGOUI_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ncurses.h>

#define MAX_NOME 50

typedef struct {
    char nome[MAX_NOME];
    int pos_x;
    int pos_y;
    int pontuacao;
    bool isPlaying;
} Jogador;

#endif