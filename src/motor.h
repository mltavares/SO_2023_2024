#ifndef MOTOR_H
#define MOTOR_H 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include "jogoUI.h"

static int child_exit_status;

typedef struct {
    Jogador jogadores[5];
    int nivel;
    int maze[16][40];
} Jogo;

#endif