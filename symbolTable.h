/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include"symbolTableDef.h"


record* insertAtEnd(record* ptr,record *temp);
int hashVal(char* key, int size);
symbolTable createSymbolTable(int size);
TableLoc* createEntry(char* type,char* varname,int size,int offset,record* ptr,paramInfo* param);
tableHeader* createTablePointer(char* fname,int size);
void fillGlobalUtils2(TableLoc* table, parseTree ast);
void fillGlobalUtils1(tableHeader* tp, parseTree ast);
void fillGlobalRecords(tableHeader* tp, parseTree ast);
void fillGlobalIds(tableHeader* tp,parseTree ast);
char* extractDatatype(parseTree datatype);
void addStmtVarUtils2(symbolTable st,tableHeader* tp,parseTree ast,int pos);
void addStmtVarUtils1(symbolTable st,tableHeader* tp,parseTree ast,int pos) ;
void addStmtVar(symbolTable st,tableHeader* tp,parseTree ast,int pos);
void addParameters(symbolTable st, parseTree ast, int pos, paramInfo* param);
void fillFuncIdsUtils2(symbolTable st,parseTree ast,char* key);
void fillFuncIdsUtils1(symbolTable st,parseTree ast) ;
void fillFuncVar(symbolTable st,parseTree ast) ;
void printSymbolTable(symbolTable st);
int lookup(TableLoc* presentList,char* newEntry);
void addEntryAtEnd(TableLoc** presentList,TableLoc* newEntry);
void globalRecordsUpdate(parseTree ast,symbolTable st,tableHeader* tp);
void globalIdsUpdate(parseTree ast,symbolTable st,tableHeader* tp);
symbolTable fillSymbolTable(parseTree ast);
void printTable(tableHeader* tp);
void printFuncVariables(record* temp,tableHeader* tp);
void printGlobalVariables(symbolTable st);
void printFnameAndSizes(symbolTable st);
void printRecDefAndSize(symbolTable st);
//void printRecord(tablePointer* tp)







