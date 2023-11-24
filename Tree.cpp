#include "Tree.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

DoublePtrString DoublePtrString::clone() const
{
	DoublePtrString s;
	int len = end-begin+1;
	s.begin = new char[len];
	s.end = s.begin + len;
	strncpy(s.begin, begin, len);
	return s;
}

DoublePtrString::DoublePtrString(const char *str_)
{
	int len = strlen(str_) + 1;
	begin = new char[strlen(str_)];
	end = begin + len;
	strncpy(begin, str_, len);
}

DoublePtrString::DoublePtrString(char *begin_, char *end_)
{
	begin = begin_;
	end = end_;
}

std::ostream& operator<< (std::ostream& out_, const DoublePtrString& s_)
{
    char *ptr = s_.begin;
	out_ << '"';
    while (ptr != s_.end - 1)
    {
        if (*ptr == '\0')
            out_ << '?';
        else
            out_ << *ptr;
        ptr++;
    }
    out_ << '"';
	return out_;
}

// Создание программы
// lst - основной блок кода программы
Program *TreeFactory::CreateProgram(StatementList *lst)
{
	std::cout << "Create program\n";
	Program *crt = new Program;
	crt->stmts = lst;
	return crt;
}

// Добавление statement'a ко списку
// lst - список
// stm - statement, который нужно добавить
StatementList *TreeFactory::AppendStatementToList(StatementList *lst, Statement *stm)
{
	std::cout << "Append to statement list\n";
	lst->lst.push_back(stm);
	return lst;
}

// Создание списка statement'ов
// stm - statement, с которого начинается список
StatementList *TreeFactory::CreateStList(Statement *stm)
{
    std::cout << "Created statement list\n";
	StatementList *lst = new StatementList;
	lst->type = STATEMENT_TYPE::STMT_LIST;
	lst->lst.push_back(stm);
	return lst;
}

// Создание пустого списка statement'ов
StatementList *TreeFactory::CreateStList()
{
    std::cout << "Created statement list\n";
	StatementList *lst = new StatementList;
	lst->type = STATEMENT_TYPE::STMT_LIST;
	return lst;
}

// Добавление выражения ко списку
// lst - список
// expr - выражение, которое нужно добавить
ExpressionList *TreeFactory::AppendExprToList(ExpressionList *lst, Expression *expr)
{
	std::cout << "Append to statement list\n";
	lst->lst.push_back(expr);
	return lst;
}

// Создание списка выражений
ExpressionList *TreeFactory::CreateExprList(Expression *expr1_, Expression *expr2_)
{
	std::cout << "Created expression list\n";
	ExpressionList *lst = new ExpressionList;
	lst->lst.push_back(expr1_);
	lst->lst.push_back(expr2_);
	return lst;
}

// Создание списка выражений
// expr - выражение, с которого начинается список
ExpressionList *TreeFactory::CreateExprList(Expression *expr)
{
	std::cout << "Created expression list\n";
	ExpressionList *lst = new ExpressionList;
	lst->lst.push_back(expr);
	return lst;
}

// Создание пустого списка выражений
ExpressionList *TreeFactory::CreateExprList()
{
	std::cout << "Created expression list\n";
	ExpressionList *lst = new ExpressionList;
	return lst;
}

// Создание элемента присваивания
// left_ - выражение, которому присваивается значение
// right_ - список присваиваемых значений
Statement *TreeFactory::CreateAssignStatement(Expression *left_, ExpressionList *right_, bool isLocal_)
{
	std::cout << "Created assign statement\n";
	StatementAssign *sa = new StatementAssign;
	sa->type = STATEMENT_TYPE::ASSIGN;
	sa->left = left_;
	sa->right = right_;
	sa->isLocal = isLocal_;
	return sa;
}

