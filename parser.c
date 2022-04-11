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

	printf("Loaded Grammar!!\n");

    fclose(fp);
}
//------------------------------------------------------------
//FIRST and FOLLOW

void first(Grammar g, rhsChar rcn, long long int* firstBitString) {
    // this function assumes that the caller has already checked whether
    // the first set for nt has been previosly computed before calling this function

    if (rcn->tag==0) {
        //symbol is a terminal
        // printf("%s $$ \n", getTermString(rcn->s.t));
        *firstBitString = *firstBitString | (1LL<<(rcn->s.t));
    }else {
        //symbol is a non-terminal => further digging needed
        lhsChar lc = g[rcn->s.nt];

        if (lc.first == 0) { //first set isn't initialized yet
            computeFirst(g, rcn->s.nt);
        }

        // update the lc values
        lc = g[rcn->s.nt];
        
        if (lc.isNullable) {
            *firstBitString = *firstBitString | (lc.first & EXCLUDE_EPS); //exclude eps
            if (rcn->next != NULL) {
                first(g, rcn->next, firstBitString);
            }else {
                //This was the last symbol in the rule
                *firstBitString = *firstBitString | INCLUDE_EPS;
            }
        } else {
            //no need to compute first sets of further nonTerminals in the rule
            *firstBitString = *firstBitString | (lc.first);
        }
    }
    return;
}

void computeFirst(Grammar g, nonTerminal nt) {
    if(g[nt].first!=0) return;
    lhsChar lc = g[nt];
    // printf("%s Starting:\n", getNonTermString(lc.heads[0]->s.nt));
    for(int i=0; i<lc.numRules; i++) {
        rhsChar tmp = lc.heads[i];
        // while(tmp!=NULL) {
        //     if(tmp->tag==0) printf("%s ", getTermString(tmp->s.t));
        //     else printf("%s ", getNonTermString(tmp->s.nt));
        //     tmp = tmp->next;
        // }
        // printf("\n");
        
        // if(lc.heads[i]->next->tag==0) printf("%s Going into:\n", getTermString(lc.heads[i]->next->s.t));
        // else printf("%s Going into:\n", getNonTermString(lc.heads[i]->next->s.nt));
        first(g, lc.heads[i]->next, &(lc.first));
        // g[nt].first = lc.first;
    }
    // printf("%s Ended:\n", getNonTermString(lc.heads[0]->s.nt));
    g[nt].first = lc.first;
}

void printTerminalList(long long bitString) {
    for(int j=0; j<NO_OF_TERMINALS; j++) {
        if(bitString&(1LL<<j)) printf("%s ", getTermString(j));
    }
    printf("\n");
}

void printFirst(Grammar g) {
    printf("\n===============First Sets=============================\n");
    for(int i=0; i<NO_OF_NONTERMINALS; i++) {
        long long firstBitString = g[i].first;
        printf("%s:%lld:: ", getNonTermString(i), firstBitString);
        printTerminalList(firstBitString);
    }
    printf("\n======================================================\n");
}

