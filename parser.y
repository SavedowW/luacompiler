%{
#include "Tree.h"
#include "TreePrint.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <malloc.h>
void yyerror(char const *s);
extern int yylex(void);
StatementList *lst_start = nullptr;
Program *prg = nullptr;
%}
%union {
	int int_const;
	double double_const;
	Expression *exp;
	ExpressionList *explst;
	Statement *stm;
	Program *prg;
	StatementList *lst;
    const char *identifier;
    DoublePtrString str_const;
    bool bool_const;
}
%type <prg>seq
%type <lst>seq1
%type <stm>stmt
%type <stm>return_stmt
%type <stm>if_else_end_sequence
%type <exp>expr
%type <explst>expr_list
%type <explst>assignable_expr_list
%type <exp>assignable_expr
%type <exp>function_call
%token PRINT
%token <int_const>INT
%token <double_const>DOUBLE
%token <str_const>STRING
%token <bool_const>BOOL
%token <identifier>IDENTIFIER;
%token IF;
%token ELSE;
%token ELSEIF;
%token THEN;
%token END;
%token VAR_CONCAT;
%token NIL;
%token RETURN;
%token TRUE;
%token FALSE;
%left '-' '+'
%left '*' '/'
%nonassoc ')' '='
%left ','
%%
seq: seq1
    ;

seq1: seq1 stmt {printf("Extended sequence\n");}
    | stmt {printf("Merged sequence\n");}
    ;

stmt: PRINT STRING {std::cout << "Created print stmt, string: " << $2 << std::endl;}
    | assignable_expr '=' expr {printf("Created assign const expr\n");}
    | assignable_expr_list '=' expr_list {printf("Created chunk assignment\n");}
    | IF expr THEN seq1 if_else_end_sequence {printf("Merged into single IF\n");}
    | function_call {std::cout << "Statement from func call\n";}
    | return_stmt {std::cout << "Return statement found\n";}
    ;

if_else_end_sequence: END {printf("Found END\n");}
    | ELSE seq1 if_else_end_sequence {printf("Merged else into if_else_end_sequence\n");}
    | ELSEIF expr THEN seq1 if_else_end_sequence {printf("Merged elseif into if_else_end_sequence\n");}
    ;

expr: expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr VAR_CONCAT expr {std::cout << "Concat vars\n";}
    | '(' expr ')'
    | INT
    | DOUBLE
    | STRING
    | NIL {std::cout << "nil value found\n";}
    | BOOL
    | assignable_expr
    | function_call  {std::cout << "Expression from function call\n";}
    ;

expr_list: expr
    | expr_list ',' expr
    | assignable_expr_list
    ;

assignable_expr: IDENTIFIER
    | PRINT
    ;

assignable_expr_list: assignable_expr
    | assignable_expr_list ',' assignable_expr
    ;

function_call: assignable_expr '(' expr_list ')' {std::cout << "Merged function call\n";}
    ;

return_stmt: RETURN
    | RETURN expr
    ;


%%

void yyerror(char const *s)
{
	printf("%s",s);
}