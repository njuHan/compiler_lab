#include "ir.h"
#include <assert.h>

int label_no = 1;
int temp_no = 1;
char* current_function;

void print_operand(Operand op)
{
	if (op!=NULL)
	{
		//typedef enum { VARIABLE, TEMP, CONSTANT, REFERENCE, ADDRESS, OPLABEL, OPFUNC, OPPARAM } op_type;
		switch(op->kind)
		{
		case VARIABLE:
			fprintf(fp, "v%d", op->u.var_no);
		break;
		
		case TEMP:
			fprintf(fp, "t%d", op->u.temp_no);
		break;
		
		case CONSTANT:
			fprintf(fp, "#%d", op->u.value);
		break;
		
		case REFERENCE:
			fprintf(fp, "&v%d", op->u.var_no);
		break;
		
		case ADDRESS:	
			fprintf(fp, "*t%d", op->u.temp_no);
		break;
		
		case OPLABEL:
			fprintf(fp, "label%d", op->u.label_no);
		break;
		
		case OPFUNC:
			fprintf(fp, "%s", op->u.func_name);
		break;
		
		case OPPARAM:
			fprintf(fp, "%s", op->u.param_name);
		break;
				
		}
			
	}
	return ;
}

void print_reltype(rel_type type)
{
	switch(type)
	{
	case EQ: 
		fprintf(fp, " == ");
	break;
	
	case NE:
		fprintf(fp, " != ");
	break;
	
	case MT:
		fprintf(fp, " > ");
	break;
	
	case LT:
		fprintf(fp, " < ");
	break;
	
	case ME:
		fprintf(fp, " >= ");
	break;
	
	case LE:
		fprintf(fp, " <= ");
	break;
	}
}
void print_ir(InterCodes ir)
{
	InterCodes temp = ir;
	fp = fopen("out1.ir", "w");
	while (temp != NULL)
	{
		//ASSIGN, ADD, SUB, MUL, DIV, RETURN, GOTO, CALL, DEC
		//ARG, RELOP, READ, WRITE, IRLABEL ,IRFUNC, IRPARAM
		switch(temp->code->kind)
		{
		case ASSIGN:
			print_operand(temp->code->u.assign.left);
			fprintf(fp, " := ");
			print_operand(temp->code->u.assign.right);
			fprintf(fp, "\n");
			
		break;
		
		case ADD:
			print_operand(temp->code->u.binop.result);
			fprintf(fp, " := ");
			print_operand(temp->code->u.binop.op1);
			fprintf(fp, " + ");
			print_operand(temp->code->u.binop.op2);
			fprintf(fp, "\n");
		break;
		
		case SUB:
			print_operand(temp->code->u.binop.result);
			fprintf(fp, " := ");
			print_operand(temp->code->u.binop.op1);
			fprintf(fp, " - ");
			print_operand(temp->code->u.binop.op2);
			fprintf(fp, "\n");
		break;
		
		case MUL:
			print_operand(temp->code->u.binop.result);
			fprintf(fp, " := ");
			print_operand(temp->code->u.binop.op1);
			fprintf(fp, " * ");
			print_operand(temp->code->u.binop.op2);
			fprintf(fp, "\n");
		break;
		
		case DIV:
			print_operand(temp->code->u.binop.result);
			fprintf(fp, " := ");
			print_operand(temp->code->u.binop.op1);
			fprintf(fp, " / ");
			print_operand(temp->code->u.binop.op2);
			fprintf(fp, "\n");
		break;
		
		case RETURN:
			fprintf(fp, "RETURN ");
			print_operand(temp->code->u.uniop.op);
			fprintf(fp, "\n");
		break;
		
		case GOTO:
			fprintf(fp, "GOTO ");
			print_operand(temp->code->u.uniop.op);
			fprintf(fp, "\n");
		break;
		
		case CALL: 
			print_operand(temp->code->u.assign.left);
			fprintf(fp, " := CALL ");
			print_operand(temp->code->u.assign.right);
			fprintf(fp, "\n");
		break;
		
		case DEC:
			fprintf(fp, "DEC ");
			print_operand(temp->code->u.assign.left);
			fprintf(fp, " %d", temp->code->u.assign.right->u.value);
			fprintf(fp, "\n");
		break;
		
		case ARG:
			fprintf(fp, "ARG ");
			if (temp->code->u.uniop.op->flag != 1)
				print_operand(temp->code->u.uniop.op);
			else
				fprintf(fp, "t%d", temp->code->u.uniop.op->u.temp_no);
			fprintf(fp, "\n");
		break;
		
		case RELOP:
			fprintf(fp, "IF ");
			print_operand(temp->code->u.cond.relop1);
			print_reltype(temp->code->u.cond.reltype);
			print_operand(temp->code->u.cond.relop2);
			fprintf(fp, " GOTO ");
			print_operand(temp->code->u.cond.label);
			fprintf(fp, "\n");
		break;
		
		case READ:
			fprintf(fp, "READ ");
			print_operand(temp->code->u.uniop.op);
			fprintf(fp, "\n");
		break;
		
		case WRITE:
			fprintf(fp, "WRITE ");
			print_operand(temp->code->u.uniop.op);
			fprintf(fp, "\n");
		break;
		
		case IRLABEL:
			fprintf(fp, "LABEL ");
			print_operand(temp->code->u.uniop.op);
			fprintf(fp, " :\n");
		break;
		
		case IRFUNC:
			fprintf(fp, "FUNCTION ");
			print_operand(temp->code->u.uniop.op);
			fprintf(fp, " :\n");
		break;
		
		case IRPARAM:
			fprintf(fp, "PARAM ");
			print_operand(temp->code->u.uniop.op);
			fprintf(fp, "\n");
		break;
		
		default :
			assert(0);
		}
		temp = temp->next;
	}
}


