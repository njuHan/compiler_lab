#include "semantic.h"
#include <string.h>
#include <assert.h>
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
	Entry e;
    for (node = rb_first(tree); node; node = rb_next(node))
	{
		e = rb_entry(node, struct Entry_, node);
		printf("name: %s, ", e->value->name);
		printf("type.kind: %d, ", e->value->type->kind);
		while (e->value->type->kind == 1)
		{
			printf("\n");
			printf("elem type: %d, size: %d",e->value->type->u.array.elem->kind, e->value->type->u.array.size);     
			e->value->type = e->value->type->u.array.elem;
			
		}
		//function
		if (e->value->type->kind == 3)
		{
			printf("return type: %d; param num: %d; ", e->value->type->u.func.return_type->kind, e->value->type->u.func.num);
			printf("param list: ");
			FieldList p = e->value->type->u.func.param;
			if (p==NULL)
				printf("No param");
			while (p!=NULL)
			{
				printf("type: %d, name: %s, ",p->type->kind, p->name);
				p = p->tail;
			}
		}
		printf("\n");
	}
   
}


void table_init()
{
	struct_table = (rb_root*)malloc(sizeof(rb_root));
	*struct_table = RB_ROOT;
	
	func_table = (rb_root*)malloc(sizeof(rb_root));
	*func_table = RB_ROOT;
	
	var_table = (rb_root*)malloc(sizeof(rb_root));
	*var_table = RB_ROOT;	
}


Type specifier_handler(Node* node)
{
	Node* child = node->child;
	if (child == NULL)
	{
		printf("error specifier_handler\n");
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
		
		return struct_specifier_handler(child);
	}
	else
	{
		printf("erorr specifier_handler\n");
		exit(1);
	}
	
}

Type struct_specifier_handler(Node* node)
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
			e->value->tail = NULL;
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
			e->value->tail = NULL;
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

FieldList vardec_handler(Node* node, Type type)
{
	Node* child = node->child;
	FieldList var;
	
	//VarDec -> ID
	if(strcmp(child->value.name, "ID")==0)
	{
		var = (FieldList)malloc(sizeof(FieldList_));
		int len = strlen(child->value.type_str);
		var->name = (char*)malloc(sizeof(len+1));
		strcpy(var->name, child->value.type_str);
		var->type = type;
		var->tail = NULL;
		return var;
	}
	//VarDec -> VarDec LB INT RB
	else if (strcmp(child->value.name, "VarDec") == 0)
	{
		var = vardec_handler(child, type);
		Type new_type = (Type)malloc(sizeof(Type_));
		new_type -> kind = ARRAY;
		new_type->u.array.size = child-> sibling->sibling->value.type_int;
		if(type->kind == BASIC || type->kind == STRUCTURE)
		{
			new_type->u.array.elem = var->type;
			var->type = new_type;
		}
		else if (type->kind == ARRAY)
		{
			while ((var->type->u.array.elem)->kind == ARRAY)
				var->type = var->type->u.array.elem;
			new_type->u.array.elem = var->type;
			var->type = new_type;
		}
		return var;
		
	}
	else
	{
		printf("ERROR, vardec_handler\n");
		exit(1);
	}
}

int extdeclist_handler(Node* node, Type type)
{
	FieldList var;
	Node* child = node->child;
	if (strcmp(child->value.name , "VarDec") == 0)
	{
		var = vardec_handler(child, type);
		
		Entry e = malloc(sizeof(struct Entry_));
		e->value = var;
		
		int ret = rb_insert(var_table, e);
		if (ret < 0) 
		{
			fprintf(stderr, "ERROR!\nThe %s already exists.\n", e->value->name);
		}
		if (child->sibling == NULL)
			return 0;
		else if (strcmp(child->sibling->value.name, "COMMA")==0)
		{
			//printf("COMMA\n");
			return extdeclist_handler(child->sibling->sibling, type);
		}
	}
	else
	{
		printf("error! extdeclist_handler\n");
		exit(1);
	}
}

