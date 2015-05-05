#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>



Node* new_tree(int arity, ...)
{
	Node* parent = (Node*)malloc(sizeof(Node));
	parent->value.flag = LINE_NO; //LINE_NO表示输出该节点时，打印行号	
	parent->child = NULL;
	parent->sibling = NULL;
	parent->level = 0;
	

	va_list arg_ptr;
	va_start(arg_ptr, arity);
	int i;
	Node* next = NULL;
	for (i = 0; i<arity; i++ )
	{
		next = va_arg(arg_ptr, Node*);
		if (parent==NULL || next==NULL)
		{
			return parent;
		}
		else if (parent->child == NULL)
		{
			parent->child = next;
		}
		else
		{
			Node* p = parent->child;
			Node* q = p->sibling;
			while(q!=NULL)
			{
				p = q;
				q = q->sibling;
			}
			p->sibling = next;
		}
		
	}
	va_end(arg_ptr);
	return parent;
}



Node* set_value(Node* p, char* name, int lineno)
{
	p->value.name = name;
	p->value.lineno = lineno;
}

/*
#define TYPE_INT 0
#define TYPE_FLOAT 1
#define TYPE_STR 2
#define NO_TYPE 3
#define LINE_NO 4
*/

//根据节点的层数，输出节点
void output_node(Node* p, int level)
{
	int i=0;
	
	if(strcmp(p->value.name,"null")!=0)  //节点内容不为空
	{	
		for (; i<level; i++) //打印缩进
			printf(" ");
		
		printf("%s", p->value.name);
	}
	else  //该节点在归约时，被替换为空串，所以不输出
	{
		return;
	}
	switch(p->value.flag)
	{
	case LINE_NO:
		printf(" (%d)\n", p->value.lineno);
	break;

	case TYPE_INT:
		printf(": %d\n", p->value.type_int);
	break;

	case TYPE_FLOAT:
		printf(": %f\n", p->value.type_float);
	break;

	case TYPE_STR:
		printf(": %s\n", p->value.type_str);
	break;
	default: printf("\n");
	}
	
}

//递归前序遍历
void output_tree(Node* root, int level)
{	
	if (errcount>0)
	{
		printf("------Error count: %d------\n", errcount);
		return;
	}
	output_node(root,level);
	if (root->child!=NULL)
		output_tree(root->child, level+1);
	if (root->sibling!=NULL)
		output_tree(root->sibling, level);
}







