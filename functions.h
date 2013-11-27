#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <string.h>

#define  BUF_SIZE  (32684)

void zeruj(char* tab);
int isDigit(char c);
int isFinished(char *c);

int pierwszyZnak(char *c);

int wczytajNajlizsza(char *c, int i);
void przerobTroszke(char *c, char *d);
int obliczLiczbe(char *c, int pocz);
#endif