Operand new_variable(int id)
{
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	op->kind = VARIABLE;
	op->flag = 0;
	op->u.var_no = id;
	return op;
}
Operand new_temp()
{
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	op->kind = TEMP;
	op->flag = 0;
	op->u.temp_no = temp_no++ ;
	return op;
}
Operand new_temp_id(int id)
{
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	op->kind = TEMP;
	op->flag = 0;
	op->u.temp_no = id;
	return op;
}
Operand new_constant(int value)
{
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	op->flag = 0;
	op->kind = CONSTANT;
	op->u.value = value;
	return op;
}
Operand new_reference(int id)
{
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	op->kind = REFERENCE;
	op->flag = 0;
	op->u.addr_no = id;
	return op;
}
Operand new_label()
{
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	op->kind = OPLABEL;
	op->flag = 0;
	op->u.label_no = label_no++;
	return op;
}
Operand new_function(char *func_name)
{
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	op->kind = OPFUNC;
	op->flag = 0;
	op->u.func_name = func_name;
	return op;
}
Operand new_param(char *param_name)
{
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	op->kind = OPPARAM;
	op->flag = 0;
	op->u.param_name = param_name;
	return op;
}

InterCodes handler_assign( Operand left, Operand right, ir_type kind)
{
	InterCodes codes = (InterCodes)malloc(sizeof(struct InterCodes_));
	codes->code = (InterCode)malloc(sizeof(struct InterCode_));
	codes->code->u.assign.left = left;
	codes->code->u.assign.right = right;
	codes->code->kind = kind;
	codes->prev = NULL;
	codes->next = NULL;
	return codes;
}
InterCodes handler_binop( Operand result, Operand op1, Operand op2, ir_type kind)
{
	InterCodes codes = (InterCodes)malloc(sizeof(struct InterCodes_));
	codes->code = (InterCode)malloc(sizeof(struct InterCode_));
	codes->code->u.binop.result = result;
	codes->code->u.binop.op1 = op1;
	codes->code->u.binop.op2 = op2;
	codes->code->kind = kind;
	codes->prev = NULL;
	codes->next = NULL;
	return codes;
}

InterCodes handler_uniop(Operand op, ir_type kind)
{
	InterCodes codes = (InterCodes)malloc(sizeof(struct InterCodes_));
	codes->code = (InterCode)malloc(sizeof(struct InterCode_));
	codes->code->u.uniop.op = op;
	codes->code->kind = kind;
	
	return codes;
}
InterCodes handler_cond( Operand label, Operand op1, Operand op2, rel_type type)
{
	InterCodes codes = (InterCodes)malloc(sizeof(struct InterCodes_));
	codes->code = (InterCode)malloc(sizeof(struct InterCode_));
	codes->code->u.cond.label = label;
	codes->code->u.cond.relop1 = op1;
	codes->code->u.cond.relop2 = op2;
	codes->code->u.cond.reltype = type;
	codes->code->kind = RELOP;
	codes->prev = NULL;
	codes->next = NULL;
	return codes;
}

InterCodes link_code(InterCodes c1, InterCodes c2)
{	
	if (c1!=NULL && c2!= NULL)
	{
		InterCodes temp = c1;
		while (temp->next!=NULL)
			temp = temp->next;
		temp->next = c2;
		c2->prev = temp;
		return c1;
	}
	else if (c1==NULL)
		return c2;
	else
		return c1;
	
}

