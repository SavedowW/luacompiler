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

void TreePrint::lst_print(ParamList *params_, int level_)
{
	for (auto &el : params_->lst)
	{
		print_indent(level_ + 1);
		std::cout << el;
	} 
	if (params_->hasVararg)
	{
		print_indent(level_ + 1);
		std::cout << "...";
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
		if (realstmt->isLocal)
			std::cout << " (LOCAL)";
		expr_print(realstmt->left, level + 1);
		lst_print(realstmt->right, level);
	}
		break;
	case STATEMENT_TYPE::MULTIPLE_ASSIGN:
	{
		auto *realstmt = dynamic_cast<StatementMultipleAssign*>(stmt);
		print_indent(level);
		printf("=");
		if (realstmt->isLocal)
			std::cout << " (LOCAL)";
		print_indent(level);
		std::cout << "Left:";
		lst_print(realstmt->left, level);
		print_indent(level);
		std::cout << "Right:";
		lst_print(realstmt->right, level);
	}
		break;
	case STATEMENT_TYPE::STMT_LIST:
	{
		auto *realstmt = dynamic_cast<StatementList*>(stmt);
		lst_print(realstmt, level);
	}
		break;
	case STATEMENT_TYPE::RETURN:
	{
		auto *realstmt = dynamic_cast<StatementReturn*>(stmt);
		print_indent(level);
		printf("return");
		if (realstmt->lst)
			lst_print(realstmt->lst, level);
	}
		break;
	case STATEMENT_TYPE::FUNCTION_CALL:
	{
		auto *realstmt = dynamic_cast<StatementFunctionCall*>(stmt);
		print_indent(level);
		std::cout << "()";
		expr_print(realstmt->functionName, level + 1);
		lst_print(realstmt->lst, level + 1);
	}
		break;
	case STATEMENT_TYPE::BREAK:
		print_indent(level);
		std::cout << "break";
		break;
	case STATEMENT_TYPE::IF_ELSE:
	{
		auto *realstmt = dynamic_cast<StatementIfElse*>(stmt);
		print_indent(level);
		std::cout << "if";
		expr_print(realstmt->condition, level + 1);
		lst_print(realstmt->trueCode, level);

		for (auto *el : realstmt->elseifs)
		{
			print_indent(level);
			std::cout << "elif";
			stmt_print(el, level);
		}

		if (realstmt->falseCode)
		{
			print_indent(level);
			std::cout << "else";
			lst_print(realstmt->trueCode, level);
		}
	}
		break;
	case STATEMENT_TYPE::WHILE_LOOP:
	{
		auto *realstmt = dynamic_cast<StatementWhileLoop*>(stmt);
		print_indent(level);
		std::cout << "while";
		expr_print(realstmt->condition, level + 1);
		lst_print(realstmt->trueCode, level);
	}
		break;
	case STATEMENT_TYPE::REPEAT_LOOP:
	{
		auto *realstmt = dynamic_cast<StatementRepeatLoop*>(stmt);
		print_indent(level);
		std::cout << "repeat";
		lst_print(realstmt->trueCode, level);
		print_indent(level);
		std::cout << "until";
		expr_print(realstmt->condition, level + 1);
	}
		break;
	case STATEMENT_TYPE::FOR_NUMERIC:
	{
		auto *realstmt = dynamic_cast<StatementForLoop*>(stmt);
		print_indent(level);
		std::cout << "for";
		print_indent(level);
		std::cout << realstmt->identifier;
		print_indent(level);
		std::cout << "begin:";
		expr_print(realstmt->begin, level + 1);
		print_indent(level);
		std::cout << "end:";
		expr_print(realstmt->end, level + 1);
		print_indent(level);
		std::cout << "step:";
		expr_print(realstmt->step, level + 1);
		print_indent(level);
		std::cout << "code";
		lst_print(realstmt->code, level);
	}
		break;
	case STATEMENT_TYPE::FOR_EACH:
	{
		auto *realstmt = dynamic_cast<StatementForeachLoop*>(stmt);
		print_indent(level);
		std::cout << "foreach";
		lst_print(realstmt->params, level);
		print_indent(level);
		std::cout << "in";
		expr_print(realstmt->data, level + 1);
		lst_print(realstmt->code, level);
	}
		break;
	case STATEMENT_TYPE::GOTO_LABEL:
	{
		auto *realstmt = dynamic_cast<StatementGotoLabel*>(stmt);
		print_indent(level);
		std::cout << realstmt->identifier << ":";
	}
		break;
	case STATEMENT_TYPE::GOTO_CALL:
	{
		auto *realstmt = dynamic_cast<StatementGotoCall*>(stmt);
		print_indent(level);
		std::cout << "goto: " << realstmt->identifier;;
	}
		break;
	}
}
void TreePrint::expr_print(Expression *expr, int level, bool noIndent)
{
	if (level > 0 && !noIndent)
		print_indent(level);
	switch(expr->type)
	{
	case EXPRESSION_TYPE::INT:
		printf("%d",expr->iValue);
		break;
	case EXPRESSION_TYPE::DOUBLE:
		std::cout << expr->fValue;
		break;
	case EXPRESSION_TYPE::STRING:
		std::cout << expr->sValue;
		break;
	case EXPRESSION_TYPE::BOOL:
		std::cout << std::boolalpha << expr->bValue;
		break;
	case EXPRESSION_TYPE::NIL:
		std::cout << "nil";
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
	case EXPRESSION_TYPE::BIN_CONCAT:
		printf("..");
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
	case EXPRESSION_TYPE::CELL_BY_EXPR:
		std::cout << "[]";
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::KEY_VALUE_ASSOC:
		std::cout << "[]=";
		expr_print(expr->left, level + 1);
		expr_print(expr->right, level + 1);
		break;
	case EXPRESSION_TYPE::TABLE_CONSTRUCT:
		std::cout << "{}";
		if (!expr->lst)
			std::cout << " *empty*";
		else
			lst_print(expr->lst, level + 1);
		break;
	case EXPRESSION_TYPE::METHOD_NAME:
		std::cout << ":";
		expr_print(expr->left, level + 1);
		print_indent(level);
		std::cout << expr->identifier << std::endl;
		break;
	case EXPRESSION_TYPE::FUNCTION_CALL:
		std::cout << "()";
		expr_print(expr->left, level + 1);
		lst_print(expr->lst, level + 1);
		break;
	case EXPRESSION_TYPE::UNNAMED_FUNCTION_DEFINITION:
		std::cout << "function ()";
		lst_print(expr->params, level);
		print_indent(level);
		std::cout << "Code:";
		lst_print(expr->code, level);
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

