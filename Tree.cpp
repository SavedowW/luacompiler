#include "Tree.h"
#include <iostream>

std::string Expression::toString() const
{
	switch(type)
	{
	case EXPRESSION_TYPE::INT:
		return std::to_string(iValue);
		break;
	case EXPRESSION_TYPE::DOUBLE:
		return std::to_string(fValue);
		break;
	case EXPRESSION_TYPE::BIN_PLUS:
		return "+";
		break;
	case EXPRESSION_TYPE::BIN_MINUS:
		return "-";
		break;
	case EXPRESSION_TYPE::BIN_MUL:
		return "*";
		break;
	case EXPRESSION_TYPE::BIN_DIV:
		return "/";
		break;
	case EXPRESSION_TYPE::IDENTIFIER:
		return identifier;
		break;
	}
}

std::string Statement::toString() const
{
	switch(type)
	{
	case STATEMENT_TYPE::PRINT :
		return "print";
		break;
	case STATEMENT_TYPE::EXPR:
		return to_print->toString();
		break;
	}
}

Program *TreeFactory::CreateProgram(StatementsList *lst)
{
	Program *crt = new Program;
	crt->stmts = lst;
	return crt;
}
StatementsList *TreeFactory::AppendStatementToList(StatementsList *lst, Statement *stm)
{
	std::cout << "Append to statement list\n";
	StatementsList *crt = new StatementsList;
	crt->next=0;
	lst->next=crt;
	crt->stm = stm;
	return crt;
}
StatementsList *TreeFactory::CreateStList(Statement *stm)
{
    std::cout << "Created statement list\n";
	StatementsList *crt = new StatementsList;
	crt->next=0;
	crt->stm=stm;
	return crt;
}
Statement *TreeFactory::CreatePrintStatement(Expression *exp)
{
	std::cout << "Created print statement\n";
	Statement *crt = new Statement;
	crt->type = STATEMENT_TYPE::PRINT;
	crt->to_print=exp;
	return crt;
}

Statement *TreeFactory::CreateAssignStatement(Expression *exp1, Expression *exp2)
{
	std::cout << "Created assign statement\n";
	Statement *crt = new Statement;
	crt->type = STATEMENT_TYPE::EXPR;
	crt->to_print=CreateBinExp(EXPRESSION_TYPE::BIN_ASSIGN, exp1, exp2);
	return crt;
}

Expression *TreeFactory::CreateBinExp(EXPRESSION_TYPE exprType_, Expression *left,Expression *right)
{
	Expression *crt = new Expression;
	crt->type=exprType_;
	crt->left=left;
	crt->right=right;
	return crt;
}
Expression *TreeFactory::CreateConstExp(int value)
{
	Expression *crt = new Expression;
	crt->type=EXPRESSION_TYPE::INT;
	crt->iValue=value;
	return crt;
}
Expression *TreeFactory::CreateConstExp(double value)
{
	Expression *crt = new Expression;
	crt->type=EXPRESSION_TYPE::DOUBLE;
	crt->fValue=value;
	return crt;
}

Expression *TreeFactory::CreateIdfExp(const char *str_)
{
	std::cout << "Create identifier expression\n";
	Expression *crt = new Expression;
	crt->type=EXPRESSION_TYPE::IDENTIFIER;
	crt->identifier = std::string(str_);
	return crt;
}