#ifndef TREE_H_
#define TREE_H_


enum class EXPRESSION_TYPE {
	INT,
	BIN_PLUS,
	BIN_MINUS,
	BIN_MUL,
	BIN_DIV
};

class Expression
{
public:
	EXPRESSION_TYPE type;
	int value;
	Expression *left;
	Expression *right;
};

class Statement
{
public:
	Expression *to_print;
};

class StatementsList
{
public:
	Statement *stm;
	StatementsList *next;
};

class Program
{
public:
	StatementsList *stmts;
};

namespace TreeFactory
{
	Program *CreateProgram(StatementsList *lst);
	StatementsList *AppendStatementToList(StatementsList *lst,Statement *stm);
	StatementsList *CreateStList(Statement *stm);
	Statement *CreateStatement(Expression *exp);
	Expression *CreateBinExp(EXPRESSION_TYPE exprType_, Expression *left,Expression *right);
	Expression *CreateIntExp(int value);
};

#endif