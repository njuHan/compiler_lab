#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "rbtree.h"
#include "stack.h"
#include "tree.h"
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;


typedef struct Type_
{
	enum { BASIC, ARRAY, STRUCTURE ,FUNCTION} kind;
	union
	{
		//basic type
		int basic;
		
		//array type
		//element type and it's size
		struct { Type elem; int size; } array;
		
		//struct type ,it's a list
		FieldList structure;
		
		//函数类型
		struct
		{
			FieldList param;
			Type return_type;
			int num;
		} func;
	} u;
}Type_;

typedef struct FieldList_
{
	char* name; //域的名字
	Type type;  //域的类型
	FieldList tail; //下一个域
}FieldList_;

//符号表的元素
typedef struct Entry_
{
	//红黑树结点
	struct rb_node node;
	//value->name 作为红黑树中的keystring
	FieldList field;
}Entry_;

typedef struct Entry_* Entry;

//变量表
Stack var_table;
rb_root* global_var_table;

	
//struct 类型表
Stack struct_table;
rb_root* global_struct_table;


//rb_root* func_table;
//函数表改用一个链表队列实现


typedef struct Func_item
{
	FieldList	field;
	struct Func_item* next;
	
}Func_item;

//函数类型表
Func_item* funclist_head;

FieldList search_func(char* name);

void insert_func_at_head(FieldList f);

FieldList search_param(char* name);



void table_init();




//查找
Entry stack_search(Stack s, char* name);
Entry rb_search(struct rb_root *root, char* name);


//插入
int rb_insert(struct rb_root *root, Entry e);

//删除
void rb_delete(struct rb_root *root, char* name);

//输出
void print_rbtree(struct rb_root *tree);


//匿名结构体id
int anonymous_id;

//specifier
Type struct_specifier_handler(Node* node);
Type specifier_handler(Node* node);

//high-level definition

int extdef_handler(Node* node);
int extdeclist_handler(Node* node, Type type);

//declarators
FieldList vardec_handler(Node* node, Type type);
FieldList fundec_handler(Node* node);
int varlist_handler(Node* node, FieldList* list, int index);
FieldList paramdec_handler(Node* node);

/*
 * flag = 0 means that deflist is in the compst(in the function)
 * flag = 1 means that deflist is in the structureSpecifier
 */
//statements
void compst_handler(Node* node);
void stmtlist_handler(Node* node);

//local definition
FieldList deflist_handler(Node* node,   int flag);
void def_handler(Node* node, FieldList *list, int flag);
void declist_handler(Node* node, Type type, FieldList *list, int flag);
FieldList dec_handler(Node* node, Type type,  int flag);

//expressions
Type exp_handler(Node* node);

void return_handler(Node* node);

void semantic(Node* node, int level);
void semantic_scan(Node* node, int level);




#endif
