#define YYERROR_VERBOSE
#ifndef TREE_H_
#define TREE_H_
#include <string.h>
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

	void printWithoutQuotes(std::ostream& out_);

	bool operator==(const DoublePtrString &rhs_) const;
	bool operator==(const std::string &rhs_) const;

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
	UNNAMED_FUNCTION_DEFINITION,
	VARARG_REF
};

// Список всех типов statement'ов
enum class STATEMENT_TYPE {
	ASSIGN,
	MULTIPLE_ASSIGN,
	STMT_LIST,
	STRETURN,
	FUNCTION_CALL,
	BREAK,
	IF_ELSE,
	WHILE_LOOP,
	REPEAT_LOOP,
	FOR_NUMERIC,
	FOR_EACH,
	GOTO_LABEL,
	GOTO_CALL
};

class ExpressionList;
class StatementList;
class ParamList;
class VarsContext;

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
	Expression();

	bool isAssignable = false;
	EXPRESSION_TYPE type;
	int iValue;
	float fValue;
	DoublePtrString sValue;
	bool bValue;
	std::string identifier;
	Expression *left = nullptr;
	Expression *right = nullptr;
	ExpressionList *lst = nullptr;
	StatementList *code = nullptr;
	ParamList *params = nullptr;

	std::string name;
	static int lastID;
	VarsContext *varContext = nullptr;
};

// Класс списка выражений
class ExpressionList
{
public:
	ExpressionList();

	std::vector<Expression*> lst;

	std::string name;
	static int lastID;
};

// Абстрактный класс Statement
class Statement
{
public:
	Statement();	
	STATEMENT_TYPE type;
	virtual ~Statement() = default;

	std::string name;
	static int lastID;
};

// Список statement'ов
class StatementList : public Statement
{
public:
	std::vector<Statement*> lst;
	virtual ~StatementList() = default;

	VarsContext *ownContext = nullptr;;
};

// Присваивание
class StatementAssign : public Statement
{
public:
	bool isLocal = false;
	Expression* left = nullptr;
	ExpressionList* right = nullptr;
	virtual ~StatementAssign() = default;
};

class StatementMultipleAssign : public Statement
{
public:
	bool isLocal = false;
	ExpressionList* left = nullptr;
	ExpressionList* right = nullptr;
	virtual ~StatementMultipleAssign() = default;
};

// Возврат
class StatementReturn : public Statement
{
public:
	ExpressionList* lst = nullptr;
	virtual ~StatementReturn() = default;
};

class StatementFunctionCall : public Statement
{
public:
	Expression *functionName;
	ExpressionList *lst;
	
	virtual ~StatementFunctionCall() = default;
};

class StatementIfElse : public Statement
{
public:
	Expression* condition = nullptr;
	StatementList* trueCode = nullptr;
	StatementList* falseCode = nullptr;
	std::vector<StatementIfElse*> elseifs;
	virtual ~StatementIfElse() = default;
};

class StatementWhileLoop : public Statement
{
public:
	Expression* condition = nullptr;
	StatementList* trueCode = nullptr;
	virtual ~StatementWhileLoop() = default;
};

class StatementRepeatLoop : public Statement
{
public:
	Expression* condition = nullptr;
	StatementList* trueCode = nullptr;
	virtual ~StatementRepeatLoop() = default;
};

class ParamList
{
public:
	ParamList();

	std::vector<const char*> lst;
	bool hasVararg = false;

	std::string name;
	static int lastID;
};

class StatementForLoop : public Statement
{
public:
	Expression* begin = nullptr;
	Expression* end = nullptr;
	Expression* step = nullptr;
	StatementList* code = nullptr;
	std::string identifier;
	VarsContext *iterContext = nullptr;
	virtual ~StatementForLoop() = default;
};

class StatementForeachLoop : public Statement
{
public:
	ParamList* params = nullptr;
	Expression* data = nullptr;
	StatementList* code = nullptr;
	virtual ~StatementForeachLoop() = default;
};

class StatementGotoLabel : public Statement
{
public:
	std::string identifier;
	virtual ~StatementGotoLabel() = default;
};

class StatementGotoCall : public Statement
{
public:
	std::string identifier;
	virtual ~StatementGotoCall() = default;
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
	ExpressionList *CreateExprList(Expression *expr1_, Expression *expr2_);
	ExpressionList *CreateExprList(Expression *expr);
	ExpressionList *CreateExprList();
	Statement *CreateAssignStatement(Expression *left_, ExpressionList *right_, bool isLocal_);
	Statement *CreateAssignStatement(ExpressionList *left_, ExpressionList *right_, bool isLocal_);
	Statement *CreateReturnStatement(ExpressionList *lst_);
	Expression *CreateExpr(EXPRESSION_TYPE exprType_, Expression *left_, Expression *right_);
	Expression *CreateExpr(EXPRESSION_TYPE exprType_, Expression *left_);
	Expression *CreateKeyValueAssoc(Expression *left_, Expression *right_);
	Expression *CreateKeyValueAssoc(const char *identifier_, Expression *right_);
	Expression *CreateTableContruct(ExpressionList *lst_);
	Expression *CreateConstExp(int value);
	Expression *CreateConstExp(float value);
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
	Expression *CreateVarargRef();
	Statement *CreateFunctionCallStatement(Expression *call_);

	ParamList *CreateParamList(const char *identifier_);
	ParamList *CreateParamList();
	ParamList *AppendParamList(ParamList *plst_, const char *identifier_);
	ParamList *AddVarargToParamList(ParamList *plst_);
	Expression *CreateUnnamedFunctionDefinition(ParamList *params_, StatementList *code_);
	Statement *CreateNamedFunctionDefinition(Expression *functionName_, ParamList *params_, StatementList *code_);
	Statement *CreateNamedFunctionDefinition(Expression *functionName_, const char *identifier_, ParamList *params_, StatementList *code_);
	Statement *makeAssignmentLocal(Statement *assign_);

	Statement *makeBreakStatement();
	Statement *makeIfElseStatement(Expression *condition_, StatementList *trueCode_);
	Statement *addElseifToIfElseStatement(Statement *ifElse_, Expression *condition_, StatementList *trueCode_);
	Statement *addElseToIfElseStatement(Statement *ifElse_, StatementList *falseCode_);
	Statement *makeWhileLoopStatement(Expression *condition_, StatementList *trueCode_);
	Statement *makeRepeatLoopStatement(Expression *condition_, StatementList *trueCode_);
	Statement *makeForLoopStatement(const char *identifier_, Expression *begin_, Expression *end_, Expression *step_, StatementList *code_);
	Statement *makeForLoopStatement(ParamList *params_, Expression *data_, StatementList *code_);
	Statement *makeGotoLabel(const char *identifier_);
	Statement *makeGotoCall(const char *identifier_);

};

#endif