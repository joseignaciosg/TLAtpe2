/*
 * Grammar.c
 *
 *  Created on: May 14, 2012
 *      Author: joseignaciosantiagogalindo
 */
#include "../include/Grammar.h"

/*Constructor-destructor*/
GrammarADT newGrammar(void){
	GrammarADT grammar;
	grammar = malloc(sizeof(struct Grammar));
	/*grammar->productions = malloc(sizeof(ProductionADT));*/
	return grammar;
}
void freeGrammar(GrammarADT grammar){
	free(grammar->nonterminals);
	free(grammar->terminals);
	free(grammar);
}

/*Getters*/
char * getTerminals(GrammarADT grammar){
	return grammar->terminals;
}
char * getNonTerminals(GrammarADT grammar){
	return grammar->nonterminals;
}
char getDistinguished(GrammarADT grammar){
	return grammar->distinguished;
}
ProductionsADT  getProductions(GrammarADT grammar){
	return grammar->productions;
}
int getQuantTerminals(GrammarADT grammar){
	return grammar->quantterminals;
}
int getQuantNonTerminals(GrammarADT grammar){
	return grammar->quantnonterminals;
}

/*Setters*/
void setTerminals(GrammarADT grammar, char * terminals, int quant){
	grammar->terminals = malloc(sizeof(char)*quant);
	memcpy(grammar->terminals, terminals, quant);
	grammar->quantterminals = quant;
}
void setNonTerminals(GrammarADT grammar, char * nonterminals, int quant){
	grammar->nonterminals = malloc(sizeof(char)*quant);
	memcpy(grammar->nonterminals,nonterminals, quant);
	grammar->quantnonterminals = quant;
}
void setDistinguished(GrammarADT grammar, char distinguished){
	grammar->distinguished = distinguished;
}
void setProductions(GrammarADT grammar,ProductionsADT productions){
	grammar->productions = productions;
}

/*Utility*/
void printGrammar(GrammarADT grammar){
	printf("Grammar\n");
	printf("Distinguished: %c \n",getDistinguished(grammar));
	int i ;
	printf("Terminals: {");
	for(i=0;i < getQuantTerminals(grammar);i++){
		printf(" %c ",getTerminals(grammar)[i]);
	}
	printf("}\n");
	printf("Non Terminals: {");
	for(i=0;i < getQuantNonTerminals(grammar);i++){
		printf(" %c ",getNonTerminals(grammar)[i]);
	}
	printf("}\n");
	printProductions(getProductions(grammar));
	printf("Production Quant %d\n",getQuant(getProductions(grammar)));
	return;
}

void removeUnitaryProductions(GrammarADT grammar){
	ProductionsADT  productions = getProductions(grammar);
	int i,j,k, productionquant = getQuant(productions), unitaryquant = 0, lastunitaryquant = 0;
	/*auxiliar array for unitary productions*/
	char * unitaries = NULL;
	/*iterate over productions and determine first unitaries:
	 * the productions that have only one non terminal symbol
	 * on the right side */
	for (i=0; i< productionquant; i++){
		char first = getProductionComponent(getProduction(productions,i),0);
		char sec = getProductionComponent(getProduction(productions,i),1);
		char third = getProductionComponent(getProduction(productions,i),2);
		if ( isNonTerminal(sec) && third == LAMDA ){
			addPair(&unitaries,&unitaryquant,first, sec);
		}else if( isNonTerminal(third) && sec == LAMDA){
			addPair(&unitaries,&unitaryquant,first, third);
		}
	}
	/*iterate over unitaries, adding the closure*/
	while(unitaryquant != lastunitaryquant){
		lastunitaryquant = unitaryquant;
		for (i=0; i<unitaryquant ; i+=2){
			char first1 = unitaries[i];
			char sec1 = unitaries[i+1];
			for (j=0; j<unitaryquant ; j+=2){
				char first2 = unitaries[j];
				char sec2 = unitaries[j+1];
				/*(A,B)(B,C)-> (A,C)*/
				if (sec1 == first2 ){
					if (!containsPair(unitaries,unitaryquant,first1,sec2) &&
							first1 != sec2 ){ /*no sense in adding (A,A) unitaries*/
						addPair(&unitaries,&unitaryquant,first1,sec2);
					}
				}
			}
		}
	}
	/*Debug*/
	//printByPairs(unitaries,unitaryquant);
	//printf("unitaries quant: %d\n\n", unitaryquant/2);
	/*create the new productions and remove the unitaries*/
	for(i=0; i<productionquant; i++){
		ProductionADT p1 = getProduction(productions,i);
		if ( isUnitary(p1) ){
			char first1 = getProductionComponent(p1,0);
			char sec1 = getProductionComponent(p1,1);
			char third1 = getProductionComponent(p1,2);
			for(j=0;j<unitaryquant;j+=2){
				char uni1 = unitaries[j];
				char uni2 = unitaries[j+1];
				//A->B and (A,B) (unitary production is localized)
				if ((first1 == uni1) && (sec1 == uni2 || third1 == uni2 )){
					for(k=0; k<productionquant; k++ ){
						ProductionADT p2 = getProduction(productions,k);
						char first2 = getProductionComponent(p2,0);
						char sec2 = getProductionComponent(p2,1);
						char third2 = getProductionComponent(p2,2);
						if(!isUnitary(p2)){
							if(first2 == uni2 ){
								addProduction(productions,newProduction(first1,sec2,third2));
							}
						}
					}
				}
			}
			removeParticularProduction(productions,p1);
			free(p1);
		}
	}
	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(grammar);
	actualizeNonTerminals(grammar);
	actualizeProductions(grammar);



}


