/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#include "typechecker.h"


int TypeChecker(parsetree root, symbolTable s){

	if(root.numChild != 0 && (root.nt == function || root.nt == mainFunction))
	{
		tp = root.tp;
	} 
	int type1, type2;
	type1 = 7;
	type2 = 8;
	if(root.terminal == NULL){
		switch(root.nt){
			
			case assignmentStmt:
				
				type1 = TypeChecker(root.children[0], s);
				
				type2 = TypeChecker(root.children[1], s);
		
				if(type1 == error || type2 == error) break;
				else if(type1==integer && type2==integer){
					return integer;
				} else if(type1==integer && type2==real){
					parsetree curr = root;
					while(curr.terminal == NULL && curr.numChild != 0){
						curr = curr.children[0];
					}
					printf("TYpe error in line %llu:Type mismatch", curr.terminal->lineNum);
					break;
				} else if(type1==real && type2==integer){
					return real;
				} else if(type1==real && type2==real){
					return real;
				}
			case singleOrRecId:

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
					}
					rtype = rtype->next;
				}

			case arithmeticExpression:
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
					return type1;
				}
			case expPrime:
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
							printf("Type error in line %llu: Logical operators are applied only to boolean types but your expression contins non-boolean types. \n", curr.terminal->lineNum);
							return error;
							break;
						}	
					}
					else{
						if((type1!= integer && type1!= real)||(type2!=integer&&type2!=real))
						{
							printf("Type error in line %llu: Relational operations are applied only on int and real but your expression contains other data types.\n", root.children[0].terminal->lineNum);
							return error;
							break;
						}
						if(type1!=type2)
						{
							printf("Type Error in line %llu: Different types\n", root.children[0].terminal->lineNum);
							return error;
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
						printf("Type error in line %llu: NOT operator can be applied only to boolean types but you expression contains non-boolean values.\n", root.children[0].terminal->lineNum);
						return error;
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
				if(tp == NULL) break;
				
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
				return uniontype;
			default:
				break;

		}
	}
	
}

