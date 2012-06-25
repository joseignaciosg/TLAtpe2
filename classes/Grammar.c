/*
 * Grammar.c
 *
 *  Created on: May 14, 2012
 *      Author: joseignaciosantiagogalindo
 */
#include "../include/Grammar.h"

/*Constructor-destructor*/
GrammarADT newGrammar(void) {
	GrammarADT grammar;
	grammar = malloc(sizeof(struct Grammar));
	/*grammar->productions = malloc(sizeof(ProductionADT));*/
	return grammar;
}
void freeGrammar(GrammarADT grammar) {
	free(grammar->nonterminals);
	free(grammar->terminals);
	free(grammar);
}

/*Getters*/
char * getTerminals(GrammarADT grammar) {
	return grammar->terminals;
}
char * getNonTerminals(GrammarADT grammar) {
	return grammar->nonterminals;
}
char getDistinguished(GrammarADT grammar) {
	return grammar->distinguished;
}
ProductionsADT getProductions(GrammarADT grammar) {
	return grammar->productions;
}
int getQuantTerminals(GrammarADT grammar) {
	return grammar->quantterminals;
}
int getQuantNonTerminals(GrammarADT grammar) {
	return grammar->quantnonterminals;
}

/*Setters*/
void setTerminals(GrammarADT grammar, char * terminals, int quant) {
	grammar->terminals = malloc(sizeof(char) * quant);
	memcpy(grammar->terminals, terminals, quant);
	grammar->quantterminals = quant;
}
void setNonTerminals(GrammarADT grammar, char * nonterminals, int quant) {
	grammar->nonterminals = malloc(sizeof(char) * quant);
	memcpy(grammar->nonterminals, nonterminals, quant);
	grammar->quantnonterminals = quant;
}
void setDistinguished(GrammarADT grammar, char distinguished) {
	grammar->distinguished = distinguished;
}
void setProductions(GrammarADT grammar, ProductionsADT productions) {
	grammar->productions = productions;
}

/*Utility*/
void printGrammar(GrammarADT grammar) {
	printf("Grammar\n");
	printf("Distinguished: %c \n", getDistinguished(grammar));
	int i;
	printf("Terminals: {");
	for (i = 0; i < getQuantTerminals(grammar); i++) {
		printf(" %c ", getTerminals(grammar)[i]);
	}
	printf("}\n");
	printf("Non Terminals: {");
	for (i = 0; i < getQuantNonTerminals(grammar); i++) {
		printf(" %c ", getNonTerminals(grammar)[i]);
	}
	printf("}\n");
	printProductions(getProductions(grammar));
	printf("Production Quant %d\n", getQuant(getProductions(grammar)));
	return;
}

