#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "rbtree.h"
#include "stack.h"
#include "tree.h"
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;


typedef struct Type_
{
	enum { BASIC, ARRAY, STRUCTURE } kind;
	union
	{
		//basic type
		int basic;
		
		//array type
		//element type and it's size
		struct { Type elem; int size; } array;
		
		//struct type ,it's a list
		FieldList structure;
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
	FieldList value;
}Entry_;

typedef struct Entry_* Entry;

//变量表
Stack var_table;
	
//struct 类型表
//Stack struct_table;
rb_root* struct_table;

//函数类型表
rb_root* func_table;


void table_init();



//查找
Entry rb_search(struct rb_root *root, char* name);


//插入
int rb_insert(struct rb_root *root, Entry e);

//删除
void rb_delete(struct rb_root *root, char* name);

//输出
void print_rbtree(struct rb_root *tree);



Type exp_handler(Node* node);

Type struct_spcifier_handler(Node* node);
Type specifer_handler(Node* node);

void semantic(Node* node, int level);

void semantic_scan(Node* node, int level);




#endif