void computeFollow(Grammar g, nonTerminal nt) {
    // the function assumes that the caller has first checked if the follow set
    // of nt has already been computed
    // fprintf(stderr, "%s Before:%lld ", getNonTermString(nt), g[nt].follow);
    // printf("%s Starting:\n", getNonTermString(nt));
    rhsChar rcn, nextrcn;
    long long firstOfNext = 0;
    for (int i=0; i<NO_OF_NONTERMINALS; i++) {
        for (int j=0; j<g[i].numRules; j++) {
            rcn = g[i].heads[j]->next; 
            while (rcn!=NULL) {
                if (rcn->tag==1 && rcn->s.nt==nt) {
                    if (rcn->next !=NULL) {

                        nextrcn = rcn->next;

                        bool hasEpsInFirst = 1;
                        while(hasEpsInFirst && nextrcn!=NULL) {
                            
                            // if(nextrcn->tag==0) printf("Using %s\n", getTermString(nextrcn->s.t));
                            // else printf("Using %s\n", getNonTermString(nextrcn->s.nt));

                            long long nextrcnFirst = 0;
                            if(nextrcn->tag==0) nextrcnFirst |= 1LL<<(nextrcn->s.t);
                            else {
                                // printf("%s: %lld\n", getNonTermString(nextrcn->s.nt), g[nextrcn->s.nt].first);
                                nextrcnFirst |= g[nextrcn->s.nt].first;
                            }
                            firstOfNext |= (nextrcnFirst & EXCLUDE_EPS);
                            g[nt].follow |= firstOfNext;

                            if((nextrcnFirst>>eps)%2==0) hasEpsInFirst = 0;                            
                            nextrcn = nextrcn->next;

                            if(nextrcn==NULL && hasEpsInFirst && i!=nt) {
                                if(g[i].follow==0) computeFollow(g, i);
                                g[nt].follow |= g[i].follow;  
                            }
                        }


                        // nextrcn = rcn->next;
                        
                        // if(nextrcn->tag==0) printf("Using %s\n", getTermString(nextrcn->s.t));
                        // else printf("Using %s\n", getNonTermString(nextrcn->s.nt));
                        
                        // // or the calculated first value of nonterminal
                        // // first(g, nextrcn, &firstOfNext);
                        // long long nextrcnFirst = 0;
                        // if(nextrcn->tag==0) nextrcnFirst |= 1LL<<(nextrcn->s.t);
                        // else {
                        //     // printf("%s: %lld\n", getNonTermString(nextrcn->s.nt), g[nextrcn->s.nt].first);
                        //     nextrcnFirst |= g[nextrcn->s.nt].first;
                        // }
                        // firstOfNext |= (nextrcnFirst & EXCLUDE_EPS);
                        // // fprintf(stderr, "%lld\n", firstOfNext);
                        // if ((nextrcnFirst>>eps)%2) { 
                        //     // first set of subsequent string contains eps
                        //     // g[nt].follow |= (firstOfNext & EXCLUDE_EPS);
                        //     g[nt].follow |= firstOfNext;
                        //     if (i!=nt) {
                        //         // if the nonTerminal on LHS is different than nt
                        //         if (g[i].follow == 0) computeFollow(g, i);
                        //         g[nt].follow |= g[i].follow;                             
                        //     }
                        // } else g[nt].follow |= firstOfNext;
                    } else {
                        //This is the last character in the RHS of the production
                        if (i!=nt) {
                            // if the nonTerminal on LHS is different than nt
                            if (g[i].follow == 0) computeFollow(g, i);
                            g[nt].follow |= g[i].follow;                             
                        }
                    }
                }
                rcn = rcn->next;
            }
        }
    }
    // printf("%s Ended:\n===========\n", getNonTermString(nt));
}

void printFollow(Grammar g) {
    printf("\n===================Follow Sets========================\n");
    for(int i=0; i<NO_OF_NONTERMINALS; i++) {
        long long followBitString = g[i].follow;
        printf("%s:%lld:: ", getNonTermString(i), followBitString);
        printTerminalList(followBitString);
    }
    printf("\n======================================================\n");
}

void createParseTable(Grammar g, int parseTable[NO_OF_NONTERMINALS][NO_OF_TERMINALS]) {
    // TODO: add error recovery in this
    for(int nt=0; nt<NO_OF_NONTERMINALS; nt++) {
        for(int t=0; t<NO_OF_TERMINALS; t++) parseTable[nt][t] = -1;
    }

    for(int nt=0; nt<NO_OF_NONTERMINALS; nt++) {
        long long followOfCurrNT = g[nt].follow;
        for(int ruleNum=0; ruleNum<g[nt].numRules; ruleNum++) {
            long long firstOfCurrProd = 0;
            first(g, g[nt].heads[ruleNum]->next, &firstOfCurrProd);
            
            for(int t=0; t<NO_OF_TERMINALS-1; t++) {
                if( (firstOfCurrProd & (1LL<<t)) 
                    || ( (firstOfCurrProd>>eps)%2 && (followOfCurrNT & (1LL<<t))) ) {
                    parseTable[nt][t] = ruleNum;
                }
            }

            if( (firstOfCurrProd>>eps)%2 && (followOfCurrNT>>dollar)%2 ) parseTable[nt][dollar] = ruleNum;
        }
    }
    fprintf(stderr, "Parse Table Created!!\n");
}

