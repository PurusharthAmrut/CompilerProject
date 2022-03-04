/* 
 * Group 05
 * Kush Mehta			    2018B5A70956P
 * Purusharth Amrut		    2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal   		2018B4A70884P
 */
 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// #define EXCLUDE_EPS -288230376151711745LL
// #define INCLUDE_EPS 288230376151711744LL

// 61 bits are one with 59 (eps) bit set to zero (total 64 bits)
#define EXCLUDE_EPS 0b1111101111111111111111111111111111111111111111111111111111111111
#define INCLUDE_EPS 0b0000010000000000000000000000000000000000000000000000000000000000

bool check[NO_OF_NONTERMINALS];

void printGram(Grammar g) {
	for(int i=0; i<NO_OF_NONTERMINALS; i++) {
		printf("%d %lld %lld %d\n", g[i].numRules, g[i].first, g[i].follow, g[i].isNullable);
		for(int j=0; j<g[i].numRules; j++) {
			rhsChar curr = g[i].heads[j];
			while(curr!=NULL) {
				if(curr->tag==0) printf("%s ", getTermString(curr->s.t));
				else printf("%s ", getNonTermString(curr->s.nt));
				curr = curr->next;
			}
			printf("\n");
		}
	}
}

void getGram(char *fname, Grammar g) {
    FILE *fp;
    fp = fopen(fname, "r");
    if(fp==NULL) {
        fprintf(stderr, "File %s file not found error\n", fname);
        exit(0);
    }
    int times = NO_OF_NONTERMINALS;
    while(times--) {
        int numRules;
        fscanf(fp, "%d", &numRules);

        char *nonTerm = malloc(MAX_TOKEN_LENGTH*sizeof(char));
        fscanf(fp, "%s", nonTerm);
        int numTokens;
        fscanf(fp, "%d", &numTokens);
        // fprintf(stderr, "%d %s %d\n", numRules, nonTerm, numTokens);
        int i = checkNonTerminal(nonTerm);

        if(i==-1) {
            fprintf(stderr, "%s - Non Terminal does not exist in ENUM\n", nonTerm);
            exit(0);
        }

        g[i].heads = malloc(numRules*sizeof(rhsChar));
        g[i].numRules = numRules;
        g[i].first = 0;
        g[i].follow = 0;
		g[i].isNullable = 0;

        for(int j=0; j<numRules; j++) {
            rhsChar curr = malloc(sizeof(struct rhsCharNode));
            curr->next = NULL;

            if(j) {
                free(nonTerm);
                nonTerm = malloc(MAX_TOKEN_LENGTH*sizeof(char));
                fscanf(fp, "%s", nonTerm);
                fscanf(fp, "%d", &numTokens);
            }

            if(checkNonTerminal(nonTerm)==eps) g[i].isNullable = 1;

            curr->tag = 1;
            curr->s.nt = checkNonTerminal(nonTerm);
            if(curr->s.nt==-1) {
                fprintf(stderr, "%s - Non Terminal does not exist in ENUM\n", nonTerm);
				exit(0);
            }

            rhsChar prev = curr;

            for(int k=0; k<numTokens; k++) {
                rhsChar tmp = malloc(sizeof(struct rhsCharNode));
                tmp->next = NULL;

                char *token = malloc(MAX_TOKEN_LENGTH*sizeof(char));
                fscanf(fp, "%s", token);
                // printf("%s ", token);

                int tokenIdx = checkTerminal(token);
                if(tokenIdx!=-1) {
                    tmp->tag = 0;
                    tmp->s.t = tokenIdx;
                    if(tokenIdx==eps) g[i].isNullable = 1;
                }else {
                    tokenIdx = checkNonTerminal(token);
                    if(tokenIdx==-1) {
                        fprintf(stderr, "%s - Non Terminal does not exist in ENUM\n", token);
						exit(0);
                    }
                    tmp->tag = 1;
                    tmp->s.nt = tokenIdx;
                }

                prev->next = tmp;
                prev = tmp;
            }
            g[i].heads[j] = curr;
        }
    }

    // for(int i=0; i<NO_OF_NONTERMINALS; i++) {
    //     int numRules;
    //     fscanf(fp, "%d", &numRules);
    //     // printf("%d\n", numRules);

    //     g[i].heads = malloc(numRules*sizeof(rhsChar));
    //     g[i].numRules = numRules;
    //     g[i].first = 0;
    //     g[i].follow = 0;
	// 	g[i].isNullable = 0;

    //     for(int j=0; j<numRules; j++) {
    //         rhsChar curr = malloc(sizeof(struct rhsCharNode));
    //         curr->next = NULL;

    //         char *nonTerm = malloc(MAX_TOKEN_LENGTH*sizeof(char));
    //         fscanf(fp, "%s", nonTerm);
    //         int numTokens;
    //         fscanf(fp, "%d", &numTokens);
    //         // printf("%s %d ", nonTerm, numTokens);

    //         curr->tag = 1;
    //         curr->s.nt = checkNonTerminal(nonTerm);
    //         if(curr->s.nt==-1) {
    //             fprintf(stderr, "%s - Non Terminal does not exist in ENUM\n", nonTerm);
	// 			exit(0);
    //         }

	// 		if(curr->s.nt==eps) g[i].isNullable = 1;

    //         rhsChar prev = curr;

    //         for(int k=0; k<numTokens; k++) {
    //             rhsChar tmp = malloc(sizeof(struct rhsCharNode));
    //             tmp->next = NULL;

    //             char *token = malloc(MAX_TOKEN_LENGTH*sizeof(char));
    //             fscanf(fp, "%s", token);
    //             // printf("%s ", token);

    //             int tokenIdx = checkTerminal(token);
    //             if(tokenIdx!=-1) {
    //                 tmp->tag = 0;
    //                 tmp->s.t = tokenIdx;
    //             }else {
    //                 tokenIdx = checkNonTerminal(token);
    //                 if(tokenIdx==-1) {
    //                     fprintf(stderr, "%s - Non Terminal does not exist in ENUM\n", token);
	// 					exit(0);
    //                 }
    //                 tmp->tag = 1;
    //                 tmp->s.nt = tokenIdx;
    //             }

    //             prev->next = tmp;
    //             prev = tmp;
    //         }
    //         g[i].heads[j] = curr;

    //         // printf("\n");
    //     }
    // }

	printf("Loaded Grammar!!!\n");

    fclose(fp);
}
//------------------------------------------------------------
//FIRST and FOLLOW

