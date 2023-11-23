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
	StatementList *lst;
	Program *prg;
    const char *identifier;
    DoublePtrString str_const;
    bool bool_const;
}
%type <lst>block_noret
%type <lst>block
%type <lst>chunk
%type <stm>stmt
%type <stm>return_stmt
%type <stm>if_stmt
%type <stm>if_unfinished
%type <stm>goto_label;
%type <stm>goto_call;
%type <stm>named_function_definition
%type <expr>unnamed_function_definition
%type <expr>function_name
%type <expr>callable_name
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
%token QDOTS;
%token GOTO;
%left ','
%left OR
%left AND
%left EQUALS NOT_EQUALS GREATER GREATER_EQUALS LESS LESS_EQUALS;
%left '|';
%left '~';
%left '&';
%left BITWISE_LEFT_SHIFT BITWISE_RIGHT_SHIFT;
%right VAR_CONCAT;
%left '-' '+';
%left '*' '/' FLOOR_DIVISION '%';
%nonassoc UMINUS NOT '#' BITWISE_UNOT;
%right '^';
%%
start: chunk {prg = TreeFactory::CreateProgram($1);}

chunk: /* empty */
    | block
    ;

block: block_noret {printf("Finished block without return\n");}
    | block_noret return_stmt {printf("Finished block with return\n");}
    | return_stmt {printf("Finished block with only return\n");}
    ;

block_noret: block stmt {printf("Extended sequence\n"); $$ = TreeFactory::AppendStatementToList($1, $2);}
    | stmt {printf("Merged sequence\n"); $$ = TreeFactory::CreateStList($1);}
    ;

stmt: assignable_expr '=' expr_listE {printf("Created assign const expr\n"); $$ = TreeFactory::CreateAssignStatement($1, $3);}
    | LOCAL assignable_expr '=' expr_listE {printf("Created assign const expr to local\n");}
    | assignable_expr_list '=' expr_listE {printf("Created chunk assignment\n");}
    | LOCAL assignable_expr_list '=' expr_listE {printf("Created chunk assignment to local\n");}
    | if_stmt {printf("Merged single IF into stmt\n");}
    | WHILE expr DO block END {printf("Merged into single WHILE\n");}
    | REPEAT block UNTIL expr {printf("Merged into single REPEAT\n");}
    | FOR IDENTIFIER '=' expr ',' expr DO block END {printf("Merged into single FOR\n");}
    | FOR IDENTIFIER '=' expr ',' expr ',' expr DO block END {printf("Merged into single FOR with step\n");}
    | FOR param_list_no_vararg IN expr DO block END {printf("Merged into single generic FOR\n");}
    | function_call {std::cout << "Statement from func call\n";}
    | BREAK {std::cout << "BREAK statement found\n";}
    | named_function_definition
    | goto_call {std::cout << "Merged goto_call\n";}
    | goto_label {std::cout << "Merged goto_label\n";}
    ;

goto_label: QDOTS IDENTIFIER QDOTS
    ;

goto_call: GOTO IDENTIFIER
    ;

if_stmt: if_unfinished END {printf("Merged into if_stmt\n");}
    | if_unfinished ELSE block END {printf("Merged into if_stmt\n");}
    ;

if_unfinished: IF expr THEN block {printf("Merged initial if_unfinished\n");}
    | if_unfinished ELSEIF expr THEN block {printf("Extended if_unfinished\n");}
    ;

