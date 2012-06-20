/*
 * Production.h
 *
 *  Created on: May 25, 2012
 *      Author: joseignaciosantiagogalindo
 */

#ifndef PRODUCTION_H_
#define PRODUCTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "utils.h"
#define PRODSIZE 3

typedef struct Production{
	char left;
	char * right;
	int symbolsquant;
}Production;

typedef struct Production * ProductionADT;


/*Constructor-destructor*/
ProductionADT newProduction(char left, char * right);
void freeProduction(ProductionADT p);

/*Getters*/
char getLeftSide(ProductionADT p);
char * getRightSide(ProductionADT p);
char getProductionComponent(ProductionADT p, int i);
int getSymbolQuant(ProductionADT p);

/*Setters*/
/* Sets a component to already created production, if the
 * index is less than 0 or greater that the quantity of
 * symbols of the production, a error should be displayed
 * p : production
 * i: index of the component to be added (should start from 0)
 * comp: component to ve added  */
void setProductionComponent(ProductionADT p, int i, char comp);

/*Utility*/
void printProduction(ProductionADT p);
int equals(ProductionADT p1, ProductionADT p2);
int isUnitary(ProductionADT p);

/*Conversion*/
/*DerivationADT toDerivation(ProductionADT p);*/

#endif /* PRODUCTION_H_ */
