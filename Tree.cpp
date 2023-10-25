#include "Tree.h"

Program *TreeFactory::CreateProgram(StatementsList *lst)
{
	Program *crt = new Program;
	crt->stmts = lst;
	return crt;
}
StatementsList *TreeFactory::AppendStatementToList(StatementsList *lst, Statement *stm)
{
	StatementsList *crt = new StatementsList;
	crt->next=0;
	lst->next=crt;
	crt->stm = stm;
	return crt;
}
StatementsList *TreeFactory::CreateStList(Statement *stm)
{
	StatementsList *crt = new StatementsList;
	crt->next=0;
	crt->stm=stm;
	return crt;
}
Statement *TreeFactory::CreateStatement(Expression *exp)
{
	Statement *crt = new Statement;
	crt->to_print=exp;
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
Expression *TreeFactory::CreateIntExp(int value)
{
	Expression *crt = new Expression;
	crt->type=EXPRESSION_TYPE::INT;
	crt->value=value;
	return crt;
}