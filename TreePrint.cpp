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

void TreePrint::lst_print(ExpressionList *exprs, int level)
{
	for (auto &el : exprs->lst)
	{
		expr_print(el, level + 1);
	} 
}

void TreePrint::stmt_print(Statement *stmt, int level)
{
	switch(stmt->type)
	{
	case STATEMENT_TYPE::ASSIGN:
	{
		auto *realstmt = dynamic_cast<StatementAssign*>(stmt);
		print_indent(level);
		printf("=");
		expr_print(realstmt->left, level + 1);
		lst_print(realstmt->right, level);
	}
		break;
	case STATEMENT_TYPE::STMT_LIST:
	{
		auto *realstmt = dynamic_cast<StatementList*>(stmt);
		lst_print(realstmt, level);
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
	case EXPRESSION_TYPE::BIN_REM_DIV:
		printf("%");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::BIN_EXPON:
		printf("^");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::BIN_AND:
		printf("&");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::BIN_OR:
		printf("|");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::BIN_NOT:
		printf("~");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::LOG_AND:
		printf("AND");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::LOG_OR:
		printf("OR");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::LOG_NOT:
		printf("NOT");
		expr_print(expr->left, level + 1);
		break;
	case EXPRESSION_TYPE::UNAR_UMINUS:
		printf("-");
		expr_print(expr->left, level + 1);
		break;
	case EXPRESSION_TYPE::UNAR_BITWISE_NOT:
		printf("~");
		expr_print(expr->left, level + 1);
		break;
	case EXPRESSION_TYPE::UNAR_LEN:
		printf("#");
		expr_print(expr->left, level + 1);
		break;
	case EXPRESSION_TYPE::REL_EQUALS:
		printf("==");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::REL_NOT_EQUALS:
		printf("~=");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::REL_GREATER:
		printf(">");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::REL_GREATER_EQUALS:
		printf(">=");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::REL_LESS:
		printf("<");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::REL_LESS_EQUALS:
		printf("<=");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::BIN_FLOOR_DIVISION:
		printf("//");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::BITWISE_LEFT_SHIFT:
		printf("<<");
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::BITWISE_RIGHT_SHIFT:
		printf(">>");
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

