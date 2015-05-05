/* test.c */
#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"


int main(int argc, char *argv[])
{
    struct rb_root mytree = RB_ROOT;
    int i, ret, num;
    struct mytype *tmp;

    if (argc < 2) {
	fprintf(stderr, "Usage: %s num\n", argv[0]);
	exit(-1);
    }

    num = atoi(argv[1]);

    printf("Please enter %d integers:\n", num);
    for (i = 0; i < num; i++) {
	tmp = malloc(sizeof(struct mytype));
	if (!tmp)
	    perror("Allocate dynamic memory");

	scanf("%d", &tmp->num);
	
	ret = rb_insert(&mytree, tmp);
	if (ret < 0) {
	    fprintf(stderr, "The %d already exists.\n", tmp->num);
	    free(tmp);
	}
    }

    printf("\nthe first test\n");
    print_rbtree(&mytree);

    rb_delete(&mytree, 21);

    printf("\nthe second test\n");
    print_rbtree(&mytree);

    return 0;
}
