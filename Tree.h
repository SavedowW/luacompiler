#define YYERROR_VERBOSE
#ifndef TREE_H_
#define TREE_H_
#include <string>
#include <vector>

class DoublePtrString
{
public:
	DoublePtrString() = default;
	DoublePtrString(const char *str_);
	DoublePtrString(char *begin_, char *end_);

	char *begin;
	char *end;

	DoublePtrString clone() const;
};

std::ostream& operator<< (std::ostream& out_, const DoublePtrString& s_);

// Список всех видов выражений
enum class EXPRESSION_TYPE {
	INT,
	DOUBLE,
	STRING,
	BOOL,
	NIL,
	IDENTIFIER,
	BIN_PLUS,
	BIN_MINUS,
	BIN_MUL,
	BIN_DIV,
	BIN_CONCAT,
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
	BITWISE_RIGHT_SHIFT,
	KEY_VALUE_ASSOC,
	TABLE_CONSTRUCT,
	METHOD_NAME,
	FUNCTION_CALL,
	UNNAMED_FUNCTION_DEFINITION
};

// Список всех типов statement'ов
enum class STATEMENT_TYPE {
	ASSIGN,
	STMT_LIST,
	RETURN
};

class ExpressionList;
class StatementList;
class ParamList;

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
	bool bValue;
	std::string identifier;
	Expression *left;
	Expression *right;
	ExpressionList *lst;
	StatementList *code;
	ParamList *params;
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

class ParamList
{
public:
	std::vector<const char*> lst;
	bool hasVararg = false;
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
	Expression *CreateExpr(EXPRESSION_TYPE exprType_, Expression *left_, Expression *right_);
	Expression *CreateExpr(EXPRESSION_TYPE exprType_, Expression *left_);
	Expression *CreateKeyValueAssoc(Expression *left_, Expression *right_);
	Expression *CreateKeyValueAssoc(const char *identifier_, Expression *right_);
	Expression *CreateTableContruct(ExpressionList *lst_);
	Expression *CreateConstExp(int value);
	Expression *CreateConstExp(double value);
	Expression *CreateConstExp(DoublePtrString value);
	Expression *CreateConstExp(bool value);
	Expression *CreateNil();
	Expression *CreateIdfExp(const char *str_);
	Expression *MakeConstant(Expression *expr);
	Expression *GetCell(Expression *expr_, Expression *rhs_);
	Expression *GetCell(Expression *expr_, const char *identifier_);
	Expression *CreateMethodName(Expression *expr_, const char *name_);
	Expression *CreateFunctionCall(Expression *callableName_, ExpressionList *args_);
	Expression *CreateFunctionCall(Expression *callableName_, DoublePtrString arg_);
	Expression *CreateFunctionCall(Expression *callableName_, Expression *tblArg_);

	ParamList *CreateParamList(const char *identifier_);
	ParamList *CreateParamList();
	ParamList *AppendParamList(ParamList *plst_, const char *identifier_);
	ParamList *AddVarargToParamList(ParamList *plst_);
	Expression *CreateUnnamedFunctionDefinition(ParamList *params_, StatementList *code_);

};

#endif