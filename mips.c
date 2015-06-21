#include "ir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

extern int temp_count;
extern int var_count;

extern void test();
extern void gen_mips();

int func_mem[32];
int func_count = 0;
int output_count = 1;

int arg_count = 0;

int* temp_mem;
int* var_mem;

char filename[] = "out.temp"; //文件名

enum type{label, func, assign, add, minus, star, divi, 
	ref, r_star, l_star, go, IF, re, dec, arg, call, param, rd, wt };

FILE* fw=NULL;

int offset = 0;
int negoffset = 4;

//addi $sp, $sp, -4
//sw $fp, 0($sp)

void savecontext()
{
	fprintf(fw, "addi $sp, $sp, -4\n");
	fprintf(fw, "sw $fp, 0($sp)\n");
	
	
}
/*
void restorecontext()
{
	fprintf(fw,"move $sp, $fp\n");
	fprintf(fw,"lw $fp, 0($sp)\n");
	
}
*/

void reset_offset()
{
	func_mem[func_count] = offset;
	func_count++;
	offset	  = 0;
	negoffset = 4;
	
}



//DEC type = 1;
//PARAM type = 2;
//other 0;
void set_mem(char* var, int type, int size)
{
	
	
	int index = atoi(var+1);
	
	if (type == 1) //DEC
	{
		if (var_mem[index]==0)
		{
			offset -= size;
			var_mem[index] = offset;
			//printf("index:%d off: v%d\n",index, offset);
			
		}
		return ;
	}
	
	else if (type == 2) //PARAM
	{
		//printf("var[1]:%d, %d\n", var_mem[index], index);
		//assert(0);
		if (var_mem[index]==0)
		{
			negoffset += 4;
			var_mem[index] = negoffset;
			
			//printf("index:%d off: v%d\n",index, negoffset);
			
			
		}
		return ;
	}
	
	else if (type==0)
	{
		
		if (var[0]=='v')
		{
			if (var_mem[index] == 0)
			{
				
				offset = offset -4;
				var_mem[index] = offset;
				//printf("index:%d off: v%d\n",index, offset);
				
			}
		}
		else if (var[0]=='t')
		{
			if (temp_mem[index] == 0)
			{
				offset = offset - 4;
				temp_mem[index] = offset;
				//printf("index:%d off: t%d\n",index, offset);
				
			}
		}
	}
	
		
	
	else
	{
		printf("variable error");
		assert(0);
	}
}
	
void get_var(char* line)
{
	//printf("line: %s, ", line);
	char* var = malloc(sizeof(char)*5);
	int i = 0;
	
//DEC type = 1;
//PARAM type = 2;
//other 0;
	int size = 0;
	int type = 0;
	
	if (strncmp(line, "PARAM", 5)==0)
	{
		//printf("%s\n", line);
		//printf("%d\n", var_mem[1]);
		type = 2;
	}
	else if (strncmp(line, "DEC", 3)==0)
	{
		//printf("%s\n", line);
		type = 1;
		int count = 0;
		int i=0;
		while (count<2)
		{
			if (line[i]==' ')
				count++;
			i++;
		}
		
		size = atoi(line+i);
		//printf("%d\n", size);
	}
	
	
	while(line[i]!='\n'&& line[i]!='\0')
	{
		if (line[i]=='v'||line[i]=='t')
		{
			var[0] = line[i];
			int j=1;
			i++;
			int flag = 1;
			while(line[i]!=' ' && line[i]!='\n' && line[i]!='\0')
			{
				//printf("*%c*, ",line[i]);
				if (line[i]<='9' && line[i]>='0')
				{
					var[j] = line[i];
					j++;
				}
				else
				{
					flag = 0;
					break;
				}
				
				i++;
			}
			if (j>1 && flag==1 )
			{
				var[j] = '\0';
				//printf("var:%s\t", var);
				set_mem(var,type,size);
			}
		}
		
		
		i++;
	}
	//printf("\n");
	
	
	
	
}
	
