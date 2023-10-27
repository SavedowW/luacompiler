#include "Tree.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

DoublePtrString DoublePtrString::clone() const
{
	DoublePtrString s;
	int len = end-begin+1;
	s.begin = new char[end-begin+1];
	s.end = s.begin + len;
	strncpy(s.begin, begin, len);
}

std::ostream& operator<< (std::ostream& out_, const DoublePtrString& s_)
{
    char *ptr = s_.begin;
	out_ << '"';
    while (ptr != s_.end)
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

std::string SingleExprStatement::toString() const
{
	switch(type)
	{
	case STATEMENT_TYPE::PRINT :
		return "print";
		break;
	case STATEMENT_TYPE::EXPR:
		return to_print->toString();
		break;
	default:
		std::cout << "SingleExprStatement is used for improper type: " << (int)type << std::endl;
		return "";
	}
}

std::string ExpressionList::toString() const
{
	return "exprlist";
}

std::string StatementList::toString() const
{
	switch(type)
	{
	case STATEMENT_TYPE::STMT_LIST:
		return "StmtList";
		break;
	default:
		std::cout << "StatementList is used for improper type: " << (int)type << std::endl;
		return "";
	}
}

std::string IfElseStmt::toString() const
{
	switch(type)
	{
	case STATEMENT_TYPE::IF_ELSE:
		return "if";
		break;
	default:
		std::cout << "StatementList is used for improper type: " << (int)type << std::endl;
		return "";
	}
}

Program *TreeFactory::CreateProgram(StatementList *lst)
{
	std::cout << "Create program\n";
	Program *crt = new Program;
	crt->stmts = lst;
	return crt;
}
StatementList *TreeFactory::AppendStatementToList(StatementList *lst, Statement *stm)
{
	std::cout << "Append to statement list\n";
	lst->lst.push_back(stm);
	return lst;
}
StatementList *TreeFactory::CreateStList(Statement *stm)
{
    std::cout << "Created statement list\n";
	StatementList *lst = new StatementList;
	lst->type = STATEMENT_TYPE::STMT_LIST;
	lst->lst.push_back(stm);
	return lst;
}

ExpressionList *TreeFactory::CreateExprList(Expression *expr)
{
	std::cout << "Created expression list\n";
	ExpressionList *lst = new ExpressionList;
	lst->lst.push_back(expr);
	return lst;
}

Statement *TreeFactory::CreatePrintStatement(Expression *exp)
{
	std::cout << "Created print statement\n";
	SingleExprStatement *crt = new SingleExprStatement;
	crt->type = STATEMENT_TYPE::PRINT;
	crt->to_print=exp;
	return crt;
}

Statement *TreeFactory::CreateAssignStatement(Expression *exp1, Expression *exp2)
{
	std::cout << "Created assign statement\n";
	SingleExprStatement *crt = new SingleExprStatement;
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

Statement *TreeFactory::CreateIfElseStatement(Expression *condition_, StatementList *stmtList_, Statement *elseStmt_)
{
	std::cout << "Created if-else statement\n";
	auto *crt = new IfElseStmt;
	crt->type = STATEMENT_TYPE::IF_ELSE;
	crt->condition = condition_;
	crt->statement = stmtList_;
	crt->elseStatement = elseStmt_;
	return crt;
}