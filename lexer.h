#ifndef LEXER
#define LEXER

#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"





//Function Declarations of lookuptable
LookUpTable* createLookUpTable(int num_pos);
void insertNodeElement(LookUpTable* lookUpTable, char* key, terminalId tokentype);
int hashFuncLUT(char* key, int num_pos);
void fillLookUpTable(LookUpTable* lookUpTable);
terminalId getTokenType(char* key, LookUpTable* lookUpTable);

//Function Declarations of lexer
void printToken(tokenInfo *t);
void getNextToken(FILE *fp, tokenInfo *t);
void removeComments(char* testcaseFile,char* cleanFile);

#endif
