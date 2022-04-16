edit: driver.o utils.o lexer.o Stack.o parser.o AST.o symbolTable1.o typechecker.o semanticAnalyzer1.o
	gcc -o driver driver.o utils.o lexer.o Stack.o parser.o AST.o symbolTable1.o typechecker.o semanticAnalyzer1.o

utils.o: utils.c utils.h
	gcc -c utils.c

lexer.o: lexer.c lexer.h lexerDef.h
	gcc -c lexer.c

Stack.o: Stack.c Stack.h
	gcc -c Stack.c

parser.o: parser.c parser.h parserDef.h
	gcc -c parser.c

AST.o: AST.c AST.h
	gcc -c AST.c

symbolTable1.o: symbolTable1.c symbolTable1.h
	gcc -c symbolTable1.c

typechecker.o: typechecker.c typechecker.h
	gcc -c typechecker.c

semanticAnalyzer1.o: semanticAnalyzer1.c semanticAnalyzer1.h
	gcc -c semanticAnalyzer1.c

driver.o: driver.c 
	gcc -c driver.c

clean:
	rm driver driver.o utils.o lexer.o Stack.o parser.o AST.o symbolTable1.o typechecker.o semanticAnalyzer1.o