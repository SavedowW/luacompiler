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
	STRING,
	IDENTIFIER,
	BIN_PLUS,
	BIN_MINUS,
	BIN_MUL,
	BIN_DIV,
	CELL_BY_IDENTIFIER,
	CELL_BY_EXPR,
	BIN_REM_DIV,
	BIN_EXPON,
	BIN_AND,
	BIN_OR,
	BIN_NOT,
	LOG_AND,
	LOG_OR,
	LOG_NOT,
	UNAR_UMINUS,
	UNAR_BITWISE_NOT,
	UNAR_LEN,
	REL_EQUALS,
	REL_NOT_EQUALS,
	REL_GREATER,
	REL_GREATER_EQUALS,
	REL_LESS,
	REL_LESS_EQUALS,
	BIN_FLOOR_DIVISION,
	BITWISE_LEFT_SHIFT,
	BITWISE_RIGHT_SHIFT
};

// Список всех типов statement'ов
enum class STATEMENT_TYPE {
	ASSIGN,
	STMT_LIST,
	RETURN
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
	DoublePtrString sValue;
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

// Возврат
class StatementReturn : public Statement
{
public:
	ExpressionList* lst = nullptr;
	virtual ~StatementReturn() = default;
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
	StatementList *CreateStList();
	ExpressionList *AppendExprToList(ExpressionList *lst, Expression *expr);
	ExpressionList *CreateExprList(Expression *expr);
	ExpressionList *CreateExprList();
	Statement *CreateAssignStatement(Expression *left_, ExpressionList *right_);
	Statement *CreateReturnStatement(ExpressionList *lst_);
	Expression *CreateExpr(EXPRESSION_TYPE exprType_, Expression *left_,Expression *right_);
	Expression *CreateExpr(EXPRESSION_TYPE exprType_, Expression *left_);
	Expression *CreateConstExp(int value);
	Expression *CreateConstExp(double value);
	Expression *CreateConstExp(DoublePtrString value);
	Expression *CreateIdfExp(const char *str_);
	Expression *MakeConstant(Expression *expr);
	Expression *GetCell(Expression *expr_, Expression *rhs_);
	Expression *GetCell(Expression *expr_, const char *identifier_);
};

#endif