void removeUnitaryProductions(GrammarADT grammar) {
	ProductionsADT productions = getProductions(grammar);
	int i, j, k, productionquant = getQuant(productions), unitaryquant = 0,
			lastunitaryquant = 0;
	/*auxiliar array for unitary productions*/
	char * unitaries = NULL;
	/*iterate over productions and determine first unitaries:
	 * the productions that have only one non terminal symbol
	 * on the right side */
	for (i = 0; i < productionquant; i++) {
		char first = getProductionComponent(getProduction(productions, i), 0);
		char sec = getProductionComponent(getProduction(productions, i), 1);
		char third = getProductionComponent(getProduction(productions, i), 2);
		if (isNonTerminal(sec) && third == LAMDA) {
			addPair(&unitaries, &unitaryquant, first, sec);
		} else if (isNonTerminal(third) && sec == LAMDA) {
			addPair(&unitaries, &unitaryquant, first, third);
		}
	}
	/*iterate over unitaries, adding the closure*/
	while (unitaryquant != lastunitaryquant) {
		lastunitaryquant = unitaryquant;
		for (i = 0; i < unitaryquant; i += 2) {
			char first1 = unitaries[i];
			char sec1 = unitaries[i + 1];
			for (j = 0; j < unitaryquant; j += 2) {
				char first2 = unitaries[j];
				char sec2 = unitaries[j + 1];
				/*(A,B)(B,C)-> (A,C)*/
				if (sec1 == first2) {
					if (!containsPair(unitaries, unitaryquant, first1, sec2)
							&& first1 != sec2) { /*no sense in adding (A,A) unitaries*/
						addPair(&unitaries, &unitaryquant, first1, sec2);
					}
				}
			}
		}
	}
	/*Debug*/
	//printByPairs(unitaries,unitaryquant);
	//printf("unitaries quant: %d\n\n", unitaryquant/2);
	/*create the new productions and remove the unitaries*/
	for (i = 0; i < productionquant; i++) {
		ProductionADT p1 = getProduction(productions, i);
		if (isUnitary(p1)) {
			char first1 = getProductionComponent(p1, 0);
			char sec1 = getProductionComponent(p1, 1);
			char third1 = getProductionComponent(p1, 2);
			for (j = 0; j < unitaryquant; j += 2) {
				char uni1 = unitaries[j];
				char uni2 = unitaries[j + 1];
				//A->B and (A,B) (unitary production is localized)
				if ((first1 == uni1) && (sec1 == uni2 || third1 == uni2)) {
					for (k = 0; k < productionquant; k++) {
						ProductionADT p2 = getProduction(productions, k);
						char first2 = getProductionComponent(p2, 0);
						char sec2 = getProductionComponent(p2, 1);
						char third2 = getProductionComponent(p2, 2);
						if (!isUnitary(p2)) {
							if (first2 == uni2) {
								char * r = malloc(sizeof(char)*2);
								r[0] = sec2;
								r[1] = third2;
								addProduction(productions,
										newProduction(first1, r));
							}
						}
					}
				}
			}
			removeParticularProduction(productions, p1);
			free(p1);
		}
	}
	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(grammar);
	actualizeNonTerminals(grammar);
	actualizeProductions(grammar);

}

void removeUnproductiveProductions(GrammarADT grammar) {
	ProductionsADT productions = getProductions(grammar);
	int i, quantproductions = getQuant(productions), productivequant = 0,
			lastproductivequant = -1;
	char * productives = NULL;
	char * aux1 = NULL;
	while (productivequant != lastproductivequant) {
		lastproductivequant = productivequant;
		for (i = 0; i < quantproductions; i++) {
			ProductionADT p1 = getProduction(productions, i);
			char first1 = getProductionComponent(p1, 0);
			char sec1 = getProductionComponent(p1, 1);
			char third1 = getProductionComponent(p1, 2);
			if (!containsChar(productives, productivequant, first1)) {
				if ((sec1 == LAMDA && third1 == LAMDA) || /*lamda*/
				(isTerminal(sec1) && isTerminal(third1)) || /*both terminal*/
				(isTerminal(sec1) && third1 == LAMDA) || /*one terminal*/
				(isTerminal(third1) && sec1 == LAMDA)
						||
						/*one terminal and one productive*/
						(isTerminal(sec1)
								&& (isNonTerminal(third1)
										&& containsChar(productives,
												productivequant, third1)))
						|| (isTerminal(third1)
								&& (isNonTerminal(sec1)
										&& containsChar(productives,
												productivequant, sec1)))
						|| (sec1 == LAMDA
								&& (isNonTerminal(third1)
										&& containsChar(productives,
												productivequant, third1)))
						|| (third1 == LAMDA
								&& (isNonTerminal(sec1)
										&& containsChar(productives,
												productivequant, sec1)))) {
					if ((aux1 = realloc(productives,
							sizeof(char) * (productivequant + 1))) == NULL) {
						fprintf(stderr, "Error doing realloc \n");
					}
					productives = aux1;
					productives[productivequant++] = first1;
				}
			}
		}
	}
	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(grammar);
	actualizeNonTerminals(grammar);
	actualizeProductions(grammar);
}

