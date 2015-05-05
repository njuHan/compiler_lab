#ifndef _STACK_H
#define _STACK_H

#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>
/*

struct rb_root * element
 */

#define MAX_CAPACITY 10


typedef struct Stack_
{
	rb_root* var_table[MAX_CAPACITY];
	int size;
	int capacity;
} Stack_ ;

typedef struct Stack_* Stack;

void stack_init(Stack s);

int push( Stack s, rb_root* r);


rb_root* pop(Stack s);

//规定栈顶元素index==0, 栈底元素index==size-1
rb_root* get_elemt_at(Stack s, int i);





#endif
