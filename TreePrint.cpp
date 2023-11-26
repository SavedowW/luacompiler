#include "TreePrint.h"

extern Program *prg;

int stlistnumber = 0;

int TreePrint::printTree(void)
{
	std::cout << "digraph G { ";
	lst_print(prg->stmts);
	std::cout << "}";
	return 0;
}

void TreePrint::lst_print(StatementList *stmts)
{
	if (stmts->lst.size() == 0)
		return;

	

	std::cout << "subgraph cluster_" << stmts->lst[0]->name << " { style=filled; color=lightgrey; node [style=filled,color=white]; ";
	std::cout << stmts->lst[0]->name;
	for (int i = 1; i < stmts->lst.size(); ++i)
	{
		std::cout << "->" << stmts->lst[i]->name;
	} 
	std::cout << " } ";


	for (int i = 0; i < stmts->lst.size(); ++i)
	{
		stmt_print(stmts->lst[i]);
	}
}

void TreePrint::lst_print(ExpressionList *exprs)
{
	if (exprs->lst.size() == 0)
		return;

	std::cout << "subgraph cluster_" << exprs->lst[0]->name << " {  ";
	for (int i = 0; i < exprs->lst.size(); ++i)
	{
		std::cout << exprs->name << "->" << exprs->lst[i]->name << " ; ";
	} 
	std::cout << " } ";



	for (auto &el : exprs->lst)
	{
		expr_print(el);
	}
}

void TreePrint::lst_print(ParamList *params_)
{
	if (params_->lst.size() == 0)
		return;

	for (int i = 0; i < params_->lst.size(); ++i)
	{
		std::cout << params_->name << "->" << params_->name << "_" << i  << " ; ";
	} 
}