int extdef_handler(Node* node)
{
	Node* child = node->child; //"Specifier"
	Type type = specifier_handler(child); //get Type
	
	//获取兄弟节点
	child = child->sibling;
	
	//ExtDef -> Specifier SEMI
	if (strcmp(child->value.name, "SEMI") == 0)
		return 0;
	
	else if (strcmp(child->value.name, "ExtDecList") == 0)
	{
		return extdeclist_handler(child, type);
	}
	else if (strcmp(child->value.name, "FunDec")==0)
	{
		
		
		Node* id = child->child;
		assert(strcmp(id->value.name, "ID")==0);
		int len = strlen(id->value.type_str);
		char* func_name  = (char*)malloc(len+1);
		strcpy(func_name, id->value.type_str);
		//printf("%s\n", func_name);
		
		FieldList func = fundec_handler(child);
		//函数返回值
		func->type->u.func.return_type = type;
		Entry e = malloc(sizeof(struct Entry_));
		e->value = func;
		int ret = rb_insert(func_table, e);
		if (ret < 0) 
		{
			fprintf(stderr, "ERROR!\nThe %s already exists.\n", e->value->name);
		}
		
	}
	else
	{
		printf("error， extdef_handler\n");
		exit(1);
	}
}

FieldList paramdec_handler(Node* node)
{
	Type type;
	Node* child = node->child;
	if (strcmp(child->value.name, "Specifier") == 0)
	{
		type = specifier_handler(child);
		return vardec_handler(child->sibling, type);
	}
	else
	{
		printf("error! paramdec_handler\n");
		exit(0);
	}
}

int varlist_handler(Node* node, FieldList* list, int index)
{
	Node* child = node->child;
	if (strcmp(child->value.name, "ParamDec")!=0)
	{
		printf("error! paramdec\n");
		exit(1);
	}
	FieldList var;
	var = paramdec_handler(child);
	
	//连接到参数列表尾部
	FieldList p = *list;
	FieldList q = *list;

	
	if (*list == NULL)
	{
		*list = var;
		(*list)->tail = NULL;
	}
	else
	{
		FieldList p = *list;
		FieldList q = (*list)->tail;
		while (q!=NULL)
		{
			p=q;
			q = q->tail;
		}
		q = var;
		p->tail = q;
			
	}
	
	index ++;
	
	if (child->sibling == NULL)
	{
		assert(list!=NULL);
		return index;
		
	}
	else if (strcmp(child->sibling->value.name, "COMMA") == 0)
	{
		return varlist_handler(child->sibling->sibling, list, index);
	}
	else
	{
		printf("error, varlist_handler\n");
		exit(1);
	}
	
		
}

FieldList fundec_handler(Node* node)
{
	FieldList func;
	FieldList param_list= NULL;
	Node* child = node->child;
	func = (FieldList)malloc(sizeof(FieldList_));
	func->type = (Type)malloc(sizeof(Type_));
	func->type->kind = FUNCTION;
	func->tail = NULL;
	int len = strlen(child->value.type_str);
	func->name = (char*)malloc(len+1);
	strcpy(func->name, child->value.type_str);
	
	int num = 0;
	int i = 0;
	//FuncDec -> ID LP VarList RP
	if (strcmp(child->sibling->sibling->value.name, "VarList") == 0)
	{
		num = varlist_handler(child->sibling->sibling, &param_list, i);
		func->type->u.func.num = num;
		func->type->u.func.param = param_list;
		

	}
	//FuncDec-> ID LP RP
	else if (strcmp(child->sibling->sibling->value.name, "RP")==0)
	{
		func->type->u.func.param = NULL;
	}
	else
	{
		printf("error, funcdec_handler\n%s\n",child->sibling->sibling->value.name);
		exit(1);
	}
	return func;
	
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
	/*
	if (strcmp(node->value.name, "Specifier")==0)
	{
		printf("find a specifier\n");
		Type  temp = specifier_handler(node);
		if (temp != NULL)
		{
			printf("%d\n", temp->kind);
		}
	}
	*/
	
	if (strcmp(node->value.name, "ExtDef")==0)
	{
		//printf("find a extdef\n");
		extdef_handler(node);
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

