void removeUnproductiveProductions(GrammarADT grammar){
	ProductionsADT  productions = getProductions(grammar);
	int i, quantproductions = getQuant(productions), productivequant=0,lastproductivequant=-1;
	char * productives = NULL;
	char * aux1 = NULL;
	while(productivequant != lastproductivequant){
		lastproductivequant = productivequant;
		for( i=0; i< quantproductions;i++ ){
			ProductionADT p1 = getProduction(productions,i);
			char first1 = getProductionComponent(p1,0);
			char sec1 = getProductionComponent(p1,1);
			char third1 = getProductionComponent(p1,2);
			if ( !containsChar(productives,productivequant,first1)  ){
				if ( ( sec1 == LAMDA && third1 == LAMDA ) || /*lamda*/
					 (isTerminal(sec1) && isTerminal(third1) ) || /*both terminal*/
					 (   isTerminal(sec1) && third1 == LAMDA   ) || /*one terminal*/
					 (   isTerminal(third1) && sec1 == LAMDA   ) ||
					 /*one terminal and one productive*/
					 (isTerminal(sec1) && ( isNonTerminal(third1) && containsChar(productives,productivequant,third1) ) ) ||
					 (isTerminal(third1) && ( isNonTerminal(sec1) && containsChar(productives,productivequant,sec1) ) ) ||
					 ( sec1 == LAMDA && ( isNonTerminal(third1) && containsChar(productives,productivequant,third1) ) ) ||
					 ( third1 == LAMDA && ( isNonTerminal(sec1) && containsChar(productives,productivequant,sec1) ) )){
					 if ( ( aux1 = realloc(productives, sizeof(char)*(productivequant+1)) ) == NULL ){
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



void removeUnreachableProductions(GrammarADT grammar){
	ProductionsADT  productions = getProductions(grammar);
	int i, quantproductions = getQuant(productions), reachablesquant=0,lastreachablesquant=0;
	char * reachables = malloc(sizeof(char));
	char * aux1 = NULL;
	/*starts only with distinguished symbol, if it is in the current productions*/
	if(inCurrentProductions(productions,getDistinguished(grammar))){
		reachables[reachablesquant++] = getDistinguished(grammar);
	}
	/*until something the quantity of reachables varies*/
	while (reachablesquant != lastreachablesquant){
		lastreachablesquant = reachablesquant;
		for(i=0; i<quantproductions; i++){
			char first = getProductionComponent(getProduction(productions,i),0);
			char sec = getProductionComponent(getProduction(productions,i),1);
			char third = getProductionComponent(getProduction(productions,i),2);
			/*if the symbol of the left is contained in the reachables, the non terminal
			 * symbols of the right must be added*/
			if (containsChar(reachables,reachablesquant,first)){
				/*if the second symbol is nonterminal and is not yet in the
				 * reachable list, it must be added*/
				if ( isNonTerminal( sec ) &&
						!containsChar(reachables,reachablesquant,sec)){
					 if ( ( aux1 = realloc(reachables, sizeof(char)*(reachablesquant+1)) ) == NULL ){
						fprintf(stderr, "Error doing realloc \n");
					 }
					 reachables = aux1;
					 reachables[reachablesquant++] = sec;
				}/*if the third symbol is nonterminal and is not yet in the
				 * reachable list, it must be added*/
				else if( isNonTerminal(third) &&
						!containsChar(reachables,reachablesquant,third) ){
					 if ( (aux1 = realloc(reachables, sizeof(char)*(reachablesquant+1)) ) == NULL ){
						fprintf(stderr, "Error doing realloc \n");
					 }
					 reachables = aux1;
					 reachables[reachablesquant++] = third;
				}
			}
		}
	}
	/*TODO: delete debug printf*/
	printf("\nReachables!!: ");
	printArray(reachables,reachablesquant);
	int symsToRemovequant=0;
	/*remove the unreachable productions*/
	/*If the quantity of reachables is equal to the quantity of nonterminals,
	 * nothing should be removed*/
	if (reachablesquant != getQuantNonTerminals(grammar)){
		char * symsToRemove = NULL;
		symsToRemovequant = getDifferents(getNonTerminals(grammar),
				getQuantNonTerminals(grammar) ,reachables, reachablesquant, &symsToRemove);
		printf("\nTO REMOVE:");
		printArray(symsToRemove,symsToRemovequant );
		for(i=0; i<symsToRemovequant; i++){
			removeProduction(productions,symsToRemove[i]);
		}
	}
	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(grammar);
	actualizeNonTerminals(grammar);
	actualizeProductions(grammar);

}


void removeOnlyRightTerminals(GrammarADT grammar){
	int j;
	int found = FALSE;
	char newsymbol;

	ProductionsADT productions = getProductions(grammar);

	/*search for A->a like productions*/
	for(j=0; j< getQuant(getProductions(grammar)); j++){
		ProductionADT p = getProduction(productions,j);
		char sec =  getProductionComponent(p,1);
		char third =  getProductionComponent(p,2);
		if(	( isTerminal(sec) && third == LAMDA ) ||
			( isTerminal(third) && sec == LAMDA ) ) {
			found = TRUE;
			break;
		}
	}
	/*if there are not productions with only terminals in the right side
	 * ,go away*/
	if(!found){
		return;
	}
	/*search for the new symbol to insert*/
	newsymbol = getNewSymbol(grammar);

	/*add new symbol to the nonterminals array*/
	char * term = getNonTerminals(grammar);
	int termquant = getQuantNonTerminals(grammar);
	char * aux = realloc(term, (termquant+1)*sizeof(char));
	if(aux == NULL){
		printf("Error: Not enought memory\n");
		exit(1);
	}
	term = aux;
	term[termquant] = newsymbol;
	termquant++;
	setNonTerminals(grammar, term, termquant);

	int isright = isRight(grammar) ;
	int productionquant = getQuant(getProductions(grammar));
	/*create the new productions with the new symbol*/
	for(j=0; j< productionquant ; j++){
		ProductionADT p = getProduction(getProductions(grammar),j);
		char sec = getProductionComponent(p,1);
		char third = getProductionComponent(p,2);
		/*is the production have only one terminal symbol on the right side */
		if ( isTerminal(sec) && third == LAMDA ){
			/*if the grammar is right sided the new symbol should be
			* added to the right*/
			if( isright ){
				setProductionComponent(p, 1, sec );
				setProductionComponent(p, 2, newsymbol);
			}else{
				setProductionComponent(p, 2, sec );
				setProductionComponent(p, 1, newsymbol);
			}
		}else if( isTerminal(third) && sec == LAMDA){
			if( isright ){
				setProductionComponent(p, 1, third );
				setProductionComponent(p, 2, newsymbol);
			}else{
				setProductionComponent(p, 1, newsymbol);
				setProductionComponent(p, 2, third );
			}
		}
	}

	addProduction(getProductions(grammar), newProduction(newsymbol,LAMDA,LAMDA));

	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(grammar);
	actualizeNonTerminals(grammar);
	return;
}

void convertToRight(GrammarADT grammar){

	int i;
	int ml = FALSE;
	char oldistiguished = getDistinguished(grammar);
	/*if the grammar is already right there is no
	 * reason to convert it*/
	if ( isRight(grammar) ){
		return;
	}

	ProductionsADT productions = getProductions(grammar);
	int quantproductions = getQuant(productions);

	for(i = 0; i < quantproductions ; i++){
		ProductionADT p1 = getProduction(productions, i);
		char first = getProductionComponent(p1, 0);
		char sec = getProductionComponent(p1, 1);
		char third = getProductionComponent(p1, 2);
		if(isNonTerminal(sec)){
			addProduction(productions, newProduction(sec , third, first));
			removeParticularProduction(productions,p1);
		}
	}
	setProductions(grammar, productions);

	/*a new nonTerminal should be created ,
	 * that joint the non terminals that were joined to lambda*/
	char * leftnontermssymbols = NULL;
	int size=0;
	for(i=0; i < quantproductions; i++){
		ProductionADT p1 = getProduction(productions, i);
		char first = getProductionComponent(p1, 0);
		char sec = getProductionComponent(p1, 1);
		char third = getProductionComponent(p1, 2);
		if(sec == LAMDA && third == LAMDA){
			addChar(&leftnontermssymbols,&size,first);
		}
	}
	/*get a new distiguished symbol*/
	char newsymbol = getNewSymbol(grammar);
	setDistinguished(grammar,newsymbol);
	/*generate new unitary productions*/
	for(i=0; i<size; i++){
		ProductionADT newprod = newProduction(newsymbol,leftnontermssymbols[i],LAMDA);
		//printProduction(newprod);
		addProduction(productions, newprod);
	}
	/*remove all old lambda productions*/
	for(i=0; i<getQuant(productions); i++){
		ProductionADT p = getProduction(productions,i);
		char sec = getProductionComponent(p,1);
		char third = getProductionComponent(p,2);
		/*if it is a lamda productions : delete*/
		if( sec == LAMDA && third == LAMDA ){
			removeParticularProduction(productions,p);
		}
	}
	if(!ml){
		addProduction(productions, newProduction(oldistiguished, LAMDA, LAMDA));
	}

	setProductions(grammar,productions);


	/*remove non terminals and terminals that are no longer there */
	actualizeTerminals(grammar);
	actualizeNonTerminals(grammar);
	actualizeProductions(grammar);
}

void toFile(GrammarADT grammar){
	FILE * fp = fopen("grammar.gr", "w");
	if(fp == NULL){
		printf("Error: File could not be created\n");
		fclose(fp);
		return;
	}

	fprintf(fp, "G1 = ({");

	int i;

	for(i = 0; i < getQuantNonTerminals(grammar); i++)	{
		fprintf(fp, "%c%s", getNonTerminals(grammar)[i], (i == getQuantNonTerminals(grammar)-1?"}":", "));
	}

	fprintf(fp, ", {");

	for(i = 0; i < getQuantTerminals(grammar); i++)	{
		fprintf(fp, "%c%s", getTerminals(grammar)[i], (i == getQuantTerminals(grammar)-1?"}":", "));
	}

	fprintf(fp, ", %c, {", getDistinguished(grammar));

	for(i = 0; i < getQuant(getProductions(grammar)); i++){
		fprintf(fp, "%c -> %c%c%s", getProductionComponent(getProduction(getProductions(grammar), i), 0),
				getProductionComponent(getProduction(getProductions(grammar), i), 1),
				getProductionComponent(getProduction(getProductions(grammar), i), 2),
				(i == getQuant(getProductions(grammar))-1? "}": ", "));
	}

	fprintf(fp, ")");
	fclose(fp);
	return;

}

void actualizeTerminals(GrammarADT grammar){
	int termquant = getQuantTerminals(grammar);
	char * termsfounded = NULL ;
	int termsfoundedsize =0;
	ProductionsADT productions = getProductions(grammar);
	int productionquant = getQuant(productions),i;
	/*detect current terminals*/
	for (i=0; i<productionquant; i++){
		ProductionADT p = getProduction(productions,i);
		char sec = getProductionComponent(p,1);
		char third = getProductionComponent(p,2);
		if (isTerminal(sec) && !containsChar(termsfounded,termsfoundedsize,sec) ){
			addChar(&termsfounded, &termsfoundedsize, sec);
		}
		if(isTerminal(third) && !containsChar(termsfounded,termsfoundedsize,third)){
			addChar(&termsfounded, &termsfoundedsize, third);
		}
	}
	/*actualize terminals*/
	if( termsfoundedsize != termquant ){
		/*there are less current terminals*/
		setTerminals(grammar,termsfounded,termsfoundedsize);
	}
}

void actualizeNonTerminals(GrammarADT grammar){
	int nontermquant = getQuantTerminals(grammar);
	char * nontermsfounded = NULL ;
	int nontermsfoundedsize =0;
	ProductionsADT productions = getProductions(grammar);
	int productionquant = getQuant(productions),i;
	/*detect current non terminals*/
	for (i=0; i<productionquant; i++){
		ProductionADT p = getProduction(productions,i);
		char first = getProductionComponent(p,0);
		char sec = getProductionComponent(p,1);
		char third = getProductionComponent(p,2);
		if (isNonTerminal(first) && !containsChar(nontermsfounded,nontermsfoundedsize,first) ){
			addChar(&nontermsfounded, &nontermsfoundedsize, first);
		}
		if (isNonTerminal(sec) && !containsChar(nontermsfounded,nontermsfoundedsize,sec) ){
			addChar(&nontermsfounded, &nontermsfoundedsize, sec);
		}
		if(isNonTerminal(third) && !containsChar(nontermsfounded,nontermsfoundedsize,third)){
			addChar(&nontermsfounded, &nontermsfoundedsize, third);
		}
	}
	/*actualize non terminals*/
	if( nontermsfoundedsize != nontermquant ){
		/*there are less current non terminals*/
		setNonTerminals(grammar,nontermsfounded,nontermsfoundedsize);
	}

}


/*should be called after actualizeNonterminals*/
void actualizeProductions(GrammarADT grammar){
	ProductionsADT productions = getProductions(grammar);
	int quantproductions = getQuant(productions);
	int quantnonterminals = getQuantNonTerminals(grammar);
	char * nonterminals = getNonTerminals(grammar);
	int i,j;
	/* por cada no terminal, si no exite ninguna producion que lo tenga
	 * en su parte izquierda, hay que elimicar la producion
	 */
	int contained = FALSE;
	for ( i=0; i<quantnonterminals; i++ ){
		contained = FALSE;
		for (j=0; j<quantproductions; j++ ){
			ProductionADT p = getProduction(productions,j);
			char first = getProductionComponent(p,0);
			if( nonterminals[i] == first ){
				contained = TRUE;
				break;
			}
		}
		/* if the symbol is no longer in the left of a production ->
		 * all the production containing that symbol should be deleted
		 */
		if (!contained){
			removeProductionsContaining(productions, nonterminals[i]);
		}
	}
}

int isRight(GrammarADT grammar){
	ProductionsADT productions = getProductions(grammar);
	int n = getQuant(productions);
	int i;
	for (i=0; i<n; i++){
		ProductionADT p = getProduction(productions,i);
		char sec = getProductionComponent(p,1);
		char third = getProductionComponent(p,2);
		/*if there is a production like A->Ba, it is a left sided grammar*/
		if (isNonTerminal(sec) && isTerminal(third)){
			return 0;
		}
	}
	return 1;
}

char getNewSymbol(GrammarADT grammar){
	char abc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int l = strlen(abc);
	char newsymbol;
	int nontermquant = getQuantNonTerminals(grammar);
	char * nonterm  = getNonTerminals(grammar);
	int i,j;
	for(i = 0; i<l; i++){
		for(j = 0; j< nontermquant && nonterm[j] != abc[i]; j++);
		if(j==nontermquant){
			newsymbol = abc[i];
			break;
		}
	}
	return newsymbol;
}

static void mainGenerator(GrammarADT grammar, FILE * file){

	char distinguished = getDistinguished(grammar);
	char * line1 = "if(PN1(t,w) && (t == sizeof(w)-1) ){";
	char * line2 = "return true;";
	char * line3 = "}else{return false;}";
	fwrite(line1, sizeof(line1[0]), sizeof(line1), file);
	fwrite(line2, sizeof(line2[0]), sizeof(line2), file);
	fwrite(line3, sizeof(line3[0]), sizeof(line3), file);
}
static void processGenerator(GrammarADT grammar, FILE * file){

}

static void proceduresGenerator(GrammarADT grammar, FILE * file){

}



/*Conversion*/
void generateASDR(GrammarADT grammar){

	FILE *file;
	char * name = "ASDR.c";
	file = fopen(name,"a+");

	int i;
	/*main generator*/
	if(file){
		mainGenerator(grammar, file);
		processGenerator(grammar, file);
		proceduresGenerator(grammar, file);
		fclose(file);
	}
}

