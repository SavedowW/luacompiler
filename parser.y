%{
#include "tree_structs.h"
#include <stdio.h>
#include <malloc.h>
void yyerror(char const *s);
extern int yylex(void);
Program *CreateProgram(Statements_List *lst);
Statements_List *AppendStatementToList(Statements_List *lst,Statement *stm);
Statements_List *CreateStList(Statement *stm);
Statement *CreateStatement(Expression *exp);
Expression *CreateBinExp(int op,Expression *left,Expression *right);
Expression *CreateIntExp(int value);
Statements_List *lst_start;
Program *prg;
%}
%union {
	int int_const;
	Expression *exp;
	Statement *stm;
	Program *prg;
	Statements_List *lst;
}
%type <prg>seq
%type <lst>seq1
%type <stm>stmt
%type <exp>expr
%token PRINT
%token <int_const>INT
%left '-' '+'
%left '*' '/'
%nonassoc ')'
%%
seq: seq1 {$$=prg=CreateProgram(lst_start);}
    ;
seq1: seq1 stmt {$$=AppendStatementToList($1,$2);}
    | stmt {$$=lst_start=CreateStList($1);}
    ;
stmt: PRINT expr ';' {$$=CreateStatement($2);}
    ;
expr: expr '+' expr {$$=CreateBinExp(1,$1,$3);}
    | expr '-' expr {$$=CreateBinExp(2,$1,$3);}
    | expr '*' expr {$$=CreateBinExp(3,$1,$3);}
    | expr '/' expr {$$=CreateBinExp(4,$1,$3);}
    | '(' expr ')' {$$=$2;}
    | INT {$$=CreateIntExp($1);}
    ;
%%

void yyerror(char const *s)
{
 printf("%s",s);
}

Program *CreateProgram(Statements_List *lst)
{
	Program *crt=(Program *)malloc(sizeof(Program));
	crt->stmts=lst;
	return crt;
}
Statements_List *AppendStatementToList(Statements_List *lst,Statement *stm)
{
	Statements_List *crt=(Statements_List *)malloc(sizeof(Statements_List));
	crt->next=0;
	lst->next=crt;
	crt->stm=stm;
	return crt;
}
Statements_List *CreateStList(Statement *stm)
{
	Statements_List *crt=(Statements_List *)malloc(sizeof(Statements_List));
	crt->next=0;
	crt->stm=stm;
	return crt;
}
Statement *CreateStatement(Expression *exp)
{
	Statement *crt=(Statement *)malloc(sizeof(Statement));
	crt->to_print=exp;
	return crt;
}
Expression *CreateBinExp(int op,Expression *left,Expression *right)
{
	Expression *crt=(Expression *)malloc(sizeof(Expression));
	crt->type=op;
	crt->left=left;
	crt->right=right;
	return crt;
}
Expression *CreateIntExp(int value)
{
	Expression *crt=(Expression *)malloc(sizeof(Expression));
	crt->type=0;
	crt->value=value;
	return crt;
}