rel_type get_relop(Node* node)
{
	char* relop = node->value.type_str;
	
	switch(relop[0])
	{
	case '=':
		return EQ;
	break;
	
	case '!':
		return NE;
	break;
	
	case '>':
		if (relop[1]=='=')
			return ME;
		else
			return MT;
	break;
	
	case '<':
		if (relop[1]=='=')
			return LE;
		else
			return LT;
	break;
	
	default: printf("RELOP ERROR!\n"); assert(0);
	}
}

InterCodes translate_exp(Node* node, Operand place)
{
	Node* child = node->child;
	
	if (strcmp(child->value.name, "INT")==0)
	{
		Operand c1 = new_constant(child->value.type_int);
		return handler_assign(place, c1, ASSIGN);
	}
	else if (strcmp(child->value.name, "ID")==0 && child->sibling == NULL)
	{
		
		Entry e = stack_search(var_table, child->value.type_str);
		Operand right;
		
		if(e==NULL)
		{
			//在函数的参数表中查找
			FieldList temp = search_param_func(child->value.type_str, current_function);
			assert(temp!=NULL);
			right = new_variable(temp->id);
		}
		else
		{
			right = new_variable(e->field->id);
		}
		
		return handler_assign(place, right, ASSIGN);
		
	}
	else if (strcmp(child->value.name, "Exp")==0 && strcmp(child->sibling->value.name, "ASSIGNOP")==0)
	{
		//child exp->id
		if (strcmp(child->child->value.name, "ID")==0)
		{
			Operand t1 = new_temp();
			char* var_name = child->child->value.type_str;
			
			Entry e = stack_search(var_table, var_name);
			//assert(e!=NULL);
			FieldList temp;
			if(e==NULL)
			{
				//在函数的参数表中查找
				temp = search_param_func(var_name, current_function);
				
			}
			else
			{
				temp = e->field;
			}
			
			Operand t2 = new_variable(temp->id);
			InterCodes code1 = translate_exp(child->sibling->sibling, t1);
			InterCodes code2 = handler_assign(t2, t1, ASSIGN);
			code1 = link_code(code1, code2);
			if (place!=NULL)
			{
				InterCodes code3 = handler_assign(place, t2, ASSIGN);
				code1 = link_code(code1, code3);
			}
			return code1;
		}
		
		//child exp->exp lb exp rb
		//child exp->exp dot id
		Operand t1 = new_temp();
		InterCodes code1, code2, code3 ,code4;
		code1 = translate_exp(child, t1);
		Operand t2 = new_temp();
		code2 = translate_exp(child->sibling->sibling, t2);
		code3 = handler_assign(t1, t2, ASSIGN);
		code1 = link_code(link_code(code1,code2),code3);
		if (place != NULL)
		{
			code4 = handler_assign(place, t1, ASSIGN);
			code1 = link_code(code1, code4);
		}
		return code1;
		
		
		
	}
	else if(strcmp(child->value.name, "Exp") == 0
		&& (strcmp(child->sibling->value.name, "PLUS") == 0 
		 || strcmp(child->sibling->value.name, "MINUS") == 0 
		 || strcmp(child->sibling->value.name, "STAR") == 0 
		 || strcmp(child->sibling->value.name, "DIV") == 0))
	{
		Operand t1 = new_temp();
		Operand t2 = new_temp();
		InterCodes code1 = translate_exp(child, t1);
		InterCodes code2 = translate_exp(child->sibling->sibling, t2);
		InterCodes code3;
		if(strcmp(child->sibling->value.name, "PLUS") == 0)
			code3 = handler_binop( place, t1, t2, ADD);
		
		else if(strcmp(child->sibling->value.name, "DIV") == 0)
			code3 = handler_binop(place, t1, t2, DIV);
		
		else if(strcmp(child->sibling->value.name, "MINUS") == 0)
			code3 = handler_binop(place, t1, t2, SUB);
		
		else if(strcmp(child->sibling->value.name, "STAR") == 0)
			code3 = handler_binop(place, t1, t2, MUL);
		
		return link_code(link_code(code1,code2),code3);
	}
	
	else if (strcmp(child->value.name, "MINUS")==0)
	{
		Operand t1 = new_temp();
		Operand t2 = new_constant(0);
		InterCodes code1 = translate_exp(child->sibling, t1);
		InterCodes code2 = handler_binop(place, t2, t1, SUB);
		return link_code(code1, code2);
	}
	else if(strcmp(child->sibling->value.name, "RELOP") == 0 
		||strcmp(child->value.name, "NOT") == 0 
		||strcmp(child->sibling->value.name, "AND") == 0 
		||strcmp(child->sibling->value.name, "OR") == 0)
	{
		Operand label1 = new_label();
		Operand label2 = new_label();
		Operand c0 = new_constant(0);
		Operand c1 = new_constant(1);
		InterCodes code0 = handler_assign(place, c0, ASSIGN);
		InterCodes code1 = translate_cond(node, label1, label2);
		InterCodes code2 = handler_uniop(label1, IRLABEL);
		InterCodes code3 = handler_assign(place, c1, ASSIGN);
		InterCodes code4 = handler_uniop(label2, IRLABEL);
		
		return link_code(link_code(link_code(link_code(code0,code1),code2),code3),code4);
		
	}
	else if (strcmp(child->sibling->value.name, "LP")==0 
		&& strcmp(child->sibling->sibling->value.name, "RP")==0)
	{
		char* func_name = child->value.type_str;
		
		if (strcmp(func_name, "read")==0)
		{
			
			InterCodes code;
			if (place!=NULL)
			{
				code = handler_uniop(place, READ);
			}
			else
			{
				Operand op = new_temp();
				code = handler_uniop(op, READ);
			}
			
			return code;
		}
		
		FieldList func = search_func(func_name);
		assert(func!=NULL);
		InterCodes code1, code2;
		Operand f1 = new_function(func_name);
		if (place != NULL && place->kind == ADDRESS)
		{
			Operand t2 = new_temp();
			code1 = handler_assign(t2, f1, CALL);
			code2 = handler_assign(place, t2, ASSIGN);
			code1 = link_code(code1, code2);
		}
		else if (place != NULL && place->kind != ADDRESS)
		{
			code1 = handler_assign(place, f1, CALL);
		}
		else
		{
			Operand t2 = new_temp();
			code1 = handler_assign(t2,f1,CALL);
		}
		
		return code1;
	}
	
	else if(strcmp(child->sibling->value.name, "LP") == 0 
		&& strcmp(child->sibling->sibling->value.name, "Args") == 0)
	{
		Operand* arg_list = malloc(sizeof(Operand)*16);
		int arg_num = 0;
		char*  func_name = child->value.type_str;
		InterCodes code1, code2, code3;
		code1 = translate_args(child->sibling->sibling, arg_list, &arg_num);
		if (strcmp(func_name, "write") != 0)
		{
			int i;
			for(i=0; i<arg_num; i++)
			{
				code2 = handler_uniop(arg_list[i], ARG);
				code1 = link_code(code1, code2);
			}
			Operand f1 = new_function(func_name);
			
			
			if(place != NULL && place->kind == ADDRESS){
				Operand t2 = new_temp();
				code1 = handler_assign(t2, f1, CALL);
				code2 = handler_assign(place, t2, ASSIGN);
				code1 = link_code(code1, code2);
				return code1;
			}
			else if(place != NULL && place->kind != ADDRESS)
			{
				code3 = handler_assign(place, f1, CALL);
				return link_code(code1, code3);
			}
			
			else
			{
				Operand t2 = new_temp();
				code3 = handler_assign(t2, f1, CALL);
				return link_code(code1, code3);
			}
			
			
		}
		else
		{
			Operand t1;
			if (arg_list[0]->kind == ADDRESS)
			{
				t1 = new_temp();
				code2 = handler_assign(t1, arg_list[0], ASSIGN);
				code3 = handler_uniop(t1, WRITE);
				return link_code(link_code(code1, code2),code3);
			}
			else
			{
				t1 = arg_list[0];
				code3 = handler_uniop(t1, WRITE);
				return link_code(code1, code3);
			}
		}
	}
	//lp exp rp
	else if (strcmp(child->value.name, "LP")==0)
	{
		return translate_exp(child->sibling, place);
	}
	//exp lb exp rb
	else if (child->sibling!=NULL && strcmp(child->sibling->value.name, "LB")==0)
	{
		return translate_array(node, place, NULL);
	}
	//exp dot id
	else if (child->sibling!=NULL && strcmp(child->sibling->value.name,"DOT")==0)
	{
		return translate_structure(node, place, NULL);
	}
	else
	{
		printf("%s\n",child->value.name);
		if (child->sibling)
			printf("%s\n%s\n", child->sibling->value.name,child->sibling->sibling->value.name);
		assert(0);
	}

}
InterCodes translate_stmt(Node* node)
{
	Node* child = node->child;
	
	
	if (strcmp(child->value.name, "Exp")==0)
	{
		return translate_exp(child, NULL);
	}
	else if (strcmp(child->value.name, "CompSt")==0)
	{
		return translate_compst(child);
	}
	//RETURN EXP SEMI
	else if (strcmp(child->value.name, "RETURN")==0)
	{
		Operand t1 = new_temp();
		InterCodes code1, code2;
		code1 = translate_exp(child->sibling, t1);
		code2 = handler_uniop(t1, RETURN);
		return link_code(code1, code2);
	}
	else if ( strcmp(child->value.name, "IF")==0 )
	{
		Node* node_else = child->sibling->sibling->sibling->sibling->sibling;
		
		//IF LP EXP RP STMT
		if (node_else==NULL)
		{
			Operand label1, label2;
			InterCodes code1,code2,code3,code4;
			label1 = new_label();
			label2 = new_label();
			code1 = translate_cond(child->sibling->sibling, label1, label2);
			code2 = handler_uniop(label1, IRLABEL);
			code3 = translate_stmt(child->sibling->sibling->sibling->sibling);
			code4 = handler_uniop(label2, IRLABEL);
			return link_code(link_code(link_code(code1,code2),code3),code4);
			
		}
		//IF LP EXP RP STMT ELSE STMT
		else if (strcmp(node_else->value.name, "ELSE")==0)
		{
			
			Operand label1, label2, label3;
			InterCodes code1, code2, code3, code4, code5, code6, code7;
			label1 = new_label();
			label2 = new_label();
			label3 = new_label();
			
			Node* exp_temp = child->sibling->sibling;
			
			code1 = translate_cond(exp_temp, label1, label2);
			
			code2 = translate_stmt(exp_temp->sibling->sibling);
			
			code3 = translate_stmt(exp_temp->sibling->sibling->sibling->sibling);
			
			code4 = handler_uniop(label1, IRLABEL);
			code5 = handler_uniop(label3, GOTO);
			code6 = handler_uniop(label2, IRLABEL);
			code7 = handler_uniop(label3, IRLABEL);
			
			//1,4,2,5,6,3,7
			return link_code(link_code(link_code(link_code(link_code(link_code(code1,code4),code2),code5),code6),code3),code7);
		
		}
		else
		{
			assert(0);
		}
	}
	//WHILE LP EXP RP STMT
	else if (strcmp(child->value.name, "WHILE")==0)
	{
		Operand label1, label2, label3;
		InterCodes code1, code2, code3, code4, code5, code6;
		Node* exp_node = child->sibling->sibling;
		assert(strcmp(exp_node->value.name, "Exp")==0);
		code1 = translate_cond(exp_node, label2, label3);
		code2 = translate_stmt(exp_node->sibling->sibling);
		code3 = handler_uniop(label1, IRLABEL);
		code4 = handler_uniop(label2, IRLABEL);
		code5 = handler_uniop(label1, GOTO);
		code6 = handler_uniop(label3, IRLABEL);
		//3,1,4,2,5,6
		return link_code(link_code(link_code(link_code(link_code(code3,code1),code4),code2),code5),code6);
		
	}
	else	
	{
		assert(0);
	}
		
}


