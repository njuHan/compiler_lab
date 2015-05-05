#ifndef _SEMANTIC_H
#define _SEMANTIC_H

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;


struct Type_
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
};

struct FieldList_
{
	char* name; //域的名字
	Type type;  //域的类型
	FieldList tail; //下一个域
};
	
	
}







#endif
