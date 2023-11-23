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
    ParamList *paramlst;
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
%type <exp>unnamed_function_definition
%type <exp>function_name
%type <exp>callable_name
%type <exp>key_value_association
%type <explst>key_value_association_list
%type <explst>key_value_association_listE
%type <exp>table_construct
%type <explst>expr_list
%type <explst>expr_listE
%type <paramlst>param_list_no_vararg
%type <paramlst>param_list
%type <paramlst>param_listE
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

chunk: /* empty */ {printf("Merged empty chunk\n"); $$ = TreeFactory::CreateStList();}
    | block
    ;

block: block_noret {printf("Finished block without return\n");}
    | block_noret return_stmt {printf("Finished block with return\n"); $$ = TreeFactory::AppendStatementToList($1, $2);}
    | return_stmt {printf("Finished block with only return\n"); $$ = TreeFactory::CreateStList($1);}
    ;

block_noret: block stmt {printf("Extended sequence\n"); $$ = TreeFactory::AppendStatementToList($1, $2);}
    | stmt {printf("Merged sequence\n"); $$ = TreeFactory::CreateStList($1);}
    ;

stmt: assignable_expr '=' expr_listE {printf("Created assign const expr\n"); $$ = TreeFactory::CreateAssignStatement($1, $3);}
    | LOCAL assignable_expr '=' expr_listE {printf("Created assign const expr to local\n");}
    | assignable_expr_list '=' expr_listE {printf("Created chunk assignment\n");}
    | LOCAL assignable_expr_list '=' expr_listE {printf("Created chunk assignment to local\n");}
    | if_stmt {printf("Merged single IF into stmt\n");}
    | WHILE expr DO chunk END {printf("Merged into single WHILE\n");}
    | REPEAT chunk UNTIL expr {printf("Merged into single REPEAT\n");}
    | FOR IDENTIFIER '=' expr ',' expr DO chunk END {printf("Merged into single FOR\n");}
    | FOR IDENTIFIER '=' expr ',' expr ',' expr DO chunk END {printf("Merged into single FOR with step\n");}
    | FOR param_list_no_vararg IN expr DO chunk END {printf("Merged into single generic FOR\n");}
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
    | if_unfinished ELSE chunk END {printf("Merged into if_stmt\n");}
    ;

if_unfinished: IF expr THEN chunk {printf("Merged initial if_unfinished\n");}
    | if_unfinished ELSEIF expr THEN chunk {printf("Extended if_unfinished\n");}
    ;

