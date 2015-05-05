#include "semantic.h"

//查找
Entry rb_search(struct rb_root *root, char* name)
{
    struct rb_node *node = root->rb_node;

    while (node) {
	Entry e = container_of(node, struct Entry_ , node);

	if (strcmp(name, e->value->name)<0)
	    node = node->rb_left;
	else if (strcmp(name, e->value->name)>0)
	    node = node->rb_right;
	else
	    return e;
    }
    
    return NULL;
}

//插入
int rb_insert(struct rb_root *root, Entry e)
{
    struct rb_node **tmp = &(root->rb_node), *parent = NULL;

    // Figure out where to put new node 
    while (*tmp) {
	Entry this = container_of(*tmp, struct Entry_, node);
	//printf("insert %d, this: %d,,%d,%d\n", data->num,this->num,(int)data,(int)this);
	parent = *tmp;
	
	if (strcmp(e->value->name, this->value->name)<0)
	    tmp = &((*tmp)->rb_left);
	else if (strcmp(e->value->name, this->value->name)>0)
	    tmp = &((*tmp)->rb_right);
	else 
	    return -1;
    }
    
    // Add new node and rebalance tree.
    rb_link_node(&e->node, parent, tmp);
    rb_insert_color(&e->node, root);
    
    return 0;
}


//删除
void rb_delete(struct rb_root *root, char* name)
{
    Entry data = rb_search(root, name);
    if (!data) 
	{ 
		fprintf(stderr, "Not found \n");
		return;
    }
    rb_erase(&data->node, root);
    free(data);
}

void print_rbtree(struct rb_root *tree)
{
    struct rb_node *node;
    for (node = rb_first(tree); node; node = rb_next(node))
		printf("%s ", rb_entry(node, struct Entry_, node)->value->name);
    printf("\n");
}
