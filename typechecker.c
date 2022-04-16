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

	if(root.nt == function || root.nt == mainFunction)
	{
		tp = root.th;
	} 

	int Atype = error + 1;
	int Btype = Atype + 2;;

	if(root.nt != -1){
		switch(root.nt){
			case assignmentStmt:			
				Atype = TypeChecker(root.children[0], s);
				Btype = TypeChecker(root.children[1], s);

				if(Atype == error || Btype == error) return error;

				else if(Atype==integer && Btype==integer){
					return integer;
				} else if(Atype==integer && Btype==real){
					parsetree curr = root;
					while(curr.terminal == NULL && curr.numChild != 0){
						curr = curr.children[0];
					}
					printf("line %llu: type mismatch", curr.terminal->lineNum);
					break;
				} else if(Atype==real && Btype==integer){
					return real;
				} else if(Atype==real && Btype==real){
					return real;
				}

			case booleanExpression:			
				if(root.numChild == 3){
					Atype = TypeChecker(root.children[0], s);
					Btype = TypeChecker(root.children[2], s);

					if((root.children[1].terminal->tokenType == TK_AND) || (root.children[1].terminal->tokenType == TK_OR)){
						if(Atype != boolean || Btype != boolean){
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
						if((Atype!= integer && Atype!= real)||(Btype!=integer&&Btype!=real))
						{
							printf("line %llu: TYPE ERROR - relational operator requires int or real operands\n", root.children[0].terminal->lineNum);
							return error;
							break;
						}
						if(Atype!=Btype)
						{
							printf("line %llu: TYPE ERROR - both operands should have same type\n", root.children[0].terminal->lineNum);
							return error;
							break;
						}
						return boolean;	
					}
					break;
				}
				else if (root.numChild == 2){
					Atype = TypeChecker(root.children[1], s);
					if(Atype == error){
						return error;
						break;
					}
						
					else if(Atype!= boolean)
					{
						printf("line %llu: TYPE ERROR - NOT operator requires boolean operands\n", root.children[0].terminal->lineNum);
						return error;
					}
					return boolean;
				}
				break;

			case arithmeticExpression:			
				Atype = TypeChecker(root.children[0], s);
				Btype = TypeChecker(root.children[1], s);
				if(Atype==error || Btype==error){
					return error;
				} else if(Atype==integer && Btype==integer){
					return integer;
				} else if(Atype==integer && Btype==real){
					return real;
				} else if(Atype==real && Btype==integer){
					return real;
				} else if(Atype==real && Btype==real){
					return real;
				}
				break;

			case term:			
				Atype = TypeChecker(root.children[0], s);
				Btype = TypeChecker(root.children[1], s);
				if(Atype==error || Btype==error){
					break;
				} else if(Atype==integer && Btype==integer){
					return integer;
				} else if(Atype==integer && Btype==real){
					return real;
				} else if(Atype==real && Btype==integer){
					return real;
				} else if(Atype==real && Btype==real){
					return real;
				}
				break;

			case termPrime:			
				if(root.numChild == 3){
					Atype = TypeChecker(root.children[1], s);
					Btype = TypeChecker(root.children[2], s);
					if(Atype==error || Btype==error){
						break;
					} else if(Atype==integer && Btype==integer){
						return integer;
					} else if(Atype==integer && Btype==real){
						return real;
					} else if(Atype==real && Btype==integer){
						return real;
					} else if(Atype==real && Btype==real){
						return real;
					}
				} else{
					Atype = TypeChecker(root.children[1], s);
					return Atype;
				}
				break;

			case expPrime:			
				if(root.numChild == 3){
					Atype = TypeChecker(root.children[1], s);
					Btype = TypeChecker(root.children[2], s);
					if(Atype==error || Btype==error){
						break;
					} else if(Atype==integer && Btype==integer){
						return integer;
					} else if(Atype==integer && Btype==real){
						return real;
					} else if(Atype==real && Btype==integer){
						return real;
					} else if(Atype==real && Btype==real){
						return real;
					}	
				}
				else{
					Atype = TypeChecker(root.children[1], s);
					return Atype;
				}
				break;	

			case singleOrRecId:			
				record* rectype;
				if(s->fTable[hashFuncLUT("global",9973)]->localTable[hashFuncLUT(root.children[0].terminal->lexeme,9973)])
					rectype  = s->fTable[hashFuncLUT("global",9973)]->localTable[hashFuncLUT(root.children[0].terminal->lexeme, 9973)]->ptr;
				else if(tp->localTable[hashFuncLUT(root.children[0].terminal->lexeme, 9973)])
					rectype = tp->localTable[hashFuncLUT(root.children[0].terminal->lexeme, 9973)]->ptr;
				else break;

				if(rectype == NULL) break;
				while(rectype != NULL){
					if(!strcmp(rectype->recname , root.children[1].terminal->lexeme)) {
						if(strcmp(rectype->type,"int")==0)
							return integer;
						else if(strcmp(rectype->type,"real")==0)
							return real;
						else 
							return rec;
					}
					rectype = rectype->next;
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
				
				char* ch;
				if(s->fTable[hashFuncLUT("global",9973)]->localTable[hashFuncLUT(root.terminal->lexeme, 9973)])
					ch = s->fTable[hashFuncLUT("global",9973)]->localTable[hashFuncLUT(root.terminal->lexeme, 9973)]->type;
				else if(tp->localTable[hashFuncLUT(root.terminal->lexeme, 9973)])
					ch = tp->localTable[hashFuncLUT(root.terminal->lexeme, 9973)]->type;
				else
				 {
				 	printf("Line %llu: variable %s is not declared\n",root.terminal->lineNum,root.terminal->lexeme);	
				 	break ;
				 }
				
				if(strcmp(ch,"int")==0)
					return integer;
				else if(strcmp(ch,"real")==0)
					return real;
				else 
					return rec;
				break;
			case TK_NUM:
				return integer;
				break;
			case TK_RNUM:
				return real;
				break;
			case TK_RECORD:
				return rec;
				break;
			case TK_UNION:
				return uniontype;
				break;
			default:
				break;

		}
	}
	
}