void load_mem(char* file)
{
	temp_mem = malloc(sizeof(int)*(temp_count+1));
	var_mem  = malloc(sizeof(int)*(var_count+1));
	
	int index = 0;
	for (index=1; index<=temp_count; index++)
	{
		temp_mem[index] = 0;
	}
	
	
	for (index=1; index<=var_count; index++)
	{
		var_mem[index] = 0;
	}
	
	
	FILE *fp; 
	char line[1024];             //每行最大读取的字符数
	if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
	{ 
		printf("error %s cannot read!\n", filename); 
		 exit(-1);
	} 
  
	while (1) 
	{ 
		fgets(line,1024,fp);  //读取一行
		if (feof(fp)) 
			 break;
		int len = strlen(line);
		
		if (strncmp(line, "FUNCTION",8)==0)
		{
			//printf("FUNC: %d\n", offset);
			reset_offset();
			
		}
		
		//printf("***************%s", line); //输出
		get_var(line);
		
		
	} 
	func_mem[func_count] = offset;
	func_count ++;
	
    fclose(fp);                     //关闭文件
	 
	// printf("temp_count:%d\n var_count:%d\n", temp_count,var_count);
	 
	
	 /*
	 int i;
	 printf("temp:\n");
	for (i=1; i<=temp_count; i++)
	{
		printf("t%d: %d ,",i,temp_mem[i]);
	}
	
	
	printf("\n var:\n");
	for (i=1; i<=var_count; i++)
	{
		printf("v%d: %d ,",i,var_mem[i]);
	}
	printf("\n");
	*/
	//test();
	
	
	gen_mips(file);
	
	/*
	for (i=0; i<func_count; i++)
	{
		printf("offset: %d\n", func_mem[i]);
	}
	*/
    return ; 
	
}
	

//enum type{label, func, assign, add, minus, star, divi, ref, 
// r_star, l_star, go, IF, re, dec, arg, call, param, rd, wt };



enum type get_type(char* line, int len)
{
	enum type t;
	
	
	if (strncmp(line, "LABEL", 5)==0)
	{
		t = label;
		
	}
	else if (strncmp(line, "FUNCTION",8 )==0)
	{
		t=func;
	}
	
	else if (strncmp(line, "GOTO",4 )==0)
	{
		t = go;
	}
	else if (strncmp(line, "IF", 2)==0)
	{
		t = IF;
	}
	else if (strncmp(line, "RETURN",6 )==0)
	{
		t = re;
	}
	else if (strncmp(line, "DEC", 3 )==0)
	{
		t = dec;
	}
	else if (strncmp(line, "ARG", 3)==0)
	{
		t = arg;
	}
	else if (strncmp(line, "PARAM", 5)==0)
	{
		t = param;
	}
	else if (strncmp(line, "READ", 4 )==0)
	{
		t = rd;
	}
	else if (strncmp(line, "WRITE", 5)==0)
	{
		t = wt;
	}
	else if (line[0]=='*')
	{
		t = l_star;
	}
	else if (strstr(line, " CALL ")!=NULL)
	{
		t = call;
	}
	else if (strstr(line,":=")!=NULL)
	{
		
		//enum type{label, func, assign, add, minus, star, divi, ref, 
		// r_star, l_star, go, IF, re, dec, arg, call, param, rd, wt };
		if (strstr(line, " + ")!=NULL)
		{
			t = add;
		}
		else if (strstr(line, " - ")!=NULL)
		{
			t = minus;
		}
		else if (strstr(line, " * ")!=NULL)
		{
			t = star;
		}
		else if (strstr(line, " / ")!=NULL)
		{
			t = divi;
		}
		else 
		{
			int i;
			for (i = 1; i<len-1; i++)
			{
			
				if (line[i] == '&' )
				{
					t = ref;
					break;
				}
				else if (line[i-1]=='=' &&line[i+1]=='*')
				{
					t = r_star;
					break;
				}
			}
		
			if (i==len-1)
				t = assign;
		}
	}
	

	return t;
}




