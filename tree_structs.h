#ifndef SIMPLE_TREE_STRUCTS
#define SIMPLE_TREE_STRUCTS
class Expression
{
public:
	int type;//0 - int, 1 +, 2-, 3*, 4/
	int value;
	Expression *left;
	Expression *right;
};

class Statement
{
public:
	Expression *to_print;
};

class Statements_List
{
public:
	Statement *stm;
	Statements_List *next;
};

class Program
{
public:
	Statements_List *stmts;
};
#endif