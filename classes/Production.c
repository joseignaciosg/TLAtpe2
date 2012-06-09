/*
 * Production.c
 *
 *  Created on: May 14, 2012
 *      Author: joseignaciosantiagogalindo
 */

# include "../include/Production.h"

/*Constructor-destructor*/
ProductionADT newProduction(int num, ...){
	ProductionADT p = malloc(sizeof(struct Production));
	p->production = malloc(sizeof(char)* num);
	p->symbolsquant = num;
	va_list ap;
	va_start(ap, num);
	int i=0;
	char symbol;
	while(i<num){
		symbol = va_arg(ap,int);
		setProductionComponent(p,i,symbol);
		i++;
	}
	void va_end(va_list ap);
	return p;
}
void freeProduction(ProductionADT p){
	free(p->production);
	free(p);
}

/*Getters*/
char getProductionComponent(ProductionADT p, int i){
	if(i < 0 ||i >= getSymbolQuant(p)){
		return -1;
	}
	return p->production[i];
}
int getSymbolQuant(ProductionADT p){
	return p->symbolsquant;
}


/*Setters*/
void setProductionComponent(ProductionADT p, int i, char comp){
	if(i >= 0 ||i < getSymbolQuant(p)){
		p->production[i] = comp;
	}else{
		fprintf(stderr, "Index out of bound");
	}

}

/*Utility*/
void printProduction(ProductionADT p){
	int i;
	printf("\t %c -> ",p->production[0]);
	for (i=1; i< p->symbolsquant; i++){
		printf("%c",p->production[i]);
	}
	printf("\n");
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
