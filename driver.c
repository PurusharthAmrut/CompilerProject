/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lexer.h"
#include "parser.h"
#include "parserDef.h"
#include "utils.h"
#include "AST.h"
#include "symbolTable.h"
#include "symbolTableDef.h"
#include "typechecker.h"
#include "semanticAnalyser.h"

int main(int argc, char * argv[]) {
  FILE * sourceFile, * parsetreeOutfile, * dstFile;
  int choice;
  tokenInfo currentToken;
  unsigned long long lineNum;
  Grammar g;
  g = malloc(sizeof(lhsChar) * NO_OF_NONTERMINALS);
  getGram("grammar.txt", g);

  //     FirstSet firstSet=(FirstSet)malloc(NO_OF_NONTERMINALS*sizeof(first));
  //     FollowSet followSet=(FollowSet)malloc(NO_OF_NONTERMINALS*sizeof(first));
  //     buildFirstSet(g,firstnfollow->first);
  //     getFollowSets(g, firstnfollow->follow,firstnfollow->first);
  for (int nt = 0; nt < NO_OF_NONTERMINALS; nt++) {
    computeFirst(g, nt);
  }
  for (int nt = 0; nt < NO_OF_NONTERMINALS; nt++) {
    if (g[nt].isNullable)
      computeFollow(g, nt);
  }

  int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS];
  parseTree root = malloc(sizeof(parsetree));
  parseTree ast;
  symbolTable st;
  int error = 0;
  createParseTable(g, t);

  clock_t start_time, end_time;
  double total_CPU_time = 0, total_CPU_time_in_seconds = 0;

  int count = 0, m1 = 0, m2 = 0, m3 = 0, m4 = 0, m5 = 0, countAST = 0;

  if (argc != 3) {
    fprintf(stderr, "error: incorrect number of arguments\n");
    return 1;
  }

  while (true) {
	printf("The following components have been sucessfully implemented:\n");
	printf("1. Lexical analyzer\n");
	printf("2. Parse tree generation\n");
	printf("3. AST generation\n");
	printf("4. Symbol table\n");
	printf("5. Type checking\n");
	printf("6. Semantic analysis\n");
	printf("All valid test cases are successfully going through all the stages of the compiler (to the best of our knowledge).\n");
	printf("The error handling capability is not complete i.e. there might be some error which the compiler might not be able to catch and end in a segmentation fault.");
	printf(" However, the semantic analysis and symbol table population still works even when the error handling crashes and must not be misunderstood as a complete failure of the entire compiler.\n");
    printf("\n0.EXIT");
    printf("\n1. Print the tokens list generated by lexer (on Console)");
    printf("\n2. Verify the syntactic correctness of the input source code and to produce parse tree (On Console)");
    printf("\n3. Print Abstract Syntax Tree and Order of Traversal (on Console)");
    printf("\n4. Display the amount of allocated memory and no. of nodes to each of parse tree and abstract syntax tree for test case use.");
    printf("\n5. Print the Symbol Table");
    printf("\n6. Print the list of global variables, their types and offsets");
    printf("\n7. Print the total memory requirement for each function");
    printf("\n8. Print the type expressions and width of globally visible record definitions");
    printf("\n9. Verify the syntactic and semantic correctness along with reporting errors and print total time taken by compiler (on Console)");
    printf("\n10. For producing assembly code");

    printf("\n\nEnter your choice: ");
    scanf("%d", & choice);
    printf("\n");

    switch (choice) {
    case 0:
      return 0;
    case 1:
      printf("\n\n------------------------------------------------------------------\n\n");
      sourceFile = fopen(argv[1], "r");
      fseek(sourceFile, 0, SEEK_SET);
      if (sourceFile == NULL) {
        fprintf(stderr, "error: %s: cannot open file\n", argv[1]);
        return 1;
      }
      printTokenizedCode(sourceFile);
      fclose(sourceFile);
      printf("\n\n------------------------------------------------------------------\n\n");

      break;

      //2. Print parse tree and output errors if any (on Console)
    case 2:
      ;
      printf("\n\n------------------------------------------------------------------\n\n");
      sourceFile = fopen(argv[1], "r");
      fseek(sourceFile, 0, SEEK_SET);
      if (sourceFile == NULL) {
        fprintf(stderr, "error: %s: cannot open file\n", argv[1]);
        return 1;
      }
      start_time = clock();
      int error = 0;
      parseInputSourceCode(sourceFile, t, g, root, & error);
      parseTree tree = malloc(sizeof(parsetree));
      printParseTree(tree, 0);

      end_time = clock();
      if (!m1) {
        total_CPU_time += ((double) end_time - start_time);
        total_CPU_time_in_seconds += (total_CPU_time / CLOCKS_PER_SEC);
        nodeCount(root, & count);
        count++;
      }
      m1++;

      break;
    case 3:
      ;
      printf("\n\n------------------------------------------------------------------\n\n");
      if (!root) {
        printf("Error, no parse Tree exists\n");
        break;
      }
      start_time = clock();
      ast = createAST(root);
      printAST(ast, program);
      end_time = clock();
      if (!m2) {
        total_CPU_time += ((double) end_time - start_time);
        total_CPU_time_in_seconds += (total_CPU_time / CLOCKS_PER_SEC);
        nodeCountAST(ast, & countAST);
        countAST++;
      }
      m2++;

    case 4:
      printf("\n\n------------------------------------------------------------------\n\n");
      if (!ast) {
        printf("Error, no ast exists\n");
        break;
      }
      printf("\n\nParse Tree number of nodes= %d\tAllocated Memory = %ld Bytes\n", count, count * sizeof(parsetree));
      printf("\nAST number of nodes= %d\tAllocated Memory = %ld Bytes\n", countAST, countAST * sizeof(parsetree));
      double compressionPerc = (((double) count - countAST) / count) * 100;
      printf("Compression Percentage = %lf\n\n\n", compressionPerc);
      printf("\n\n------------------------------------------------------------------\n\n");
      break;

    case 5:
      printf("\n\n------------------------------------------------------------------\n\n");

      //ast = createAST(root);
      if (!ast) {
        printf("Error, no ast exists\n");
        break;
      }
      if (!m4) {
        start_time = clock();
        st = fillSymbolTable(ast);
        end_time = clock();
        total_CPU_time += ((double) end_time - start_time);
        total_CPU_time_in_seconds += (total_CPU_time / CLOCKS_PER_SEC);
      }
      m4++;
      printSymbolTable(st);
      printf("\n\n------------------------------------------------------------------\n\n");
      break;
    case 6:
      printf("\n\n------------------------------------------------------------------\n\n");
      if (!st) {
        printf("\n\nError, no symbol table exists\n\n");
        break;
      }
      printGlobalVariables(st);
      printf("\n\n------------------------------------------------------------------\n\n");
      break;

    case 7:
      printf("\n\n------------------------------------------------------------------\n\n");
      if (!st) {
        printf("\n\nError, no symbol table exists\n\n");
        break;
      }
      printFnameAndSizes(st);
      printf("\n\n------------------------------------------------------------------\n\n");
      break;

    case 8:
      printf("\n\n------------------------------------------------------------------\n\n");
      if (!st) {
        printf("\n\nError, no symbol table exists\n\n");
        break;
      }
      printRecDefAndSize(st);
      printf("\n\n------------------------------------------------------------------\n\n");
      break;

    case 9:
      printf("\n\n------------------------------------------------------------------\n\n");
      if (!st) {
        printf("\n\nError, no symbol table exists\n\n");
        break;
      }
      if (!m5) {
        start_time = clock();
        TypeChecker( * ast, st);
        checkFunctionSemantics(ast, st);
        end_time = clock();
        total_CPU_time += ((double) end_time - start_time);
        total_CPU_time_in_seconds += (total_CPU_time / CLOCKS_PER_SEC);
      }
      m5++;
      printf("\n\ntotal_CPU_time:%lf\n", total_CPU_time);
      printf("total_CPU_time_in_seconds:%lf  seconds\n\n", total_CPU_time_in_seconds);
      printf("\n\n------------------------------------------------------------------\n\n");
      break;

    case 10:
      printf("\n\n------------------------------------------------------------------\n\n");
      printf("Not completed\n");
      printf("\n\n------------------------------------------------------------------\n\n");
      break;

    default:
      // invalid option
      fprintf(stderr, "\nstage2: error: invalid option\n");
    }
  }

  return 0;
}
