%{
#include "Tree.h"
#include "TreePrint.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <malloc.h>
void yyerror(char const *s);
extern int yylex(void);
StatementList *lst_start;
Program *prg;
%}
%union {
	int int_const;
	double double_const;
	Expression *exp;
	Statement *stm;
	Program *prg;
	StatementList *lst;
    const char *identifier;
}
%type <prg>seq
%type <lst>seq1
%type <stm>stmt
%type <exp>expr
%type <exp>assignable_expr
%token PRINT
%token <int_const>INT
%token <double_const>DOUBLE
%token <identifier>IDENTIFIER;
%left '-' '+'
%left '*' '/'
%nonassoc ')'
%%
seq: seq1 {$$=prg=TreeFactory::CreateProgram(lst_start);}
    ;

seq1: seq1 stmt {$$=TreeFactory::AppendStatementToList($1,$2);}
    | stmt {$$=lst_start=TreeFactory::CreateStList($1);}
    ;

stmt: PRINT expr {$$=TreeFactory::CreatePrintStatement($2);}
    | assignable_expr '=' expr {$$=TreeFactory::CreateAssignStatement($1, $3);}
    | assignable_expr '=' assignable_expr {$$=TreeFactory::CreateAssignStatement($1, $3);}
    | expr '=' expr {yyerror("Unexpected symbol near '='");}
    | expr stmt {yyerror((std::string("Unexpected symbol near ") + $2->toString()).c_str());}
    | expr {yyerror("Unexpected symbol near <EOF>");}
    ;

expr: expr '+' expr {$$=TreeFactory::CreateBinExp(EXPRESSION_TYPE::BIN_PLUS,$1,$3);}
    | expr '-' expr {$$=TreeFactory::CreateBinExp(EXPRESSION_TYPE::BIN_MINUS,$1,$3);}
    | expr '*' expr {$$=TreeFactory::CreateBinExp(EXPRESSION_TYPE::BIN_MUL,$1,$3);}
    | expr '/' expr {$$=TreeFactory::CreateBinExp(EXPRESSION_TYPE::BIN_DIV,$1,$3);}
    | '(' expr ')' {$$=$2;}
    | INT {std::cout << "FOUND INT\n"; $$=TreeFactory::CreateConstExp($1);}
    | DOUBLE {std::cout << "FOUND DOUBLE\n"; $$=TreeFactory::CreateConstExp($1);}
    ;

assignable_expr: IDENTIFIER {std::cout << "FOUND IDENTIFIER " << $1 << std::endl; $$=TreeFactory::CreateIdfExp($1);}
    ;
%%

void yyerror(char const *s)
{
	printf("%s",s);
}