void test()
{
	
	
	FILE *fp; 
	char line[1024];             //每行最大读取的字符数
	if((fp = fopen("test.ir","r")) == NULL) //判断文件是否存在及可读
	{ 
		printf("error!"); 
		 exit(-1);
	} 
  
	while (1) 
	{ 
		fgets(line,1024,fp);  //读取一行
		if (feof(fp)) 
			 break;
		int len = strlen(line);
		
		
		
		printf("line:%s ,%d\n",line, get_type(line, len));
		
		
	} 
	
    fclose(fp);                     //关闭文件
}


extern void switch_type(enum type t, char* line);

void gen_mips(char* file)
{
	fw=fopen(file,"w");
	//fprintf(fw,"test");

	
	FILE *fp; 
	char line[1024];             //每行最大读取的字符数
	if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
	{ 
		printf("error!"); 
		 exit(-1);
	} 
	
	fprintf(fw,".data\n_prompt: .asciiz \"Enter an integer:\"\n_ret: .asciiz \"\\n\"\n\
.globl main\n.text\nread:\nli $v0, 4\nla $a0, _prompt\nsyscall\nli $v0, 5\n\
syscall\njr $ra\n\nwrite:\nli $v0, 1\nsyscall\nli $v0, 4\nla $a0, _ret\n\
syscall\nmove $v0, $0\njr $ra\n\n");
	
  
	while (1) 
	{ 
		fgets(line,1024,fp);  //读取一行
		if (feof(fp)) 
			 break;
		int len = strlen(line);
		enum type t = get_type(line, len);
		switch_type(t,line);
		
		//printf("line:%s,%d\n",line, get_type(line, len));
		
		
	} 
	
	
    fclose(fp);                     //关闭文件
	fclose(fw);
}

int get_offset(char* str)
{
	char temp [32];
	
	int i = 1;
	int j=0;
	while (str[i]!='\n' && str[i]!='\0')
	{
		temp[j] = str[i];
		
		i++;
		j++;
	}
	temp[j]='\0';
	
	int index = atoi(temp);
	//printf("index: %d", index);
	if (str[0]=='v')
	{
		return var_mem[index];
	}
	else if (str[0]=='t')
	{
		return temp_mem[index];
	}
	else
	{
		printf("%s\n", str);
		assert(0);
	}
}

void handler_label(char* line)
{
	int i = 0;
	while(line[i] != ' ')
	{
		i++;
	}
	i++;
	while(line[i]!=' ')
	{
		fprintf(fw, "%c",line[i]);
		i++;
	}
	fprintf(fw, ":\n");
}

void handler_func(char* line)
{
	char func_name[32];
	int i = 0;
	while(line[i]!=' ')
	{
		i++;
	}
	i++;
	int j = 0;
	while (line[i]!=' ')
	{
		func_name[j] = line[i];
		i++;
		j++;
	}
	func_name[j] = '\0';
	if (strcmp(func_name,"main")!=0)
		fprintf(fw, "\n%s_:\n", func_name);
	else
		fprintf(fw, "\n%s:\n", func_name);
	fprintf(fw, "addi $sp, $sp, -4\n");
	fprintf(fw, "sw $fp, 0($sp)\n");
	fprintf(fw,"move $fp, $sp\n");
	fprintf(fw,"addi $sp, $fp, %d\n",func_mem[output_count++]);
}
handler_as(char* line)
{
	char op1[32],op2[32];
	int i=0;
	int j = 0;
	while(line[i]!=' ')
	{
		op1[j] = line[i];
		i++;
		j++;
	}
	op1[j] = '\0';
	
	i = i+3;
	assert(line[i]==' ');
	i++;
	j = 0;
	while (line[i]!='\n'&&line[i]!='\0')
	{
		op2[j] =  line[i];
		i++;
		j++;
	}
	op2[j] = '\0';
	//printf("%s, %s\n", op1, op2);
	
	if (op2[0]=='#')//imm
	{
		int imm = atoi(op2+1);
		int off = get_offset(op1);
		//printf("imm: %d, offset: %d\n",imm,off );
		fprintf(fw, "li $t0, %d\n",imm);
		fprintf(fw, "sw $t0, %d($fp)\n", off);
	}
	else if (op2[0]=='*')
	{
		int off1 = get_offset(op1);
		int off2 = get_offset(op2+1);
		
		fprintf(fw, "lw $t1, %d($fp)\n",off2);
		fprintf(fw, "lw $t0, 0($t1)\n");
		fprintf(fw, "sw $t0, %d($fp)\n", off1);
	}
	else
	{
		int off1 = get_offset(op1);
		int off2 = get_offset(op2);
		
		fprintf(fw, "lw $t0, %d($fp)\n", off2);
		
		fprintf(fw, "sw $t0, %d($fp)\n", off1);
		
	}
}

