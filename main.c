#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "semantic.h"
int main(int argc, char** argv)
{
	table_init();
	
	errcount = 0;
	if (argc <= 1)
		return 1;
	FILE* f = fopen(argv[1], "r");
	if (!f)
	{
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);
	yyparse();
	
	output_tree(root,0);
	printf("\n------struct-----\n");
	
	semantic_scan(root,0);
	print_rbtree(global_struct_table);
	
	printf("\n-----global var------\n");
	print_rbtree(global_var_table);
	
	int n = var_table->size;
	int i=0;
	for (i=0; i<n-1; i++)
	{
		printf("\n-----compst %d var------\n", i);
		print_rbtree(pop(var_table));
	}

	
	printf("\n-----function------\n");
	print_rbtree(func_table);
	return 0;
}
