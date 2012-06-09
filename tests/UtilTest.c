/*
 * UtilTest.c
 *
 *  Created on: May 25, 2012
 *      Author: joseignaciosantiagogalindo
 */

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include "../include/utils.h"

void containsCharTest();
void getDifferentsTest();
void addPairTest();
void containsPairTest();

char array1[5] = {'A','B','C','D','E'};
char array2[3] = {'A','B','C'};
char * array3;
char * unitaries;
int unitaiessize=0;
int main(){
	containsCharTest();
	getDifferentsTest();
	addPairTest();
	containsPairTest();
	return 0;
}

void addPairTest(){
	addPair(&unitaries,&unitaiessize,'A','B');
	printf("curr size: %d\n", unitaiessize);
	addPair(&unitaries,&unitaiessize,'C','D');
	printf("curr size: %d\n", unitaiessize);
	addPair(&unitaries,&unitaiessize,'E','F');
	printf("curr size: %d\n", unitaiessize);
	printByPairs(unitaries,unitaiessize);
}

void containsPairTest(){
	printf("contains (C,D): %d \n", containsPair(unitaries,unitaiessize,'C','D'));
	printf("contains (D,E): %d \n", containsPair(unitaries,unitaiessize,'D','E'));
	printf("contains (A,B): %d \n", containsPair(unitaries,unitaiessize,'A','B'));
}


void containsCharTest(){
	printf("\n Testing containsChar Method \n");
	printArray(array1, 5);
	printf("contains A: %d\n",containsChar(array1,sizeof(array1),'A'));
	printf("contains E: %d\n",containsChar(array1,sizeof(array1),'E'));
	printf("contains F: %d\n",containsChar(array1,sizeof(array1),'F'));

}

void getDifferentsTest(){
	printf("\n Testing getDifferents Method \n");
	printArray(array1, 5);
	printArray(array2, 3);
	int size = getDifferents(array1, 5,array2, 3, &array3);
	printf("size: %i\n",size);
	printArray(array3, size);
}
