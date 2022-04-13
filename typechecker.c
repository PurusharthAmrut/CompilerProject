/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */


#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "parserDef.h"
#include "lexer.h"
#include "symbolTable.h"
#include "typechecker.h"

char *idRepr(int id);

typedef enum{
integer,
real,
rec,
union,
boolean,
error
}types;

struct tableHeader* tp;
int TypeChecker(parsetree root, symbolTable s){
	//printf("idahr\n");
	//if(root.numChild == 0)
	//printf("%s\n",root.terminal->lexeme);
	//else printf("%s\n",idRepr(root.nonTerminal));
	if(root.numChild != 0 && (root.nt == function || root.nt == mainFunction))
	{
		//printf("%s\n",idRepr(root.nonTerminal)) ;
		tp = root.tp;
		//if(!tp)
		//printf("%s\n",idRepr(root.nonTerminal));
	} 
	int type1, type2;
	type1 = 7;
	type2 = 8;
	if(root.terminal == NULL){
		switch(root.nt){
			
			case assignmentStmt:
				//printf("idahr333\n");
				type1 = TypeChecker(root.children[0], s);
				
				type2 = TypeChecker(root.children[1], s);
				//printf("%d\t%d", type1,type2);
				if(type1 == error || type2 == error) break;
				else if(type1==integer && type2==integer){
					return integer;
				} else if(type1==integer && type2==real){
					parsetree curr = root;
					while(curr.terminal == NULL && curr.numChild != 0){
						curr = curr.children[0];
					}
					printf("line %llu:Type mismatch", curr.terminal->lineNum);
					break;
				} else if(type1==real && type2==integer){
					return real;
				} else if(type1==real && type2==real){
					return real;
				}
			case singleOrRecId:
				//if(root.terminal != NULL) return root.stEntry->type;
					//if(root.children[0].stEntry == NULL) return error;
					
						// we have to access the record typedefinitions here. Fill in the required detais
				//record* rtype = tp->localTable[hashVal(root.children[0].terminal->lexeme, 10000)]->ptr;
				record* rtype;
				if(s->fTable[hashVal("global",10000)]->localTable[hashVal(root.children[0].terminal->lexeme,10000)])
				rtype = s->fTable[hashVal("global",10000)]->localTable[hashVal(root.children[0].terminal->lexeme, 10000)]->ptr;
				else if(tp->localTable[hashVal(root.children[0].terminal->lexeme, 10000)])
				rtype = tp->localTable[hashVal(root.children[0].terminal->lexeme, 10000)]->ptr;
				else break ;

				if(rtype == NULL) break;
				while(rtype != NULL){
					if(!strcmp(rtype->rname , root.children[1].terminal->lexeme)) {
						if(strcmp(rtype->type,"int")==0)
							return integer;
						else if(strcmp(rtype->type,"real")==0)
							return real;
						else 
							return rec;
						//return rtype->type;
					}
					rtype = rtype->next;
				}

			case arithmeticExpression:
				//printf("idhar444\n");
				type1 = TypeChecker(root.children[0], s);
				type2 = TypeChecker(root.children[1], s);
				if(type1==error || type2==error){
					break;
				} else if(type1==integer && type2==integer){
					return integer;
				} else if(type1==integer && type2==real){
					return real;
				} else if(type1==real && type2==integer){
					return real;
				} else if(type1==real && type2==real){
					return real;
				}

			case term:
				type1 = TypeChecker(root.children[0], s);
				type2 = TypeChecker(root.children[1], s);
				if(type1==error || type2==error){
					break;
				} else if(type1==integer && type2==integer){
					return integer;
				} else if(type1==integer && type2==real){
					return real;
				} else if(type1==real && type2==integer){
					return real;
				} else if(type1==real && type2==real){
					return real;
				}

			case termPrime:
				if(root.numChild == 3){
					type1 = TypeChecker(root.children[1], s);
					type2 = TypeChecker(root.children[2], s);
					if(type1==error || type2==error){
						break;
					} else if(type1==integer && type2==integer){
						return integer;
					} else if(type1==integer && type2==real){
						return real;
					} else if(type1==real && type2==integer){
						return real;
					} else if(type1==real && type2==real){
						return real;
					}
				} else{
					type1 = TypeChecker(root.children[1], s);
					//printf("%d\n", type1);
					return type1;
				}
			case expPrime:
				//printf("idhar555\n");
				if(root.numChild == 3){
					type1 = TypeChecker(root.children[1], s);
					type2 = TypeChecker(root.children[2], s);
					if(type1==error || type2==error){
						break;
					} else if(type1==integer && type2==integer){
						return integer;
					} else if(type1==integer && type2==real){
						return real;
					} else if(type1==real && type2==integer){
						return real;
					} else if(type1==real && type2==real){
						return real;
					}	
				}
				else{
					type1 = TypeChecker(root.children[1], s);
					//printf("%d\n", type1);
					return type1;
				}	
			case booleanExpression:
				if(root.numChild == 3){
					type1 = TypeChecker(root.children[0], s);
					type2 = TypeChecker(root.children[2], s);
					if((root.children[1].terminal->tokenType == TK_AND) || (root.children[1].terminal->tokenType == TK_OR)){
						if(type1 != boolean || type2 != boolean){
							parsetree curr = root;
							while(curr.terminal == NULL && curr.numChild != 0){
			
								curr = curr.children[0];
							}
							printf("line %llu:Logical operators can only be applied to boolean types", curr.terminal->lineNum);
							break;
						}	
					}
					else{
						if((type1!= integer && type1!= real)||(type2!=integer&&type2!=real))
						{
							printf("line %llu: Invalid expression. Relational operations possible only on int and real\n", root.children[0].terminal->lineNum);
							//return error;
							break;
						}
						if(type1!=type2)
						{
							printf("line %llu:types of expressions should be same while comparing\n", root.children[0].terminal->lineNum);
							//return error;
							break;
						}
						return boolean;	
					}
				}
				else if (root.numChild == 2){
					type1 = TypeChecker(root.children[1], s);
					if(type1 == error)
						break;//return error;
					else if(type1!= boolean)
					{
						printf("line %llu: NOT operator can only be applied to boolean types\n", root.children[0].terminal->lineNum);
					}
					return boolean;
				}	
			default:
				for(int i = 0; i < root.numChild; i++){
					TypeChecker(root.children[i] , s);
				}
				break;
		}
	}
	else{
		switch(root.terminal->tokenType){
			case TK_ID:
				//printf("idahr\n");
				if(tp == NULL) break;
				//printf("idahr777\n");
				
				char* temp;
				if(s->fTable[hashVal("global",10000)]->localTable[hashVal(root.terminal->lexeme, 10000)])
				temp = s->fTable[hashVal("global",10000)]->localTable[hashVal(root.terminal->lexeme, 10000)]->type;
				else if(tp->localTable[hashVal(root.terminal->lexeme, 10000)])
				temp = tp->localTable[hashVal(root.terminal->lexeme, 10000)]->type;
				else
				 {
				 	printf("Line %llu: variable %s not declared\n",root.terminal->lineNum,root.terminal->lexeme);	
				 	break ;
				 }
				
				if(strcmp(temp,"int")==0)
					return integer;
				else if(strcmp(temp,"real")==0)
					return real;
				else 
					return rec;
			case TK_NUM:
				return integer;
			case TK_RNUM:
				return real;
			case TK_RECORD:
				return rec;
			case TK_UNION:
				return union;
			default:
				break;

		}
	}
	
}

