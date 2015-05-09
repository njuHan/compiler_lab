/* test.c */
#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"
#include "semantic.h"

#define NAME_LEN 20

int main(int argc, char *argv[])
{
    //struct rb_root mytree = RB_ROOT;
	rb_root* mytree = (rb_root*)malloc(sizeof(rb_root));
	*mytree = RB_ROOT;
	if (mytree==NULL)
		printf("NULL\n");
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
	
	tmp-> value->name = (char*)malloc(sizeof(char)*NAME_LEN);
	tmp->value->type = (Type_*)malloc(sizeof(Type_));
		
	rb_search(mytree, "test");
	printf("------\n");
	scanf("%s", tmp->value->name);
	
	ret = rb_insert(mytree, tmp);
	if (ret < 0) {
	    fprintf(stderr, "The %s already exists.\n", tmp->value->name);
	    free(tmp);
	}
    }

    printf("\nthe first test\n");
    print_rbtree(mytree);

    rb_delete(mytree, "test");

    printf("\nthe second test\n");
    print_rbtree(mytree);

    return 0;
}
