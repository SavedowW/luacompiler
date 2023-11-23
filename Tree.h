#define YYERROR_VERBOSE
#ifndef TREE_H_
#define TREE_H_
#include <string>
#include <vector>

class DoublePtrString
{
public:
	DoublePtrString clone() const;

	char *begin;
	char *end;
};

std::ostream& operator<< (std::ostream& out_, const DoublePtrString& s_);

// Список всех видов выражений
enum class EXPRESSION_TYPE {
	INT,
	DOUBLE,
	IDENTIFIER,
	BIN_PLUS,
	BIN_MINUS,
	BIN_MUL,
	BIN_DIV,
	UNAR_LEN
};

// Список всех типов statement'ов
enum class STATEMENT_TYPE {
	ASSIGN,
	STMT_LIST
};


/*
	Класс выражения

	У каждого выражения должен быть type
	isAssignable опеределяет, является ли выражение Expr или AssignableExpr
	iValue, fValue, identifier используются только для литерал (значений в коде)
	left используется для бинарных операторов для левого элемента и для унарных
	right используется для бинарных операторов для правого элемента
*/
class Expression
{
public:
	bool isAssignable = false;
	EXPRESSION_TYPE type;
	int iValue;
	double fValue;
	std::string identifier;
	Expression *left;
	Expression *right;
};

// Класс списка выражений
class ExpressionList
{
public:
	std::vector<Expression*> lst;
};

// Абстрактный класс Statement
class Statement
{
public:
	STATEMENT_TYPE type;
	virtual ~Statement() = default;
};

// Список statement'ов
class StatementList : public Statement
{
public:
	std::vector<Statement*> lst;
	virtual ~StatementList() = default;
};

// Присваивание
class StatementAssign : public Statement
{
public:
	Expression* left = nullptr;
	ExpressionList* right = nullptr;
	virtual ~StatementAssign() = default;
};

// Класс программы, с которого начинается дерево
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
	ExpressionList *AppendExprToList(ExpressionList *lst, Expression *expr);
	ExpressionList *CreateExprList(Expression *expr);
	ExpressionList *CreateExprList();
	Statement *CreateAssignStatement(Expression *left_, ExpressionList *right_);
	Expression *CreateExpr(EXPRESSION_TYPE exprType_, Expression *left_,Expression *right_);
	Expression *CreateExpr(EXPRESSION_TYPE exprType_, Expression *left_);
	Expression *CreateConstExp(int value);
	Expression *CreateConstExp(double value);
	Expression *CreateIdfExp(const char *str_);
};

#endif