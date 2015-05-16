%{
#include <stdio.h>
#include "tree.h"
extern yylineno;
%}

/* declared types */
%union
{
	int type_int;
	float type_float;
	//double type_double;
	char* type_str;
	Node* type_node;
}



/*declared tokens*/
%token <type_node> INT
%token <type_node> FLOAT
%token <type_node> ID
%token <type_node> SEMI
%token <type_node> COMMA
%token <type_node> ASSIGNOP
%token <type_node> RELOP
%token <type_node> PLUS
%token <type_node> MINUS
%token <type_node> STAR
%token <type_node> DIV
%token <type_node> AND
%token <type_node> OR
%token <type_node> NOT
%token <type_node> DOT
%token <type_node> TYPE
%token <type_node> LP RP LB RB LC RC
%token <type_node> STRUCT RETURN IF ELSE WHILE

/*declared non-terminals*/
%type <type_node> Program ExtDefList ExtDef ExtDecList //high-level definitions
%type <type_node> Specifier StructSpecifier OptTag Tag //specifiers
%type <type_node> VarDec FunDec VarList ParamDec //declarators
%type <type_node> CompSt StmtList Stmt       //stataments
%type <type_node> DefList Def DecList Dec    //local definitions
%type <type_node> Exp Args  //expressions

//优先级，结合性定义
%right ASSIGNOP  
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT LB RB LP RP
%nonassoc LOWER_THAN_ELSE 
%nonassoc ELSE
%nonassoc STRUCT RETURN WHILE
%nonassoc error

%%
//high-level definitions
Program : ExtDefList
			{  
				//printf("Program -> ExtDefList\n"); 
				$$ = new_tree(1, $1);
				set_value($$, "Program", @$.first_line);
				root = $$;
				
			}
		;


ExtDefList : ExtDef ExtDefList
			{ 
				//printf("ExtDefList -> ExtDef ExtDefList\n"); 
				$$ = new_tree(2, $1, $2);
				set_value($$, "ExtDefList", @$.first_line);
			}
		|	{
				
				$$ = new_tree(1, NULL);
				set_value($$, "null", @$.first_line);
			}
		;


ExtDef : Specifier ExtDecList SEMI
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "ExtDef", @$.first_line);
				
			}
	

		| Specifier SEMI
			{
				$$ = new_tree(2, $1, $2);
				set_value($$, "ExtDef", @$.first_line);
				
			}
		
		| Specifier FunDec CompSt
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "ExtDef", @$.first_line);
				
			}
		| Specifier FunDec SEMI
			{
				printf("Error type B at Line %d: Incomplete definition of function .\n",yylineno);
				errcount++;
				
			}
	

		|  error SEMI{ printf("Error type B at Line %d: Missing \";\".\n", yylineno);errcount++;}

		
		;


ExtDecList	:	VarDec
			{
				$$ = new_tree(1, $1);
				set_value($$, "ExtDecList", @$.first_line);
				
			}
		|	VarDec COMMA ExtDecList
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "ExtDecList", @$.first_line);
				
			}
		;


//specifiers
Specifier : TYPE
			{
				$$ = new_tree(1, $1);
				set_value($$, "Specifier", @$.first_line);
			}
		|	StructSpecifier
			{
				$$ = new_tree(1, $1);
				set_value($$, "Specifier", @$.first_line);			
			}
		;
StructSpecifier	: STRUCT OptTag LC DefList RC
			{
				$$ = new_tree(5, $1, $2, $3, $4, $5);
				set_value($$, "StructSpecifier", @$.first_line);
				
			}
		|	STRUCT Tag
			{
				$$ = new_tree(2, $1, $2);
				set_value($$, "StructSpecifier", @$.first_line);
				
			}
		;
OptTag : ID
			{
				$$ = new_tree(1, $1);
				set_value($$, "OptTag", @$.first_line);
			}
		|	{
				$$ = new_tree(1, NULL);
				set_value($$, "null", @$.first_line);

			}
		;
Tag : ID
			{
				$$ = new_tree(1, $1);
				set_value($$, "Tag", @$.first_line);
			}
		;

//declarations
VarDec : ID
			{
				$$ = new_tree(1, $1);
				set_value($$, "VarDec", @$.first_line);
			}
		|	VarDec LB INT RB
			{
				$$ = new_tree(4, $1, $2, $3, $4);
				set_value($$, "VarDec", @$.first_line);
			}
		| INT ID { printf("Error type B at Line %d: Syntax error.\n", yylineno); errcount++; }
		| FLOAT ID { printf("Error type B at Line %d: Syntax error.\n", yylineno); errcount++; }
		| VarDec LB FLOAT RB  { printf("Error type B at Line %d: Syntax error.\n", yylineno); errcount++; }
		;
FunDec : ID LP VarList RP
			{
				$$ = new_tree(4, $1, $2, $3, $4);
				set_value($$, "FunDec", @$.first_line);
				
			}
		|	ID LP RP
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "FunDec", @$.first_line);
				
			}
		| INT ID LP RP { printf("Error type B at Line %d: Syntax error.\n", yylineno);errcount++;}
		|  ID LP  { printf("Error type B at Line %d: Missing \")\".\n", yylineno);errcount++;}
		|  ID RP  { printf("Error type B at Line %d: Missing \"(\".\n", yylineno);errcount++;}
		| error RP  { printf("Error type B at Line %d: Syntax error.\n", yylineno);errcount++;}
		| error LP { printf("Error type B at Line %d: Syntax error.\n", yylineno);errcount++;}
		;
VarList	: ParamDec COMMA VarList
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "VarList", @$.first_line);
			}
		|	ParamDec
			{
				$$ = new_tree(1, $1);
				set_value($$, "VarList", @$.first_line);
			}
		;
