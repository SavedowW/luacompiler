#ifndef TREE_PRINT_H_
#define TREE_PRINT_H_

#include <stdio.h>
#include <iostream>
#include "Tree.h"

namespace TreePrint
{
	int printTree(void);
	void lst_print(StatementList *stmts, int level);
	void lst_print(ExpressionList *exprs, int level);
	void stmt_print(Statement *stmt, int level);
	void expr_print(Expression *expr, int level);
	void print_indent(int level);
};

#endif