InterCodes translate_cond(Node* node, Operand label_true, Operand label_false)
{
	
	Node* child = node->child;
	//EXP1 RELOP EXP2
	
	if (child->sibling!=NULL && strcmp(child->sibling->value.name, "RELOP")==0)
	{
		
		Operand t1, t2;
		InterCodes code1, code2, code3, code4;
		t1 = new_temp();
		t2 = new_temp();
		code1 = translate_exp(child, t1);
		code2 = translate_exp(child->sibling->sibling, t2);
		
		rel_type rel = get_relop(child->sibling);
		
		code3 = handler_cond(label_true, t1, t2, rel);
		
		code4 = handler_uniop(label_false, GOTO);
		
		return link_code(link_code(link_code(code1, code2),code3),code4);
		
	}
	//NOT EXP1
	else if (strcmp(child->value.name, "NOT")==0)
	{
		return translate_cond(child->sibling, label_false, label_true);
	}
	//EXP1 AND EXP2
	else if (child->sibling!=NULL && strcmp(child->sibling->value.name, "AND")==0)
	{
		Operand label1 = new_label();
		InterCodes code1, code2, code3;
		code1 = translate_cond(child, label1, label_false);
		code2 = translate_cond(child->sibling->sibling, label_true, label_false);
		code3 = handler_uniop(label1, IRLABEL);
		return link_code(link_code(code1,code3),code2);
	}
	//EXP1 OR EXP2
	else if (child->sibling!=NULL && strcmp(child->sibling->value.name, "OR")==0)
	{
		Operand label1 = new_label();
		InterCodes code1, code2, code3;
		code1 = translate_cond(child, label_true, label1);
		code2 = translate_cond(child->sibling->sibling, label_true, label_false);
		code3 = handler_uniop(label1, IRLABEL);
		return link_code(link_code(code1,code3),code2);
	}
	//other cases
	else
	{
		Operand t1 = new_temp();
		InterCodes code1, code2, code3;
		code1 = translate_exp(node, t1);
		code2 = handler_cond(label_true, t1, new_constant(0), NE);
		code3 = handler_uniop(label_false, GOTO);
		return link_code(link_code(code1, code2),code3);
	}
		
		
}
InterCodes translate_args(Node* node, Operand *arg_list, int *arg_num)
{
	Node* child = node->child;
	assert(strcmp(child->value.name, "Exp")==0);
	
	//exp comma args
	if (child->sibling != NULL )
	{
		InterCodes code1, code2;
		assert(strcmp(child->sibling->value.name, "COMMA")==0);
		if (strcmp(child->child->value.name, "ID")==0)
		{
			Entry e = stack_search(var_table, child->child->value.type_str);
			
			
			FieldList temp;
			if(e==NULL)
			{
				//在函数的参数表中查找
				temp = search_param_func(child->child->value.type_str, current_function);
				
			}
			else
			{
				temp = e->field;
			}
			
			if (temp->type->kind ==ARRAY || temp->type->kind ==STRUCTURE)
			{
				Operand r1 = new_reference(temp->id);
				Operand t1 = new_temp();
				code1 = handler_assign(t1,r1, ASSIGN);
				t1->flag = 1;
				arg_list[(*arg_num)++] = t1;
			}
			else
			{
				Operand t1 = new_temp();
				code1 = translate_exp(child, t1);
				arg_list[(*arg_num)++] = t1;
			}
		}
		else
		{
			Operand t1 = new_temp();
			code1 = translate_exp(child, t1);
			arg_list[(*arg_num)++] = t1;
		}
		code2 = translate_args(child->sibling->sibling, arg_list, arg_num);
		return link_code(code1, code2);
	}
	// exp
	else 
	{
		InterCodes code1;
		if (strcmp(child->child->value.name, "ID")==0 && child->child->sibling == NULL)
		{
			Entry e = stack_search(var_table, child->child->value.type_str);
			
			FieldList temp;
			if(e==NULL)
			{
				//在函数的参数表中查找
				temp = search_param_func(child->child->value.type_str, current_function);
				
			}
			else
			{
				temp = e->field;
			}
			
			
			if (temp->type->kind ==ARRAY || temp->type->kind ==STRUCTURE)
			{
				Operand r1 = new_reference(temp->id);
				Operand t1 = new_temp();
				code1 = handler_assign(t1,r1, ASSIGN);
				t1->flag = 1;
				arg_list[(*arg_num)++] = t1;
				return code1;
			}
		}
		Operand t1 = new_temp();
		code1 = translate_exp(child, t1);
		if(t1->kind == ADDRESS && t1->flag!=2)
		{
			t1->flag = 1;
		}
		arg_list[(*arg_num)++] = t1;
		return code1;
	}
}




