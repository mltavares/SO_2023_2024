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
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>


#define LINHAS 16
#define COLUNAS 40

static int child_exit_status;

typedef struct {
    Jogador jogadores[5];
    int nivel;
    char maze[LINHAS][COLUNAS];  
} Jogo;

#endif
