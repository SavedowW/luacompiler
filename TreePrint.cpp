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
	auto savednum = stlistnumber;
	stlistnumber++;

	std::cout << "ST_" << savednum << "_0";

	for (int i = 1; i < stmts->lst.size(); ++i)
	{
		std::cout << "->ST_" << savednum << "_" << i;
	} 

	std::cout << " ; ";

	for (int i = 0; i < stmts->lst.size(); ++i)
	{
		stmt_print(stmts->lst[i], savednum, i);
	}
}

void TreePrint::lst_print(ExpressionList *exprs)
{
	if (exprs->lst.size() == 0)
		return;
	auto savednum = stlistnumber;
	stlistnumber++;

	for (int i = 0; i < exprs->lst.size(); ++i)
	{
		std::cout << "EXL_" << savednum << "->ST_" << savednum << "_" << i << " ; ";
	} 

	for (auto &el : exprs->lst)
	{
		expr_print(el);
	}
}

void TreePrint::lst_print(ParamList *params_)
{
	for (auto &el : params_->lst)
	{
		std::cout << el;
	} 
	if (params_->hasVararg)
	{
		std::cout << "...";
	}

	auto savednum = stlistnumber;
	stlistnumber++;

	for (int i = 0; i < params_->lst.size(); ++i)
	{
		std::cout << "PLS_" << savednum << "->EXPR_" << savednum << "_" << i << " ; ";
	} 

	for (int i = 0; i < params_->lst.size(); ++i)
	{
		std::cout << "EXPR_" << savednum << "_" << i << " [label=\"" << params_->lst[i] << "\"] ; ";
	} 
}

void TreePrint::stmt_print(Statement *stmt, int savedNum_, int ownNum_)
{
	switch(stmt->type)
	{
	case STATEMENT_TYPE::ASSIGN:
	{
		std::cout << "ST_" << savedNum_ << "_" << ownNum_ << " [label=\"=\"] ; ";
		expr_print(realstmt->left, savedNum_, );
		lst_print(realstmt->right);
	}
		break;
	/*case STATEMENT_TYPE::MULTIPLE_ASSIGN:
	{
		auto *realstmt = dynamic_cast<StatementMultipleAssign*>(stmt);
		printf("=");
		if (realstmt->isLocal)
			std::cout << " (LOCAL)";
		std::cout << "Left:";
		lst_print(realstmt->left);
		std::cout << "Right:";
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
		
		printf("return");
		if (realstmt->lst)
			lst_print(realstmt->lst);
	}
		break;
	case STATEMENT_TYPE::FUNCTION_CALL:
	{
		auto *realstmt = dynamic_cast<StatementFunctionCall*>(stmt);
		
		std::cout << "()";
		expr_print(realstmt->functionName);
		lst_print(realstmt->lst);
	}
		break;
	case STATEMENT_TYPE::BREAK:
		
		std::cout << "break";
		break;
	case STATEMENT_TYPE::IF_ELSE:
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
void TreePrint::expr_print(Expression *expr, int savedNum_, int ownNum_)
{
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
		printf("-");
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
		std::cout << expr->identifier;
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