InterCodes translate_compst(Node* node)
{
	assert(strcmp(node->value.name, "CompSt")==0);
	Node* deflist = node->child->sibling;
	Node* stmtlist = deflist->sibling;
	InterCodes code1, code2;
	code1 = translate_deflist(deflist);
	
	code2 = translate_stmtlist(stmtlist);
	return link_code(code1, code2);
}
InterCodes translate_stmtlist(Node* node)
{
	if (node==NULL)
		return NULL;
	if (strcmp(node->value.name, "null")==0)
		return NULL;
	Node* child = node->child;
	InterCodes code1, code2, code3;
	code1 = translate_stmt(child);
	code2 = translate_stmtlist(child->sibling);
	
	return link_code(code1, code2);
}

InterCodes translate_fundec(Node* node)
{
	Node* child = node->child;
	char* func_name = child->value.type_str;
	current_function = func_name;
	FieldList func = search_func(func_name);
	assert(func!=NULL);
	Operand f1 = new_function(func_name);
	InterCodes code1, code2;
	code1 = handler_uniop(f1, IRFUNC);
	//ID LP VARLIST RP
	if (child->sibling->sibling->sibling!=NULL)
	{
		FieldList param = func->type->u.func.param;
		while(param!=NULL)
		{
			if(param->type->kind == ARRAY)
			{
				printf("Param can not be array\n");
				assert(0);
			}
			Operand p1 = new_variable(param->id);
			code2 = handler_uniop(p1, IRPARAM);
			code1 = link_code(code1, code2);
			param = param->tail;
		}
		return code1;
	}
	else
	{
		return code1;
	}
		
}