void removeUnreachableProductions(GrammarADT grammar) {
	ProductionsADT productions = getProductions(grammar);
	int i, quantproductions = getQuant(productions), reachablesquant = 0,
			lastreachablesquant = 0;
	char * reachables = malloc(sizeof(char));
	char * aux1 = NULL;
	/*starts only with distinguished symbol, if it is in the current productions*/
	if (inCurrentProductions(productions, getDistinguished(grammar))) {
		reachables[reachablesquant++] = getDistinguished(grammar);
	}
	/*until something the quantity of reachables varies*/
	while (reachablesquant != lastreachablesquant) {
		lastreachablesquant = reachablesquant;
		for (i = 0; i < quantproductions; i++) {
			int j=1;
			char nextS;
			char first = getProductionComponent(getProduction(productions, i),
								0);
			if (containsChar(reachables, reachablesquant, first)) {
				for(;getProductionComponent(getProduction(productions, i),
						j)!=-1;j++){
					nextS=getProductionComponent(getProduction(productions, i),
							j);
					if (isNonTerminal(nextS)
											&& !containsChar(reachables, reachablesquant, nextS)) {
										if ((aux1 = realloc(reachables,
												sizeof(char) * (reachablesquant + 1))) == NULL) {
											fprintf(stderr, "Error doing realloc \n");
										}
										reachables = aux1;
										reachables[reachablesquant++] = nextS;
									}
				}
			}

		}
	}
	/*TODO: delete debug printf*/
	printf("\nReachables!!: ");
	printArray(reachables, reachablesquant);
	int symsToRemovequant = 0;
	/*remove the unreachable productions*/
	/*If the quantity of reachables is equal to the quantity of nonterminals,
	 * nothing should be removed*/
	if (reachablesquant != getQuantNonTerminals(grammar)) {
		char * symsToRemove = NULL;
		symsToRemovequant = getDifferents(getNonTerminals(grammar),
				getQuantNonTerminals(grammar), reachables, reachablesquant,
				&symsToRemove);
		printf("\nTO REMOVE:");
		printArray(symsToRemove, symsToRemovequant);
		for (i = 0; i < symsToRemovequant; i++) {
			removeProduction(productions, symsToRemove[i]);
		}
	}
	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(grammar);
	actualizeNonTerminals(grammar);
	actualizeProductions(grammar);

}

void removeOnlyRightTerminals(GrammarADT grammar) {
	int j;
	int found = FALSE;
	char newsymbol;

	ProductionsADT productions = getProductions(grammar);

	/*search for A->a like productions*/
	for (j = 0; j < getQuant(getProductions(grammar)); j++) {
		ProductionADT p = getProduction(productions, j);
		char sec = getProductionComponent(p, 1);
		char third = getProductionComponent(p, 2);
		if ((isTerminal(sec) && third == LAMDA)
				|| (isTerminal(third) && sec == LAMDA)) {
			found = TRUE;
			break;
		}
	}
	/*if there are not productions with only terminals in the right side
	 * ,go away*/
	if (!found) {
		return;
	}
	/*search for the new symbol to insert*/
	newsymbol = getNewSymbol(grammar);

	/*add new symbol to the nonterminals array*/
	char * term = getNonTerminals(grammar);
	int termquant = getQuantNonTerminals(grammar);
	char * aux = realloc(term, (termquant + 1) * sizeof(char));
	if (aux == NULL) {
		printf("Error: Not enought memory\n");
		exit(1);
	}
	term = aux;
	term[termquant] = newsymbol;
	termquant++;
	setNonTerminals(grammar, term, termquant);

	int isright = isRight(grammar);
	int productionquant = getQuant(getProductions(grammar));
	/*create the new productions with the new symbol*/
	for (j = 0; j < productionquant; j++) {
		ProductionADT p = getProduction(getProductions(grammar), j);
		char sec = getProductionComponent(p, 1);
		char third = getProductionComponent(p, 2);
		/*is the production have only one terminal symbol on the right side */
		if (isTerminal(sec) && third == LAMDA) {
			/*if the grammar is right sided the new symbol should be
			 * added to the right*/
			if (isright) {
				setProductionComponent(p, 1, sec);
				setProductionComponent(p, 2, newsymbol);
			} else {
				setProductionComponent(p, 2, sec);
				setProductionComponent(p, 1, newsymbol);
			}
		} else if (isTerminal(third) && sec == LAMDA) {
			if (isright) {
				setProductionComponent(p, 1, third);
				setProductionComponent(p, 2, newsymbol);
			} else {
				setProductionComponent(p, 1, newsymbol);
				setProductionComponent(p, 2, third);
			}
		}
	}

	char * r = malloc(sizeof(char)*2);
	r[0] = r[1] = LAMDA;
	addProduction(getProductions(grammar),
			newProduction(newsymbol, r));

	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(grammar);
	actualizeNonTerminals(grammar);
	return;
}

