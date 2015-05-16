#include "semantic.h"
#include <string.h>
#include <assert.h>

FieldList search_func(char* name)
{
	if(funclist_head == NULL)
		return NULL;
	Func_item* p = funclist_head;
	while(p!=NULL)
	{
		if (strcmp(p->field->name, name)==0)
			return p->field;
		p = p->next;
	}
	return NULL;
}

void insert_func_at_head(FieldList f)
{
	Func_item* temp = (Func_item*)malloc(sizeof(Func_item));
	temp->field = f;
	temp->next=funclist_head;
	funclist_head = temp;
}

FieldList search_param(char* name)
{
	if (funclist_head == NULL)
		return NULL;
	
	FieldList temp = funclist_head->field;
	
	FieldList p = temp->type->u.func.param;
	if (p==NULL)
		return NULL;
	while (p!=NULL)
	{
		//printf("type: %d, name: %s, ",p->type->kind, p->name);
		if (strcmp(p->name, name)==0)
			return p;
		
		p = p->tail;
	}
	
	
}

Entry stack_search(Stack s, char* name)
{
	
	int n = s->size;
	int i;
	struct rb_root* root;
	Entry e;
	
	for (i=0; i<n; i++)
	{
		root = get_elemt_at(s, i);
		e = rb_search(root, name);
		if (e!=NULL)
			break;
	}
	return e;
}