InterCodes translate_extdef(Node* node) {
	InterCodes code1, code2;
	Node* child = node->child;
	assert(child != NULL);
	//Specifier FunDec CompSt
	if(strcmp(child->sibling->value.name, "FunDec") == 0){
		code1 = translate_fundec(child->sibling);
		
		code2 = translate_compst(child->sibling->sibling);
		
		return link_code(code1, code2);
	}
	/*
	else
	{
		printf("ERROR! program has global variale!\n");
		assert(0);
	}
	*/
	return NULL;
}



int get_structure_size(Type type) {
	FieldList member;
	int size = 0;
	switch(type->kind)
	{
	case BASIC:
		size = 4;
		break;
	case ARRAY:
		size = get_structure_size(type->u.array.elem);
		size =  size * type->u.array.size;
		break;
	case STRUCTURE:
		member = type->u.structure;
		while(member != NULL)
		{
			size += get_structure_size(member->type);
			member = member->tail;
		}
		break;
	}
	return size;
}

InterCodes translate_vardec(Node* node, Operand *place)
{
	Node* child = node->child;
	if (strcmp(child->value.name, "ID")==0)
	{
		InterCodes code1;
		Entry e = stack_search(var_table, child->value.type_str);
		Operand v1 = new_variable(e->field->id);
		if (place!=NULL)
			*place = v1;
		if(e->field->type->kind == BASIC)
		{
			return NULL;
		}
		else if(e->field->type->kind == ARRAY || e->field->type->kind == STRUCTURE)
		{
			int size = get_structure_size(e->field->type);
			Operand c1 = new_constant(size);
			code1 = handler_assign(v1, c1, DEC);
			return code1;
		}
	}
	return translate_vardec(child, NULL);
}

