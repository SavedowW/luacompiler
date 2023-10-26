#ifndef TREE_H_
#define TREE_H_
#include <string>

enum class EXPRESSION_TYPE {
	INT,
	DOUBLE,
	IDENTIFIER,
	BIN_PLUS,
	BIN_MINUS,
	BIN_MUL,
	BIN_DIV,
	BIN_ASSIGN
};

enum class STATEMENT_TYPE {
	PRINT,
	EXPR
};

class Expression
{
public:
	std::string toString() const;

	EXPRESSION_TYPE type;
	int iValue;
	double fValue;
	std::string identifier;
	Expression *left;
	Expression *right;
};

class Statement
{
public:
	std::string toString() const;

	STATEMENT_TYPE type;
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
	Statement *CreatePrintStatement(Expression *exp);
	Statement *CreateAssignStatement(Expression *exp1, Expression *exp2);
	Expression *CreateBinExp(EXPRESSION_TYPE exprType_, Expression *left,Expression *right);
	Expression *CreateConstExp(int value);
	Expression *CreateConstExp(double value);
	Expression *CreateIdfExp(const char *str_);
};

#endif