// void createParseTable(Grammar g, int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS]) {
// 	for(int i = 0; i < NO_OF_NONTERMINALS; i++) {
//         for(int j = 0; j < NO_OF_TERMINALS; j++) t[i][j] = -1;
//     }

// 	int shiftFirst;
// 	int shiftFollow;
// 	long long int first;
// 	long long int follow;
//     // for(int i=0; i<NO_OF_NONTERMINALS; i++) {
//     //     fprintf(stderr, "%lld: %lld\n", g[i].first, g[i].follow);
//     // }
// 	for (int i=0; i<NO_OF_NONTERMINALS; i++) {
// 		for (int j=0; j<g[i].numRules; j++) {
// 			first = g[i].first;
// 			follow = g[i].follow;
//             //NO_OF_TERMINALS-1 in order to not consider eps while making parse table
// 			for (shiftFirst = 0; shiftFirst<NO_OF_TERMINALS-1; shiftFirst++) {
// 				// if (((first>>shiftFirst) & 1) == 1) {
// 				// 	t[i][shiftFirst] = j;
// 				// }
//                 //TODO: shouldn't this be the first(B), where the rule is
//                 // A -> B ..., A -> C ... and so on an we are currently
//                 // processing A -> B ... [as first of g[i] includes first for
//                 // productions of A]
//                 if(first & (1<<shiftFirst)) t[i][shiftFirst] = j;
// 			}
//             // now shiftFirst has acquired the value of eps            
// 			if((first>>eps) & 1 == 1){  //TODO: correct this boolean arithmetic
// 				for (shiftFollow= 0; shiftFollow<NO_OF_TERMINALS-1; shiftFollow++)
// 				{
// 					if(((follow>>shiftFollow) & 1) == 1){ //TODO: correct this boolean arithmetic
// 						t[i][shiftFollow] = j;
// 					}
// 				}
// 			}
//             //TODO: shouldn't this be the first(B), where the rule is
//             // A -> B ..., A -> C ... and so on an we are currently
//             // processing A -> B ... [as first of g[i] includes first for
//             // productions of A]
//             //TODO: correct this boolean arithmetic
//             //TODO: dollar > 59, which is the number of no_of_terminals allowed (fix dimensions)
// 			if((((first>>eps) & 1) == 1) && (((follow>>dollar)  & 1) == 1)){
// 				t[i][dollar] = j;
// 			}
// 		}
// 	}
// }

void printParseTable(int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS]) {
    printf("\n===================Parse Table========================\n");
    for(int i=0; i<NO_OF_NONTERMINALS; i++) {
        printf("| ");
        for(int j=0; j<NO_OF_TERMINALS; j++) {
            if(t[i][j]==-1) printf("- ");
            else printf("%d ", t[i][j]);
        }
        printf("|\n");
    }
    printf("\n======================================================\n");
}