Statement *TreeFactory::CreateAssignStatement(ExpressionList *left_, ExpressionList *right_, bool isLocal_)
{
	std::cout << "Created multiple assign statement\n";
	StatementMultipleAssign *sa = new StatementMultipleAssign;
	sa->type = STATEMENT_TYPE::MULTIPLE_ASSIGN;
	sa->left = left_;
	sa->right = right_;
	sa->isLocal = isLocal_;
	return sa;
}

// Создание элемента return
// lst_ - список возвращаемых значений
Statement *TreeFactory::CreateReturnStatement(ExpressionList *lst_)
{
	std::cout << "Created return statement\n";
	StatementReturn *ret = new StatementReturn;
	ret->type = STATEMENT_TYPE::RETURN;
	ret->lst = (lst_->lst.size() > 0 ? lst_ : nullptr);
	return ret;
}

// Создание бинарного выражения
// exprType_ - тип выражения
// left_ - левая часть выражения
// right_ - правая часть выражения
Expression *TreeFactory::CreateExpr(EXPRESSION_TYPE exprType_, Expression *left_, Expression *right_)
{
	Expression *crt = new Expression;
	crt->type = exprType_;
	crt->left = left_;
	crt->right = right_;
	return crt;
}

// Создание унарного выражения
// exprType_ - тип выражения
// left_ - операнд выражения
Expression *TreeFactory::CreateExpr(EXPRESSION_TYPE exprType_, Expression *left_)
{
	Expression *crt = new Expression;
	crt->type=exprType_;
	crt->left=left_;
	return crt;
}

// Создание ассоциации между двумя выражениями в конструкторе таблицы
// left_ - операнд выражения
// right_ - операнд выражения
Expression *TreeFactory::CreateKeyValueAssoc(Expression *left_, Expression *right_)
{
	Expression *expr = new Expression;
	expr->type=EXPRESSION_TYPE::KEY_VALUE_ASSOC;
	expr->left = left_;
	expr->right = right_;
	return expr;
}

Expression *TreeFactory::CreateKeyValueAssoc(const char *identifier_, Expression *right_)
{
	std::cout << "Created key value assoc for " << identifier_ << std::endl;
	Expression *expr = new Expression;
	const char *idf2 = identifier_ + 1;
	expr->type=EXPRESSION_TYPE::KEY_VALUE_ASSOC;
	expr->left = new Expression;
	expr->left->type=EXPRESSION_TYPE::STRING;
	expr->left->sValue = DoublePtrString(identifier_);
	expr->right = right_;
	return expr;
}

Expression *TreeFactory::CreateTableContruct(ExpressionList *lst_)
{
	Expression *expr = new Expression;
	expr->type=EXPRESSION_TYPE::TABLE_CONSTRUCT;
	if (lst_ != nullptr && lst_->lst.size() == 0)
	{
		delete lst_;
		expr->lst = nullptr;
	}
	else
		expr->lst = lst_;
	return expr;
}

// Создание int константы
// value - значение
Expression *TreeFactory::CreateConstExp(int value)
{
	Expression *crt = new Expression;
	crt->type=EXPRESSION_TYPE::INT;
	crt->iValue=value;
	return crt;
}

// Создание double константы
// value - значение
Expression *TreeFactory::CreateConstExp(double value)
{
	Expression *crt = new Expression;
	crt->type=EXPRESSION_TYPE::DOUBLE;
	crt->fValue=value;
	return crt;
}

// Создание string константы
// value - значение
Expression *TreeFactory::CreateConstExp(DoublePtrString value)
{
	Expression *crt = new Expression;
	crt->type=EXPRESSION_TYPE::STRING;
	crt->sValue = value.clone();
	return crt;
}

// Создание bool константы
// value - значение
Expression *TreeFactory::CreateConstExp(bool value)
{
	Expression *crt = new Expression;
	crt->type=EXPRESSION_TYPE::BOOL;
	crt->bValue = value;
	return crt;
}

