#include "semantic.h"
#include <string.h>
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


void table_init()
{
	struct_table = (rb_root*)malloc(sizeof(rb_root));
	*struct_table = RB_ROOT;
	
	func_table = (rb_root*)malloc(sizeof(rb_root));
	*func_table = RB_ROOT;
	
}


Type specifer_handler(Node* node)
{
	Node* child = node->child;
	if (child == NULL)
	{
		printf("error specifer_handler\n");
		exit(1);
	}
	Type type;
	
	if (strcmp(child->value.name, "TYPE")==0)
	{
		type = (Type)malloc(sizeof(Type_));
		type->kind = BASIC;
		type->u.basic = child->value.flag;
		return type;
	}
	else if (strcmp(child->value.name, "StructSpecifier") == 0)
	{
		
		return struct_spcifier_handler(child);
	}
	else
	{
		printf("erorr specifier_handler\n");
		exit(1);
	}
	
}

Type struct_spcifier_handler(Node* node)
{		
	Node* child = node->child;
	//printf("%s\n", child->sibling->value.name );
	//StructSpecifier -> STRUCT OpTag LC DefList RC
	if (strcmp(child->sibling->value.name, "OptTag") == 0 || strcmp(child->sibling->value.name, "null") == 0)
	{
		
		int len = strlen(child->sibling->child->value.type_str);
		char* struct_name = (char*)malloc(sizeof(char)*(len+1));
		strcpy(struct_name, child->sibling->child->value.type_str);
		Entry e = rb_search(struct_table, struct_name);
		if (e==NULL)
		{
			
			Entry e = malloc(sizeof(struct Entry_));
			e->value = malloc(sizeof(struct FieldList_));
			e->value->name = (char*)malloc(len+1);
			e->value->type = (Type_*)malloc(sizeof(Type_));
			strcpy(e->value->name, child->sibling->child->value.type_str);
			e->value->type->kind = STRUCTURE;
			int ret = rb_insert(struct_table, e);
			if (ret < 0) 
			{
				fprintf(stderr, "ERROR!\nThe %s already exists.\n", e->value->name);
			}
			return e->value->type;
			
		}
		else
		{
			return e->value->type;
		}
		
		
		
	}
	
	//StructSpecifier -> STRUCT Tag 
	//Tag -> ID
	else if (strcmp(child->sibling->value.name, "Tag") == 0)
	{
		int len = strlen(child->sibling->child->value.type_str);
		//printf("****%s\n",child->sibling->child->value.type_str);
		char* struct_name = (char*)malloc(sizeof(char)*(len+1));
		strcpy(struct_name, child->sibling->child->value.type_str);
		
		Entry e = rb_search(struct_table, struct_name);
		
		if (e==NULL)
		{
			//return NULL;
			Entry e = malloc(sizeof(struct Entry_));
			e->value = malloc(sizeof(struct FieldList_));
			e->value->name = (char*)malloc(len+1);
			e->value->type = (Type_*)malloc(sizeof(Type_));
			strcpy(e->value->name, child->sibling->child->value.type_str);
			e->value->type->kind = STRUCTURE;
			int ret = rb_insert(struct_table, e);
			if (ret < 0) 
			{
				fprintf(stderr, "ERROR!\nThe %s already exists.\n", e->value->name);
			}
			return e->value->type;
		}
		else
		{
			return e->value->type;
		}
		
		
	}
	else
	{
		printf("error in struct specifier\n");
	}
}

void semantic(Node* node, int level)
{
	/*
	int i=0;
	if(strcmp(node->value.name,"null")!=0)  //节点内容不为空
	{	
		for (i = 0; i<level; i++) //打印缩进
			printf(" ");
		
		printf("%s", node->value.name);
	}
	else  //该节点在归约时，被替换为空串，所以不输出
	{
		return;
	}
	*/
	
	if (strcmp(node->value.name, "Specifier")==0)
	{
		printf("find a specifier\n");
		Type  temp = specifer_handler(node);
		if (temp != NULL)
		{
			printf("%d\n", temp->kind);
		}
	}
	
}

void semantic_scan(Node* node , int level)
{
	
	semantic(node,level);
	if (node->child!=NULL)
		semantic_scan(node->child, level+1);
	
	if (node->sibling!=NULL)
		semantic_scan(node->sibling, level);
	
		
	//printf("------------\n");
	 //print_rbtree(struct_table);
}

