// flag  = 0,1,2,3
//       add, minus,star, div 
void handler_3op(char* line, int flag)
{
	char op1[32],op2[32],op3[32];
	int i=0;
	int j = 0;
	while(line[i]!=' ')
	{
		op1[j] = line[i];
		i++;
		j++;
	}
	op1[j] = '\0';
	
	i = i+3;
	assert(line[i]==' ');
	i++;
	j = 0;
	while (line[i]!=' ')
	{
		op2[j] =  line[i];
		i++;
		j++;
	}
	op2[j] = '\0';
	
	i = i+3;
	j=0;
	
	while (line[i]!='\n' &&line[i]!='\0')
	{
		
		op3[j] = line[i];
		i++;
		j++;
	}
	op3[j] = '\0';
	
	int off1 = get_offset(op1);
	
	
	//printf("3op :%s, %s, %s\n", op1,op2,op3);
	
	
	if (op2[0]=='#')
	{
		int imm = atoi(op2+1);
		fprintf(fw, "li $t0, %d\n",imm);
	}
	else if(op2[0]=='*')
	{
		//x = *y
		/*
		fprintf(fw, "lw $t1, %d($fp)\n",off2);
		fprintf(fw, "lw $t0, 0($t1)\n");
		fprintf(fw, "sw $t0, %d($fp)\n",off1);
		*/
		int off2 = get_offset(op2+1);
		fprintf(fw, "lw $t1, %d($fp)\n",off2);
		fprintf(fw, "lw $t0, 0($t1)\n");
	}
	else if(op2[0]=='&')
	{
		/*
		x = &y
		fprintf(fw,"addi $t0, $fp, %d\n", off2);	
		fprintf(fw, "sw $t0, %d($fp)\n",off1);
		*/
		int off2 = get_offset(op2+1);
		fprintf(fw,"addi $t0, $fp, %d\n", off2);	
	}
	else
	{
		int off2 = get_offset(op2);
		fprintf(fw, "lw $t0, %d($fp)\n",off2);
	}
	
	if (op3[0]=='#')
	{
		int imm = atoi(op3+1);
		fprintf(fw, "li $t1, %d\n", imm);
	}
	else if(op3[0]=='*')
	{
		
		int off3 = get_offset(op3+1);
		fprintf(fw, "lw $t2, %d($fp)\n",off3);
		fprintf(fw, "lw $t1, 0($t2)\n");
	}
	else if(op3[0]=='&')
	{
		
		int off3 = get_offset(op3+1);
		fprintf(fw,"addi $t1, $fp, %d\n", off3);	
	}
	
	else
	{
		int off3 = get_offset(op3);
		fprintf(fw, "lw $t1, %d($fp)\n",off3);
	}
	
	switch(flag)
	{
		case 0: fprintf(fw, "add $t0, $t0, $t1\n"); break;
		case 1: fprintf(fw, "sub $t0, $t0, $t1\n"); break;
		case 2: fprintf(fw, "mul $t0, $t0, $t1\n"); break;
		case 3: 
			fprintf(fw, "lw $t2, %d($fp)\n",off1);
			fprintf(fw, "div $t0, $t1\n");
			fprintf(fw, "mflo $t2\n");
	}
	if (flag==3)
		fprintf(fw, "sw $t2, %d($fp)\n",off1);
	else
		fprintf(fw, "sw $t0, %d($fp)\n",off1);
	
}

