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
%type <stm>named_function_definition
%type <expr>unnamed_function_definition
%type <expr>key_value_association
%type <expr>key_value_association_list
%type <expr>table_construct
%type <exp>expr
%type <explst>expr_list
%type <explst>param_list_no_vararg
%type <explst>param_list
%type <explst>assignable_expr_list
%type <exp>assignable_expr
%type <exp>function_call
%token PRINT
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
%token VAR_CONCAT;
%token NIL;
%token RETURN;
%token TRUE;
%token FALSE;
%token NOT;
%token EQUALS;
%token GREATER;
%token GREATER_EQUALS;
%token LESS;
%token LESS_EQUALS;
%token FLOOR_DIVISION;
%token BITWISE_LEFT_SHIFT;
%token BITWISE_RIGHT_SHIFT;
%left AND OR
%left '-' '+'
%left '*' '/'
%nonassoc ')' '='
%left ','
%left '[' ']'
%%
start: seq

seq: seq1
    ;

seq1: seq1 stmt {printf("Extended sequence\n");}
    | stmt {printf("Merged sequence\n");}
    ;

stmt: PRINT STRING {std::cout << "Created print stmt, string: " << $2 << std::endl;}
    | assignable_expr '=' expr {printf("Created assign const expr\n");}
    | assignable_expr_list '=' expr_list {printf("Created chunk assignment\n");}
    | IF expr THEN seq1 if_else_end_sequence {printf("Merged into single IF\n");}
    | IF expr THEN seq1 END {printf("Merged into single IF\n");}
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

if_else_end_sequence:
    | ELSE seq1 END {printf("Merged else into if_else_end_sequence\n");}
    | ELSEIF expr THEN seq1 if_else_end_sequence {printf("Merged elseif into if_else_end_sequence\n");}
    | ELSEIF expr THEN seq1 END {printf("Merged elseif into if_else_end_sequence\n");}
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
    | expr EQUALS expr
    | expr GREATER expr
    | expr GREATER_EQUALS expr
    | expr LESS expr
    | expr LESS_EQUALS expr
    | expr FLOOR_DIVISION expr
    | expr BITWISE_LEFT_SHIFT expr
    | expr BITWISE_RIGHT_SHIFT expr
    | '~' expr
    | '#' expr
    | expr AND expr
    | expr OR expr
    | NOT expr
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

expr_list: expr
    | expr_list ',' expr
    | assignable_expr_list
    ;

assignable_expr: IDENTIFIER
    | LOCAL IDENTIFIER {std::cout << "Local identifier found\n";};
    | assignable_expr '.' assignable_expr {std::cout << "Found . call\n";};
    | assignable_expr ':' assignable_expr {std::cout << "Found : call\n";};
    | assignable_expr '[' expr ']'
    | PRINT
    ;

assignable_expr_list: assignable_expr
    | assignable_expr_list ',' assignable_expr
    ;

function_call: assignable_expr '(' expr_list ')' {std::cout << "Merged function call\n";}
    | assignable_expr '('')' {std::cout << "Merged function call\n";}
    | assignable_expr STRING {std::cout << "Merged function call\n";}
    | assignable_expr table_construct {std::cout << "Merged function call\n";}
    ;

named_function_definition: FUNCTION IDENTIFIER '(' param_list ')' seq1 END {std::cout << "Merged function definition\n";}
    ;

unnamed_function_definition: FUNCTION '(' param_list ')' seq1 END {std::cout << "Merged function definition\n";}
    ;

return_stmt: RETURN
    | RETURN expr
    ;

param_list: param_list_no_vararg {std::cout << "Created final param list\n";}
    | param_list_no_vararg ',' VARARG_PARAM {std::cout << "Created final param list with vararg\n";}
    |
    ;

param_list_no_vararg: IDENTIFIER {std::cout << "Created param list\n";}
    | param_list_no_vararg ',' IDENTIFIER  {std::cout << "Extended param list\n";}
    |
    ;

key_value_association: '[' expr ']' '=' expr
    | IDENTIFIER '=' expr
    ;

key_value_association_list: key_value_association
    | key_value_association_list ',' key_value_association
    | /* empty */
    ;

table_construct: '{' key_value_association_list '}'
;

%%

void yyerror(char const *s)
{
	printf("%s",s);
}