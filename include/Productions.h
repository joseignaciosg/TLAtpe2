/*
 * Productions.h
 *
 *  Created on: May 14, 2012
 *      Author: joseignaciosantiagogalindo
 */

#ifndef PRODUCTIONS_H_
#define PRODUCTIONS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Production.h"

typedef struct Productions{
	int n;
	ProductionADT * productions;
}Productions;

typedef struct Productions * ProductionsADT;

/*Constructor-destructor*/
ProductionsADT newProductions(int n);
void freeProductions(ProductionsADT productions);

/*Getters*/
int getQuant(ProductionsADT productions);
ProductionADT getProduction(ProductionsADT productions, int i);

/*Setters*/
void setProduction(ProductionsADT productions, int i, ProductionADT pr);

/*Utility*/
void printProductions(ProductionsADT productions);
void removeProduction(ProductionsADT productions, char rightsymbol);
int in(ProductionsADT productions, ProductionADT new);
void addProduction(ProductionsADT productions, ProductionADT new);
void removeParticularProduction(ProductionsADT productions, ProductionADT p);
void removeProductionsContaining(ProductionsADT productions, char nonterm);
int inCurrentProductions(ProductionsADT productions, char c);


#endif /* PRODUCTIONS_H_ */
