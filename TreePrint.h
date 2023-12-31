#ifndef TREE_PRINT_H_
#define TREE_PRINT_H_

#include <stdio.h>
#include <iostream>
#include "Tree.h"

namespace TreePrint
{
	int printTree(void);
	void lst_print(StatementList *stmts_);
	void lst_print(ExpressionList *exprs_);
	void lst_print(ParamList *params_);
	void stmt_print(Statement *stmt);
	void expr_print(Expression *expr);
	void print_indent();
};

#endif