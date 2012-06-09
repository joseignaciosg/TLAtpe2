/*
 * ProductionTest.c
 *
 *  Created on: Jun 9, 2012
 *      Author: joseignaciosantiagogalindo
 */
#include "../include/Production.h"

#define ERROR 1
#define SUCCESS printf("success\n");

ProductionADT p1;
ProductionADT p2;
ProductionADT p3;
ProductionADT p4;
ProductionADT p5;


void init();
void destroy();
int getProductionComponentTest();
int getSymbolQuantTest();
int setProductionComponentTest();
int equalsTest();

int main(){

	init();

	if(equalsTest() == ERROR  || getProductionComponentTest() == ERROR ||
			getSymbolQuantTest() == ERROR ||
			setProductionComponentTest() == ERROR
			){
		destroy();
		return ERROR;
	}

	destroy();
	return 0;
}

void init(){
	p1 = newProduction(4,'S','A','b','c');
	p2 = newProduction(2,'S','C');
	p3 = newProduction(4,'S','A','b','c');
	p4 = newProduction(8,'S','A','b','c','D','c','d','a');
	p5 = newProduction(4,'S','A','b','d');
}

int equalsTest(){
	printf("TESTING equals()\n");
	if (!equals(p1, p3) ||
		equals(p1, p4) ||
		equals(p1, p5)){
		fprintf(stderr,"The function equals() is not working properly\n");
		return ERROR;
	}
	SUCCESS
	return 0;

}

int getProductionComponentTest(){
	printf("TESTING getProductionComponentTest()\n");
	if (getProductionComponent(p1, 0) != 'S' ||
		getProductionComponent(p1, 3) != 'c' ||
		getProductionComponent(p4, 7) != 'a' ){
		fprintf(stderr,"The function getProductionComponent is not working properly\n");
		return ERROR;
	}
	SUCCESS
	return 0;
}

int getSymbolQuantTest(){
	printf("TESTING getSymbolQuantTest()\n");
	if (getSymbolQuant(p1) != 4 ||
		getSymbolQuant(p2) != 2 ||
		getSymbolQuant(p3) != 4 ||
		getSymbolQuant(p4) != 8 ){
		fprintf(stderr,"The function getProductionComponent is not working properly");
		return ERROR;
	}
	SUCCESS
	return 0;
}

int setProductionComponentTest(){
	printf("TESTING setProductionComponentTest()\n");
	setProductionComponent(p1,0,'A');
	setProductionComponent(p4,7,'A');
	if (	getProductionComponent(p1, 0) != 'A' ||
			getProductionComponent(p4, 7) != 'A' ){
			perror("The function setProductionComponentTest is not working properly");
			return ERROR;
	}
	SUCCESS
	return 0;

}




void destroy(){
	free(p1);
	free(p2);
	free(p3);
	free(p4);
	free(p5);
}
