/*
 * Production.c
 *
 *  Created on: May 14, 2012
 *      Author: joseignaciosantiagogalindo
 */

# include "../include/Production.h"

/*Constructor-destructor*/
ProductionADT newProduction(char left, char * right){
	ProductionADT p = malloc(sizeof(struct Production));
	p->left = left;
	p->right = right;
	p->symbolsquant = strlen(right)+1;
	return p;
}
void freeProduction(ProductionADT p){
	free(p->right);
	free(p);
}

/*Getters*/
char getProductionComponent(ProductionADT p, int i){
	if(i < 0 ||i >= getSymbolQuant(p)){
		return -1;
	}
	return (i == 0)? p->left : p->right[i-1];
}
int getSymbolQuant(ProductionADT p){
	return p->symbolsquant;
}


/*Setters*/
void setProductionComponent(ProductionADT p, int i, char comp){
	if(i >= 0 ||i < getSymbolQuant(p)){
		if(i == 0)
		{
			p->left = comp;
		}
		else
		{
			p->right[i-1] = comp;
		}
	}else{
		fprintf(stderr, "Index out of bound");
	}

}

/*Utility*/
void printProduction(ProductionADT p){
	printf("\t %c -> %s\n",p->left, p->right);
}

int equals(ProductionADT p1, ProductionADT p2){
	int i;
	if(getSymbolQuant(p1) != getSymbolQuant(p2)){
		return 0;
	}
	for (i=0; i< getSymbolQuant(p1); i++){
		if( getProductionComponent(p1,i) != getProductionComponent(p2,i)) {
			return 0;
		}
	}
	return 1;
}
//TODO: Deprecated
int isUnitary(ProductionADT p){
	char sec = getProductionComponent(p,1);
	char third = getProductionComponent(p,2);
	if( ( isNonTerminal(sec) && third == '\\' ) ||
			( isNonTerminal(third) && sec == '\\') ){
		return 1;
	}
	return 0;
}

char getLeftSide(ProductionADT p){
	return p->left;
}
char * getRightSide(ProductionADT p){
	return p->right;
}