void parseInputSourceCode(FILE* sourceFile, int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS], Grammar g, parseTree *root, int* error) {
    Stack parseStack = newStack();

    symbol begin, finish;
	begin.nt = program;
	finish.t = dollar;
	// Key start = newKey(begin, NULL, 1, root);
	Key start = newKey(begin, NULL, 1, NULL);
	Key end = newKey(finish, NULL, 0, NULL);
	push(parseStack, end);
	push(parseStack, start);

    bool retrieveNextToken = 1;
    tokenInfo token;
    Key k;
    do {
        if(retrieveNextToken) getNextToken(sourceFile, &token);
		k = top1(parseStack);

        // printf("Key: ");
        // printKey(k);
        // printf("Token: ");
        // printToken(&token);
        // printf("####\n");

        if(token.tokenType==TK_COMMENT) continue;

        if(k->tag==0 && k->id.t==eps) {
            // correcting the terminal lexeme
            if(k->subtree->terminal==NULL) printf("Terminal Node terminal value is not initialized!\n");
            else {
                // TODO: what should be eps's lexeme and linenum value
                // strcpy(k->subtree->terminal->lexeme, token.lexeme);
                strcpy(k->subtree->terminal->lexeme, "eps");
                // k->subtree->terminal->lineNum = 0;
                k->subtree->terminal->lineNum = token.lineNum;
            }
            pop(parseStack);
            continue;
        }
		
        if(k->tag==1) {
            // TODO: fix lexeme values for nodes
            if(token.tokenType==TK_EOF) {
                printf("%lld: end of file reached unexpectedly\n", token.lineNum);
                printf("exiting parser...\n");
                break;
            }else if(t[k->id.nt][token.tokenType]!=-1) {
                retrieveNextToken = 0;

                parsetree parent;
                parent.nt = k->id.nt;

                int prodIdx = t[k->id.nt][token.tokenType];
                pop(parseStack);

                rhsChar rcn = g[k->id.nt].heads[prodIdx]->next;
                int prodLen = 0;
                Stack tempStack = newStack();

                while(rcn!=NULL) {
                    push(tempStack, newKey(rcn->s, token.lexeme, rcn->tag, NULL));
                    prodLen++;
                    rcn = rcn->next;
                }

                parent.numChild = prodLen;
                parent.children = (parseTree)malloc(prodLen*sizeof(parsetree));

                for(int idx=prodLen-1; idx>=0; idx--) {
                    Key topElem = top1(tempStack);
                    pop(tempStack);

                    // if(topElem->tag==0) printf("pushing %s into the parse stack\n", getTermString(topElem->id.t));
                    // else printf("pushing %s into the parse stack\n", getNonTermString(topElem->id.nt));

                    // store the parent pointer in subtree, which will be used to reference it later
                    // printf("Pointer before update: %p\n", (void*)topElem->subtree);
                    topElem->subtree = &parent.children[idx];
                    // printf("Pointer after update: %p\n", (void*)topElem->subtree);

                    // printKey(topElem);

                    if(topElem->tag==0) {
                        parent.children[idx].terminal = (tokenInfo*)malloc(sizeof(tokenInfo));

                        parent.children[idx].terminal->tokenType = topElem->id.t;
                        if(topElem->lexeme!=NULL) strcpy(parent.children[idx].terminal->lexeme, topElem->lexeme);
                        parent.children[idx].nt = -1;
                        parent.children[idx].numChild = 0;
                        parent.children[idx].children = NULL;
                    }else {
                        parent.children[idx].nt = topElem->id.nt;
                        // default initialization
                        parent.children[idx].numChild = 0;
                        parent.children[idx].children = NULL;
                    }
                    push(parseStack, topElem);
                    // if(topElem->tag==0) printf("pushed %s into the parse stack\n", getTermString(topElem->id.t));
                    // else printf("pushed %s into the parse stack\n", getNonTermString(topElem->id.nt));
                }

                // printParseTree(&parent);

                if(parent.nt!=program) {
                    if(parent.terminal!=NULL) {
                        (*k->subtree).terminal = (tokenInfo*)malloc(sizeof(tokenInfo));
                        (*k->subtree).terminal->tokenType = parent.terminal->tokenType;
                        (*k->subtree).terminal->lineNum = parent.terminal->lineNum;
                        if(parent.terminal->lexeme!=NULL) strcpy((*k->subtree).terminal->lexeme, parent.terminal->lexeme);
                    }
                    (*k->subtree).nt = parent.nt;
                    (*k->subtree).numChild = parent.numChild;
                    (*k->subtree).children = parent.children;
                }else {
                    if((*root)==NULL) *root = (parseTree)malloc(sizeof(parsetree));
                    (*root)->nt = parent.nt;
                    (*root)->numChild = parent.numChild;
                    (*root)->children = parent.children;
                }

                // printParseTree(*root);
            }else {
                // enter recovery mode
				*error += 1;
                printf("%lld: Error entry: %s\n", token.lineNum, token.lexeme);
                printf("Parse table entry is %d\n", t[k->id.nt][token.tokenType]);
                // printf("%lld: unexpected token: %s\n", token.lineNum, token.lexeme);
                //TODO: this code is incorrect, check both follow (push the checked token) and first of productions
                // nonTerminal ntOnTop = k->id.nt;
                // long long followOfNT = g[ntOnTop].follow;
                // while(1) {
                    
                // }


                // nonTerminal ntOnTop = (k->id).nt;
                // long long int followSyn = g[ntOnTop].follow;
                // while(followSyn & (1LL<<token.tokenType) == 0) {
                //     getNextToken(sourceFile, &token);
                // }
            }
        }else {
            // k is a terminal
            if(k->id.t==dollar) {
                if(token.tokenType==TK_EOF) {
                    // terminate the program
                    printf("Parsing completed. Compilation successful!!\n");
                }else {
                    //enter recover mode (terminating program by reading every token till TK_EOF)
                    *error += 1;
                    printf("%lld: parsing error: expecting end of program\n", token.lineNum);
                    while(token.tokenType!=TK_EOF) getNextToken(sourceFile, &token);
                }
            }else if(k->id.t==token.tokenType) {
                // since both terminals match, pop it from the stack
                retrieveNextToken = 1;
                
                // this next line might not be needed
                strcpy(k->lexeme, token.lexeme);

                // correcting the terminal lexeme and linenum
                if(k->subtree->terminal==NULL) printf("Terminal Node terminal value is not initialized!\n");
                else {
                    strcpy(k->subtree->terminal->lexeme, token.lexeme);
                    k->subtree->terminal->lineNum = token.lineNum;
                }
                
                // printKey(k);
                
                pop(parseStack);
            }else {
                // enter recovery mode (pop every non terminal, till you hit a non terminal)
                *error += 1;
                printf("%lld: unexpected token: %s\n", token.lineNum, token.lexeme);
                // TODO: dollar is at the bottom of the stack so maybe k!=NULL is not needed
                while(k!=NULL && k->tag!=1 && k->id.t!=dollar) {
                    pop(parseStack);
                    k = top1(parseStack);
                }
            }
        }
    }while(token.tokenType!=TK_EOF);
}

