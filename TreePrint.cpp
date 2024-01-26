#include "TreePrint.h"
#include "ClassTable.h"

extern Program *prg;

int stlistnumber = 0;

int TreePrint::printTree(void)
{
	if (!prg)
	{
		std::cout << "Cannot print tree, root node was not created\n";
		return 1;
	}
	std::cout << "digraph G { ";
	lst_print(prg->stmts);
	std::cout << "}\n";
	return 0;
}

void TreePrint::lst_print(StatementList *stmts)
{
	if (stmts->lst.size() == 0)
		return;

	

	std::cout << "subgraph cluster_" << stmts->lst[0]->name << " { label = \" CONTEXT_" << stmts->ownContext->contextID << "\" style=filled; color=lightgrey; node [style=filled,color=white]; ";
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
		std::cout << params_->name << "->" << params_->name << "_" << i  << " [label=\"" << i << "\"] ; ";
		std::cout << params_->name << "_" << i << " [label=\"" << params_->lst[i] << "\"] ; ";
	} 

	if (params_->hasVararg)
	{
		int i = params_->lst.size();
		std::cout << params_->name << "->" << params_->name << "_" << i << " [label=\"" << i << "\"] ; ";
		std::cout << params_->name << "_" << i << " [label=\"...\"] ; ";
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
	case STATEMENT_TYPE::IF_ELSE:
	{
		auto *realstmt = dynamic_cast<StatementIfElse*>(stmt);
		expr_print(realstmt->condition);
		lst_print(realstmt->trueCode);
		if (realstmt->falseCode)
			lst_print(realstmt->falseCode);

		std::cout << realstmt->name << " [label=\"" << "if-else" << "\"] ; ";
		std::cout << realstmt->name << "->" << realstmt->condition->name << " [label=\"condition\"]; ";
		if (realstmt->trueCode->lst.size() > 0)
			std::cout << realstmt->name << "->" << realstmt->trueCode->lst[0]->name << " [label=\"true\"]; ";
		if (realstmt->falseCode && realstmt->falseCode->lst.size() > 0)
			std::cout << realstmt->name << "->" << realstmt->falseCode->lst[0]->name << " [label=\"false\"]; ";

		int it = 1;
		for (auto *el : realstmt->elseifs)
		{
			std::cout << realstmt->name << "->" << el->name << " [label=\"elseif " << it << "\"]; ";
			stmt_print(el);
			it++;
		}
	}
		break;
	case STATEMENT_TYPE::WHILE_LOOP:
	{
		auto *realstmt = dynamic_cast<StatementWhileLoop*>(stmt);
		
		std::cout << realstmt->name << " [label=\"" << "while" << "\"] ; ";
		expr_print(realstmt->condition);
		std::cout << realstmt->name << "->" << realstmt->condition->name << " [label=\"condition\"]; ";
		if (realstmt->trueCode)
		{
			lst_print(realstmt->trueCode);
			std::cout << realstmt->name << "->" << realstmt->trueCode->lst[0]->name << " [label=\"code\"]; ";
		}

	}
		break;
	case STATEMENT_TYPE::REPEAT_LOOP:
	{
		auto *realstmt = dynamic_cast<StatementRepeatLoop*>(stmt);
		
		std::cout << realstmt->name << " [label=\"" << "repeat" << "\"] ; ";
		expr_print(realstmt->condition);
		std::cout << realstmt->name << "->" << realstmt->condition->name << " [label=\"until\"]; ";
		if (realstmt->trueCode)
		{
			lst_print(realstmt->trueCode);
			std::cout << realstmt->name << "->" << realstmt->trueCode->lst[0]->name << " [label=\"code\"]; ";
		}
	}
		break;
	case STATEMENT_TYPE::FOR_NUMERIC:
	{
		auto *realstmt = dynamic_cast<StatementForLoop*>(stmt);

		expr_print(realstmt->begin);
		expr_print(realstmt->end);
		expr_print(realstmt->step);
		lst_print(realstmt->code);

		std::cout << realstmt->name << " [label=\"" << "for" << "\"] ; ";

		std::cout << realstmt->name << "->" << realstmt->name << "_idf" << " [label=\"identifier\"]; ";
		std::cout << realstmt->name << "_idf" << " [label=\"" << realstmt->identifier << "\"]; ";

		std::cout << realstmt->name << "->" << realstmt->begin->name << " [label=\"begin\"]; ";
		std::cout << realstmt->name << "->" << realstmt->end->name << " [label=\"end\"]; ";
		std::cout << realstmt->name << "->" << realstmt->step->name << " [label=\"step\"]; ";

		if (realstmt->code->lst.size() > 0)
			std::cout << realstmt->name << "->" << realstmt->code->lst[0]->name << " [label=\"code\"]; ";
	}
		break;
	case STATEMENT_TYPE::FOR_EACH:
	{
		auto *realstmt = dynamic_cast<StatementForeachLoop*>(stmt);


		lst_print(realstmt->params);
		expr_print(realstmt->data);
		lst_print(realstmt->code);
		std::cout << realstmt->name << " [label=\"" << "foreach" << "\"] ; ";
		std::cout << realstmt->name << "->" << realstmt->params->name << " [label=\"parameters\"]; ";
		std::cout << realstmt->name << "->" << realstmt->data->name << " [label=\"in\"]; ";

		if (realstmt->code->lst.size() > 0)
			std::cout << realstmt->name << "->" << realstmt->code->lst[0]->name << " [label=\"code\"]; ";

	}
		break;
	case STATEMENT_TYPE::GOTO_LABEL:
	{
		auto *realstmt = dynamic_cast<StatementGotoLabel*>(stmt);
		
		std::cout << realstmt->name << " [label=\"::" << realstmt->identifier << "::\"] ; ";
	}
		break;
	case STATEMENT_TYPE::GOTO_CALL:
	{
		auto *realstmt = dynamic_cast<StatementGotoCall*>(stmt);
		
		std::cout << realstmt->name << " [label=\"goto: " << realstmt->identifier << "\"] ; ";
	}
		break;
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
		std::cout << std::boolalpha << expr->name << " [label=\"" << expr->bValue << "\"] ; ";
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
		std::cout << expr->name << " [label=\"" << "-" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_MUL:
		std::cout << expr->name << " [label=\"" << "*" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_DIV:
		std::cout << expr->name << " [label=\"" << "/" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_CONCAT:
		std::cout << expr->name << " [label=\"" << ".." << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_REM_DIV:
		std::cout << expr->name << " [label=\"" << "%" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_EXPON:
		std::cout << expr->name << " [label=\"" << "^" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_AND:
		std::cout << expr->name << " [label=\"" << "&" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_OR:
		std::cout << expr->name << " [label=\"" << "|" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_NOT:
		std::cout << expr->name << " [label=\"" << "~" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::LOG_AND:
		std::cout << expr->name << " [label=\"" << "AND" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::LOG_OR:
		std::cout << expr->name << " [label=\"" << "OR" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::LOG_NOT:
		std::cout << expr->name << " [label=\"" << "NOT" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		expr_print(expr->left);
		break;
	case EXPRESSION_TYPE::UNAR_UMINUS:
		std::cout << expr->name << " [label=\"" << "-" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		expr_print(expr->left);
		break;
	case EXPRESSION_TYPE::UNAR_BITWISE_NOT:
		std::cout << expr->name << " [label=\"" << "~" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		expr_print(expr->left);
		break;
	case EXPRESSION_TYPE::UNAR_LEN:
		std::cout << expr->name << " [label=\"" << "#" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		expr_print(expr->left);
		break;
	case EXPRESSION_TYPE::REL_EQUALS:
		std::cout << expr->name << " [label=\"" << "==" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::REL_NOT_EQUALS:
		std::cout << expr->name << " [label=\"" << "~=" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::REL_GREATER:
		std::cout << expr->name << " [label=\"" << ">" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::REL_GREATER_EQUALS:
		std::cout << expr->name << " [label=\"" << ">=" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::REL_LESS:
		std::cout << expr->name << " [label=\"" << "<" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::REL_LESS_EQUALS:
		std::cout << expr->name << " [label=\"" << "<=" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BIN_FLOOR_DIVISION:
		std::cout << expr->name << " [label=\"" << "//" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BITWISE_LEFT_SHIFT:
		std::cout << expr->name << " [label=\"" << "<<" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::BITWISE_RIGHT_SHIFT:
		std::cout << expr->name << " [label=\"" << ">>" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		std::cout << expr->name << "->" << expr->right->name  << " ; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::IDENTIFIER:
		std::cout << expr->name << " [label=\"CONTEXT_" << expr->varContext->contextID << "::" << expr->identifier << "\"] ; ";
		break;
	case EXPRESSION_TYPE::CELL_BY_EXPR:
		std::cout << expr->name << " [label=\"" << "[]" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " [label=\"table\"]; ";
		std::cout << expr->name << "->" << expr->right->name  << " [label=\"index\"]; ";
		expr_print(expr->left);
		expr_print(expr->right);
		break;
	case EXPRESSION_TYPE::KEY_VALUE_ASSOC:
		if (expr->left)
		{
			std::cout << expr->name << " [label=\"" << "[]=" << "\"] ; ";
			std::cout << expr->name << "->" << expr->left->name << " [label=\"index\"]; ";
			expr_print(expr->left);
		}
		else
		{
			std::cout << expr->name << " [label=\"" << "[AUTO_INDEX]=" << "\"] ; ";
		}
		
		std::cout << expr->name << "->" << expr->right->name  << " [label=\"assigned value\"]; ";
		expr_print(expr->right);

		break;
	case EXPRESSION_TYPE::TABLE_CONSTRUCT:
		std::cout << expr->name << " [label=\"" << "{}" << "\"] ; ";

		if (expr->lst)
		{
			for (int i = 0; i < expr->lst->lst.size(); ++i)
			{
				std::cout << expr->name << "->" << expr->lst->lst[i]->name << " ; ";
			} 
		
			for (auto &el : expr->lst->lst)
			{
				expr_print(el);
			}
		}
		break;
	case EXPRESSION_TYPE::METHOD_NAME:
		std::cout << expr->name << " [label=\"" << ":" << expr->identifier << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " ; ";
		expr_print(expr->left);
		break;
	case EXPRESSION_TYPE::FUNCTION_CALL:
		std::cout << expr->name << " [label=\"" << "function call" << "\"] ; ";
		std::cout << expr->name << "->" << expr->left->name << " [label=\"function name\"]; ";
		std::cout << expr->name << "->" << expr->lst->name  << " [label=\"arguments\"]; ";
		expr_print(expr->left);
		lst_print(expr->lst);
		break;
	case EXPRESSION_TYPE::UNNAMED_FUNCTION_DEFINITION:
		lst_print(expr->params);
		lst_print(expr->code);
		std::cout << expr->name << " [label=\"" << "function definition" << "\"] ; ";
		std::cout << expr->name << "->" << expr->params->name << " [label=\"parameters\"]; ";
		if (expr->code->lst.size() > 0)
		{
			std::cout << expr->name << "->" << expr->code->lst[0]->name << " [label=\"code\"]; ";
		}
		break;
	case EXPRESSION_TYPE::VARARG_REF:
		std::cout << expr->name << " [label=\"" << "..." << "\"] ; ";
		break;
	}
}

