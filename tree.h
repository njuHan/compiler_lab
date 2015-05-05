
#ifndef _TREE_H_
#define _TREE_H_

#define TYPE_INT 0
#define TYPE_FLOAT 1
#define TYPE_STR 2
#define NO_TYPE 3 
#define LINE_NO 4
/*
typedef struct YYLTYPE
{
	int first_line;
	int first_column;
	int last_line;
	int last_column;
}YYLTYPE;

YYLTYPE yyloc;
*/
typedef struct Value
{
	char* name;
	int lineno;
	int flag; //用于标志union时什么类型，便于输出
	union
	{
		int type_int;
		float type_float;
		char* type_str;
	};
}Value;

typedef struct Node //语法树的节点
{
	Value value;
	int level;
	struct Node* child;
	struct Node* sibling;
}Node;

Node* new_node(char* name, int flag, char* text);

Node* new_tree(int arity, ...);

Node* set_value(Node* p, char* name, int lineno);

Node* root;

int errcount;


#endif