//查找
Entry rb_search(struct rb_root *root, char* name)
{
    struct rb_node *node = root->rb_node;

    while (node) {
	Entry e = container_of(node, struct Entry_ , node);

	if (strcmp(name, e->field->name)<0)
	    node = node->rb_left;
	else if (strcmp(name, e->field->name)>0)
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
    while (*tmp) 
	{
		Entry this = container_of(*tmp, struct Entry_, node);
		//printf("insert %d, this: %d,,%d,%d\n", data->num,this->num,(int)data,(int)this);
		parent = *tmp;
	
		if (strcmp(e->field->name, this->field->name)<0)
			tmp = &((*tmp)->rb_left);
		else if (strcmp(e->field->name, this->field->name)>0)
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
		printf("name: %s, ", e->field->name);
		printf("type.kind: %d, ", e->field->type->kind);
		while (e->field->type->kind == 1)
		{
			printf("\n");
			printf("elem type: %d, size: %d",e->field->type->u.array.elem->kind, e->field->type->u.array.size);     
			e->field->type = e->field->type->u.array.elem;
			
		}
		//function
		if (e->field->type->kind == 3)
		{
			printf("return type: %d; param num: %d; ", e->field->type->u.func.return_type->kind, e->field->type->u.func.num);
			printf("param list: ");
			FieldList p = e->field->type->u.func.param;
			if (p==NULL)
				printf("No param");
			while (p!=NULL)
			{
				printf("type: %d, name: %s, ",p->type->kind, p->name);
				p = p->tail;
			}
		}
		
		else if (e->field->type->kind==STRUCTURE)
		{
			FieldList p = e->field->type->u.structure;
			printf("struct member: \n");
			while(p!=NULL)
			{
				printf("name: %s, type: %d\n",p->name, p->type->kind);
				p=p->tail;
			}
		}
		printf("\n");
	}
   
}


void table_init()
{
	//匿名结构体计数
	anonymous_id = 0;
	
	//func_table = (rb_root*)malloc(sizeof(rb_root));
	//*func_table = RB_ROOT;
	
	global_struct_table = (rb_root*)malloc(sizeof(rb_root));
	*global_struct_table = RB_ROOT;
	
	global_var_table = (rb_root*)malloc(sizeof(rb_root));
	*global_var_table = RB_ROOT;	
	
	var_table = malloc(sizeof(Stack_));
	stack_init(var_table);
	push(var_table, global_var_table);
	
	struct_table = malloc(sizeof(Stack_));
	stack_init(struct_table);
	push(struct_table, global_struct_table);
	
	funclist_head = NULL;
	
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
	
		
	if (strcmp(child->sibling->value.name, "OptTag") == 0 )
	{
		Node* def_list = child->sibling->sibling->sibling;
		assert(strcmp(def_list->value.name, "DefList")==0);
		int in_struct = 1;
		
		FieldList structure = deflist_handler(def_list, in_struct);
		
		int len = strlen(child->sibling->child->value.type_str);
		char* struct_name = (char*)malloc(sizeof(char)*(len+1));
		strcpy(struct_name, child->sibling->child->value.type_str);
		
		//Entry e = rb_search(global_struct_table, struct_name);
		//全栈搜索
		Entry e = stack_search(struct_table, struct_name);
		
		if (e==NULL)
		{
			Entry e = malloc(sizeof(struct Entry_));
			e->field = malloc(sizeof(struct FieldList_));
			e->field->name = (char*)malloc(len+1);
			e->field->type = (Type_*)malloc(sizeof(Type_));
			strcpy(e->field->name, struct_name);
			e->field->type->kind = STRUCTURE;
			e->field->tail = NULL;
			e->field->type->u.structure = structure;
			/*
		FieldList p = e->field->type->u.structure;
		
		while(p!=NULL)
		{
			printf("*** name: %s, type: %d\n",p->name, p->type->kind);
			p=p->tail;
		}
		*/
			//int ret = rb_insert(global_struct_table, e);
			
			int ret = rb_insert(get_elemt_at(struct_table, 0), e);
			if (ret < 0) 
			{
				fprintf(stderr, "ERROR!\nThe %s already exists.\n", e->field->name);
			}
			return e->field->type;
			
		}
		else
		{
			
			
			if (e->field->type->u.structure==NULL && structure!= NULL)
			{
				
				printf("Error at line %d: this struct contains it's own type member\n",child->value.lineno);
			}
			else
			{
				printf("Error type 16 at Line %d: Duplicated name '%s'.\n", child->value.lineno, struct_name);
			}
			
			e->field->type->u.structure = structure;
			return e->field->type;
			
		}
		
		
		
	}
	//匿名结构体
	else if (strcmp(child->sibling->value.name, "null") == 0)
	{
		char temp[20];
		sprintf(temp,"anonymous %d",anonymous_id);
		
		int len = strlen(temp);
		char* struct_name = (char*)malloc(sizeof(char)*(len+1));
		strcpy(struct_name, temp);
		printf("struct name: %s\n", struct_name);
		Entry e = stack_search(struct_table, struct_name);
		if (e==NULL)
		{
			Entry e = malloc(sizeof(struct Entry_));
			e->field = malloc(sizeof(struct FieldList_));
			e->field->name = (char*)malloc(len+1);
			e->field->type = (Type_*)malloc(sizeof(Type_));
			strcpy(e->field->name, struct_name);
			e->field->type->kind = STRUCTURE;
			e->field->tail = NULL;
			
			int ret = rb_insert(get_elemt_at(struct_table, 0), e);
			if (ret < 0) 
			{
				fprintf(stderr, "ERROR!\nThe %s already exists.\n", e->field->name);
			}
			return e->field->type;
			
		}
		else
		{
			return e->field->type;
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
		
		Entry e = stack_search(struct_table, struct_name);
		
		if (e==NULL)
		{
			if (struct_table->size>1)
			{
				printf("Error type 17 at Line %d: Undefined structure '%s'.\n", child->value.lineno, struct_name);
			}
			
			//printf("%s\n",child->sibling->child->value.type_str );
			Entry e = malloc(sizeof(struct Entry_));
			e->field = malloc(sizeof(struct FieldList_));
			e->field->name = (char*)malloc(len+1);
			e->field->type = (Type_*)malloc(sizeof(Type_));
			strcpy(e->field->name, child->sibling->child->value.type_str);
			e->field->type->kind = STRUCTURE;
			e->field->tail = NULL;
			e->field->type->u.structure =NULL;
			
			int ret = rb_insert(get_elemt_at(struct_table, 0), e);
			if (ret < 0) 
			{
				fprintf(stderr, "ERROR!\nThe %s already exists.\n", e->field->name);
			}
			return e->field->type;
			
		}
		else
		{
			
			//printf("Error type 16 at Line %d: Duplicated name '%s'.\n", child->value.lineno, struct_name);
			
			return e->field->type;
		}
		
		
	}
	else
	{
		printf("error in struct specifier\n");
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
		/*
		Entry e = rb_search(func_table, func_name);
		if (e == NULL)
		{
			FieldList func = fundec_handler(child);
			//函数返回值
			func->type->u.func.return_type = type;
			e = malloc(sizeof(struct Entry_));
			e->field = func;
			

			int ret = rb_insert(func_table, e);
			if (ret < 0) 
			{
				fprintf(stderr, "ERROR!\nThe %s already exists.\n", e->field->name);
				assert(0);
			}
		
		}
		*/
		FieldList func = search_func(func_name);
		if (func == NULL)
		{
			func = fundec_handler(child);
			func->type->u.func.return_type = type;
			insert_func_at_head(func);
		}
		
		else
		{
			printf("Error type 4 at Line %d: Redefined function '%s'.\n",child->value.lineno, func_name);
		}
		
		
		
	
		
	}
	else
	{
		printf("error， extdef_handler\n");
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
		var->tail = NULL;
		Entry e = malloc(sizeof(struct Entry_));
		e->field = var;
		
		//int ret = rb_insert(global_var_table, e);
		int ret = rb_insert(get_elemt_at(var_table,0), e);
		if (ret < 0) 
		{
			fprintf(stderr, "ERROR!\nThe %s already exists.\n", e->field->name);
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
			//new_type->u.array.elem = var->type;

			if (var->type->kind == BASIC)
			{
				//printf("%d------------\n",var->type->kind);
				new_type->u.array.elem = var->type;
				var->type = new_type;
			}
			else
			{
				Type q = var->type;
				Type p = var->type->u.array.elem;
				while (p->kind != BASIC)
				{
					q = p;
					p = p->u.array.elem;
				}
				new_type->u.array.elem = p;
				q->u.array.elem = new_type;
			}
				
		
		}
		else
			assert(0);
	
		return var;
		
	}
	else
	{
		printf("ERROR, vardec_handler\n");
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

void compst_handler(Node* node)
{
	//局部作用域变量表初始化
	rb_root* temp_table;
	temp_table = (rb_root*)malloc(sizeof(rb_root));
	*temp_table = RB_ROOT;
	push(var_table, temp_table);
	
	//局部作用域struct表初始化
	rb_root* temp_st;
	temp_st = (rb_root*)malloc(sizeof(rb_root));
	*temp_st = RB_ROOT;
	push(struct_table, temp_st);
	

	Node* def_list = node->child->sibling;
	Node* stmt_list = def_list->sibling;
	
	
	int in_struct = 0; //not in struct
	FieldList list = NULL;
	
	if (strcmp(def_list->value.name, "DefList")==0)
		list = deflist_handler(def_list, in_struct);
	while (list!=NULL)
	{
		
		FieldList var = list;
		list = list->tail;
		var->tail = NULL;
		Entry e = malloc(sizeof(struct Entry_));
		e->field = var;
		//printf("insert var:  %s \n", var->name);
		int ret = rb_insert(temp_table, e);
		if (ret < 0) 
		{
			fprintf(stderr, "ERROR!\nThe %s already exists.\n", e->field->name);
		}
		
		
	}
	
	
	if (strcmp(stmt_list->value.name, "StmtList")==0)
		stmtlist_handler(stmt_list);
	
	rb_root* temp = pop(var_table);
	assert(temp!=global_var_table);
	/*
	printf("-----now pop var table:--------\n");
	print_rbtree(temp);
	printf("-------------------\n");
	*/
	
	
	rb_root* st = pop(struct_table);
	assert(st!=global_struct_table);
	//printf("---********************--now pop struct table, size: %d--------\n", struct_table->size);
	
	//print_rbtree(st);
	//printf("-------------------\n");
	
	
	
}

void stmtlist_handler(Node* node)
{
	if(strcmp(node->value.name, "Exp")==0)
		 exp_handler(node);
	if (strcmp(node->value.name, "RETURN")==0)
	{
		return_handler(node);
	}
	if (node->child!=NULL)
		return stmtlist_handler(node->child);
	if(node->sibling!=NULL)
		return stmtlist_handler(node->sibling);
	
}


FieldList deflist_handler(Node* node,  int flag)
{
	
	FieldList func;
	FieldList structure = NULL;
	Node* p = node->child;
	while (strcmp(p->value.name, "Def")==0)
	{
		
		
		def_handler(p, &structure, flag);
		p = p->sibling;
		
		if (p!=NULL)
		{
			
			p = p->child;
			if (p==NULL)
				break;
		}
	}
		
	return structure;
		
	
	
}
void def_handler(Node* node, FieldList *list, int flag)
{
	
	Node* child = node->child;
	
	assert(strcmp(child->value.name, "Specifier")==0);
	
	Type type = specifier_handler(child);
	
	declist_handler(child->sibling, type, list, flag);
	
	
}
void declist_handler(Node* node, Type type, FieldList* list, int flag)
{
	Node* child = node->child;
	assert(strcmp(child->value.name, "Dec")==0);
	FieldList var;
	var = dec_handler(child, type, flag);
	
	//检查是否重定义
	FieldList head = *list;
	int flag_redefine = 0;
	while(head!=NULL)
	{
		if (strcmp(head->name, var->name)==0)
		{
			flag_redefine = 1;
			
			if (flag == 1) //in structure
			{
				printf("Error type 15 at Line %d: Redefined field '%s'.\n", child->value.lineno, var->name);
			}
			else //not in structure
			{
				
				printf("Error type 3 at Line %d: Redefined variable '%s'.\n",child->value.lineno, var->name);
			}
		}
			
		head = head->tail;
	}
	
	if (flag_redefine==0) //没有重定义则添加在链表中
	{
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
	}
	
	if (child->sibling!=NULL && strcmp(child->sibling->value.name, "COMMA")==0)
	{
		
		declist_handler(child->sibling->sibling, type, list, flag);
	}
	
	
}

FieldList dec_handler(Node* node, Type type, int flag)
{
	Node* child = node->child;
	assert(strcmp(child->value.name, "VarDec")==0);
	
	if (flag == 1) //在结构体中
	{
		if (child->sibling != NULL)
			printf("Error type 15 at line %d: Cannot assign variables in structure\n", child->value.lineno);
		
	}
	if (child->sibling != NULL && child->sibling->sibling!=NULL)
	{
	//	FieldList l = vardec_handler(child, type);
		
		Type temp = exp_handler(child->sibling->sibling);
		//if (temp==NULL || type==NULL)
		//	assert(0);
		
		if (is_type_eq(temp, type)!=1)
		{
			printf("Error type 5 at line %d:Type mismatched for assignment\n", child->value.lineno);
			
		}
	}
	
	return vardec_handler(child, type);
}

int is_leftvalue(Node* node)
{
	Node* child = node->child;
	if (strcmp(child->value.name, "INT")==0 || strcmp(child->value.name, "FLOAT")==0 )
		return 0;
	if (strcmp(child->value.name, "LP")==0
		||strcmp(child->value.name, "MINUS")==0
		||strcmp(child->value.name, "NOT")==0 )
		
		return is_leftvalue(child->sibling);
	
	if (strcmp(child->value.name, "ID")==0)
		return 1;
		
	if (strcmp(child->value.name, "Exp")==0)
		return is_leftvalue(child);
	
	
	return 1;
}

int is_type_eq(Type lt, Type rt)
{
	
	if (lt==NULL || rt==NULL)
		return 0;
	
	if (lt->kind != rt->kind)
		return 0;
	
	if (lt->kind == BASIC)
	{
		if (lt->u.basic == rt->u.basic)
			return 1;
		else
			return 0;
	}
	if (lt->kind == ARRAY)
		return is_type_eq(lt->u.array.elem, rt->u.array.elem);
	
	if (lt->kind == STRUCTURE)
	{
		if(strcmp(lt->u.structure->name, rt->u.structure->name)==0)
			return 1;
		else
			return 0;
	}
	
	return 0;
}

int is_param_eq(FieldList param, Node* node)
{
	
	//没有参数
	if (param==NULL && strcmp(node->value.name, "RP")==0)
		return 1;
	assert(strcmp(node->value.name, "Args")==0);
	
	Type type = exp_handler(node->child);
	if (is_type_eq(param->type, type)==1)
	{
		if (param->tail==NULL && node->child->sibling ==NULL )
			return 1;
		else if (param->tail==NULL && node->child->sibling !=NULL )
			return 0;
		else if (param->tail!=NULL && node->child->sibling ==NULL )
			return 0;
		else
			return is_param_eq(param->tail, node->child->sibling->sibling);
	}
	else
		return 0;
	
}

Type exp_handler(Node* node)
{
	Node* child = node->child;
	Type type = malloc(sizeof(Type_));
	if (strcmp(child->value.name, "Exp") == 0)
	{
		//printf("%s\n", child->sibling->value.name);
		//assert(0);
		//exp -> exp assignop exp
		if (child->sibling!=NULL && strcmp(child->sibling->value.name, "ASSIGNOP")==0)
		{
			if (! is_leftvalue(child))
			{
				printf("Error Type 6 at line %d: The left-hand side of an assignment must be a variable\n", 
					child->value.lineno);
			}
			//printf("%s %s\n", child->child->value.name, child->sibling->sibling->value.name);
			Type left_type = exp_handler(child);
			Type right_type = exp_handler(child->sibling->sibling);
			//assert(left_type!=NULL);
			
			//assert(right_type!=NULL);
			//printf("%d, %d\n", left_type->kind, left_type->u.basic);
			//printf("%d, %d\n", right_type->kind, right_type->u.basic);
			if (left_type==NULL || right_type==NULL)
				return NULL;
			if (is_type_eq(left_type, right_type)==0)
			{
				printf("Error type 5 at line %d: Type mismatched for assignment.\n", child->value.lineno);
				
			}
			
			return NULL;	
		}
		
		//exp -> exp and exp 
		//   | exp or exp
		else if (child->sibling!=NULL && (strcmp(child->sibling->value.name, "AND")==0 || (strcmp(child->sibling->value.name, "OR")==0 )))
		{
			Type left_type = exp_handler(child);
			Type right_type = exp_handler(child->sibling->sibling);
			if (left_type==NULL || right_type == NULL)
				return NULL;
			if (is_type_eq(left_type, right_type)==0)
			{
				printf("Error type 7 at Line %d: Type mismatched for operands\n",child->value.lineno);
				return NULL;
			}
			else if (left_type->kind!=BASIC || right_type->kind!=BASIC || left_type->u.basic!=TYPE_INT || right_type->u.basic!=TYPE_INT)
			{
				printf("Error type 7 at Line %d: Type mismatched for operands\n",child->value.lineno);
				return NULL;
			}
			type->kind = BASIC;
			type->u.basic = left_type->u.basic;
			return type;
		}
		
		else if(child->sibling != NULL &&(strcmp(child->sibling->value.name, "RELOP") == 0 
			||strcmp(child->sibling->value.name, "PLUS") == 0 
			||strcmp(child->sibling->value.name, "MINUS") == 0 
			||strcmp(child->sibling->value.name, "STAR") == 0 
			||strcmp(child->sibling->value.name, "DIV") == 0 ))
		{
			Type left_type = exp_handler(child);
			Type right_type = exp_handler(child->sibling->sibling);
			
			if (left_type==NULL || right_type == NULL)
				return NULL;
			
			if (is_type_eq(left_type, right_type)==0)
			{
				printf("Error type 7 at Line %d: Type mismatched for operands\n",child->value.lineno);
				return NULL;
			}
			if (strcmp(child->sibling->value.name, "RELOP") == 0)
			{
				type -> kind = BASIC;
				type -> u.basic = TYPE_INT;
			}
			else
			{
				type -> kind = left_type->kind;
				type->u.basic = left_type->u.basic;
			}
			return type;
			
		}
		
		else if(child->sibling!=NULL && strcmp(child->sibling->value.name, "LB")==0)
		{
			Type type1 = exp_handler(child);
			Type type2 = exp_handler(child->sibling->sibling);
			if (type1!=NULL&&type2!=NULL)
			{
				if (type1->kind != ARRAY)
				{
					printf("Error Type 10 at line %d: '%s' is not an array\n", 
						child->value.lineno, child->child->value.type_str);
					return NULL;
				}
				else
				{
					if (type2->kind != BASIC || type2->u.basic != TYPE_INT)
					{
						if (type2->u.basic==TYPE_FLOAT)
							printf("Error Type 12 at line %d: array index is not an interger\n", 
								   child->value.lineno);
						else
							printf("Error Type 12 at line %d: array index is not an interger\n", 
								   child->value.lineno);
						return NULL;
					}
					else
					{
						type->kind = type1->u.array.elem->kind;
						if(type->kind == BASIC)
							type->u.basic = type1->u.array.elem->u.basic;
						else if (type->kind == ARRAY)
							type->u.array = type1->u.array.elem->u.array;
						else
							type->u.structure = type1->u.array.elem->u.structure;
						
						return type;
					}
				}
			}
			else
				return NULL;
			
		}
		else if(child->sibling!=NULL && strcmp(child->sibling->value.name, "DOT")==0)
		{
			
			Type temp = exp_handler(child);
			//printf("%s %d %d\n", child->child->value.name,temp->kind, STRUCTURE);
			Node* id = child->sibling->sibling;
			if (temp!=NULL)
			{
				if (temp->kind != STRUCTURE)
				{
					printf("Error Type 13 at line %d: Illegal use of '.'\n", child->value.lineno);
					return NULL;
				}
				FieldList st = temp->u.structure;
				while (st!=NULL)
				{
					if (strcmp(st->name, id->value.type_str)==0)
						break;
					st = st->tail;
				}
				if (st==NULL)
				{
					printf("Error type 14 at line %d: Un-existed field '%s'\n", 
						child->value.lineno, id->value.type_str);
					return NULL;
				}
				return st->type;
			}
			return NULL;
		}
		
		else 
		{
			printf("exp->exp ***\n");
			assert(0);
		}
		
	}
	
	else if (strcmp(child->value.name, "LP")==0 
		|| strcmp(child->value.name, "MINUS")==0 
		|| strcmp(child->value.name, "NOT")==0 )
	{
		type = exp_handler(child->sibling);
		return type;
	}
	
	
	else if (strcmp(child->value.name, "ID")==0 && child->sibling!=NULL)
	{
		//Entry rb_search(struct rb_root *root, char* name)
		//printf("func name:%s\n", child->value.type_str);
		FieldList func_temp = search_func(child->value.type_str);
		
		if (func_temp == NULL)
		{
			Entry e;
			e = stack_search(var_table, child->value.type_str);
			if (e==NULL)
				e = stack_search(struct_table, child->value.type_str);
			if (e ==NULL)
				printf("Error type 2 at line %d: Undefined function '%s'\n",child->value.lineno,child->value.type_str);
			else
				printf("Error type 11 at line %d: '%s' is not a function\n",child->value.lineno,child->value.type_str);
			return NULL;
		}
		
		
		else if ((func_temp->type->u.func.param == NULL 
			&& strcmp(child->sibling->sibling->value.name, "RP")!=0)
			||(func_temp->type->u.func.param != NULL 
			&& strcmp(child->sibling->sibling->value.name, "RP")==0))
		{
			printf("Error Type 9 at line %d: The method '%s' is not applicable for the arguments\n", child->value.lineno, child->value.type_str);
		}
		
		else if (is_param_eq(func_temp->type->u.func.param,  child->sibling->sibling)!=1)
		{
			printf("Error Type 9 at line %d: The method '%s' is not applicable for the arguments\n", child->value.lineno, child->value.type_str);
		}
		
		type->kind = func_temp->type->u.func.return_type->kind;
		if (func_temp->type->u.func.return_type->kind == BASIC)
		{
			type->u.basic = func_temp->type->u.func.return_type->u.basic;
		}
		else if (func_temp->type->u.func.return_type->kind == ARRAY)
		{
			type->u.array = func_temp->type->u.func.return_type->u.array;
		}
		else
		{
			type->u.structure = func_temp->type->u.func.return_type->u.structure;
		}
		return type;
			
	}
	else if(strcmp(child->value.name, "INT") == 0)
	{
		type->kind = BASIC;
		assert(TYPE_INT == 0);
		type->u.basic = TYPE_INT;
		return type;
	}
	
	else if (strcmp(child->value.name, "FLOAT") == 0)
	{
		type->kind = BASIC;
		assert(TYPE_FLOAT == 1);
		type->u.basic = TYPE_FLOAT;
		return type;
	}
	else if (strcmp(child->value.name, "ID")==0 && child->sibling == NULL)
	{
		
		//Entry e = rb_search(func_table, child->value.type_str);
		Entry e = stack_search(var_table, child->value.type_str);
		if (e==NULL)
		{	
			//在函数的参数表中查找
			FieldList temp = search_param(child->value.type_str);
			if (temp==NULL)
			{
				printf("Error type 1 at line %d: Undefined variable '%s'\n",child->value.lineno, child->value.type_str);
				return NULL;
			}
			
			else
			{
				return temp->type;
			}
		}
		return e->field->type;
	}
	else
	{
		printf("-- %s --\n",child->value.name);
		assert(0);
	}
}

void return_handler(Node* node)
{
	Node* exp = node->sibling;
	assert(strcmp(exp->value.name, "Exp")==0);
	
	Type type = exp_handler(exp);
	
	FieldList temp = funclist_head->field;
	
	Type rtype = temp->type->u.func.return_type;
	
	if (is_type_eq(type, rtype)!=1)
	{
		printf("Error type 8 at Line %d: Type mismatched for return.\n", node->value.lineno);
	}
}

void semantic(Node* node, int level)
{
	

	if (strcmp(node->value.name, "ExtDef")==0)
	{
		//printf("find a extdef\n");
		extdef_handler(node);
	}
	
	else if (strcmp(node->value.name, "CompSt")==0)
	{
		
		
		compst_handler(node);
		
	
	}

}

void semantic_scan(Node* node , int level)
{
	if (errcount>0)
	{	
		printf("Program has syntax error! semantic scan stopped!\n");
		return;
	}
	
	semantic(node,level);
	if (node->child!=NULL)
		semantic_scan(node->child, level+1);
	
	if (node->sibling!=NULL)
		semantic_scan(node->sibling, level);
	
		
	//printf("------------\n");
	 //print_rbtree(struct_table);
}

