ParamDec : Specifier VarDec
			{
				$$ = new_tree(2, $1, $2);
				set_value($$, "ParamDec", @$.first_line);
			}
		;

// statements 
CompSt : LC DefList StmtList RC
			{
				$$ = new_tree(4, $1, $2, $3, $4);
				set_value($$, "CompSt", @$.first_line);
			}
		| error RC {printf("Error type B at Line %d: Missing \";\".\n", yylineno);errcount++;}
		
		;
StmtList : Stmt StmtList
			{
				$$ = new_tree(2, $1, $2);
				set_value($$, "StmtList", @$.first_line);
				
			}
		|	{
				$$ = new_tree(1, NULL);
				set_value($$, "null", @$.first_line);
				
			}
		;
Stmt : Exp SEMI
			{
				$$ = new_tree(2, $1, $2);
				set_value($$, "Stmt", @$.first_line);
				
			}
		|	CompSt
			{
				$$ = new_tree(1, $1);
				set_value($$, "Stmt", @$.first_line);
				
			}
		|	RETURN Exp SEMI
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Stmt", @$.first_line);
				
			}
		|	IF LP Exp RP Stmt	%prec LOWER_THAN_ELSE
			{
				$$ = new_tree(5, $1, $2, $3, $4, $5);
				set_value($$, "Stmt", @$.first_line);
				
			}
		|	IF LP Exp RP Stmt ELSE Stmt
			{
				$$ = new_tree(7, $1, $2, $3, $4, $5, $6, $7);
				set_value($$, "Stmt", @$.first_line);
				
			}
		|	WHILE LP Exp RP Stmt
			{
				$$ = new_tree(5, $1, $2, $3, $4, $5);
				set_value($$, "Stmt", @$.first_line);
			}
		|  	error SEMI { printf("Error type B at Line %d: Missing \";\".\n", yylineno); errcount++;}
		
		
		;

// local definitions 
DefList		:	Def DefList
			{
				$$ = new_tree(2, $1, $2);
				set_value($$, "DefList", @$.first_line);
				
			}
		|	
			{
				$$ = new_tree(1, NULL);
				set_value($$, "null", @$.first_line);
			}
		;
Def		:	Specifier DecList SEMI
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Def", @$.first_line);
			}
		|	Specifier error SEMI { printf("Error type B at Line %d: Missing \";\".\n", yylineno);errcount++;}
		
		;
DecList		:	Dec
			{
				$$ = new_tree(1, $1);
				set_value($$, "DecList", @$.first_line);
				
			}
		|	Dec COMMA DecList
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "DecList", @$.first_line);
				
			}
		;
Dec		:	VarDec
			{
				$$ = new_tree(1, $1);
				set_value($$, "Dec", @$.first_line);
				
			}
		|	VarDec ASSIGNOP Exp
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Dec", @$.first_line);
			}
		;

// expressions 
Exp		:	Exp ASSIGNOP Exp
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Exp", @$.first_line);
			}
		|	Exp AND Exp
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Exp", @$.first_line);
			}
		|	Exp OR Exp
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Exp", @$.first_line);
				
			}
		|	Exp RELOP Exp{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Exp", @$.first_line);
			}
		|	Exp PLUS Exp
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Exp", @$.first_line);
				
			}
		|	Exp MINUS Exp
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Exp", @$.first_line);
			}
		|	Exp STAR Exp
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Exp", @$.first_line);
			}
		|	Exp DIV	Exp
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Exp", @$.first_line);
			}
		|	LP Exp RP
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Exp", @$.first_line);
			}
		|	MINUS Exp 
			{
				$$ = new_tree(2, $1, $2);
				set_value($$, "Exp", @$.first_line);
			}
		|	NOT Exp
			{
				$$ = new_tree(2, $1, $2);
				set_value($$, "Exp", @$.first_line);
				
			}
		|	ID LP Args RP
			{
				$$ = new_tree(4, $1, $2, $3, $4);
				set_value($$, "Exp", @$.first_line);
			}
		|	ID LP RP
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Exp", @$.first_line);
				
			}
		|	Exp LB Exp RB
			{
				$$ = new_tree(4, $1, $2, $3, $4);
				set_value($$, "Exp", @$.first_line);
				
			}
		|	Exp DOT ID
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Exp", @$.first_line);
				
			}
		|	ID
			{
				$$ = new_tree(1, $1);
				set_value($$, "Exp", @$.first_line);
			}
		|	INT
			{
				$$ = new_tree(1, $1);
				set_value($$, "Exp", @$.first_line);
			}
		|	FLOAT
			{
				$$ = new_tree(1, $1);
				set_value($$, "Exp", @$.first_line);
			}
		|	Exp LB error RB  { printf("Error type B at Line %d: Missing \"]\".\n", yylineno);errcount++; }
		|	LB error RB  { printf("Error type B at Line %d: Missing \"]\".\n", yylineno);errcount++; }
		|  INT INT  {printf("Error type B at Line %d: Missing \";\".\n", yylineno);errcount++;}
		|  INT ID  {printf("Error type B at Line %d: Missing \";\".\n", yylineno);errcount++;}
		|  FLOAT ID  {printf("Error type B at Line %d: Syntax error.\n", yylineno);errcount++;}
		;
Args : Exp COMMA Args
			{
				$$ = new_tree(3, $1, $2, $3);
				set_value($$, "Args", @$.first_line);
			}
		|	Exp
			{
				$$ = new_tree(1, $1);
				set_value($$, "Args", @$.first_line);
			}
		;

%%
#include "lex.yy.c"
yyerror(char* msg) {}