InterCodes translate_deflist(Node* node)
{
	if (node!=NULL && strcmp(node->value.name,"null")!=0)
	{
		InterCodes code1, code2;
		Node* child = node->child;
		code1 = translate_def(child);
		code2 = translate_deflist(child->sibling);
		return link_code(code1, code2);
	}
	return NULL;
	
}
InterCodes translate_def(Node* node)
{
	Node* child = node->child;
	assert(strcmp(child->sibling->value.name, "DecList")==0);
	return translate_declist(child->sibling);
}

InterCodes translate_declist(Node* node)
{
	Node* child = node->child;
	//dec
	if (child->sibling ==NULL)
	{
		return translate_dec(child);
	}
	//dec comma declist
	else
	{
		InterCodes code1, code2;
		code1 = translate_dec(child);
		code2 = translate_declist(child->sibling->sibling);
		return link_code(code1, code2);
	}
}
InterCodes translate_dec(Node* node)
{
	Node* child = node->child;
	if (child->sibling==NULL)
	{
		return translate_vardec(child, NULL);
	}
	else
	{
		Operand t1 = NULL;
		InterCodes code1, code2, code3;
		code1 = translate_vardec(child, &t1);
		Operand t2 = new_temp();
		code2 = translate_exp(child->sibling->sibling, t2);
		code3 = handler_assign(t1, t2, ASSIGN);
		return link_code(link_code(code1, code2),code3);
	}
}


