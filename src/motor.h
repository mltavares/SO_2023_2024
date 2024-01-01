  GNU nano 7.2                                                                                          motor.h                                                                                                    
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
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <pthread.h>
#include "comuns.h"

static int child_exit_status;

void comandoUsers(KeyboardHandlerToken *token);
void comandoBots(KeyboardHandlerToken *token);
void comandoBMOV(KeyboardHandlerToken *token);
void comandoRBM(KeyboardHandlerToken *token);
void comandoBegin(KeyboardHandlerToken *token);
void initBmov(KeyboardHandlerToken *token, Bmov *bmov);
int generateRandom(int min, int max);

void lerComandosDosJogadores(PlayerArray *players, Jogo *jogo);


#endif
