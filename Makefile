CC = gcc
FLEX = flex
FLOPTS = -o
FILES = ./classes/Grammar.c ./classes/Production.c ./classes/Productions.c ./classes/Utils.c
OUT_DIR=./
GEN_OUT_EXE = genASDR 

COPTS= -Wall -g
LDOPTS= -lfl
MATH = -lm

GEN_OUT_FL = genASDR.yy.c
GEN_FL_FILES = ./parser/lex/gr-parser.l
GEN_CC_FILES = ./parser/c/gr-parser.c $(GEN_OUT_FL)
genASDR: $(FILES) $(GEN_CC_FILES)
	$(CC) $(COPTS) -o $(GEN_OUT_EXE) $(FILES) $(GEN_CC_FILES) $(LDOPTS)
	-rm -f $(GEN_OUT_FL)

genASDR.yy.c:	
	$(FLEX) $(FLOPTS) $(GEN_OUT_FL) $(GEN_FL_FILES) 
	

ASDR_FILES = ./ASDR.c ./classes/Production.c ./classes/Utils.c
ASDR_OUT_EXE = ASDR
ASDR: $(ASDR_FILES) $(ASDR_FILES)
	$(CC) $(COPTS) -o $(ASDR_OUT_EXE) $(ASDR_FILES)
	
#rebuild: clean build