void convertToRight(GrammarADT grammar) {

	int i;
	int ml = FALSE;
	char oldistiguished = getDistinguished(grammar);
	/*if the grammar is already right there is no
	 * reason to convert it*/
	if (isRight(grammar)) {
		return;
	}

	ProductionsADT productions = getProductions(grammar);
	int quantproductions = getQuant(productions);

	for (i = 0; i < quantproductions; i++) {
		ProductionADT p1 = getProduction(productions, i);
		char first = getProductionComponent(p1, 0);
		char sec = getProductionComponent(p1, 1);
		char third = getProductionComponent(p1, 2);
		if (isNonTerminal(sec)) {
			char * r = malloc(sizeof(char)*2);
			r[0] = third;
			r[1] = first;
			addProduction(productions, newProduction(sec, r));
			removeParticularProduction(productions, p1);
		}
	}
	setProductions(grammar, productions);

	/*a new nonTerminal should be created ,
	 * that joint the non terminals that were joined to lambda*/
	char * leftnontermssymbols = NULL;
	int size = 0;
	for (i = 0; i < quantproductions; i++) {
		ProductionADT p1 = getProduction(productions, i);
		char first = getProductionComponent(p1, 0);
		char sec = getProductionComponent(p1, 1);
		char third = getProductionComponent(p1, 2);
		if (sec == LAMDA && third == LAMDA) {
			addChar(&leftnontermssymbols, &size, first);
		}
	}
	/*get a new distiguished symbol*/
	char newsymbol = getNewSymbol(grammar);
	setDistinguished(grammar, newsymbol);
	/*generate new unitary productions*/
	for (i = 0; i < size; i++) {
		char * r = malloc(sizeof(char)*2);
		r[0] = leftnontermssymbols[i];
		r[1] = LAMDA;
		ProductionADT newprod = newProduction(newsymbol, r);
		//printProduction(newprod);
		addProduction(productions, newprod);
	}
	/*remove all old lambda productions*/
	for (i = 0; i < getQuant(productions); i++) {
		ProductionADT p = getProduction(productions, i);
		char sec = getProductionComponent(p, 1);
		char third = getProductionComponent(p, 2);
		/*if it is a lamda productions : delete*/
		if (sec == LAMDA && third == LAMDA) {
			removeParticularProduction(productions, p);
		}
	}
	if (!ml) {
		char * r = malloc(sizeof(char)*2);
		r[0] = r[1] = LAMDA;
		addProduction(productions, newProduction(oldistiguished, r));
	}

	setProductions(grammar, productions);

	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(grammar);
	actualizeNonTerminals(grammar);
	actualizeProductions(grammar);
}

void toFile(GrammarADT grammar) {
	FILE * fp = fopen("grammar.gr", "w");
	if (fp == NULL) {
		printf("Error: File could not be created\n");
		fclose(fp);
		return;
	}

	fprintf(fp, "G1 = ({");

	int i;

	for (i = 0; i < getQuantNonTerminals(grammar); i++) {
		fprintf(fp, "%c%s", getNonTerminals(grammar)[i],
				(i == getQuantNonTerminals(grammar) - 1 ? "}" : ", "));
	}

	fprintf(fp, ", {");

	for (i = 0; i < getQuantTerminals(grammar); i++) {
		fprintf(fp, "%c%s", getTerminals(grammar)[i],
				(i == getQuantTerminals(grammar) - 1 ? "}" : ", "));
	}

	fprintf(fp, ", %c, {", getDistinguished(grammar));

	for (i = 0; i < getQuant(getProductions(grammar)); i++) {
		fprintf(
				fp,
				"%c -> %c%c%s",
				getProductionComponent(
						getProduction(getProductions(grammar), i), 0),
				getProductionComponent(
						getProduction(getProductions(grammar), i), 1),
				getProductionComponent(
						getProduction(getProductions(grammar), i), 2),
				(i == getQuant(getProductions(grammar)) - 1 ? "}" : ", "));
	}

	fprintf(fp, ")");
	fclose(fp);
	return;

}

