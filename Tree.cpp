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
Statement *TreeFactory::CreateAssignStatement(Expression *left_, ExpressionList *right_)
{
	std::cout << "Created assign statement\n";
	StatementAssign *sa = new StatementAssign;
	sa->type = STATEMENT_TYPE::ASSIGN;
	sa->left = left_;
	sa->right = right_;
	return sa;
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