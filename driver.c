/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */

/*
 * driver.c: contains the main function
 * provides an interface for testing the lexer and parser modules
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lexer.h"
#include "parser.h"
#include "parserDef.h"
#include "symbolTable.h"
#include "utils.h"


int main(int argc, char *argv[]) 
{
    FILE *sourceFile,*parsetreeOutfile, *dstFile;
    int choice;
    tokenInfo currentToken;
    extern int bufSize;
    extern unsigned long long lineNum;
    Grammar g;
    g = malloc(sizeof(lhs) * NO_OF_NONTERMINALS);
    getGram("grammar.txt", g);
    lhs firstnfollow = (lhs)malloc(sizeof(struct lhsChar));
//     FirstSet firstSet=(FirstSet)malloc(NO_OF_NONTERMINALS*sizeof(first));
//     FollowSet followSet=(FollowSet)malloc(NO_OF_NONTERMINALS*sizeof(first));
//     buildFirstSet(g,firstnfollow->first);
//     getFollowSets(g, firstnfollow->follow,firstnfollow->first);
for (int nt=0; nt<NO_OF_NONTERMINALS; nt++) {
	computeFirst(g, nt);
}
for (int nt=0; nt<NO_OF_NONTERMINALS; nt++) {
	if (g[nt].isNullable)
	    computeFollow(g, nt);
}
    int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS+1];
    parseTree root=malloc(sizeof(parsetree)),ast=NULL;
    int error = 0;
    createParseTable(firstnfollow->first,firstnfollow->first,g,t);
    
    symbolTable st ;
    clock_t    start_time, end_time;
    double total_CPU_time = 0, total_CPU_time_in_seconds = 0;

	int ans = 0,m1=0,m2=0,nodes,m3=0,m4=0;
					
    if(argc != 3) 
    {
        fprintf(stderr, "error: incorrect number of arguments\n");
        return 1;
    }
        
    
    printf("\tLEVEL 4:Symbol table/type Checking/ Semantic rules modules work\n\n");
    while(true) 
    {
		printf("\n0.EXIT");
        printf("\n1.Removal of Comments");
        printf("\n2. Print the tokens list generated by lexer (on Console)");
        printf("\n3. Print parse tree and output errors if any (on Console)");
        // printf("\n3. Print Abstract Syntax Tree and Order of Traversal (on Console)");
        // printf("\n4. Display the amount of allocated memory and no. of nodes to each of parse tree and abstract syntax tree.");
        // printf("\n5. Print the Symbol Table");
        // printf("\n6. Print the list of global variables, their types and offsets");
        // printf("\n7. Print the total memory requirement for each function");
        // printf("\n8. Print the type expressions and width of globally visible record definitions");
        // printf("\n9. Verify the syntactic and semantic correctness and Print total time taken by compiler (on Console)");
        // printf("\n10. For producing assembly code");


        
        
        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch(choice) 
        {
        	//0.EXIT
		    case 0:
		     		return 0;
		     
			//1. Print the tokens list generated by lexer (on Console)
		    case 1:
		    		printf("\n\n------------------------------------------------------------------\n\n");
				    sourceFile = fopen(argv[1], "r");
				    dstFile = fopen(argv[2], "w");
				    removeComments(&sourceFile, &dstFile);
		    case 2:
		    			printf("\n\n------------------------------------------------------------------\n\n");
		    			sourceFile = fopen(argv[1], "r");
		    			fseek(sourceFile,0,SEEK_SET);
					    if(sourceFile == NULL) 
					    {
						fprintf(stderr, "error: %s: cannot open file\n", argv[1]);
						return 1;
					    }
					printf("\n \t\t\t\t\tLexer module Invoked \n\n\n\n\n");
					currentToken.tokenType = 0;
					while(currentToken.tokenType != TK_EOF)
					{
							getNextToken(sourceFile, &currentToken);
						        printToken(&currentToken);
					}
					fclose(sourceFile);
					printf("\n\n------------------------------------------------------------------\n\n");
					
		            break;
		            
		  	//2. Print parse tree and output errors if any (on Console)
		    case 3:
		    		printf("\n\n------------------------------------------------------------------\n\n");
		     		printf("\n\t\t\t\tBoth Parser and Lexer Module Invoked\n\n\n\n\n");
		     		sourceFile = fopen(argv[1], "r");
		    			fseek(sourceFile,0,SEEK_SET);
					    if(sourceFile == NULL) 
					    {
							fprintf(stderr, "error: %s: cannot open file\n", argv[1]);
							return 1;
						}
		     		start_time = clock();		     			
					int error=0;					
					parseInputSourceCode(sourceFile,t,g,root,&error);
					end_time = clock();
					if(!m1)
					{ 
						total_CPU_time +=  (double) (end_time - start_time);
                		total_CPU_time_in_seconds +=  total_CPU_time / CLOCKS_PER_SEC;
						nodeCount(root,&ans);
						ans++; 
					}
				
					m1++;
					if(error==1)
					break;
					if(!root)
					printf("NULL\n");
					printf(" LEXEME       ");
                    printf("LINENO   ");
                   	printf(" TOKENNAME   ");
        			printf("    VALUENUMBER   ");
        			printf("   PARENT NODE SYMBOL   ");
        			printf("      IS LEAF NODE   ");
        			printf("      NODE SYMBOL   ");
        			printf("\n\n");
        			printf("-------\t\t");
					printf("-------\t\t");
					printf("-------\t\t");
					printf("-------\t\t");
					printf("root   \t\t");
					printf("NO     \t\t");
					printf("program\t\t\n");
					printParseTree(root);
					printf("\t\t\t\tParsed Successfully\n");
					
                		fclose(sourceFile);
                	printf("\n\n------------------------------------------------------------------\n\n");
                		
					break;
			//3. Print Abstract Syntax Tree and Order of Traversal (on Console)
		// 	case 3:
		// 			;
		// 			printf("\n\n------------------------------------------------------------------\n\n");
		// 			if(!root->numChild)
		// 			{
		// 				printf("\n\nFirst Build ParseTree using option 2\n\n");
		// 				break;
		// 			}
		// 			printf("\n\nPRE-ORDER TRAVERSAL OF AST\n\n");
		// 			if(!m2) 
		// 			{
		// 				start_time = clock();
		// 				ast=createAST(root);
		// 				end_time = clock();
		// 				total_CPU_time  += (double) (end_time - start_time);
                // 		total_CPU_time_in_seconds +=  total_CPU_time / CLOCKS_PER_SEC;
		// 			}
		// 			printAST(ast);
		// 			m2++;
		// 			printf("\n\n------------------------------------------------------------------\n\n");
		// 			break;
					
		//      //4. Display the amount of allocated memory and no. of nodes to each of parse tree and abstract syntax tree.
		//      case 4:
		//      		;
		//      		//ast = createAST(root);
		//      		printf("\n\n------------------------------------------------------------------\n\n");
		// 			if(!ast)
		// 			{
		// 				printf("\n\nFirst Build AST using option 3\n\n");
		// 				break;
		// 			}
		//      		if(!m3)
		//      		{
		//      			nodes = 1;
		//      			nodeCountAST(ast,&nodes);
		//      		}
		//      		m3++;
		//      		printf("\n\nparseTree node count: %d\t\t\tAllocated Memory = %ld Bytes\n",ans,ans*sizeof(parsetree));
		//      		printf("AST node count: %d\t\t\tAllocated Memory = %ld Bytes\n",nodes,nodes*sizeof(parsetree));	
		//      		float compression = (float)(ans-nodes)*100 ;
		// 			compression /= ans ;
		// 			printf("compression percentage = %f\n\n\n",compression);
		// 			printf("\n\n------------------------------------------------------------------\n\n");
		//      		break;
		     		
		     
		//      //5. Print the Symbol Table
		//      case 5:
		//      		;
		//      		printf("\n\n------------------------------------------------------------------\n\n");
		//      		//ast = createAST(root);
		//      		if(!ast)
		// 			{
		// 				printf("\n\nFirst Build AST using option 3\n\n");
		// 				break;
		// 			}
		//      		if(!m4) 
		//      		{
		//      			start_time = clock();
		//      			st = fillSymbolTable(ast);
		//      			end_time = clock();
		//      			total_CPU_time  +=  (double) (end_time - start_time);
                // 		total_CPU_time_in_seconds  +=  total_CPU_time / CLOCKS_PER_SEC;
		//      		}
		//      		m4++;
		//      		printSymbolTable(st);
		//      		printf("\n\n------------------------------------------------------------------\n\n");
		//      		break;
		     
		//      //6. Print the list of global variables, their types and offsets		
		//      case 6:
		//      		;
		//      		//ast = createAST(root);
		//      		//st = fillSymbolTable(ast);
		//      		//int globHashVal = hashVal("global",10000);
		//      		printf("\n\n------------------------------------------------------------------\n\n");
		//      		if(!st)
		//      		{
		//      			printf("\n\nFirst Build Symbol Table using option 5\n\n");
		// 				break;
		// 			}
		//      		printGlobalVariables(st);
		//      		printf("\n\n------------------------------------------------------------------\n\n");		     		
		//      		break;
		     
		//      //7. Print the total memory requirement for each function		
		//      case 7:
		//      		;
		//      		printf("\n\n------------------------------------------------------------------\n\n");
		//      		//ast = createAST(root);
		//      		//st = fillSymbolTable(ast);
		//      		if(!st)
		//      		{
		//      			printf("\n\nFirst Build Symbol Table using option 5\n\n");
		// 				break;
		// 			}
		//      		printFnameAndSizes(st);
		//      		printf("\n\n------------------------------------------------------------------\n\n");
		// 			break;
		     		
		     
		//      //8. Print the type expressions and width of globally visible record definitions
		//      case 8:
		//      		;
		//      		//ast = createAST(root);
		//      		//st = fillSymbolTable(ast);
		//      		printf("\n\n------------------------------------------------------------------\n\n");
		//      		if(!st)
		//      		{
		//      			printf("\n\nFirst Build Symbol Table using option 5\n\n");
		// 				break;
		// 			}
		//      		printRecDefAndSize(st);
		//      		printf("\n\n------------------------------------------------------------------\n\n");
		//      		break;
		     
		//      //9. Verify the syntactic and semantic correctness and Print total time taken by compiler (on Console)
		//      case 9:
		// 		     printf("\n\n------------------------------------------------------------------\n\n");
		//      		if(!st)
		//      		{
		//      			printf("\n\nFirst Build Symbol Table using option 5\n\n");
		// 				break;
		// 			}
		//      		TypeChecker(*ast,st);
		//      		checkFunctionSemantics(ast,st);
		//      		printf("\n\ntotal_CPU_time:%lf\n",total_CPU_time);
                // 		printf("total_CPU_time_in_seconds:%lf  seconds\n\n",total_CPU_time_in_seconds);
		//      		printf("\n\n------------------------------------------------------------------\n\n");
		//      		break;
		     
		//      //10. For producing assembly code		
		//      case 10:
		//      		printf("\n\n------------------------------------------------------------------\n\n");
		//      		printf("work in progress");
		//      		printf("\n\n------------------------------------------------------------------\n\n");
		     		
		     		
		//      		break;
		       
		     default:
		     		// invalid option
		            fprintf(stderr, "\nstage2: error: invalid option\n");
        }

        //rewind(sourceFile);
        bufSize = -1;
        lineNum = 1;
    }

    return 0;
}