handler_ref(char* line)
{
	char op1[32],op2[32];
	int i=0;
	int j = 0;
	while(line[i]!=' ')
	{
		op1[j] = line[i];
		i++;
		j++;
	}
	op1[j] = '\0';
	
	i = i+4;
	assert(line[i]=='&');
	i++;
	j = 0;
	while (line[i]!='\n' && line[i]!='\0')
	{
		op2[j] =  line[i];
		i++;
		j++;
	}
	op2[j] = '\0';
	
	int off1 = get_offset(op1);
	int off2 = get_offset(op2);
	
	fprintf(fw,"addi $t0, $fp, %d\n", off2);	
	fprintf(fw, "sw $t0, %d($fp)\n",off1);
	
	
}

handler_rstar(char* line)
{
	char op1[32],op2[32];
	int i=0;
	int j = 0;
	while(line[i]!=' ')
	{
		op1[j] = line[i];
		i++;
		j++;
	}
	op1[j] = '\0';
	
	i = i+4;
	assert(line[i]=='*');
	i++;
	j = 0;
	while (line[i]!='\n' && line[i]!='\0')
	{
		op2[j] =  line[i];
		i++;
		j++;
	}
	op2[j] = '\0';
	
	int off1 = get_offset(op1);
	int off2 = get_offset(op2);
	
	fprintf(fw, "lw $t1, %d($fp)\n",off2);
	fprintf(fw, "lw $t0, 0($t1)\n");
	fprintf(fw, "sw $t0, %d($fp)\n",off1);
}

handler_lstar(char* line)
{
	assert(line[0]=='*');
	line = line + 1;
	char op1[32],op2[32];
	int i=0;
	int j = 0;
	while(line[i]!=' ')
	{
		op1[j] = line[i];
		i++;
		j++;
	}
	op1[j] = '\0';
	
	i = i+3;
	assert(line[i]==' ');
	i++;
	j = 0;
	while (line[i]!='\n' && line[i]!='\0')
	{
		op2[j] =  line[i];
		i++;
		j++;
	}
	op2[j] = '\0';
	
	int off1 = get_offset(op1);
	
	if (op2[0]=='*')
	{
		
		int off2 = get_offset(op2+1);
		
		fprintf(fw, "lw $t1, %d($fp)\n",off2);
		fprintf(fw, "lw $t0, 0($t1)\n");
		fprintf(fw, "sw $t0, %d($fp)\n", off1);
	
	}
	else
	{
		int off2 = get_offset(op2);
		fprintf(fw, "lw $t0, %d($fp)\n",off2);
		fprintf(fw, "sw $t0, %d($fp)\n",off1);
	}
}

void handler_goto(char* line)
{
	int i = 0;
	while(line[i] != ' ')
	{
		i++;
	}
	i++;
	fprintf(fw, "j " );
	while(line[i]!='\n' && line[i]!='\0')
	{
		fprintf(fw, "%c",line[i]);
		i++;
	}
	fprintf(fw,"\n");
}