void actualizeTerminals(GrammarADT grammar) {
	int termquant = getQuantTerminals(grammar);
	char * termsfounded = NULL;
	int termsfoundedsize = 0;
	ProductionsADT productions = getProductions(grammar);
	int productionquant = getQuant(productions), i;
	/*detect current terminals*/
	for (i = 0; i < productionquant; i++) {
		ProductionADT p = getProduction(productions, i);
		int j=1;
		char aux;
		for(;(aux=getProductionComponent(p, j))!=-1;j++){
			if (isTerminal(aux)
							&& !containsChar(termsfounded, termsfoundedsize, aux)) {
						addChar(&termsfounded, &termsfoundedsize, aux);
					}
		}
	}
	/*actualize terminals*/
	if (termsfoundedsize != termquant) {
		/*there are less current terminals*/
		setTerminals(grammar, termsfounded, termsfoundedsize);
	}
}

void actualizeNonTerminals(GrammarADT grammar) {
	int nontermquant = getQuantNonTerminals(grammar);
	char * nontermsfounded = NULL;
	int nontermsfoundedsize = 0;
	ProductionsADT productions = getProductions(grammar);
	int productionquant = getQuant(productions), i;
	/*detect current non terminals*/
	for (i = 0; i < productionquant; i++) {
		ProductionADT p = getProduction(productions, i);
		int j=0;
		char aux;
		for(;(aux=getProductionComponent(p, j))!=-1;j++){
			if (isNonTerminal(aux)
							&& !containsChar(nontermsfounded, nontermsfoundedsize, aux)) {
						addChar(&nontermsfounded, &nontermsfoundedsize, aux);
					}
		}
	}
	/*actualize non terminals*/
	if (nontermsfoundedsize != nontermquant) {
		/*there are less current non terminals*/
		setNonTerminals(grammar, nontermsfounded, nontermsfoundedsize);
	}

}

/*should be called after actualizeNonterminals*/
void actualizeProductions(GrammarADT grammar) {
	ProductionsADT productions = getProductions(grammar);
	int quantproductions = getQuant(productions);
	int quantnonterminals = getQuantNonTerminals(grammar);
	char * nonterminals = getNonTerminals(grammar);
	int i, j;
	/* por cada no terminal, si no exite ninguna producion que lo tenga
	 * en su parte izquierda, hay que elimicar la producion
	 */
	int contained = FALSE;
	for (i = 0; i < quantnonterminals; i++) {
		contained = FALSE;
		for (j = 0; j < quantproductions; j++) {
			ProductionADT p = getProduction(productions, j);
			char first = getProductionComponent(p, 0);
			if (nonterminals[i] == first) {
				contained = TRUE;
				break;
			}
		}
		/* if the symbol is no longer in the left of a production ->
		 * all the production containing that symbol should be deleted
		 */
		if (!contained) {
			removeProductionsContaining(productions, nonterminals[i]);
		}
	}
}

int isRight(GrammarADT grammar) {
	ProductionsADT productions = getProductions(grammar);
	int n = getQuant(productions);
	int i;
	for (i = 0; i < n; i++) {
		ProductionADT p = getProduction(productions, i);
		char sec = getProductionComponent(p, 1);
		char third = getProductionComponent(p, 2);
		/*if there is a production like A->Ba, it is a left sided grammar*/
		if (isNonTerminal(sec) && isTerminal(third)) {
			return 0;
		}
	}
	return 1;
}

char getNewSymbol(GrammarADT grammar) {
	char abc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int l = strlen(abc);
	char newsymbol;
	int nontermquant = getQuantNonTerminals(grammar);
	char * nonterm = getNonTerminals(grammar);
	int i, j;
	for (i = 0; i < l; i++) {
		for (j = 0; j < nontermquant && nonterm[j] != abc[i]; j++)
			;
		if (j == nontermquant) {
			newsymbol = abc[i];
			break;
		}
	}
	return newsymbol;
}


