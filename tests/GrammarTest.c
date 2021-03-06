/*
 * GrammarTest.c
 *
 *  Created on: May 14, 2012
 *      Author: joseignaciosantiagogalindo
 */
#include <stdio.h>
#include <stdlib.h>
#include "../include/Grammar.h"
#include "../include/Productions.h"
#include "../include/Production.h"

void init();
void destroy();
void basicTest();
void removeProductionsTest();
void addProductionsTest();
void removeUnreachableProductionsTest();
void removeOnlyRightTerminalsTest();
void removeUnitaryProductionsTest();
void removeParticularProductionTest();
void convertToRightTest();
void grammar1Test();
void grammar4Test();
void grammar5Test();
void generateASDRTest();

ProductionsADT prods;
GrammarADT g1;
char * nonterminals;
char * term;
ProductionADT p1;
ProductionADT p2;
ProductionADT p3;
ProductionADT p4;
ProductionADT p5;
ProductionADT p6;
ProductionADT p7;
ProductionADT p8;

int main(void) {
	init();
	//basicTest();
	//addProductionsTest();
	//grammarToAutomata();
	//removeProductionsTest();
	//removeUnreachableProductionsTest();
	//removeOnlyRightTerminalsTest();
	//removeParticularProductionTest();
	//removeUnitaryProductionsTest();
	//convertToRightTest();
	//grammar1Test();
	//grammar4Test();
	//grammar5Test();
	//printGrammar(g1);
	generateASDRTest();

	destroy();
	return 0;
}

void generateASDRTest() {
	generateASDR(g1);
}

void init() {
	g1 = newGrammar();
	setDistinguished(g1, 'S');
	nonterminals = malloc(sizeof(char) * 44);
	nonterminals[0] = 'S';
	nonterminals[1] = 'A';
	nonterminals[2] = 'B';
	nonterminals[3] = 'C';
	setNonTerminals(g1, nonterminals, 4);
	term = malloc(sizeof(char) * 3);
	term[0] = 'a';
	term[1] = 'b';
	term[2] = 'c';
	setTerminals(g1, term, 3);

	p1 = newProduction(4, 'S', 'a', 'A', 'a');
	p2 = newProduction(4, 'A', 'b','B','b');
	p3 = newProduction(4, 'B', 'c','C','c');
	p4 = newProduction(4, 'B', 'c','S','c');
	p5 = newProduction(2, 'C', 'a');
	prods = newProductions(5);
	setProduction(prods, 0, p1);
	setProduction(prods, 1, p2);
	setProduction(prods, 2, p3);
	setProduction(prods, 3, p4);
	setProduction(prods, 4, p5);


	setProductions(g1, prods);
}

void destroy() {
	free(nonterminals);
	free(term);
	freeProduction(p1);
	freeProduction(p2);
	freeProductions(prods);
	freeGrammar(g1);
}

void basicTest() {
	printf("Testing Grammar Structures / Basic \n");
	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(g1);
	actualizeNonTerminals(g1);
	printGrammar(g1);
	printf("productions quant: %d\n", getQuant(getProductions(g1)));
}

void removeProductionsTest() {
	printf("\nTesting Remove Productions Method \n");
	printf("to remove: all starting with 'A' \n");
	removeProduction(getProductions(g1), 'A');
	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(g1);
	actualizeNonTerminals(g1);
	printGrammar(g1);
	printf("productions quant: %d\n", getQuant(getProductions(g1)));
}

void addProductionsTest() {
	printf("\nTesting Add Productions Method \n");
	addProduction(getProductions(g1), newProduction(3, 'S', LAMDA, 'A'));
	addProduction(getProductions(g1), newProduction(3, 'B', LAMDA, 'C'));
	addProduction(getProductions(g1), newProduction(3, 'C', LAMDA, 'D'));
	printGrammar(g1);
	printf("productions quant: %d\n", getQuant(getProductions(g1)));
}

void removeUnreachableProductionsTest() {
	printf("\nTesting Remove Unreachable Productions Method \n");
	removeUnreachableProductions(g1);
	printGrammar(g1);
	printf("productions quant: %d\n", getQuant(getProductions(g1)));
}

void removeUnitaryProductionsTest() {
	printf("\nTesting Remove Unitary Productions Method \n");
	printGrammar(g1);
	removeUnitaryProductions(g1);
	printGrammar(g1);
	printf("productions quant: %d\n", getQuant(getProductions(g1)));
}

void removeOnlyRightTerminalsTest() {
	printf("\nTesting Remove Only Right Terminals Method \n");
	printGrammar(g1);
	removeOnlyRightTerminals(g1);
	printGrammar(g1);
}

void removeParticularProductionTest() {
	printf("\nTesting Remove Particular Productions Method \n");
	removeParticularProduction(getProductions(g1),
			newProduction(3, 'A', LAMDA, 'b'));
	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(g1);
	actualizeNonTerminals(g1);
	printGrammar(g1);
	printf("productions quant: %d\n", getQuant(getProductions(g1)));
}

void convertToRightTest() {
	printf("\nTesting Convert To Right Method\n");
	convertToRight(g1);
	printGrammar(g1);
	printf("productions quant: %d\n", getQuant(getProductions(g1)));
}