void handler_relop(char* line)
{
	char x[32],y[32],z[32],relop[3];
	assert(line[0]=='I' && line[1]=='F');
	
	int i = 3;
	int j = 0;
	while (line[i]!=' ')
	{
		x[j] = line[i];
		i++;
		j++;
	}
	x[j] = '\0';
	
	i++;
	j=0;
	while(line[i]!=' ')
	{
		relop[j]=line[i];
		i++;
		j++;
	}
	relop[j]='\0';
	
	i++;
	j=0;
	while (line[i]!=' ')
	{
		y[j] = line[i];
		i++;
		j++;
	}
	y[j] = '\0';
	
	
	i = i+6;
	j=0;
	while (line[i]!='\n' && line[i]!='\0')
	{
		z[j] = line[i];
		i++;
		j++;
	}
	z[j] = '\0';
	//printf ("%s %s %s goto %s\n",x,relop,y,z);
	
	char temp [4];
	if (strcmp(relop,"==")==0)
		strcpy(temp, "beq");
	
	else if (strcmp(relop,"!=")==0)
		strcpy(temp, "bne");
	
	else if (strcmp(relop,">")==0)
		strcpy(temp, "bgt");
	
	else if (strcmp(relop,"<")==0)
		strcpy(temp, "blt");
	
	else if (strcmp(relop,">=")==0)
		strcpy(temp, "bge");
	
	else if (strcmp(relop,"<=")==0)
		strcpy(temp, "ble");
	else 
		assert(0);
	
	
	if(x[0]=='t' || x[0]=='v')
	{
		int off1 = get_offset(x);
		fprintf(fw, "lw $t0, %d($fp)\n",off1);
	}
	else if (x[0]=='#')
	{
		int imm = atoi(x+1);
		//printf("imm: %d, offset: %d\n",imm,off );
		fprintf(fw, "li $t0, %d\n",imm);	
	}
	else if (x[0]=='*')
	{
		/*
		int off1 = get_offset(op1);
		int off2 = get_offset(op2);
	
		fprintf(fw, "lw $t1, %d($fp)\n",off2);
		fprintf(fw, "lw $t0, 0($t1)\n");
		fprintf(fw, "sw $t0, %d($fp)\n",off1);
		*/
		int off = get_offset(x+1);
		fprintf(fw, "lw $t2, %d($fp)\n",off);
		fprintf(fw, "lw $t0, 0($t2)\n");
		
	}
	else
	{
		assert(0);
	}
	
	
	if (y[0]=='t' || y[0]=='v')
	{
		int off2 = get_offset(y);
		fprintf(fw, "lw $t1, %d($fp)\n",off2);
	}
	else if (y[0]=='#')
	{
		int imm = atoi(y+1);
		//printf("imm: %d, offset: %d\n",imm,off );
		fprintf(fw, "li $t1, %d\n",imm);	
	}
	else if (y[0]=='*')
	{
		
		int off = get_offset(y+1);
		fprintf(fw, "lw $t2, %d($fp)\n",off);
		fprintf(fw, "lw $t1, 0($t2)\n");
		
	}
	else 
	{
		assert(0);
	}
	
	fprintf(fw, "%s $t0, $t1, %s\n",temp, z);
}

void handler_return(char* line)
{
	int i=0;
	while(line[i]!=' ')
	{
		i++;
	}
	i++;
	char temp[32];
	int j = 0;
	while (line[i]!='\n' && line[i]!='\0')
	{
		temp[j] = line[i];
		i++;
		j++;
	}
	temp[j] = '\0';
	//printf("return %s\n", temp);
	int off = get_offset(temp);
	fprintf(fw, "lw $t0, %d($fp)\n", off);
	fprintf(fw, "move $v0, $t0\n");
	//restorecontext();
	fprintf(fw,"move $sp, $fp\n");
	fprintf(fw,"lw $fp, 0($sp)\n");
	
	fprintf(fw,"addi $sp, $sp, 4\n");
	fprintf(fw, "jr $ra\n");
}
void handler_arg(char* line)
{
	arg_count ++;
	int i=0;
	while(line[i]!=' ')
	{
		i++;
	}
	i++;
	char temp[32];
	int j = 0;
	while (line[i]!='\n'&& line[i]!='\0')
	{
		temp[j] = line[i];
		i++;
		j++;
	}
	temp[j] = '\0';
	int off = get_offset(temp);
	
	fprintf(fw, "lw $t0, %d($fp)\n", off);
	fprintf(fw, "addi $sp, $sp, -4\n");
	fprintf(fw, "sw $t0, 0($sp)\n");
	
	
}


