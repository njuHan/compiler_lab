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
	printf("-----------\n");

	semantic_scan(root,0);
	return 0;
}