void first(Grammar g, rhsChar rcn, long long int* firstBitString) {
    // this function assumes that the caller has already checked whether
    // the first set for nt has been previosly computed before calling this function
    if (rcn->tag==0) { //symbol is a terminal
        // fprintf(stderr, "%s#\n", getTermString(rcn->s.t));
        // int tmp = *firstBitString;
        // tmp |= 1<<(rcn->s.t);
        // *firstBitString = tmp;
        *firstBitString = *firstBitString | 1<<(rcn->s.t);
        // fprintf(stderr, "%d#%d#\n", rcn->s.t, *firstBitString);
    } 
    else { //symbol is a non-terminal => further digging needed
        lhsChar lc = g[rcn->s.nt];
        // fprintf(stderr, "%s$\n", getNonTermString(rcn->s.nt));
        if (lc.first == 0) { //first set isn't initialized yet
            computeFirst(g, rcn->s.nt);
        }
        
        if (lc.isNullable) {
            *firstBitString = *firstBitString | (lc.first & EXCLUDE_EPS); //exclude eps
            if (rcn->next != NULL)
                first(g, rcn->next, firstBitString);
            else
                //This was the last symbol in the rule
                *firstBitString = *firstBitString | INCLUDE_EPS;            
                // fprintf(stderr, "%d*%d*\n", rcn->s.t, *firstBitString);
        } else {
            //no need to compute first sets of further nonTerminals in the rule
            *firstBitString = *firstBitString | (lc.first);
            // fprintf(stderr, "%d$%d$\n", rcn->s.t, *firstBitString);
        }
    }
    return;
}

void computeFirst(Grammar g, nonTerminal nt) {
    lhsChar lc = g[nt];
    for (int i=0; i<lc.numRules; i++) {
        // fprintf(stderr, "%s:%d\n", getNonTermString(nt), i+1);
        // fprintf(stderr, "%s Before:%lld ", getNonTermString(nt), lc.first);
        first(g, lc.heads[i]->next, &(lc.first));
        // fprintf(stderr, "After: %lld\n", lc.first);
        g[nt].first = lc.first;
    }

    // rhsChar curr = lc.heads[0]->next;

    // fprintf(stderr, "%s->%s\n", getNonTermString(nt), getNonTermString(curr->s.nt));
    
    // while(curr!=NULL) {
    //     if(curr->tag==0) fprintf(stderr, "%s ", getTermString(curr->s.t));
    //     else fprintf(stderr, "%s ", getNonTermString(curr->s.nt));
    //     curr = curr->next;
    // }
    // fprintf(stderr, "\n");
}

