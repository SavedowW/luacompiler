%{
#include "Tree.h"
#include <stdio.h>
#include <malloc.h>
void yyerror(char const *s);
extern int yylex(void);
StatementsList *lst_start;
Program *prg;
%}
%union {
	int int_const;
	Expression *exp;
	Statement *stm;
	Program *prg;
	StatementsList *lst;
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
seq: seq1 {$$=prg=TreeFactory::CreateProgram(lst_start);}
    ;
seq1: seq1 stmt {$$=TreeFactory::AppendStatementToList($1,$2);}
    | stmt {$$=lst_start=TreeFactory::CreateStList($1);}
    ;
stmt: PRINT expr {$$=TreeFactory::CreateStatement($2);}
    ;
expr: expr '+' expr {$$=TreeFactory::CreateBinExp(EXPRESSION_TYPE::BIN_PLUS,$1,$3);}
    | expr '-' expr {$$=TreeFactory::CreateBinExp(EXPRESSION_TYPE::BIN_MINUS,$1,$3);}
    | expr '*' expr {$$=TreeFactory::CreateBinExp(EXPRESSION_TYPE::BIN_MUL,$1,$3);}
    | expr '/' expr {$$=TreeFactory::CreateBinExp(EXPRESSION_TYPE::BIN_DIV,$1,$3);}
    | '(' expr ')' {$$=$2;}
    | INT {$$=TreeFactory::CreateIntExp($1);}
    ;
%%

void yyerror(char const *s)
{
	printf("%s",s);
}