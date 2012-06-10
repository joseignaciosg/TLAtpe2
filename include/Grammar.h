/*
 * Grammar.h
 *
 *  Created on: May 14, 2012
 *      Author: joseignaciosantiagogalindo
 */

#ifndef GRAMMAR_H_
#define GRAMMAR_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Productions.h"
#include "Production.h"
#include "utils.h"

#ifndef LAMDA
#define LAMDA '\\'
#endif

#ifndef _BOOL
#define _BOOL
#define FALSE 0
#define TRUE 1
#endif


typedef struct Grammar{
	ProductionsADT productions;
	char * terminals;
	char * nonterminals;
	int quantterminals;
	int quantnonterminals;
	char distinguished;
}Grammar;

typedef struct Grammar * GrammarADT;

/*Constructor-destructor*/
GrammarADT newGrammar(void);
void freeGrammar(GrammarADT  grammar);

/*Getters*/
char * getTerminals(GrammarADT grammar);
char * getNonTerminals(GrammarADT grammar);
char getDistinguished(GrammarADT grammar);
ProductionsADT  getProductions(GrammarADT grammar);
int getQuantTerminals(GrammarADT grammar);
int getQuantNonTerminals(GrammarADT grammar);

/*Setters*/
void setTerminals(GrammarADT grammar, char * terminals, int quant);
void setNonTerminals(GrammarADT grammar, char * nonterminals, int quant);
void setDistinguished(GrammarADT grammar, char distinguished);
void setProductions(GrammarADT grammar,ProductionsADT productions);

/*Utility*/
void printGrammar(GrammarADT grammar);
void removeUnitaryProductions(GrammarADT grammar);
void removeUnproductiveProductions(GrammarADT grammar);
void removeUnreachableProductions(GrammarADT grammar);
void removeOnlyRightTerminals(GrammarADT grammar);
void convertToRight(GrammarADT grammar);
void toFile(GrammarADT grammar);
int isTerminal(char symbol);
int isNonTerminal(char symbol);
void actualizeTerminals(GrammarADT grammar);
void actualizeNonTerminals(GrammarADT grammar);
void actualizeProductions(GrammarADT grammar);
int isRight(GrammarADT grammar);
char getNewSymbol(GrammarADT grammar);

/*Formalization*/
void formalize(GrammarADT grammar);

/*Conversion*/
void generateASDR(GrammarADT grammar);



#endif /* STRUCTURES_H_ */
