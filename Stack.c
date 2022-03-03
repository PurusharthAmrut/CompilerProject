/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#include "Stack.h"


Stack newStack(){
	Stack s = malloc(sizeof( struct stack));
	s->head = NULL;
	s->tail = NULL;
	s->count = 0;
	return s;
}


void insert_at_front(Stack s, Element e){
	//pushing into the stack
	if(s->count == 0){
		s->head = e;
		s->tail=e;
		e->next = NULL;
		s->count++;
	}
	else{
		e->next = s->head;
		s->head= e;
		s->count++;
	}
}
void insert_at_end(Stack s, Element e){
	if(s->count == 0){
		s->head = e;
		s->tail=e;
		e->next = NULL;
		s->count++;
	}
	else{
		s->tail->next = e;
		s->tail=e;
		e->next = NULL;
		s->count++;
	}
}

void delete_at_front(Stack s){
	if(s->count == 0){
		printf("empty stack\n");
	}
	else if(s->count == 1){
		Element curr = s->head;
		s->head = NULL;
		s->tail = NULL;
		s->count = 0;
	}
	else if(s->count > 1){
		Element enext = s->head->next;
		Element curr = s->head;
		s->head = enext;
		curr->next = NULL;
		s->count--;
	}
}

Key top1(Stack s){
	if(s->head != NULL){return s->head->k;}
	else{
		printf("Empty Stack\n");
		return NULL;
	}
}

		
Element newElement(Key k)
{
	Element a = malloc(sizeof(struct element));
	a->k = k;
	a->next=NULL;
	return a;

}		   

Key newKey(symbol id, unsigned int tag, parseTree pt){
	Key k = malloc(sizeof(struct key));
	(k)->id = id;
	(k)->subtree = pt;
	return k;
}
		
void push(Stack s, Key key){
	Element e = newElement(key);
	insert_at_front(s, e);
}

void pop(Stack s){
	delete_at_front(s);
}		
			
		