/*Conversion*/
void generateASDR(GrammarADT grammar) {

	FILE *file;
	char * name = "ASDR.c";
	file = fopen(name, "w+");
	char * nonterminals = getNonTerminals(grammar);
	int nonterminalsquant = getQuantNonTerminals(grammar);
	ProductionsADT productions = getProductions(grammar);
	int i;
	ProductionADT ** prodsbyNonTerm = malloc(
			sizeof(ProductionADT*) * nonterminalsquant);
	int * prodsSizes = malloc(sizeof(int) * nonterminalsquant);
	for ( i = 0; i < nonterminalsquant; i++) {
		prodsbyNonTerm[i] = getProductionsNonTerminal(productions,
				nonterminals[i], &(prodsSizes[i]));
	}

	printf("%s\n", "STARTING FILE CREATION");

	/*main generator*/
	if (file) {

		printf("------------generating headers---------------\n");
		fprintf(file, "#include <stdio.h>\n");
		fprintf(file, "#include <stdlib.h>\n");
		fprintf(file, "#include <string.h>\n");
		fprintf(file, "#include \"include/Production.h\"\n");
		fprintf(file, "#include \"include/utils.h\"\n");
		fprintf(file, "#define TRUE 1\n");
		fprintf(file, "#define FALSE 0\n");

		int j;
		for (j = 0; j < getQuantNonTerminals(grammar); j++) {
			fprintf(file, "ProductionADT * prods%d;\n", j);
			fprintf(file, "int prods%dquant;\n", j);
			fprintf(file, "int pn%d(int *, char * w);\n",j);
		}
		fprintf(file, "int procesar(ProductionADT p, char * w, int * t);\n");
		fprintf(file, "void init();\n");

		printf("------------generating main---------------\n");

		fprintf(file, "\nint main(int argc, char *argv[]) {\n");
		fprintf(file, "\tif(argc!=2) { /* argc should be 2 for correct execution*/\n");
		fprintf(file, "\t\tprintf(\"usage : %%s string \\n \", argv[0]);\n");
		fprintf(file, "\t\texit(1);\n");
		fprintf(file, "\t} else {\n");
		fprintf(file, "\t\tprintf(\"%%s is being processed\\n\", argv[1]);\n\t}\n");
		fprintf(file, "\tchar * w = argv[1];\n");
		fprintf(file, "\tinit();\n");
		fprintf(file, "\tint t = 0;\n");
		fprintf(file, "\tif(!pn0(&t,w) && (t==strlen(w))){\n");
		fprintf(file, "\t\tprintf(\"(The chain of used derivations is upside down)\\n\");\n");
		fprintf(file, "\t\tprintf(\"The string has been accepted \\n\");\n");
		fprintf(file, "\t} else {\n");
		fprintf(file, "\t\tprintf(\"The string has NOT been accepted\\n\");\n");
		fprintf(file, "\t}\n");
		fprintf(file, "\treturn 0;\n");
		fprintf(file, "}\n");
		/*main*/

		printf("------------generating init---------------\n");

		/*init*/

		fprintf(file, "\n/*initializes all the structures*/\n");
		fprintf(file, "void init(){\n");

		for (j = 0; j < nonterminalsquant; j++) {
			ProductionADT * ps = prodsbyNonTerm[j];
			fprintf(file, "\tprods%d = malloc(sizeof(ProductionADT*) * %d);\n",j, prodsSizes[j]);
			fprintf(file, "\tprods%dquant = %d;\n", j, prodsSizes[j]);
			int k;
			for (k = 0; k < prodsSizes[j]; k++) {
				ProductionADT p = ps[k];
				char* rside = getRightSide(p);
				int l=0;
				while(isTerminal(rside[l]) || isNonTerminal(rside[l])) l++;
				char aux = rside[l];
				if(rside[0]!='\\') rside[l]=0;
				fprintf(file, "\tprods%d[%d] = newProduction(\'%c\', \"%s\");\n", j, k, getLeftSide(p), (rside[0]=='\\'?"\\\\":rside));
				rside[l]=aux;
			}
		}
		fprintf(file, "}\n");

		printf("------------generating procesar---------------\n");

		/*procesar*/
		fprintf(file, "\n/*returns TRUE if there is an error*/\n");
		fprintf(file, "int procesar(ProductionADT p, char * w, int * t) {\n");
		fprintf(file, "\tint i;\n");
		fprintf(file, "\tint n = getSymbolQuant(p); /*number of symbols in the right side of the production*/\n");
		fprintf(file, "\tfor (i = 1; i < n; i++) {\n");
		fprintf(file, "\t\tchar comp = getProductionComponent(p, i);\n");
		fprintf(file, "\t\tif (isTerminal(comp)) {\n");
		fprintf(file, "\t\t\tif (w[*t] == comp) {\n");
		fprintf(file, "\t\t\t\t(*t) += 1;\n");
		fprintf(file, "\t\t\t} else {\n");
		fprintf(file, "\t\t\t\treturn TRUE;/*error*/\n\t\t\t}\n");
		fprintf(file, "\t\t} else {\n");
		fprintf(file, "\t\t\tint error= FALSE;\n");
		fprintf(file, "\t\t\tswitch(comp){\n");

		for (j = 0; j < nonterminalsquant; j++) {
			fprintf(file, "\t\t\tcase '%c':\n", nonterminals[j]);
			fprintf(file, "\t\t\t\terror = pn%d(t,w);\n", j);
			fprintf(file, "\t\t\t\tbreak;\n");
		}
		fprintf(file, "\t\t\t}\n");
		fprintf(file, "\t\t\tif(error){\n");
		fprintf(file, "\t\t\t\treturn TRUE;\n");
		fprintf(file, "\t\t\t}\n\t\t}\n\t}\n");
		fprintf(file, "\treturn FALSE;\n}\n");


		//TODO: debug
		printf("------------procedures generator---------------\n");

		/*procedures for each non terminal*/
		/*corresponding to S*/
		for (j = 0; j < nonterminalsquant; j++) {
			fprintf(file, "\nint pn%d(int *t, char * w) {\n", j);
			fprintf(file, "\tint j;\n");
			fprintf(file, "\tint n = prods%dquant; /*quantity of productions with this terminal */\n", j);
			fprintf(file, "\tint error = TRUE;\n");
			fprintf(file, "\tfor(j = 0; j < n && error; j++) {\n");
			fprintf(file, "\t\terror = procesar(prods%d[j], w, t);\n", j);
			fprintf(file, "\t\tif(!error){\n");
			fprintf(file, "\t\t\tprintProduction(prods%d[j]);\n", j);
			fprintf(file, "\t\t}\n\t}\n");
			fprintf(file, "\treturn error;\n}\n");
		}

		//TODO: debug
		printf("------------writing file---------------\n");

		fclose(file);
	}
}

