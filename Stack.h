/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#include "parserDef.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct element* Element;
//typedef int parseTree;
typedef struct key* Key;
struct key{
	symbol id;
	unsigned int tag : 1;
	parseTree parent;
};

struct element{
	Key k;
	Element next;
};

struct stack{
	int count;
	Element head;
	Element tail;
};
typedef struct stack* Stack;


Stack newStack();

void insert_at_front(Stack s, Element e);
void insert_at_end(Stack s, Element e);

void delete_at_front(Stack s);

void push(Stack s, int id, parseTree pt);
void pop(Stack s);
Key top1(Stack s);
Element newElement(Key k);
Key newKey(int id, parseTree pt);