void grammar1Test() {
	//G1 = ({A, B, C}, {a, b, c},A, {A -> aB|c, B -> aA|b})

	/*Initialization*/
	GrammarADT grammar1 = newGrammar();
	grammar1 = newGrammar();
	setDistinguished(grammar1, 'A');
	char * nonterminals1 = malloc(sizeof(char) * 3);
	nonterminals1[0] = 'A';
	nonterminals1[1] = 'B';
	nonterminals1[2] = 'C';
	setNonTerminals(grammar1, nonterminals1, 3);
	char * term1 = malloc(sizeof(char) * 3);
	term1[0] = 'a';
	term1[1] = 'b';
	term1[2] = 'c';
	setTerminals(grammar1, term1, 3);

	ProductionADT prod1 = newProduction(3, 'A', 'a', 'B');
	ProductionADT prod2 = newProduction(3, 'A', 'c', LAMDA);
	ProductionADT prod3 = newProduction(3, 'B', 'a', 'A');
	ProductionADT prod4 = newProduction(3, 'B', LAMDA, 'b');
	//ProductionsADT productions1 = newProductions(0);

	ProductionsADT productions1 = newProductions(4);
	setProduction(productions1, 0, prod1);
	setProduction(productions1, 1, prod2);
	setProduction(productions1, 2, prod3);
	setProduction(productions1, 3, prod4);

	/*addProduction(productions1,prod1);
	 addProduction(productions1,prod2);
	 addProduction(productions1,prod3);
	 addProduction(productions1,prod4);*/

	setProductions(grammar1, productions1);

	printf("Before Nomalization\n");
	printGrammar(grammar1);

}

void grammar4Test() {
	//Gramatica4 = ({S, B, C}, {a,b,c}, S,
	//{S->B | a,
	//B->cS | \,
	//C->bC | \})

	GrammarADT grammar4 = newGrammar();
	grammar4 = newGrammar();
	setDistinguished(grammar4, 'S');
	char * nonterminals1 = malloc(sizeof(char) * 3);
	nonterminals1[0] = 'S';
	nonterminals1[1] = 'B';
	nonterminals1[2] = 'C';
	setNonTerminals(grammar4, nonterminals1, 3);
	char * term1 = malloc(sizeof(char) * 3);
	term1[0] = 'a';
	term1[1] = 'b';
	term1[2] = 'c';
	setTerminals(grammar4, term1, 3);

	ProductionADT prod1 = newProduction(3, 'S', LAMDA, 'B');
	ProductionADT prod2 = newProduction(3, 'S', 'a', LAMDA);
	ProductionADT prod3 = newProduction(3, 'B', 'c', 'S');
	ProductionADT prod4 = newProduction(3, 'B', LAMDA, LAMDA);
	ProductionADT prod5 = newProduction(3, 'C', 'b', 'C');
	ProductionADT prod6 = newProduction(3, 'C', LAMDA, LAMDA);
	//ProductionsADT productions1 = newProductions(0);

	ProductionsADT productions1 = newProductions(6);
	setProduction(productions1, 0, prod1);
	setProduction(productions1, 1, prod2);
	setProduction(productions1, 2, prod3);
	setProduction(productions1, 3, prod4);
	setProduction(productions1, 4, prod5);
	setProduction(productions1, 5, prod6);

	/*addProduction(productions1,prod1);
	 addProduction(productions1,prod2);
	 addProduction(productions1,prod3);
	 addProduction(productions1,prod4);*/

	setProductions(grammar4, productions1);

	printf("Before Nomalization\n");
	printGrammar(grammar4);

}

void grammar5Test() {
	/*Gr5 = ({S, B, C}, {a,b, c}, S,
	 {S->B | a,
	 B->cC | \,
	 C->bC})*/

	GrammarADT grammar5 = newGrammar();
	grammar5 = newGrammar();
	setDistinguished(grammar5, 'S');
	char * nonterminals1 = malloc(sizeof(char) * 3);
	nonterminals1[0] = 'S';
	nonterminals1[1] = 'B';
	nonterminals1[2] = 'C';
	setNonTerminals(grammar5, nonterminals1, 3);
	char * term1 = malloc(sizeof(char) * 3);
	term1[0] = 'a';
	term1[1] = 'b';
	term1[2] = 'c';
	setTerminals(grammar5, term1, 3);

	ProductionADT prod1 = newProduction(3, 'S', 'B', LAMDA);
	ProductionADT prod2 = newProduction(3, 'S', 'a', LAMDA);
	ProductionADT prod3 = newProduction(3, 'B', 'c', 'C');
	ProductionADT prod4 = newProduction(3, 'B', LAMDA, LAMDA);
	ProductionADT prod5 = newProduction(3, 'C', 'b', 'C');
	//ProductionsADT productions1 = newProductions(0);

	ProductionsADT productions1 = newProductions(5);
	setProduction(productions1, 0, prod1);
	setProduction(productions1, 1, prod2);
	setProduction(productions1, 2, prod3);
	setProduction(productions1, 3, prod4);
	setProduction(productions1, 4, prod5);

	/*addProduction(productions1,prod1);
	 addProduction(productions1,prod2);
	 addProduction(productions1,prod3);
	 addProduction(productions1,prod4);*/

	setProductions(grammar5, productions1);

	//formalize(grammar5);
	printGrammar(grammar5);
	generateASDR(grammar5);


}
