/*
 * utils.h
 *
 *  Created on: May 25, 2012
 *      Author: joseignaciosantiagogalindo
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int containsChar(char * array , int size, char c);
int getDifferents( char * first,  int firstquant, char * sec, int secquant, char ** notinfirst);
void printArray(char * array, int size);
void addChar(char ** array, int * size, char c);

/*for unitaries auxiliar array*/
void addPair(char ** unitaries, int * size,char first, char sec );
int containsPair(char * unitaries,int size, char first, char sec);
void printByPairs(char * unitaries,int size);


/*for grammar*/
int isTerminal(char symbol);
int isNonTerminal(char symbol);
#endif /* UTILS_H_ */
