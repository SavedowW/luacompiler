#include "TreePrint.h"

extern Program *prg;

int TreePrint::printTree(Program *ptr)
{
	printf("\nProgram:");
	lst_print(prg->stmts, 0);
	return 0;
}

void TreePrint::lst_print(StatementsList*stmts, int level)
{
	while(stmts!=0)
	{
		stmt_print(stmts->stm, level + 1);
		stmts=stmts->next;
	} 
}
void TreePrint::stmt_print(Statement *stmt, int level)
{
	print_indent(level);
	printf("print");
	expr_print(stmt->to_print, level + 1);
}
void TreePrint::expr_print(Expression *expr, int level)
{
	print_indent(level);
	switch(expr->type)
	{
	case EXPRESSION_TYPE::INT:
		printf("%d",expr->value);
		break;
	case EXPRESSION_TYPE::BIN_PLUS:
		printf("+");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::BIN_MINUS:
		printf("-");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::BIN_MUL:
		printf("*");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::BIN_DIV:
		printf("/");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	}
}

void TreePrint::print_indent(int level)
{
	int i;
	printf("\n");
	for(i=0;i<level;i++)
	{
		printf("  ");
	}
}

