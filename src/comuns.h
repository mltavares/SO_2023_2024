#ifndef COMUNS_H
#define COMUNS_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/select.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#define PRINT(...)  fprintf(__VA_ARGS__)
#define FORMAT(X)   "<ERROR> " X "\n"
#define PERROR(X)   PRINT(stderr, FORMAT(X))

#define MAX_PLAYERS 5
#define MAX_BMOVS 5
#define MAX_BOTS 10
#define PLAYER_NAME_SIZE 20
#define MAX_PIPE_SIZE 12
#define MAX_HEIGHT 16
#define MAX_WIDTH 80
#define LINHAS 16
#define COLUNAS 40

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
    Player players[5];
    int nivel;
    char maze[LINHAS][COLUNAS];  
} Jogo;

typedef struct {
    PlayerArray *players;
    BotArray *bots;
    BmovArray *bmovs;
    Map *map;
    int keyboardFeed;
} KeyboardHandlerToken;

#endif
