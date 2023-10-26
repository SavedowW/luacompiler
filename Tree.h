#ifndef TREE_H_
#define TREE_H_
#include <string>
#include <vector>

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
	EXPR,
	STMT_LIST
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
	virtual std::string toString() const = 0;

	STATEMENT_TYPE type;
};

class SingleExprStatement : public Statement
{
public:
	virtual std::string toString() const override;

	Expression *to_print;
};

class StatementList : public Statement
{
public:
	virtual std::string toString() const override;

	std::vector<Statement*> lst;
};

class Program
{
public:
	StatementList *stmts;
};

namespace TreeFactory
{
	Program *CreateProgram(StatementList *lst);
	StatementList *AppendStatementToList(StatementList *lst, Statement *stm);
	StatementList *CreateStList(Statement *stm);
	Statement *CreatePrintStatement(Expression *exp);
	Statement *CreateAssignStatement(Expression *exp1, Expression *exp2);
	Expression *CreateBinExp(EXPRESSION_TYPE exprType_, Expression *left,Expression *right);
	Expression *CreateConstExp(int value);
	Expression *CreateConstExp(double value);
	Expression *CreateIdfExp(const char *str_);
};

#endif