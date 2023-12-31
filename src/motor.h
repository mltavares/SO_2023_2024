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
#include <pthread.h>
    
#define MAX_PLAYERS 5
#define MAX_BMOVS 5
#define MAX_BOTS 10
#define PLAYER_NAME_SIZE 20
#define MAX_PIPE_SIZE 12
#define MAX_HEIGHT 16
#define MAX_WIDTH 80
#define LINHAS 16
#define COLUNAS 40

static int child_exit_status;

typedef struct {
    Jogador jogadores[5];
    int nivel;
    char maze[LINHAS][COLUNAS];  
} Jogo;

typedef struct {
    char name[PLAYER_NAME_SIZE];
    char pipe[MAX_PIPE_SIZE];
    pid_t pid;
    char icone;
    int xCoordinate;
    int yCoordinate;
    int isPlaying;
} Player;

typedef struct {
    Player array[MAX_PLAYERS];
    int playerFd[MAX_PLAYERS];
    int nPlayers;
} PlayerArray;

typedef struct {
    int interval;
    int duration;
    pthread_t *botThread;
} Bot;

typedef struct {
    int interval;
    int duration;
} BotToken;

typedef struct {
    Bot bots[MAX_BOTS];
    int nBots;
} BotArray;

typedef struct {
    int x;
    int y;
} Bmov;

typedef struct {
    Bmov bmovs[MAX_BMOVS];
    int nbmovs;
} BmovArray;

typedef struct {
    char array[MAX_HEIGHT][MAX_WIDTH];
} Map;

typedef struct {
    PlayerArray *players;
    BotArray *bots;
    BmovArray *bmovs;
    Map *map;
    int keyboardFeed;
} KeyboardHandlerToken;

void comandoUsers(KeyboardHandlerToken *token);
void comandoBots(KeyboardHandlerToken *token);
void comandoBMOV(KeyboardHandlerToken *token);
void comandoRBM(KeyboardHandlerToken *token);
void comandoBegin(KeyboardHandlerToken *token);
void initBmov(KeyboardHandlerToken *token, Bmov *bmov);
int generateRandom(int min, int max);

void lerComandosDosJogadores(PlayerArray *players, Jogo *jogo);


#endif
