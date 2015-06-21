#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "semantic.h"
#include "ir.h"

extern int temp_count;
extern int var_count;
int main(int argc, char** argv)
{
	table_init();
	
	errcount = 0;
	
	if(argc!=3)
	{
		//printf("argv error! usage: ./parser test1.c test1output.ir\n");
		printf("argv error! usage: ./parser test1.c test1output.s\n");
		exit(0);
	}
	
	
	FILE* f = fopen(argv[1], "r");
	if (!f)
	{
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);
	yyparse();
	
	//output_tree(root,0);
	
	
	semantic_scan(root,0);
	ir_scan(root);
	InterCodes temp = ir_list;

	
	//print_ir(ir_list, argv[2]);
	print_ir(ir_list, "out.temp");
	
	
	load_mem(argv[2]);
	
	//printf("\n------struct-----\n");
	//print_rbtree(global_struct_table);
	
	//printf("\n-----global var------\n");
	//print_rbtree(global_var_table);
	
	//printf("\n-----function------\n");
	//print_rbtree(func_table);
	
	/*
	FieldList temp;
	while(funclist_head!=NULL)
	{
		
		temp = funclist_head->field;
		printf("func name: %s\n", temp->name);
		printf("return type: %d; param num: %d; ", temp->type->u.func.return_type->kind, temp->type->u.func.num);
		printf("param list: ");
		FieldList p = temp->type->u.func.param;
		if (p==NULL)
		printf("No param");
		while (p!=NULL)
		{
			printf("type: %d, name: %s, ",p->type->kind, p->name);
			p = p->tail;
		}
		funclist_head = funclist_head -> next;
		printf("\n");
	}
	*/
	
	
	return 0;
}
