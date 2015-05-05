#include "stack.h"

void stack_init(Stack s)
{
	s->size = 0;
	s->capacity = MAX_CAPACITY;
}

int push( Stack s, rb_root* r)
{
	if(s->size == s->capacity)
		return -1;
	
	s->var_table[s->size] = r;
	s->size ++;
	return 1;
}

rb_root* pop(Stack s)
{
	s->size --;
	return s->var_table[s->size];
}

//规定栈顶元素index==0, 栈底元素index==size-1
rb_root* get_elemt_at(Stack s, int i)
{
	if (i<0 || i>s->size-1)
		return NULL;
	
	return s->var_table[s->size-1-i];
}

