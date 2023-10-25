#ifndef TREE_PRINT_H_
#define TREE_PRINT_H_

#include <stdio.h>
#include "Tree.h"

namespace TreePrint
{
	int printTree(Program *ptr);
	void lst_print(StatementsList*stmts, int level);
	void stmt_print(Statement *stmt, int level);
	void expr_print(Expression *expr, int level);
	void print_indent(int level);
};

#endif