#ifndef _IR_H
#define _IR_H
#include <stdio.h>
#include <stdlib.h>
#include "semantic.h"

typedef enum { ASSIGN, ADD, SUB, MUL, DIV, RETURN, GOTO, CALL, DEC, ARG, RELOP, READ, WRITE, IRLABEL ,IRFUNC, IRPARAM } ir_type;
typedef enum { VARIABLE, TEMP, CONSTANT, REFERENCE, ADDRESS, OPLABEL, OPFUNC, OPPARAM } op_type;
typedef enum { EQ, NE, MT, LT, ME, LE } rel_type;

typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;
typedef struct InterCodes_* InterCodes;

struct Operand_
{
	op_type kind;
	int flag;
	union
	{
		int var_no;
		int value;
		int temp_no;
		int label_no;
		int addr_no;
		char* func_name;
		char* param_name;
	} u;
	
};


struct InterCode_
{
	ir_type kind;
	union
	{
		struct {Operand right, left; } assign;
		struct {Operand result, op1, op2;} binop;
		struct {Operand op; } uniop;
		struct {Operand label, relop1, relop2; rel_type reltype; }cond;
	}u;
	
};

//双向链表
struct InterCodes_
{
	InterCode code;
	struct InterCodes_* prev, *next; 
};

//文件指针
FILE* fp;

//中间代码
InterCodes ir_list;

//输出
void print_operand(Operand op);
void print_reltype(rel_type type);
void print_ir(InterCodes ir, char* filename);

Operand new_variable(int id);
Operand new_temp();
Operand new_temp_id(int id);
Operand new_constant(int value);
Operand new_reference(int id);
Operand new_label();
Operand new_function(char *func_name);
Operand new_param(char *param_name);

InterCodes handler_assign( Operand left, Operand right, ir_type kind);
InterCodes handler_binop(Operand result, Operand op1, Operand op2, ir_type kind);
InterCodes handler_uniop(Operand op, ir_type kind);
InterCodes hander_cond(Operand label, Operand op1, Operand op2, rel_type type);


InterCodes translate_exp(Node* node, Operand place);
InterCodes translate_stmt(Node* node);
InterCodes translate_cond(Node* node, Operand label_true, Operand label_false);
InterCodes translate_compst(Node* node);
InterCodes translate_stmtlist(Node* node);
InterCodes translate_args(Node* node, Operand *arg_list, int *arg_num);
InterCodes translate_fundec(Node* node);
InterCodes translate_vardec(Node* node, Operand *place);
InterCodes translate_deflist(Node* node);
InterCodes translate_def(Node* node);
InterCodes translate_declist(Node* node);
InterCodes translate_dec(Node* node);
InterCodes translate_structure(Node* node, Operand place, FieldList *list);
InterCodes translate_array(Node* node, Operand place, Type *list);


void gen_ir(Node* node);
void ir_scan(Node* node);

#endif