void handler_call(char* line)
{
	char x[32], f[32];
	int i=0;
	int j=0;
	while(line[i]!=' ')
	{
		x[j] = line[i];
		
		i++;
		j++;
	}
	x[j] = '\0';
	
	i = i+8;
	assert(line[i] == ' ');
	i++;
	j = 0;
	while (line[i]!='\n' && line[i]!='\0')
	{
		f[j] = line[i];
		i++;
		j++;
	}
	f[j] = '\0';
	//printf("call: %s, %s\n", x, f);
	
	int off = get_offset(x);
	
	fprintf(fw, "addi $sp, $sp, -4\n");
	fprintf(fw, "sw $ra, 0($sp)\n");
	fprintf(fw,"jal %s_\n", f);
	fprintf(fw, "lw $ra, 0($sp)\n");
	fprintf(fw, "addi $sp, $sp, 4\n");
	
	while (arg_count>0)
	{
		//fprintf(fw, "#pop arg\n");
		fprintf(fw, "addi $sp, $sp, 4\n");
		arg_count--;
	}
	fprintf(fw, "move  $t0, $v0\n");
	fprintf(fw, "sw $t0, %d($fp)\n",off);
	
	
}

void handler_read(char* line)
{
	
	char x[32];
	int i = 5;
	int j = 0;
	while (line[i]!='\n' && line[i]!='\0')
	{
		x[j] = line[i];
		//printf("%c,%d", line[i],i);
		i++;
		j++;
	}
	x[j]='\0';
	
	int off = get_offset(x);
	fprintf(fw,"addi $sp, $sp, -4\n");
	fprintf(fw,"sw $ra, 0($sp)\n");
	
	fprintf(fw,"jal read\n");
	
	fprintf(fw,"lw $ra, 0($sp)\n");
	fprintf(fw,"addi $sp, $sp, 4\n");
	fprintf(fw,"move $t0, $v0\n");
	fprintf(fw,"sw $t0, %d($fp)\n", off);
	
	//fprintf(fw, "sw $t0, %d($fp)\n",off1);
}

void handler_w(char* line)
{
	char x[32];
	int i = 6;
	int j = 0;
	while (line[i]!='\n' && line[i]!='\0')
	{
		x[j] = line[i];
		//printf("%c,%d", line[i],i);
		i++;
		j++;
	}
	x[j]='\0';
	int off = get_offset(x);
	
	fprintf(fw,"lw $t0, %d($fp)\n", off);
	fprintf(fw,"move $a0, $t0\n");
	
	fprintf(fw,"addi $sp, $sp, -4\n");
	fprintf(fw,"sw $ra, 0($sp)\n");
	
	fprintf(fw,"jal write\n");
	fprintf(fw,"lw $ra, 0($sp)\n");
	fprintf(fw,"addi $sp, $sp, 4\n");
}

//enum type{label, func, assign, add, minus, star, divi, ref, 
// r_star, l_star, go, IF, re, dec, arg, call, param, rd, wt };
void switch_type(enum type t, char* line)
{
	//fprintf(fw, "\n# %s", line);
	//fprintf(fw,"\n");
	switch(t)
	{
	case label:
		handler_label(line);
	break; 
	
	case func:
		handler_func(line);
	break;
	
	case assign:
		handler_as(line);
	break;
	
	case add:
		handler_3op(line,0);
	break;
	
	case minus:
		handler_3op(line,1);
	break;
	
	case star:
		handler_3op(line,2);
	break;
	
	case divi:
		handler_3op(line,3);
	break;
	
	case ref:
		handler_ref(line);
	break;
	
	// r_star, l_star, go, IF, re, dec, arg, call, param, rd, wt };
	case r_star:
		handler_rstar(line);
	break;
	
	case l_star:
		handler_lstar(line);
	break;
	
	case go:
		handler_goto(line);
	break;
	
	case IF:
		handler_relop(line);
	break;
	
	case re:
		handler_return(line);
	break;
	
	case arg:
		handler_arg(line);
	break;
	
	case call:
		handler_call(line);
	break;
	
	case rd:
		handler_read(line);
	break;
	
	case wt:
		handler_w(line);
	break;
	
	default:;
	}
}