expr: expr '+' expr     {printf("Merged into single +\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_PLUS, $1, $3);}
    | expr '-' expr     {printf("Merged into single -\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_MINUS, $1, $3);}
    | expr '*' expr     {printf("Merged into single *\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_MUL, $1, $3);}
    | expr '/' expr     {printf("Merged into single /\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_DIV, $1, $3);}
    | expr '%' expr     {printf("Merged into single %\n");}
    | expr '^' expr     {printf("Merged into single ^\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_EXPON, $1, $3);}
    | expr '&' expr     {printf("Merged into single &\n");}
    | expr '|' expr     {printf("Merged into single |\n");}
    | expr '~' expr     {printf("Merged into single ~\n");}
    | '-' expr %prec UMINUS     {printf("Merged into single UMINUS\n");}
    | '~' expr %prec BITWISE_UNOT     {printf("Merged into single BITWISE_UNOT\n");}
    | expr EQUALS expr          {printf("Merged into single ==\n");}
    | expr NOT_EQUALS expr    {printf("Merged into single ~=\n");}
    | expr GREATER expr         {printf("Merged into single >\n");}
    | expr GREATER_EQUALS expr  {printf("Merged into single >=\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::REL_GREATER_EQUALS, $1, $3);}
    | expr LESS expr            {printf("Merged into single <\n");}
    | expr LESS_EQUALS expr     {printf("Merged into single <=\n");}
    | expr FLOOR_DIVISION expr
    | expr BITWISE_LEFT_SHIFT expr      {printf("Merged into single <<\n");}
    | expr BITWISE_RIGHT_SHIFT expr     {printf("Merged into single >>\n");}
    | '#' expr          {printf("Merged into single #\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::UNAR_LEN, $2);}
    | expr AND expr     {printf("Merged into single AND\n");}
    | expr OR expr      {printf("Merged into single OR\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::LOG_OR, $1, $3);}
    | NOT expr          {std::cout << "Merged into single NOT\n";}
    | expr VAR_CONCAT expr {std::cout << "Concat vars\n";}
    | '(' expr ')'      {printf("Merged into single ()\n"); $$ = $2;}
    | INT {$$ = TreeFactory::CreateConstExp($1);}
    | DOUBLE {$$ = TreeFactory::CreateConstExp($1);}
    | STRING
    | NIL {std::cout << "nil value found\n";}
    | BOOL
    | assignable_expr {$$ = $1;}
    | function_call  {std::cout << "Expression from function call\n";}
    | unnamed_function_definition {std::cout << "Unnamed function definition\n";}
    | table_construct
    ;

expr_list: /* empty */ {std::cout << "Merged empty expr_list\n"; $$ = TreeFactory::CreateExprList();}
    | expr_listE {std::cout << "Merged expr_list from E\n";}
    ;

expr_listE: expr {std::cout << "Merged expr_listE\n";  $$ = TreeFactory::CreateExprList($1);}
    | expr_listE ',' expr {std::cout << "Merged expr_listE\n";  $$ = TreeFactory::AppendExprToList($1, $3);}
    ;

// Присвоение возможно:
// Переменной
// Элементу таблицы (обращение через . или [])
// Таблица может быть возвращена из функции, может быть элементом таблицы и может быть значением переменной
assignable_expr: IDENTIFIER {$$ = TreeFactory::CreateIdfExp($1);}
    | assignable_expr '.' IDENTIFIER  {std::cout << "Merged table member from dot\n";}
    | assignable_expr '[' expr ']' {std::cout << "Merged table member from []\n";}
    | function_call '.' IDENTIFIER  {std::cout << "Merged table member from dot (func call)\n";}
    | function_call '[' expr ']' {std::cout << "Merged table member from [] (func call)\n";}
    ; // TODO: разобраться с function_name, callable_name, function_call, table_field
    // Уменьшить количество нетерминалов

assignable_expr_list: assignable_expr ',' assignable_expr {std::cout << "Merged assignable_expr_list\n";}
    | assignable_expr_list ',' assignable_expr {std::cout << "Extended assignable_expr_list\n";}
    ;


// В имени функции в определении может быть:
// IDENTIFIER '.' IDENTIFIER '.' ... '.' IDENTIFIER ':' IDENTIFIER (т.е. не может быть обращения к элементу таблицы через вызов функции или [])
function_name: IDENTIFIER {std::cout << "Merged function name\n";}
    | IDENTIFIER '.' IDENTIFIER {std::cout << "Merged function name\n";}
    ;

// В имени функции в определении может быть:
// Все, что есть в function_name
// Вызовы функций
// Обращения к элементам таблиц через []
// ':'
callable_name: assignable_expr {std::cout << "Merged callable from assignable expr\n";}
    | assignable_expr ':' IDENTIFIER {std::cout << "Merged callable from function name\n";}
    | function_call {std::cout << "Merged callable from function call\n";}
    ;

function_call: callable_name '(' expr_list ')' {std::cout << "Merged function call\n";}
    | callable_name STRING {std::cout << "Merged function call\n";}
    | callable_name table_construct {std::cout << "Merged function call\n";}
    ;

named_function_definition: FUNCTION function_name '(' param_list ')' block END {std::cout << "Merged function definition\n";}
    | FUNCTION function_name ':' IDENTIFIER '(' param_list ')' block END {std::cout << "Merged function definition with :\n";}
    ;

unnamed_function_definition: FUNCTION '(' param_list ')' block END {std::cout << "Merged function definition\n";}
    ;

return_stmt: RETURN expr_list {std::cout << "Merged return\n";}
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