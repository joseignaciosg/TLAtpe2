#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/Production.h"
#include "include/utils.h"
#define TRUE 1
#define FALSE 0
ProductionADT * prods0;
int prods0quant;
int pn0(int *, char * w);
ProductionADT * prods1;
int prods1quant;
int pn1(int *, char * w);
int procesar(ProductionADT p, char * w, int * t);
void init();

int main(int argc, char *argv[]) {
	if(argc!=2) { /* argc should be 2 for correct execution*/
		printf("usage : %s string \n ", argv[0]);
		exit(1);
	} else {
		printf("%s is being processed\n", argv[1]);
	}
	char * w = argv[1];
	init();
	int t = 0;
	if(!pn0(&t,w) && (t==strlen(w))){
		printf("(The chain of used derivations is upside down)\n");
		printf("The string has been accepted \n");
	} else {
		printf("The string has NOT been accepted\n");
	}
	return 0;
}

/*initializes all the structures*/
void init(){
	prods0 = malloc(sizeof(ProductionADT*) * 2);
	prods0quant = 2;
	prods0[0] = newProduction('A', "aBC");
	prods0[1] = newProduction('A', "cB");
	prods1 = malloc(sizeof(ProductionADT*) * 2);
	prods1quant = 2;
	prods1[0] = newProduction('B', "aA");
	prods1[1] = newProduction('B', "b");
}

/*returns TRUE if there is an error*/
int procesar(ProductionADT p, char * w, int * t) {
	int i;
	int n = getSymbolQuant(p); /*number of symbols in the right side of the production*/
	for (i = 1; i < n; i++) {
		char comp = getProductionComponent(p, i);
		if (isTerminal(comp)) {
			if (w[*t] == comp) {
				(*t) += 1;
			} else {
				return TRUE;/*error*/
			}
		} else {
			int error= FALSE;
			switch(comp){
			case 'A':
				error = pn0(t,w);
				break;
			case 'B':
				error = pn1(t,w);
				break;
			}
			if(error){
				return TRUE;
			}
		}
	}
	return FALSE;
}

int pn0(int *t, char * w) {
	int j;
	int n = prods0quant; /*quantity of productions with this termina */
	int error = TRUE;
	for(j = 0; j < n && error; j++) {
		error = procesar(prods0[j], w, t);
		if(!error){
			printProduction(prods0[j]);
		}
	}
	return error;
}

int pn1(int *t, char * w) {
	int j;
	int n = prods1quant; /*quantity of productions with this termina */
	int error = TRUE;
	for(j = 0; j < n && error; j++) {
		error = procesar(prods1[j], w, t);
		if(!error){
			printProduction(prods1[j]);
		}
	}
	return error;
}