expr: expr '+' expr     {printf("Merged into single +\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_PLUS, $1, $3);}
    | expr '-' expr     {printf("Merged into single -\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_MINUS, $1, $3);}
    | expr '*' expr     {printf("Merged into single *\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_MUL, $1, $3);}
    | expr '/' expr     {printf("Merged into single /\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_DIV, $1, $3);}
    | expr '%' expr     {printf("Merged into single %\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_REM_DIV, $1, $3);} 
    | expr '^' expr     {printf("Merged into single ^\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_EXPON, $1, $3);}
    | expr '&' expr     {printf("Merged into single &\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_AND, $1, $3);}
    | expr '|' expr     {printf("Merged into single |\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_OR, $1, $3);}
    | expr '~' expr     {printf("Merged into single ~\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_NOT, $1, $3);}
    | '-' expr %prec UMINUS         {printf("Merged into single UMINUS\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::UNAR_UMINUS, $2);}
    | '~' expr %prec BITWISE_UNOT   {printf("Merged into single BITWISE_UNOT\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::UNAR_BITWISE_NOT, $2);}
    | expr EQUALS expr          {printf("Merged into single ==\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::REL_EQUALS, $1, $3);}
    | expr NOT_EQUALS expr    {printf("Merged into single ~=\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::REL_NOT_EQUALS, $1, $3);}
    | expr GREATER expr         {printf("Merged into single >\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::REL_GREATER, $1, $3);}
    | expr GREATER_EQUALS expr  {printf("Merged into single >=\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::REL_GREATER_EQUALS, $1, $3);}
    | expr LESS expr            {printf("Merged into single <\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::REL_LESS, $1, $3);}
    | expr LESS_EQUALS expr     {printf("Merged into single <=\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::REL_LESS_EQUALS, $1, $3);}
    | expr FLOOR_DIVISION expr  {printf("Merged into single //\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_FLOOR_DIVISION, $1, $3);}
    | expr BITWISE_LEFT_SHIFT expr      {printf("Merged into single <<\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BITWISE_LEFT_SHIFT, $1, $3);}
    | expr BITWISE_RIGHT_SHIFT expr     {printf("Merged into single >>\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BITWISE_RIGHT_SHIFT, $1, $3);}
    | '#' expr          {printf("Merged into single #\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::UNAR_LEN, $2);}
    | expr AND expr     {printf("Merged into single AND\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::LOG_AND, $1, $3);}
    | expr OR expr      {printf("Merged into single OR\n"); $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::LOG_OR, $1, $3);}
    | NOT expr          {std::cout << "Merged into single NOT\n"; $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::LOG_NOT, $2);}
    | expr VAR_CONCAT expr {std::cout << "Concat vars\n"; $$ = TreeFactory::CreateExpr(EXPRESSION_TYPE::BIN_CONCAT, $1, $3);}
    | '(' expr ')'      {printf("Merged into single ()\n"); $$ = $2;}
    | INT       {$$ = TreeFactory::CreateConstExp($1);}
    | DOUBLE    {$$ = TreeFactory::CreateConstExp($1);}
    | STRING    {$$ = TreeFactory::CreateConstExp($1);}
    | NIL {std::cout << "nil value found\n"; $$ = TreeFactory::CreateNil();}
    | BOOL {$$ = TreeFactory::CreateConstExp($1);}
    | assignable_expr {$$ = TreeFactory::MakeConstant($1);}
    | function_call  {std::cout << "Expression from function call\n"; $$ = $1;}
    | unnamed_function_definition {std::cout << "Unnamed function definition\n"; $$ = $1;} // TODO:
    | table_construct {$$ = $1;}
    ;

expr_list: /* empty */ {std::cout << "Merged empty expr_list\n"; $$ = TreeFactory::CreateExprList();}
    | expr_listE {std::cout << "Merged expr_list from E\n"; $$ = $1;}
    ;

expr_listE: expr {std::cout << "Merged expr_listE\n";  $$ = TreeFactory::CreateExprList($1);}
    | expr_listE ',' expr {std::cout << "Merged expr_listE\n";  $$ = TreeFactory::AppendExprToList($1, $3);}
    ;

// Присвоение возможно:
// Переменной
// Элементу таблицы (обращение через . или [])
// Таблица может быть возвращена из функции, может быть элементом таблицы и может быть значением переменной
assignable_expr: IDENTIFIER {$$ = TreeFactory::CreateIdfExp($1);}
    | assignable_expr '.' IDENTIFIER  {std::cout << "Merged table member from dot\n"; $$ = TreeFactory::GetCell($1, $3);}
    | assignable_expr '[' expr ']' {std::cout << "Merged table member from []\n"; $$ = TreeFactory::GetCell($1, $3);}
    | function_call '.' IDENTIFIER  {std::cout << "Merged table member from dot (func call)\n";}
    | function_call '[' expr ']' {std::cout << "Merged table member from [] (func call)\n";}
    ;

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
callable_name: assignable_expr {std::cout << "Merged callable from assignable expr\n"; $$ = $1;}
    | assignable_expr ':' IDENTIFIER {std::cout << "Merged callable from function name\n"; $$ = TreeFactory::CreateMethodName($1, $3);}
    | function_call {std::cout << "Merged callable from function call\n"; $$ = $1;}
    | function_call ':' IDENTIFIER {std::cout << "Merged callable from function_call:function_name\n"; $$ = TreeFactory::CreateMethodName($1, $3);}
    ;

function_call: callable_name '(' expr_list ')' {std::cout << "Merged function call\n"; $$ = TreeFactory::CreateFunctionCall($1, $3);}
    | callable_name STRING {std::cout << "Merged function call\n"; $$ = TreeFactory::CreateFunctionCall($1, $2);}
    | callable_name table_construct {std::cout << "Merged function call\n"; $$ = TreeFactory::CreateFunctionCall($1, $2);}
    ;

named_function_definition: FUNCTION function_name '(' param_list ')' chunk END {std::cout << "Merged function definition\n";}
    | FUNCTION function_name ':' IDENTIFIER '(' param_list ')' chunk END {std::cout << "Merged function definition with :\n";}
    ;

unnamed_function_definition: FUNCTION '(' param_list ')' chunk END {std::cout << "Merged function definition\n"; $$ = TreeFactory::CreateUnnamedFunctionDefinition($3, $5);}
    ;

return_stmt: RETURN expr_list {std::cout << "Merged return\n"; $$ = TreeFactory::CreateReturnStatement($2);}
    ;

param_list:  /* empty */ {$$ = TreeFactory::CreateParamList();}
    | param_listE {$$ = $1;}
    | param_list_no_vararg {std::cout << "Created final param list without vararg\n"; $$ = $1;}
    ;

param_listE: param_list_no_vararg ',' VARARG_PARAM {std::cout << "Created final param list with vararg\n"; $$ = TreeFactory::AddVarargToParamList($1);}
    ;

param_list_no_vararg: IDENTIFIER {std::cout << "Created param list\n"; $$ = TreeFactory::CreateParamList($1);}
    | param_list_no_vararg ',' IDENTIFIER  {std::cout << "Extended param list\n"; $$ = TreeFactory::AppendParamList($1, $3);}
    ;

key_value_association: '[' expr ']' '=' expr {$$ = TreeFactory::CreateKeyValueAssoc($2, $5);}
    | IDENTIFIER '=' expr {std::cout << "HERE: " << $1 << std::endl; $$ = TreeFactory::CreateKeyValueAssoc($1, $3);}
    ;

key_value_association_list: /* empty */ {$$ = TreeFactory::CreateExprList();}
    | key_value_association_listE {$$ = $1;}
    ;

key_value_association_listE: key_value_association {$$ = TreeFactory::CreateExprList($1);}
    | key_value_association_listE ',' key_value_association {$$ = TreeFactory::AppendExprToList($1, $3);}
    ;

table_construct: '{' key_value_association_list '}' {$$ = TreeFactory::CreateTableContruct($2);}
;

%%

void yyerror(char const *s)
{
	printf("%s",s);
}