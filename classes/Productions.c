/*
 * Productions.c
 *
 *  Created on: May 14, 2012
 *      Author: joseignaciosantiagogalindo
 */

#include "../include/Productions.h"

/*Constructor-destructor*/
ProductionsADT newProductions(int n){
	ProductionsADT p = malloc(sizeof(Productions));
	p->productions = calloc(n,sizeof(ProductionADT));
	p->n = n;
	return p;
}
void freeProductions(ProductionsADT  productions){
	free(productions->productions);
	free(productions);
}

/*Getters*/
int  getQuant(ProductionsADT productions){
	return productions->n;
}
ProductionADT getProduction(ProductionsADT productions, int i){
	return productions->productions[i];

}

/*Setters*/
void setProduction(ProductionsADT productions, int i, ProductionADT pr){
	productions->productions[i] = pr;
}

/*Utility*/
void printProductions(ProductionsADT productions){
	printf("Productions: { \n");
	int i;
	for(i=0; i<getQuant(productions);i++){
		printProduction(getProduction(productions,i));
	}
	printf("}\n");
}
int in(ProductionsADT productions, ProductionADT new){
	int i;
	for(i=0; i<getQuant(productions);i++){
		if(equals(new, getProduction(productions,i))){
			return 1;
		}
	}
	return 0;
}
void addProduction(ProductionsADT productions, ProductionADT new){
	ProductionADT * aux;
	int n = getQuant(productions);
	if( !in(productions, new) ){
		if ( ( aux = realloc(productions->productions, (n+1)*sizeof(ProductionADT)) ) == NULL ){
			fprintf(stderr, "Error doing realloc \n");
		}
		productions->productions = aux;
		productions->productions[n] = new;
		productions->n = n+1;
	}
}
void removeProduction(ProductionsADT productions, char leftsymbol){
	/*if rightsymbol islower return -1 */
	int i, n = getQuant(productions);
	ProductionADT * auxi = NULL;
	for(i=0; i<n; i++){
		if ( getProductionComponent(getProduction(productions,i),0) == leftsymbol){
			if (n-1 != i ){ /*is not the last production*/
				while ( getProductionComponent(getProduction(productions,n-1),0) == leftsymbol &&
						n-1 != i){
					n--;
				}
				ProductionADT aux = getProduction(productions,n-1);
				setProduction(productions,i,aux);
			}
			if ( (auxi = realloc(productions->productions,(--n)*sizeof(ProductionADT))) == NULL ){
				fprintf(stderr, "Error doing realloc \n");
			}
			productions->productions = auxi;
			productions->n = n;
		}
	}
}

void removeParticularProduction(ProductionsADT productions, ProductionADT p){
	int i,n = getQuant(productions);
	ProductionADT * auxi = NULL;
	for(i=0; i<n;i++){
		if ( equals(getProduction(productions,i),p) ){
			if (n-1 != i ){ /*is not the last production*/
				setProduction(productions,i,getProduction(productions,n-1));
			}
			if ( (auxi = realloc(productions->productions,(--n)*sizeof(ProductionADT))) == NULL ){
				fprintf(stderr, "Error doing realloc \n");
			}
			productions->productions = auxi;
			productions->n = n;
		}
	}
}

void removeProductionsContaining(ProductionsADT productions, char nonterm){
	if( isTerminal(nonterm)){
		return;
	}
	int quantproductions = getQuant(productions);
	int i;
	for(i=0;i< quantproductions; i++){
		ProductionADT p = getProduction(productions,i);
		char first = getProductionComponent(p,0);
		char sec = getProductionComponent(p,1);
		char third = getProductionComponent(p,2);
		if ( first == nonterm || sec == nonterm  || third == nonterm ){
			removeParticularProduction(productions,p);
		}
	}

}

int inCurrentProductions(ProductionsADT productions, char c){
	int quantproductions = getQuant(productions),i;
	for (i=0; i<quantproductions; i++){
		char first = getProductionComponent(getProduction(productions,i),0);
		if ( first == c ){
			return 1;
		}
	}
	return 0;
}



