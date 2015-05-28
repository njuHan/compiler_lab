%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "syntax.tab.h"
	#include "tree.h"
	int yycolumn = 1;
	extern YYLTYPE yylloc;
	#define YY_USER_ACTION\
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;

	Node* new_node(char* name, int flag, char* text);


%}

digit [0-9]
letter [a-zA-Z]

%option yylineno

%%


";"  {yylval.type_node =  new_node("SEMI",NO_TYPE, "") ;return SEMI ;}
","   {yylval.type_node = new_node("COMMA", NO_TYPE, ""); return COMMA ;}
"="   {yylval.type_node = new_node("ASSIGNOP", NO_TYPE, "");  return ASSIGNOP ;}
">"|"<"|">="|"<="|"=="|"!="  {yylval.type_node = new_node("RELOP", TYPE_STR, yytext);  return RELOP ;}
"+" {yylval.type_node = new_node("PLUS", NO_TYPE, ""); return PLUS ;}
"-" {yylval.type_node = new_node("MINUS", NO_TYPE, ""); return MINUS ;}
"*" {yylval.type_node = new_node("STAR", NO_TYPE, ""); return STAR ;};
"/" {yylval.type_node = new_node("DIV", NO_TYPE, ""); return DIV ;}
"&&" {yylval.type_node = new_node("AND", NO_TYPE, ""); return AND ;}
"||" {yylval.type_node = new_node("OR", NO_TYPE, ""); return OR ;} 
"." {yylval.type_node = new_node("DOT", NO_TYPE, ""); return DOT ;}
"!" {yylval.type_node = new_node("NOT", NO_TYPE, "");  return NOT ;}
"int" {yylval.type_node = new_node("TYPE", TYPE_INT, yytext);  return TYPE ;}
"float" {yylval.type_node = new_node("TYPE", TYPE_FLOAT, yytext);  return TYPE ;}
"("  {yylval.type_node = new_node("LP", NO_TYPE, ""); return LP ;}
")"  {yylval.type_node = new_node("RP", NO_TYPE, ""); return RP ;}
"["  {yylval.type_node = new_node("LB", NO_TYPE, ""); return LB ;}
"]"  {yylval.type_node = new_node("RB", NO_TYPE, ""); return RB ;}
"{"  {yylval.type_node = new_node("LC", NO_TYPE, ""); return LC ;}
"}"  {yylval.type_node = new_node("RC", NO_TYPE, ""); return RC ;}
"struct"  {yylval.type_node = new_node("STRUCT", NO_TYPE, ""); return STRUCT ;}
"return"  {yylval.type_node = new_node("RETURN", NO_TYPE, ""); return RETURN ;}
"if"  {yylval.type_node = new_node("IF", NO_TYPE, ""); return IF ;}
"else"  {yylval.type_node = new_node("ELSE", NO_TYPE, ""); return ELSE ;}
"while"  {yylval.type_node = new_node("WHILE", NO_TYPE, ""); return WHILE ;}

0|([1-9]{digit}*)    {yylval.type_node = new_node("INT", TYPE_INT, yytext); return INT ;}

0([0-7]+)  {	
		char temp[12];
		sprintf(temp,"%d",octstr_to_int(yytext));
		yylval.type_node = new_node("INT",TYPE_INT, temp);
		return INT ;
	}
	
0(x|X)[0-9a-fA-F]+   {                  
		char temp[12];
		sprintf(temp,"%d",hexstr_to_int(yytext));
		yylval.type_node = new_node("INT",TYPE_INT, temp);
		return INT ;
	}

0|([1-9]{digit}*)(\.{digit}+)?       {yylval.type_node = new_node("FLOAT", TYPE_FLOAT, yytext); return FLOAT ;}

(({digit}+\.{digit}*)|({digit}*\.{digit}+))[Ee][+-]?{digit}+  {yylval.type_node = new_node("FLOAT", TYPE_FLOAT, yytext); return FLOAT ;}

"//" {
	char c = input();
	while (c!='\n')
		c = input();
	yycolumn = 1;
}


"/*" {
	char c = input();
	int flag = 0;

	while ( !(c=='/' && flag) )
	{	
		if (c=='*')
			flag = 1;
		else 
			flag = 0;
		if (c=='\n')
			yycolumn = 1;
		c = input();
	}
}

"*/" { printf("Error type B at Line %d: Syntax error.\n", yylineno); errcount++;}
		

({letter}|_)({letter}|{digit}|_)*    {yylval.type_node = new_node("ID", TYPE_STR, yytext); return ID ;}



(\t)+  { } 
(" ")+  { } 
\n    { yycolumn = 1; }


 
. {	printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext); errcount++; }


%%


int octstr_to_int(char* str)
{
	int len = strlen(str);
	int i = len-1;
	int j = 1;
	int temp = 0;
	while (i>=0)
	{
		temp = temp + (str[i]-'0')*j;
		j = j*8;
		i--;
	}
	return temp;
}

int hexstr_to_int (char* str)
{
	int len = strlen(str);
	int i = len-1;
	int j = 1;
	int temp = 0;
	while (i>=2)
	{
		if (str[i]>='0' && str[i]<='9')
			temp = temp + (str[i]-'0')*j;
		else if(str[i]>='a' && str[i]<='f')
			temp = temp + (str[i]-'a'+10)*j;
		else if(str[i]>='A' && str[i]<='F')
			temp = temp + (str[i]-'A'+10)*j;
		else 
			printf("error\n");
		
		j = j*16;
		i--;
	}
	return temp;
}


Node* new_node(char* name, int flag, char* text)
{
	Node* temp = (Node*)malloc(sizeof(Node));
	
	switch(flag)
	{
		case TYPE_INT:
			temp->value.type_int = atoi(text);
		break;
		case TYPE_FLOAT :
			temp->value.type_float = atof(text);	
		break;
		case TYPE_STR:
			temp->value.type_str = (char*)malloc(sizeof(char)*(strlen(text)+1));
			strcpy(temp->value.type_str, text);
			temp->value.type_str[strlen(text)] = '\0';
		break;
		
		default: ;
	}

	temp->value.name = name;
	temp->value.flag = flag;
	temp->value.lineno = yylineno;
	
	return temp;
}





