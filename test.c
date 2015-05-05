/* test.c */
#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"
#include "semantic.h"

int main(int argc, char *argv[])
{
    struct rb_root mytree = RB_ROOT;
    int i, ret, num;
    Entry tmp;

    if (argc < 2) {
	fprintf(stderr, "Usage: %s num\n", argv[0]);
	exit(-1);
    }

    num = atoi(argv[1]);

    printf("Please enter %d integers:\n", num);
    for (i = 0; i < num; i++) {
	tmp = malloc(sizeof(struct Entry_));
	tmp->value = malloc(sizeof(struct FieldList_));
	
	tmp-> value->name = (char*)malloc(sizeof(char)*20);
	
	printf("------\n");
	scanf("%s", tmp->value->name);
	
	ret = rb_insert(&mytree, tmp);
	if (ret < 0) {
	    fprintf(stderr, "The %s already exists.\n", tmp->value->name);
	    free(tmp);
	}
    }

    printf("\nthe first test\n");
    print_rbtree(&mytree);

    rb_delete(&mytree, "test");

    printf("\nthe second test\n");
    print_rbtree(&mytree);

    return 0;
}