// void parseInputSourceCode(FILE* sourceFile, int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS], Grammar g, parseTree root, int* error){
// 	Stack parseStack=newStack();
// 	Stack tempStack = newStack();
// 	tokenInfo token;
// 	parseTree leaf=NULL, current, parent;
// 	symbol begin, finish;
// 	begin.nt = program;
// 	finish.t = dollar;
// 	Key start = newKey(begin, NULL, 1, root);
// 	Key end = newKey(finish, NULL, 0, NULL);
// 	push(parseStack, end);
// 	push(parseStack, start);
// 	Key k, child, temp;
// 	int productionIndex, ruleLen, i;
// 	rhsChar rcn;
// 	do {
// 		getNextToken(sourceFile, &token);
// 		k = top1(parseStack);
// 		//top of stack is a nonTerminal
//         printf("Key: ");
//         printKey(k);
//         printf("Token: ");
//         printToken(&token);
//         printf("####\n");
//         if(token.tokenType==TK_COMMENT) continue;
// 		if (k->tag==1) {
//             if (token.tokenType == TK_EOF) {
//                 printf("%lld: end of file reached unexpectedly\n", token.lineNum);
//                 printf("exiting parser...\n");
//                 break;
//             // } else if(t[(k->id).nt][token.tokenType] >= 0) {
//             } else if(t[(k->id).nt][token.tokenType] != -1) {
// 				//this parsetree node is now going to expand and
// 				//bear children, hence naming it parent :)
//                 printf("we can use production rule %d\n", t[(k->id).nt][token.tokenType]+1);
// 				parent = k->subtree;
// 				parent->nt = (k->id).nt;
// 				productionIndex = t[(k->id).nt][token.tokenType];
// 				pop(parseStack);
// 				rcn = g[(k->id).nt].heads[productionIndex]->next;
// 				ruleLen = 0;
// 				while (rcn->next != NULL) {
// 					//currently, there is no parsetree corresponding
// 					//to this key object
// 					//pushing into temporary stack for order reversal
// 					push(tempStack, newKey(rcn->s, token.lexeme, rcn->tag, NULL));
// 					ruleLen++;
// 					rcn = rcn->next;
// 				}
// 				parent->children = (parsetree*)malloc(ruleLen*sizeof(parsetree));
// 				for (int i=ruleLen-1; i>=0; i--) {
// 					child = top1(tempStack);
// 					pop(tempStack);
// 					//storing the location of this node
// 					//(in the parsetree) in the key object
// 					child->subtree = &(parent->children)[i];
// 					//populating the parsetree data structure
// 					//using the key obtained from the stack
// 					if (child->tag == 0) {
// 						(parent->children)[i].terminal->tokenType = child->id.t;
// 						strcpy((parent->children)[i].terminal->lexeme, child->lexeme);
// 						(parent->children)[i].numChild = 0;
// 						(parent->children)[i].nt = -1;
// 						(parent->children)[i].children = NULL;
// 					} else
// 						(parent->children)[i].nt = child->id.nt;
// 					push(parseStack, child);
// 				}
// 			} else {
// 				//enter recovery mode
// 				// *error = 1;
//                 // printf("%lld: Error entry: %s\n", token.lineNum, token.lexeme);
//                 // printf("Parse table entry is %d\n", t[k->id.nt][token.tokenType]);
//                 // // printf("%lld: unexpected token: %s\n", token.lineNum, token.lexeme);
//                 // nonTerminal ntOnTop = (k->id).nt;
//                 // long long int followSyn = g[ntOnTop].follow;
//                 // while(followSyn & (1<<token.tokenType) == 0) {
//                 //     getNextToken(sourceFile, &token);
//                 // }
//                 printf("Error 1\n");
//                 exit(0);
// 			}
// 		} else {
//             if (k->id.t == dollar) {
//                 if (token.tokenType==TK_EOF)
//                     printf("\n\nParsing completed. Compilation successful!\n");
//                 else {
//                     //enter recover mode
//                     // *error=1;
//                     // printf("%lld: parsing error: expecting end of program\n", token.lineNum);
//                     // while (token.tokenType != TK_EOF) getNextToken(sourceFile, &token);
//                     printf("Error 2\n");
//                     exit(0);
//                 }
//             }else if (k->id.t == token.tokenType) {                
// 				pop(parseStack);
//             } else {
//                 // enter recovery mode
//                 // *error = 1;
//                 // printf("%lld: unexpected token: %s\n", token.lineNum, token.lexeme);
//                 // // TODO: add k->id.t != token.tokenType in while conditions
//                 // while (k->tag != 1) {
//                 //     if (k->id.t == dollar) break;
//                 //     pop(parseStack);
//                 //     k = top1(parseStack);
//                 // }
//                 printf("Error 3\n");
//                 exit(0);
//             }				
// 		}
// 	} while (token.tokenType != TK_EOF);
//     fprintf(stderr, "Parse Tree Created Successfully\n");
// }

void printParseTree(parseTree root) {

    if(root==NULL) return;

    if(root->nt==-1) {
        // terminal
        printf("Terminal: %s, Lexeme: %s, LineNum: %lld\n", getTermString(root->terminal->tokenType), 
        root->terminal->lexeme, root->terminal->lineNum);
        return;
    }

    printf("Non-Terminal: %s, Children: %d\n", getNonTermString(root->nt), root->numChild);
    for(int i=0; i<root->numChild; i++) {
        printf("%s \t\t Child %d: ", getNonTermString(root->nt), i+1);
        printParseTree(&root->children[i]);
    }


    // printf("We are the Resistance!!!\n");

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

