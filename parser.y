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
%type <lst>seq1
%type <stm>stmt
%type <stm>return_stmt
%type <stm>if_stmt
%type <stm>if_unfinished
%type <stm>named_function_definition
%type <expr>unnamed_function_definition
%type <expr>function_name
%type <expr>callable_name
%type <expr>table_field
%type <expr>key_value_association
%type <expr>key_value_association_list
%type <expr>key_value_association_listE
%type <expr>table_construct
%type <explst>expr_list
%type <explst>expr_listE
%type <explst>param_list_no_vararg
%type <explst>param_list
%type <explst>param_listE
%type <explst>assignable_expr_list
%type <exp>expr
%type <exp>assignable_expr
%type <exp>function_call
%token <int_const>INT
%token <double_const>DOUBLE
%token <str_const>STRING
%token <bool_const>BOOL
%token <identifier>IDENTIFIER;
%token VARARG_PARAM;
%token FUNCTION;
%token LOCAL;
%token BREAK;
%token WHILE;
%token DO;
%token IN;
%token REPEAT;
%token UNTIL;
%token FOR;
%token IF;
%token ELSE;
%token ELSEIF;
%token THEN;
%token END;
%token NIL;
%token RETURN;
%token TRUE;
%token FALSE;
%left OR
%left AND
%left EQUALS;
%left GREATER;
%left GREATER_EQUALS;
%left LESS;
%left LESS_EQUALS;
%left '|';
%left '&';
%left '~';
%left BITWISE_LEFT_SHIFT;
%left BITWISE_RIGHT_SHIFT;
%left VAR_CONCAT;
%left '-' '+'
%left '*' '/' FLOOR_DIVISION '%'
%nonassoc NOT '#';
%left '^';
%nonassoc ')' '='
%left ','
%left '[' ']'
%%
start: seq1

seq1: seq1 stmt {printf("Extended sequence\n");}
    | stmt {printf("Merged sequence\n");}
    ;

stmt: assignable_expr '=' expr {printf("Created assign const expr\n");}
    | assignable_expr_list '=' expr_list {printf("Created chunk assignment\n");}
    | if_stmt {printf("Merged single IF into stmt\n");}
    | WHILE expr DO seq1 END {printf("Merged into single WHILE\n");}
    | REPEAT seq1 UNTIL expr {printf("Merged into single REPEAT\n");}
    | FOR IDENTIFIER '=' expr ',' expr DO seq1 END {printf("Merged into single FOR\n");}
    | FOR IDENTIFIER '=' expr ',' expr ',' expr DO seq1 END {printf("Merged into single FOR with step\n");}
    | FOR param_list_no_vararg IN expr DO seq1 END {printf("Merged into single generic FOR\n");}
    | function_call {std::cout << "Statement from func call\n";}
    | return_stmt {std::cout << "Return statement found\n";}
    | BREAK {std::cout << "BREAK statement found\n";}
    | named_function_definition
    ;

if_stmt: if_unfinished END {printf("Merged into if_stmt\n");}
    | if_unfinished ELSE seq1 END {printf("Merged into if_stmt\n");}
    ;

if_unfinished: IF expr THEN seq1 {printf("Merged initial if_unfinished\n");}
    | if_unfinished ELSEIF expr THEN seq1 {printf("Extended if_unfinished\n");}
    ;

expr: expr '+' expr
    | expr '-' expr
    | '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '%' expr
    | expr '^' expr
    | expr '&' expr
    | expr '|' expr
    | expr '~' expr
    | expr EQUALS expr {printf("Merged into single ==\n");}
    | expr GREATER expr {printf("Merged into single >\n");}
    | expr GREATER_EQUALS expr {printf("Merged into single >=\n");}
    | expr LESS expr {printf("Merged into single <\n");}
    | expr LESS_EQUALS expr {printf("Merged into single <=\n");}
    | expr FLOOR_DIVISION expr
    | expr BITWISE_LEFT_SHIFT expr
    | expr BITWISE_RIGHT_SHIFT expr
    | '~' expr
    | '#' expr
    | expr AND expr
    | expr OR expr
    | NOT expr {std::cout << "Merged into single NOT\n";}
    | expr VAR_CONCAT expr {std::cout << "Concat vars\n";}
    | '(' expr ')'
    | INT
    | DOUBLE
    | STRING
    | NIL {std::cout << "nil value found\n";}
    | BOOL
    | assignable_expr
    | function_call  {std::cout << "Expression from function call\n";}
    | unnamed_function_definition {std::cout << "Unnamed function definition\n";}
    | table_construct
    ;

expr_list: /* empty */
    | expr_listE {std::cout << "Merged expr_list from E\n";}
    ;

expr_listE: expr {std::cout << "Merged expr_listE\n";}
    | expr_listE ',' expr {std::cout << "Merged expr_listE\n";}
    | assignable_expr_list {std::cout << "Merged expr_list\n";}
    ;

assignable_expr: IDENTIFIER
    | LOCAL IDENTIFIER {std::cout << "Local identifier found\n";};
    | table_field
    ;

assignable_expr_list: assignable_expr {std::cout << "Merged assignable_expr_list\n";}
    | assignable_expr_list ',' assignable_expr {std::cout << "Merged assignable_expr_list\n";}
    ;

function_name: assignable_expr {std::cout << "Merged function name\n";}
    | assignable_expr ':' IDENTIFIER {std::cout << "Merged function name\n";}
    ;

callable_name: function_name {std::cout << "Merged callable from function name\n";}
    | function_call {std::cout << "Merged callable from function call\n";}
    ;

function_call: callable_name '(' expr_list ')' {std::cout << "Merged function call\n";}
    | callable_name STRING {std::cout << "Merged function call\n";}
    | callable_name table_construct {std::cout << "Merged function call\n";}
    ;

named_function_definition: FUNCTION function_name '(' param_list ')' seq1 END {std::cout << "Merged function definition\n";}
    ;

unnamed_function_definition: FUNCTION '(' param_list ')' seq1 END {std::cout << "Merged function definition\n";}
    ;

return_stmt: RETURN
    | RETURN expr
    ;

param_list:  /* empty */ 
    | param_listE
    | param_list_no_vararg {std::cout << "Created final param list without vararg\n";}
    ;

param_listE: param_list_no_vararg ',' VARARG_PARAM {std::cout << "Created final param list with vararg\n";}
    ;

param_list_no_vararg: IDENTIFIER {std::cout << "Created param list\n";}
    | param_list_no_vararg ',' IDENTIFIER  {std::cout << "Extended param list\n";}
    ;

table_field: assignable_expr '.' IDENTIFIER  {std::cout << "Merged table member from dot\n";}
    | assignable_expr '[' expr ']' {std::cout << "Merged table member from []\n";}
    | function_call '.' IDENTIFIER  {std::cout << "Merged table member from dot (func call)\n";}
    | function_call '[' expr ']' {std::cout << "Merged table member from [] (func call)\n";}
    ;

key_value_association: '[' expr ']' '=' expr
    | IDENTIFIER '=' expr
    ;

key_value_association_list: /* empty */
    | key_value_association_listE
    ;

key_value_association_listE: key_value_association
    | key_value_association_listE ',' key_value_association
    ;

table_construct: '{' key_value_association_list '}'
;

%%

void yyerror(char const *s)
{
	printf("%s",s);
}