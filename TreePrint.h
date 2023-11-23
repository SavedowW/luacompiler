#ifndef TREE_PRINT_H_
#define TREE_PRINT_H_

#include <stdio.h>
#include <iostream>
#include "Tree.h"

namespace TreePrint
{
	int printTree(void);
	void lst_print(StatementList *stmts_, int level_);
	void lst_print(ExpressionList *exprs_, int level_);
	void lst_print(ParamList *params_, int level_);
	void stmt_print(Statement *stmt, int level);
	void expr_print(Expression *expr, int level, bool noIndent = false);
	void print_indent(int level);
};

#endif