void computeFollow(Grammar g, nonTerminal nt) {
    // the function assumes that the caller has first checked if the follow set
    // of nt has already been computed
    // fprintf(stderr, "%s Before:%lld ", getNonTermString(nt), g[nt].follow);
    rhsChar rcn, nextrcn;
    long long int firstOfNext;
    for (int i=0; i<NO_OF_NONTERMINALS; i++) {
        for (int j=0; j<g[i].numRules; j++) {
            rcn = g[i].heads[j];
            while (rcn!=NULL) {
                if (rcn->tag==1 && rcn->s.nt==nt) {
                    if (rcn->next !=NULL) {
                        nextrcn = rcn->next;
                        first(g, nextrcn, &firstOfNext);
                        // fprintf(stderr, "%lld\n", firstOfNext);
                        if (((firstOfNext>>eps) & 1) == 1) {
                            // first set of subsequent string contains eps
                            firstOfNext = firstOfNext & EXCLUDE_EPS;
                            g[nt].follow = g[nt].follow | firstOfNext;
                            if (i!=nt) // if the nonTerminal on LHS is different than nt
                            {
                                if (g[i].follow == 0)
                                    computeFollow(g, i);
                                g[nt].follow = g[nt].follow | g[i].follow;                             
                            }
                        } else {
                            g[nt].follow = g[nt].follow | firstOfNext;
                        }
                    } else {
                        //This is the last character in the RHS of the production
                        if (i!=nt) // if the nonTerminal on LHS is different than nt
                        {
                            if (g[i].follow == 0)
                                computeFollow(g, i);
                            g[nt].follow = g[nt].follow | g[i].follow;                             
                        }
                    }
                }
                rcn = rcn->next;
            }
        }
    }
    // fprintf(stderr, "After: %lld\n", g[nt].follow);
}


void createParseTable(Grammar g, int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS]) {
	for(int i = 0; i < NO_OF_NONTERMINALS; i++)
        for(int j = 0; j < NO_OF_TERMINALS; j++)
        {
        	t[i][j] = -1;
        }

	int shiftFirst;
	int shiftFollow;
	long long int first;
	long long int follow;
    // for(int i=0; i<NO_OF_NONTERMINALS; i++) {
    //     fprintf(stderr, "%lld: %lld\n", g[i].first, g[i].follow);
    // }
	for (int i=0; i<NO_OF_NONTERMINALS; i++) {
		for (int j=0; j<g[i].numRules; j++) {
			first = g[i].first;
			follow = g[i].follow;
            //NO_OF_TERMINALS-1 in order to not consider eps while making parse table
			for (shiftFirst = 0; shiftFirst<NO_OF_TERMINALS-1; shiftFirst++) {
				// if (((first>>shiftFirst) & 1) == 1) {
				// 	t[i][shiftFirst] = j;
				// }
                if(first & (1<<shiftFirst)) t[i][shiftFirst] = j;
			}
            // now shiftFirst has acquired the value of eps            
			if((first>>eps) & 1 == 1){
				for (shiftFollow= 0; shiftFollow<NO_OF_TERMINALS-1; shiftFollow++)
				{
					if(((follow>>shiftFollow) & 1) == 1){
						t[i][shiftFollow] = j;
					}
				}
			}
			if((((first>>eps) & 1) == 1) && (((follow>>dollar)  & 1) == 1)){
				t[i][dollar] = j;
			}
		}
	}
}