InterCodes translate_structure(Node* node, Operand place, FieldList *list)
{
	Node* child = node->child;
	int offset = 0;
	Type id_type;
	//exp dot id
	//exp->id
	int param_flag = 0;
	if (strcmp(child->child->value.name, "ID")==0)
	{
		Entry e = stack_search(var_table, child->child->value.type_str);
		FieldList temp;
		if(e==NULL)
		{
			param_flag = 1;
			//在函数的参数表中查找
			temp = search_param_func(child->child->value.type_str, current_function);
		}
		else
		{
			temp = e->field;
		}
		assert(temp!=NULL);
		char* member_name = child->sibling->sibling->value.type_str;
		FieldList member = temp->type->u.structure;
		while(member!=NULL)
		{
			if (strcmp(member->name, member_name)==0)
				break;
			offset += get_structure_size(member->type);
			member = member->tail;
		}
		assert(member!=NULL);
		if (list != NULL)
			*list = member;
		Operand r1;
		
		if ( (temp->type->kind == STRUCTURE || temp->type->kind == STRUCTURE) && param_flag==1)
			//r1 = new_reference(temp->id);
			r1 = new_variable(temp->id);
		else
			r1 = new_reference(temp->id);
			
		
		Operand c1 = new_constant(offset);
		Operand t1 = new_temp_id(place->u.temp_no);
		InterCodes code1 = handler_binop(t1,r1,c1, ADD);
		place->kind = ADDRESS;
		return code1;
	}
	//exp lb exp rb
	else if (strcmp(child->child->sibling->value.name, "LB")==0)
	{
		Operand t1 = new_temp();
		InterCodes code1 ,code2;
		code1 = translate_array(child, t1, &id_type);
		char* member_name = child->sibling->sibling->value.type_str;
		FieldList member = id_type->u.structure;
		while(member != NULL)
		{
			if (strcmp(member->name, member_name)==0)
				break;
			offset += get_structure_size(member->type);
			member = member->tail;
		}
		if (list != NULL)
			*list = member;
		Operand c1 = new_constant(offset);
		Operand t2 = new_temp_id(place->u.temp_no);
		Operand t3 = new_temp_id(t1->u.temp_no);
		code2 = handler_binop(t2,t3,c1,ADD);
		code1 = link_code(code1, code2);
		
	}
	//exp.id.id
	else if (strcmp(child->child->sibling->value.name, "DOT")==0)
	{
		FieldList id_node;
		InterCodes code1, code2;
		Operand t1 = new_temp();
		code1 = translate_structure(child, t1, &id_node);
		char* member_name = child->sibling->sibling->value.type_str;
		FieldList member = id_node->type->u.structure;
		while(member != NULL)
		{
			if (strcmp(member->name, member_name)==0)
			{
				break;
			}
			offset += get_structure_size(member->type);
			member = member->tail;
		}
		if (list != NULL)
			*list = member;
		Operand c1 = new_constant(offset);
		Operand t2 = new_temp_id(place->u.temp_no);
		Operand t3 = new_temp_id(t1->u.temp_no);
		code2 = handler_binop(t2,t3,c1,ADD);
		code1 = link_code(code1, code2);
		place->kind = ADDRESS;
		return code1;
	}
}
InterCodes translate_array(Node* node, Operand place, Type *list)
{
	Node* child = node->child;
	
	int size = 0;
	
	InterCodes code1, code2, code3, code4;
	code1 = NULL;
	Operand t1;
	FieldList temp = NULL;
	//ID[Exp]
	int param_flag = 0;
	if (strcmp(child->child->value.name, "ID")==0)
	{
		Entry e = stack_search(var_table, child->child->value.type_str);
		
		if(e==NULL)
		{
			param_flag = 1;
			//在函数的参数表中查找
			temp = search_param_func(child->child->value.type_str, current_function);
		}
		else
		{
			temp = e->field;
		}
		assert(temp!=NULL);
		
		if ( (temp->type->kind == STRUCTURE || temp->type->kind == STRUCTURE) && param_flag==1)
			t1 = new_variable(temp->id);
		else
			t1 = new_reference(temp->id);
		
		if(temp->type->u.array.elem->kind == STRUCTURE)
		{
			size = get_structure_size(temp->type->u.array.elem);
		}
		else if (temp->type->u.array.elem->kind == BASIC)
		{
			size = 4;
			place->flag = 2;
		}
		if (list!=NULL)
			*list = temp->type->u.array.elem;
	}
	//Exp.ID[Exp]
	else if (child->child->sibling != NULL 
		&& strcmp(child->child->sibling->value.name, "DOT") == 0)
	{
		t1 = new_temp();
		code1 = translate_structure(child, t1, &temp);
		t1->kind = TEMP;
		size = get_structure_size(temp->type->u.array.elem);
		if (size==4)
			place->flag = 2;
		if(list!=NULL)
			*list = temp->type->u.array.elem;
	}
	//exp[exp][exp]
	else if(child->child->sibling != NULL 
		&& strcmp(child->child->sibling->value.name, "LB") == 0)
	{
		printf("Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type.\n");
		exit(0);
	}
	
	Operand t2 = new_temp();
	code2 = translate_exp(child->sibling->sibling, t2);
	code1 = link_code(code1, code2);
	Operand c1 = new_constant(size);
	Operand t3 = new_temp();
	code3 = handler_binop(t3, t2, c1, MUL);
	code1 = link_code(code1, code3);
	
	Operand t4 = new_temp_id(place->u.temp_no);
	code4 = handler_binop(t4,t1,t3, ADD);
	code1 = link_code(code1, code4);
	place->kind = ADDRESS;
	return code1;
}



void gen_ir(Node* node)
{
	InterCodes code;
	//printf("%s\n",node->value.name);	
	if (strcmp(node->value.name, "ExtDef")==0)
	{
		
		code = translate_extdef(node);
		ir_list = link_code(ir_list, code);
		return ;
	}
	
}

void ir_scan(Node* node)
{
	if (errcount>0)
	{	
		printf("Program has errors!\n");
		return;
	}
	
	gen_ir(node);
	if (node->child!=NULL)
		ir_scan(node->child);
	
	if (node->sibling!=NULL)
		ir_scan(node->sibling);

}