void formalize(GrammarADT grammar){

	/*unitary productions must be removed*/
//	removeUnitaryProductions(grammar);
//	printf("\n AFTER UNITARY1 ----------------------- ");
//	printGrammar(grammar);

	/*unproductive productiones must be removed*/
	removeUnproductiveProductions(grammar);
	printf("\n AFTER UNPRODUCTIVE1 ----------------------- ");
	printGrammar(grammar);

	/*unreachable productions must be deleted*/
	removeUnreachableProductions(grammar);
	printf("\n AFTER UNREACHABLE1 ----------------------- ");
	printGrammar(grammar);

	/*all productions must be in the form A-Ba or A-> /*/
	removeOnlyRightTerminals(grammar);
	printf("\n AFTER OnlyRightTerminals ----------------------- ");
	printGrammar(grammar);


	/*AGAIN ; unitary productions must be removed*/
//	removeUnitaryProductions(grammar);
//	printf("\n AFTER UNITARY2 ----------------------- ");
//	printGrammar(grammar);

	/*AGAIN : unproductive productiones must be removed*/
	removeUnproductiveProductions(grammar);
	printf("\n AFTER UNPRODUCTIVE2 ----------------------- ");
	printGrammar(grammar);

	/*AGAIN : unreachable productions must be deleted*/
	removeUnreachableProductions(grammar);
	printf("\n AFTER UNREACHABLE2 ----------------------- ");
	printGrammar(grammar);

}
