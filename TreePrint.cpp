#include "TreePrint.h"

extern Program *prg;

int TreePrint::printTree(void)
{
	printf("\nProgram:");
	lst_print(prg->stmts, 0);
	return 0;
}

void TreePrint::lst_print(StatementList *stmts, int level)
{
	for (auto &el : stmts->lst)
	{
		stmt_print(el, level + 1);
	} 
}
void TreePrint::stmt_print(Statement *stmt, int level)
{
	switch(stmt->type)
	{
	case STATEMENT_TYPE::PRINT:
	{
		auto *realstmt = dynamic_cast<SingleExprStatement*>(stmt);
		print_indent(level);
		printf("print");
		expr_print(realstmt->to_print, level + 1);
	}
		break;
	case STATEMENT_TYPE::EXPR:
	{
		auto *realstmt = dynamic_cast<SingleExprStatement*>(stmt);
		expr_print(realstmt->to_print, level);
	}
	case STATEMENT_TYPE::STMT_LIST:
	{
		auto *realstmt = dynamic_cast<StatementList*>(stmt);
		lst_print(realstmt, level);
	}
	case STATEMENT_TYPE::IF_ELSE:
	{
		auto *realstmt = dynamic_cast<IfElseStmt*>(stmt);
		print_indent(level);
		std::cout << "if ";
		expr_print(realstmt->condition, 0);
		std::cout << " then";
		lst_print(realstmt->statement, level);
		if (realstmt->elseStatement != nullptr)
		{
			std::cout << "\nelse";
			stmt_print(realstmt->elseStatement, level + 1);
		}
	}
		break;
	}
}
void TreePrint::expr_print(Expression *expr, int level)
{
	print_indent(level);
	switch(expr->type)
	{
	case EXPRESSION_TYPE::INT:
		printf("%d",expr->iValue);
		break;
	case EXPRESSION_TYPE::DOUBLE:
		std::cout << expr->fValue;
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
	case EXPRESSION_TYPE::BIN_ASSIGN:
		printf("=");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::IDENTIFIER:
		std::cout << expr->identifier;
		break;
	}
}

void TreePrint::print_indent(int level)
{
	int i;
	if (level > 0)
		printf("\n");
	for(i=0;i<level;i++)
	{
		printf("  ");
	}
}