// Создание nil
Expression *TreeFactory::CreateNil()
{
	Expression *crt = new Expression;
	crt->type=EXPRESSION_TYPE::NIL;
	return crt;
}

// Создание identifier константы
// value - значение
Expression *TreeFactory::CreateIdfExp(const char *str_)
{
	std::cout << "Create identifier expression\n";
	Expression *crt = new Expression;
	crt->type=EXPRESSION_TYPE::IDENTIFIER;
	crt->identifier = std::string(str_);
	crt->isAssignable = true;
	return crt;
}

// Сделать Expr из AssignableExpr
// expr - выражение для изменения
Expression *TreeFactory::MakeConstant(Expression *expr)
{
	expr->isAssignable = false;
	return expr;
}

Expression *TreeFactory::GetCell(Expression *expr_, Expression *rhs_)
{
	std::cout << "Create index by expression\n";
	Expression *expr = new Expression;
	expr->type = EXPRESSION_TYPE::CELL_BY_EXPR;
	expr->left = expr_;
	expr->right = rhs_;
	expr->isAssignable = true;
	return expr;
}

Expression *TreeFactory::GetCell(Expression *expr_, const char *identifier_)
{
	std::cout << "Create index by identifier\n";
	Expression *expr = new Expression;
	expr->type = EXPRESSION_TYPE::CELL_BY_EXPR;
	expr->left = expr_;
	expr->right = new Expression;
	expr->right->type=EXPRESSION_TYPE::STRING;
	expr->right->sValue = DoublePtrString(identifier_);
	delete identifier_; // TODO: double-check
	expr->isAssignable = true;
	return expr;
}

Expression *TreeFactory::CreateMethodName(Expression *expr_, const char *name_)
{
	std::cout << "Create method name\n";
	Expression *expr = new Expression;
	expr->type = EXPRESSION_TYPE::METHOD_NAME;
	expr->left = expr_;
	expr->identifier = name_;
	return expr;
}

Expression *TreeFactory::CreateFunctionCall(Expression *callableName_, ExpressionList *args_)
{
	std::cout << "Create function call\n";
	Expression *expr = new Expression;
	expr->type = EXPRESSION_TYPE::FUNCTION_CALL;
	expr->left = callableName_;
	expr->lst = args_;
	return expr;
}

Expression *TreeFactory::CreateFunctionCall(Expression *callableName_, DoublePtrString arg_)
{
	std::cout << "Create function call\n";
	Expression *expr = new Expression;
	expr->type = EXPRESSION_TYPE::FUNCTION_CALL;
	expr->left = callableName_;
	expr->lst = TreeFactory::CreateExprList(CreateConstExp(arg_));
	return expr;
}

Expression *TreeFactory::CreateFunctionCall(Expression *callableName_, Expression *tblArg_)
{
	std::cout << "Create function call\n";
	Expression *expr = new Expression;
	expr->type = EXPRESSION_TYPE::FUNCTION_CALL;
	expr->left = callableName_;
	expr->lst = TreeFactory::CreateExprList(tblArg_);
	return expr;
}

Statement *TreeFactory::CreateFunctionCallStatement(Expression *call_)
{
	auto *stmt = new StatementFunctionCall();
	stmt->type = STATEMENT_TYPE::FUNCTION_CALL;
	stmt->functionName = call_->left;
	stmt->lst = call_->lst;
	return stmt;
}

ParamList *TreeFactory::CreateParamList(const char *identifier_)
{
	std::cout << "Create function call\n";
	ParamList *plist = new ParamList;
	plist->lst.push_back(identifier_);
	return plist;
}

ParamList *TreeFactory::CreateParamList()
{
	std::cout << "Create function call\n";
	ParamList *plist = new ParamList;
	return plist;
}

ParamList *TreeFactory::AppendParamList(ParamList *plst_, const char *identifier_)
{
	plst_->lst.push_back(identifier_);
	return plst_;
}

