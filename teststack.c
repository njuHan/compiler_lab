#include "stack.h"
#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main(char argc, char* argv[])
{
	Stack s = (Stack)malloc(sizeof(Stack_));
	stack_init(s);

    if (argc < 2) 
	{
		fprintf(stderr, "Usage: %s num\n", argv[0]);
		exit(-1);
    }
    int i, ret, num;
    struct mytype *tmp;

    num = atoi(argv[1]);
	i=0;
	rb_root* table[num] ;
	for(i=0; i<num; i++)
		table[i] = (rb_root*)malloc(sizeof(rb_root)*num);
	
	
for (i=0; i<num; i++)
{
    printf("Please enter integers, enter -1 to exit\n");
	
	
	printf("node----%d----------\n", i);
   
	do {

		tmp = malloc(sizeof(struct mytype));
		scanf("%d", &tmp->num);
		if (tmp->num == -1)
			break;
		//printf("****%d****\n",tmp->num);
		ret = rb_insert(table[i], tmp);
		if (ret < 0) 
		{
		    fprintf(stderr, "The %d already exists.\n", tmp->num);
			free(tmp);
		}	
    } while (tmp->num!=-1);

} 

 	for (i=0 ; i<num; i++)
		push(s, table[i]);

	for (i=0 ; i<num; i++)
	{
		printf("------%d-------\n", i);
		print_rbtree(pop(s));
	}
    
}