void parseInputSourceCode(FILE* sourceFile, int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS], Grammar g, parseTree root, int* error){
	Stack parseStack=newStack();
	Stack tempStack = newStack();
	tokenInfo token;
	parseTree leaf=NULL, current, parent;
	symbol begin, finish;
	begin.nt = program;
	finish.t = dollar;
	Key start = newKey(begin, NULL, 1, root);
	Key end = newKey(finish, NULL, 0, NULL);
	push(parseStack, end);
	push(parseStack, start);
	Key k, child, temp;
	int productionIndex, ruleLen, i;
	rhsChar rcn;
	do {
		getNextToken(sourceFile, &token);
		k = top1(parseStack);
		//top of stack is a nonTerminal
		if (k->tag==1) {
            if (token.tokenType == TK_EOF) {
                printf("%lld: end of file reached unexpectedly\n", token.lineNum);
                printf("exiting parser...\n");
                break;
            } else if(t[(k->id).nt][token.tokenType] >= 0) {
				//this parsetree node is now going to expand and
				//bear children, hence naming it parent :)
				parent = k->subtree;
				parent->nt = (k->id).nt;
				productionIndex = t[(k->id).nt][token.tokenType];
				pop(parseStack);
				rcn = g[(k->id).nt].heads[productionIndex];
				ruleLen = 0;
				while (rcn->next != NULL) {
					//currently, there is no parsetree corresponding
					//to this key object
					//pushing into temporary stack for order reversal
					push(tempStack, newKey(rcn->s, token.lexeme, rcn->tag, NULL));
					ruleLen++;
					rcn = rcn->next;
				}
				parent->children = (parsetree*)malloc(ruleLen*sizeof(parsetree));
				for (int i=ruleLen-1; i>=0; i--) {
					child = top1(tempStack);
					pop(tempStack);
					//storing the location of this node
					//(in the parsetree) in the key object
					child->subtree = &(parent->children)[i];
					//populating the parsetree data structure
					//using the key obtained from the stack
					if (child->tag == 0) {
						(parent->children)[i].terminal->tokenType = child->id.t;
						strcpy((parent->children)[i].terminal->lexeme, child->lexeme);
						(parent->children)[i].numChild = 0;
						(parent->children)[i].nt = -1;
						(parent->children)[i].children = NULL;
					} else
						(parent->children)[i].nt = child->id.nt;
					push(parseStack, child);
				}
			} else {
				//enter recovery mode
				*error = 1;
                printf("%lld: Error entry: %s\n", token.lineNum, token.lexeme);
                printf("Parse table entry is %d\n", t[k->id.nt][token.tokenType]);
                // printf("%lld: unexpected token: %s\n", token.lineNum, token.lexeme);
                nonTerminal ntOnTop = (k->id).nt;
                long long int followSyn = g[ntOnTop].follow;
                while(followSyn & (1<<token.tokenType) == 0) {
                    getNextToken(sourceFile, &token);
                }
			}
		} else {
            if (k->id.t == dollar) {
                if (token.tokenType==TK_EOF)
                    printf("\n\nParsing completed. Compilation successful!\n");
                else {
                    //enter recover mode
                    *error=1;
                    printf("%lld: parsing error: expecting end of program\n", token.lineNum);
                    while (token.tokenType != TK_EOF)
                        getNextToken(sourceFile, &token);
                }
            }
			if (k->id.t == token.tokenType) {                
				pop(parseStack);
            } else {
                // enter recovery mode
                *error = 1;
                printf("%lld: unexpected token: %s\n", token.lineNum, token.lexeme);
                while (k->tag != 1) {
                    if (k->id.t == dollar)
                        break;
                    pop(parseStack);
                    k = top1(parseStack);
                }
            }				
		}
	} while (token.tokenType != TK_EOF);
    fprintf(stderr, "Parse Tree Created Successfully\n");
}

void printParseTree(parseTree root) {

    printf("We are the Resistance!!!\n");

	// parseTree current;
	// int class;
	// for(int i=0;i<root->numChild;i++)
	// {
	// 	current=&(root->children[i]);
	// 	if(!current)
	// 	printf("NULL\n");
	// 	if(current->numChild==0 && current->terminal->tokenType==eps)
	// 	continue;
	// 	if(current->numChild>0)
	// 	{
	// 		printf("-------\t\t");
	// 		printf("-------\t\t");
	// 		printf("-------\t\t");
	// 		printf("-------\t\t");
	// 	}
	// 	else{
	// 		printf("%s\t\t",current->terminal->lexeme);
	// 		printf("%lld\t\t",current->terminal->lineNum);
	// 		printf("%s\t\t",tokenRepr(current->terminal->tokenType));
	// 		if(current->terminal->tokenType==TK_NUM || current->terminal->tokenType==TK_RNUM)
	// 			printf("%s\t\t",current->terminal->lexeme);
	// 		else printf("-------\t\t");	
	// 	}
	// 	printf("%s\t\t",idRepr(root->nonTerminal));
	// 	if(current->numChild==0)
	// 	{
	// 		printf("YES\t\t");
	// 		printf("-------\t\t");
	// 	}
	// 	else{
	// 		printf("NO\t\t");
	// 		printf("%s\t\t",idRepr(current->nonTerminal));
	// 	}
	// 	printf("\n");
	// 	printParseTree(current);
	// }
}