ParamList *TreeFactory::AddVarargToParamList(ParamList *plst_)
{
	plst_->hasVararg = true;
	return plst_;
}

Expression *TreeFactory::CreateUnnamedFunctionDefinition(ParamList *params_, StatementList *code_)
{
	std::cout << "Create unnamed function definition\n";
	Expression *expr = new Expression;
	expr->type = EXPRESSION_TYPE::UNNAMED_FUNCTION_DEFINITION;
	expr->params = params_;
	expr->code = code_;
}

Statement *TreeFactory::CreateNamedFunctionDefinition(Expression *functionName_, ParamList *params_, StatementList *code_)
{
	auto *func = TreeFactory::CreateUnnamedFunctionDefinition(params_, code_);
	auto *res = TreeFactory::CreateAssignStatement(functionName_, TreeFactory::CreateExprList(func), false);
	return res;
}

Statement *TreeFactory::CreateNamedFunctionDefinition(Expression *functionName_, const char *identifier_, ParamList *params_, StatementList *code_)
{
	auto *func = TreeFactory::CreateUnnamedFunctionDefinition(params_, code_);
	auto *name = TreeFactory::CreateMethodName(functionName_, identifier_);
	auto *res = TreeFactory::CreateAssignStatement(name, TreeFactory::CreateExprList(func), false);
	return res;
}

Statement *TreeFactory::makeAssignmentLocal(Statement *assign_)
{
	auto *realstmt = dynamic_cast<StatementAssign*>(assign_);
	realstmt->isLocal = true;
}

Statement *TreeFactory::makeBreakStatement()
{
	auto *stmt = new Statement();
	stmt->type = STATEMENT_TYPE::BREAK;
	return stmt;
}

Statement *TreeFactory::makeIfElseStatement(Expression *condition_, StatementList *trueCode_)
{
	auto *stmt = new StatementIfElse();
	stmt->type = STATEMENT_TYPE::IF_ELSE;
	stmt->condition = condition_;
	stmt->trueCode = trueCode_;
	return stmt;

}

Statement *TreeFactory::addElseifToIfElseStatement(Statement *ifElse_, Expression *condition_, StatementList *trueCode_)
{
	auto *realstmt = dynamic_cast<StatementIfElse*>(ifElse_);
	auto *newElif = TreeFactory::makeIfElseStatement(condition_, trueCode_);
	realstmt->elseifs.push_back(dynamic_cast<StatementIfElse*>(newElif));
	return realstmt;
}

Statement *TreeFactory::addElseToIfElseStatement(Statement *ifElse_, StatementList *falseCode_)
{
	auto *realstmt = dynamic_cast<StatementIfElse*>(ifElse_);
	realstmt->falseCode = falseCode_;
	return realstmt;
}

Statement *TreeFactory::makeWhileLoopStatement(Expression *condition_, StatementList *trueCode_)
{
	auto *stmt = new StatementWhileLoop();
	stmt->type = STATEMENT_TYPE::WHILE_LOOP;
	stmt->condition = condition_;
	stmt->trueCode = trueCode_;
	return stmt;
}

Statement *TreeFactory::makeRepeatLoopStatement(Expression *condition_, StatementList *trueCode_)
{
	auto *stmt = new StatementRepeatLoop();
	stmt->type = STATEMENT_TYPE::REPEAT_LOOP;
	stmt->condition = condition_;
	stmt->trueCode = trueCode_;
	return stmt;
}

Statement *TreeFactory::makeForLoopStatement(const char *identifier_, Expression *begin_, Expression *end_, Expression *step_, StatementList *code_)
{
	auto *stmt = new StatementForLoop();
	stmt->type = STATEMENT_TYPE::FOR_NUMERIC;
	stmt->identifier = std::string(identifier_);
	stmt->begin = begin_;
	stmt->end = end_;
	stmt->step = (step_ ? step_ : TreeFactory::CreateConstExp(1));
	stmt->code = code_;
	return stmt;
}