void TreePrint::stmt_print(Statement *stmt)
{
	switch(stmt->type)
	{
	case STATEMENT_TYPE::ASSIGN:
	{
		auto *realstmt = dynamic_cast<StatementAssign*>(stmt);
		std::cout << stmt->name << " [label=\"=\"] ; ";
		std::cout << stmt->name << "->" << realstmt->left->name << " [label=\"left side\"]; ";
		std::cout << stmt->name << "->" << realstmt->right->name << " [label=\"right side\"]; ";
		expr_print(realstmt->left);
		lst_print(realstmt->right);
	}
		break;
	case STATEMENT_TYPE::MULTIPLE_ASSIGN:
	{
		auto *realstmt = dynamic_cast<StatementMultipleAssign*>(stmt);
		std::cout << stmt->name << " [label=\"=\"] ; ";
		std::cout << stmt->name << "->" << realstmt->left->name << " [label=\"left side\"]; ";
		std::cout << stmt->name << "->" << realstmt->right->name << " [label=\"right side\"]; ";
		lst_print(realstmt->left);
		lst_print(realstmt->right);
	}
		break;
	case STATEMENT_TYPE::STMT_LIST:
	{
		auto *realstmt = dynamic_cast<StatementList*>(stmt);
		lst_print(realstmt);
	}
		break;
	case STATEMENT_TYPE::RETURN:
	{
		auto *realstmt = dynamic_cast<StatementReturn*>(stmt);
		
		std::cout << stmt->name << " [label=\"return\"] ; ";
		if (realstmt->lst)
		{
			std::cout << stmt->name << "->" << realstmt->lst->name << " ; ";
			lst_print(realstmt->lst);
		}
	}
		break;
	case STATEMENT_TYPE::FUNCTION_CALL:
	{
		auto *realstmt = dynamic_cast<StatementFunctionCall*>(stmt);
		
		std::cout << stmt->name << " [label=\"function call\"] ; ";
		std::cout << stmt->name << "->" << realstmt->functionName->name << " [label=\"function name\"]; ";
		std::cout << stmt->name << "->" << realstmt->lst->name << " [label=\"arguments\"]; ";
		expr_print(realstmt->functionName);
		lst_print(realstmt->lst);
	}
		break;
	case STATEMENT_TYPE::BREAK:
		
		std::cout << stmt->name << " [label=\"break\"] ; ";
		break;
	/*case STATEMENT_TYPE::IF_ELSE:
	{
		auto *realstmt = dynamic_cast<StatementIfElse*>(stmt);
		
		std::cout << "if";
		expr_print(realstmt->condition);
		lst_print(realstmt->trueCode);

		for (auto *el : realstmt->elseifs)
		{
			
			std::cout << "elif";
			stmt_print(el);
		}

		if (realstmt->falseCode)
		{
			
			std::cout << "else";
			lst_print(realstmt->falseCode);
		}
	}
		break;
	case STATEMENT_TYPE::WHILE_LOOP:
	{
		auto *realstmt = dynamic_cast<StatementWhileLoop*>(stmt);
		
		std::cout << "while";
		expr_print(realstmt->condition);
		lst_print(realstmt->trueCode);
	}
		break;
	case STATEMENT_TYPE::REPEAT_LOOP:
	{
		auto *realstmt = dynamic_cast<StatementRepeatLoop*>(stmt);
		
		std::cout << "repeat";
		lst_print(realstmt->trueCode);
		
		std::cout << "until";
		expr_print(realstmt->condition);
	}
		break;
	case STATEMENT_TYPE::FOR_NUMERIC:
	{
		auto *realstmt = dynamic_cast<StatementForLoop*>(stmt);
		
		std::cout << "for";
		
		std::cout << realstmt->identifier;
		
		std::cout << "begin:";
		expr_print(realstmt->begin);
		
		std::cout << "end:";
		expr_print(realstmt->end);
		
		std::cout << "step:";
		expr_print(realstmt->step);
		
		std::cout << "code";
		lst_print(realstmt->code);
	}
		break;
	case STATEMENT_TYPE::FOR_EACH:
	{
		auto *realstmt = dynamic_cast<StatementForeachLoop*>(stmt);
		
		std::cout << "foreach";
		lst_print(realstmt->params);
		
		std::cout << "in";
		expr_print(realstmt->data);
		lst_print(realstmt->code);
	}
		break;
	case STATEMENT_TYPE::GOTO_LABEL:
	{
		auto *realstmt = dynamic_cast<StatementGotoLabel*>(stmt);
		
		std::cout << realstmt->identifier << ":";
	}
		break;
	case STATEMENT_TYPE::GOTO_CALL:
	{
		auto *realstmt = dynamic_cast<StatementGotoCall*>(stmt);
		
		std::cout << "goto: " << realstmt->identifier;;
	}
		break;*/
	}
}
void TreePrint::expr_print(Expression *expr)
{
	switch(expr->type)
	{
	case EXPRESSION_TYPE::INT:
		std::cout << expr->name << " [label=\"" << expr->iValue << "\"] ; ";
		break;
	case EXPRESSION_TYPE::DOUBLE:
		std::cout << expr->name << " [label=\"" << expr->fValue << "\"] ; ";
		break;
	case EXPRESSION_TYPE::STRING:
		std::cout << expr->name << " [label=\"\\\"";
		expr->sValue.printWithoutQuotes(std::cout);
		std::cout << "\\\"\"] ; ";
		break;
	case EXPRESSION_TYPE::BOOL:
		std::cout << std::boolalpha << expr->name << " [label=\"" << expr->bValue << "\"] ; ";;
		break;
	case EXPRESSION_TYPE::NIL:
		std::cout << expr->name << " [label=\"nil\"] ; ";
		break;
	case EXPRESSION_TYPE::BIN_PLUS:
		std::cout << expr->name << " [label=\"" << "+" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_MINUS:
		printf("-");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_MUL:
		printf("*");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_DIV:
		printf("/");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_CONCAT:
		printf("..");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_REM_DIV:
		printf("%");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_EXPON:
		printf("^");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_AND:
		printf("&");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_OR:
		printf("|");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_NOT:
		printf("~");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::LOG_AND:
		printf("AND");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::LOG_OR:
		printf("OR");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::LOG_NOT:
		printf("NOT");
		expr_print(expr->left);
		break;
	case EXPRESSION_TYPE::UNAR_UMINUS:
		std::cout << expr->name << " [label=\"" << "-" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		expr_print(expr->left);
		break;
	case EXPRESSION_TYPE::UNAR_BITWISE_NOT:
		printf("~");
		expr_print(expr->left);
		break;
	case EXPRESSION_TYPE::UNAR_LEN:
		printf("#");
		expr_print(expr->left);
		break;
	case EXPRESSION_TYPE::REL_EQUALS:
		printf("==");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::REL_NOT_EQUALS:
		printf("~=");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::REL_GREATER:
		printf(">");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::REL_GREATER_EQUALS:
		printf(">=");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::REL_LESS:
		printf("<");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::REL_LESS_EQUALS:
		printf("<=");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_FLOOR_DIVISION:
		printf("//");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BITWISE_LEFT_SHIFT:
		printf("<<");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BITWISE_RIGHT_SHIFT:
		printf(">>");
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::IDENTIFIER:
		std::cout << expr->name << " [label=\"" << expr->identifier << "\"] ; ";
		break;
	case EXPRESSION_TYPE::CELL_BY_EXPR:
		std::cout << "[]";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::KEY_VALUE_ASSOC:
		std::cout << "[]=";
		if (expr->left)
			expr_print(expr->left);
		else
		{
			std::cout << "AUTO_INDEX";
		}
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::TABLE_CONSTRUCT:
		std::cout << "{}";
		if (!expr->lst)
			std::cout << " *empty*";
		else
			lst_print(expr->lst);
		break;
	case EXPRESSION_TYPE::METHOD_NAME:
		std::cout << ":";
		expr_print(expr->left);
		std::cout << expr->identifier << std::endl;
		break;
	case EXPRESSION_TYPE::FUNCTION_CALL:
		std::cout << "()";
		expr_print(expr->left);
		lst_print(expr->lst);
		break;
	case EXPRESSION_TYPE::UNNAMED_FUNCTION_DEFINITION:
		std::cout << "function ()";
		lst_print(expr->params);
		std::cout << "Code:";
		lst_print(expr->code);
		break;
	case EXPRESSION_TYPE::VARARG_REF:
		std::cout << "...";
		break;
	}
}

