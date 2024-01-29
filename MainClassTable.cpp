#include "MainClassTable.h"
#include <algorithm>

extern Program *prg;

MainClassTable::MainClassTable() :
    ClassTable()
{

}

void MainClassTable::generateClassTable(const std::string &classname_)
{
    m_classname = classname_;

    treeBypassVarLinking(prg);

    m_ownContext;

    generateUniversalTable();

    auto mainNameID = addOrConfirmUtf8ToTable(std::string("main"));
    auto mainTypeID = addOrConfirmUtf8ToTable(std::string("([Ljava/lang/String;)V"));


    // =========== MAIN =================
    m_function = new MethodInfo();
	m_currentCodeRecorder = m_function;
    m_function->m_accessFlags = 0x0009;
    m_function->m_nameIndex = mainNameID;
    m_function->m_descIndex = mainTypeID;
    m_function->m_attribCount = 1;
    m_function->m_codeAttrNameIndex = m_codeAttrNameID;
    m_function->m_maxStack = 50;
    m_function->m_maxLocals = 1;

    // === Initialize DynamicType with print function and put into field
    auto printdata = createFunctionField(m_function, "CONTEXT_0_print", "print", classname_);
    auto nextdata = createFunctionField(m_function, "CONTEXT_0_next", "next", classname_);
    auto pairsdata = createFunctionField(m_function, "CONTEXT_0_pairs", "pairs", classname_);
    auto setmetatabledata = createFunctionField(m_function, "CONTEXT_0_setmetatable", "setmetatable", classname_);
    auto getmetatabledata = createFunctionField(m_function, "CONTEXT_0_getmetatable", "getmetatable", classname_);
    auto rawlendata = createFunctionField(m_function, "CONTEXT_0_rawlen", "rawlen", classname_);
    auto typedata = createFunctionField(m_function, "CONTEXT_0_type", "typeFunc", classname_);
    auto tonumberdata = createFunctionField(m_function, "CONTEXT_0_tonumber", "tonumber", classname_);
    auto tostringdata = createFunctionField(m_function, "CONTEXT_0_tostring", "tostring", classname_);
    auto stringdata = createTableField(m_function, "CONTEXT_0_string", classname_);
    createFunctionInTableField(m_function, "upper", "stringupper", stringdata.m_fieldRefID, classname_);
    createFunctionInTableField(m_function, "lower", "stringlower", stringdata.m_fieldRefID, classname_);
    createFunctionInTableField(m_function, "sub", "stringsub", stringdata.m_fieldRefID, classname_);
    auto iodata = createTableField(m_function, "CONTEXT_0_io", classname_);
    createFunctionInTableField(m_function, "read", "ioread", iodata.m_fieldRefID, classname_);

    m_methodPool.push_back(m_function);

    std::cout << "VAR DATA: " << m_classname << std::endl;
    generateFunctionClassVariables(m_ownContext);
    for (auto &el : m_functionClasses)
    {
        el->generateClassTable(el->generateClassName());
    }

    generateCode();

    m_function->m_codeLength = m_function->m_byteCode.size();
    m_function->m_codeAttrLength = m_function->m_codeLength + 12;
}

void MainClassTable::generateClassFiles()
{
    system(std::string("del output.jar").c_str());

	std::vector<std::string> classFiles = {
		"DynamicType.class",
		"FunctionClass.class",
		"getmetatable.class",
		"setmetatable.class",
		"ioread.class",
		"next.class",
		"pairs.class",
		"print.class",
		"rawlen.class",
		"tonumber.class",
		"typeFunc.class",
		"ValueType.class",
		"VarList.class",
		"tostring.class",
		"stringlower.class",
		"stringupper.class",
		"stringsub.class",
	};

	classFiles.push_back(m_classname + ".class");

    ClassTable::generateClassFile();
    for (auto &el : m_functionClasses)
    {
        el->generateClassFile();
		classFiles.push_back(el->m_classname + ".class");
    }

	std::string command("jar cfe output.jar ");
	command += m_classname + " ";
	for (const auto &filename : classFiles)
		command += filename + " ";
	std::cout << command << std::endl;
	system(command.c_str());
}

void MainClassTable::generateCode()
{
    treeBypassCodeGen(prg->stmts);
    m_function->addBytes(0xb1, 1);

    for (auto &el : m_functionClasses)
    {
        el->generateCode();
    }
}

void MainClassTable::initVar(const std::string &identifier_, VarsContext *context_, int fieldref)
{
    createDynamicType(m_function);
    m_function->addBytes(0xb3, 1); // putstatic
    m_function->addBytes(fieldref, 2); // putstatic
}

// Tree bypass for variables
void MainClassTable::treeBypassVarLinking(Program *node)
{
	if (node==NULL)	return;
	std::cout << node->stmts << std::endl;
    m_ownContext = new VarsContext(nullptr);
    m_ownContext->className = m_classname;
    m_currentContext = m_ownContext;
	treeBypassVarLinking(node->stmts);
}

void MainClassTable::treeBypassVarLinking(StatementGotoCall *node)
{
    // TODO: No idea what to do with it
	if (node==NULL)	return;
	std::cout << node->identifier << std::endl;
}

void MainClassTable::treeBypassVarLinking(StatementGotoLabel *node)
{
    // TODO: No idea what to do with it
	if (node==NULL)	return;
	std::cout << node->identifier << std::endl;
}

void MainClassTable::treeBypassVarLinking(StatementForeachLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

	m_currentContext = m_currentContext->createChildContext();
	node->iterContext = m_currentContext;
	for (auto &el : node->params->lst)
	{
		m_currentContext->confirmLocalVar(el);
	}
	
	treeBypassVarLinking(node->code);
	treeBypassVarLinking(node->data);	

    m_currentContext = m_currentContext->m_parentContext;

}

void MainClassTable::treeBypassVarLinking(StatementForLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	std::cout << node->identifier << std::endl;
	treeBypassVarLinking(node->begin);
	treeBypassVarLinking(node->end);
	treeBypassVarLinking(node->step);

    m_currentContext = m_currentContext->createChildContext();
	node->iterContext = m_currentContext->confirmLocalVar(node->identifier);
	treeBypassVarLinking(node->code);
    m_currentContext = m_currentContext->m_parentContext;
}

void MainClassTable::treeBypassVarLinking(ParamList *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	for (int i = 0; i < node->lst.size(); i++)
    {
		std::cout << "Param: " << node->lst[i] << std::endl;
        m_currentContext->confirmParameter(node->lst[i], i);
	}
}

void MainClassTable::treeBypassVarLinking(StatementRepeatLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

    m_currentContext = m_currentContext->createChildContext();
	treeBypassVarLinking(node->trueCode);
    m_currentContext = m_currentContext->m_parentContext;

	treeBypassVarLinking(node->condition);
}

void MainClassTable::treeBypassVarLinking(StatementWhileLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

	treeBypassVarLinking(node->condition);

    m_currentContext = m_currentContext->createChildContext();
	treeBypassVarLinking(node->trueCode);
    m_currentContext = m_currentContext->m_parentContext;
}

void MainClassTable::treeBypassVarLinking(StatementIfElse *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

	treeBypassVarLinking(node->condition);

    m_currentContext = m_currentContext->createChildContext();
	treeBypassVarLinking(node->trueCode);
    m_currentContext = m_currentContext->m_parentContext;

    for (int i = 0; i < node->elseifs.size(); i++)
    {
		treeBypassVarLinking(node->elseifs[i]->condition);
        m_currentContext = m_currentContext->createChildContext();
		treeBypassVarLinking(node->elseifs[i]->trueCode);
        m_currentContext = m_currentContext->m_parentContext;
	}

    m_currentContext = m_currentContext->createChildContext();
	treeBypassVarLinking(node->falseCode);
    m_currentContext = m_currentContext->m_parentContext;
}

void MainClassTable::treeBypassVarLinking(StatementFunctionCall *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	treeBypassVarLinking(node->functionName);
	treeBypassVarLinking(node->lst);
}

void MainClassTable::treeBypassVarLinking(StatementReturn *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	treeBypassVarLinking(node->lst);
}

void MainClassTable::treeBypassVarLinking(StatementMultipleAssign *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

	// Since rights side should ignore newly defined local vars, its vars are calculated first
	treeBypassVarLinking(node->right);

    if (node->isLocal)
        m_defineLocalVars = true;
	treeBypassVarLinking(node->left);
    if (node->isLocal)
        m_defineLocalVars = false;
}

void MainClassTable::treeBypassVarLinking(StatementAssign *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

    // Since rights side should ignore newly defined local vars, its vars are calculated first
	treeBypassVarLinking(node->right);

    if (node->isLocal)
        m_defineLocalVars = true;
	treeBypassVarLinking(node->left);
    if (node->isLocal)
        m_defineLocalVars = false;
}

void MainClassTable::treeBypassVarLinking(StatementList *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
    node->ownContext = m_currentContext;
	for (int i = 0; i < node->lst.size(); i++)
    {
		treeBypassVarLinking(node->lst[i]);
	}
}

void MainClassTable::treeBypassVarLinking(Statement *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

	switch(node->type)
	{
		case STATEMENT_TYPE::ASSIGN:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypassVarLinking(realnode);
		}
		break;
		case STATEMENT_TYPE::MULTIPLE_ASSIGN:
		{
			auto *realnode = dynamic_cast<StatementMultipleAssign*>(node);
			treeBypassVarLinking(realnode);
		}
		break;
		case STATEMENT_TYPE::STMT_LIST:
		{
			throw std::string("Statement list should not be kept as a statement");
		}
		break;
		case STATEMENT_TYPE::STRETURN:
		{
			auto *realnode = dynamic_cast<StatementReturn*>(node);
			treeBypassVarLinking(realnode);
		}
		break;
		case STATEMENT_TYPE::FUNCTION_CALL:
		{
			auto *realnode = dynamic_cast<StatementFunctionCall*>(node);
			treeBypassVarLinking(realnode);
		}
		break;
		case STATEMENT_TYPE::BREAK:
		{
			// TODO: No type, just logic
		}
		break;
		case STATEMENT_TYPE::IF_ELSE:
		{
			auto *realnode = dynamic_cast<StatementIfElse*>(node);
			treeBypassVarLinking(realnode);
		}
		break;
		case STATEMENT_TYPE::WHILE_LOOP:
		{
			auto *realnode = dynamic_cast<StatementWhileLoop*>(node);
			treeBypassVarLinking(realnode);
		}
		break;
		case STATEMENT_TYPE::REPEAT_LOOP:
		{
			auto *realnode = dynamic_cast<StatementRepeatLoop*>(node);
			treeBypassVarLinking(realnode);
		}
		break;
		case STATEMENT_TYPE::FOR_NUMERIC:
		{
			auto *realnode = dynamic_cast<StatementForLoop*>(node);
			treeBypassVarLinking(realnode);
		}
		break;
		case STATEMENT_TYPE::FOR_EACH:
		{
			auto *realnode = dynamic_cast<StatementForeachLoop*>(node);
			treeBypassVarLinking(realnode);
		}
		break;
		case STATEMENT_TYPE::GOTO_LABEL:
		{
			auto *realnode = dynamic_cast<StatementGotoLabel*>(node);
			treeBypassVarLinking(realnode);
		}
		break;
		case STATEMENT_TYPE::GOTO_CALL:
		{
			auto *realnode = dynamic_cast<StatementGotoCall*>(node);
			treeBypassVarLinking(realnode);
		}
		break;
	}
}

void MainClassTable::treeBypassVarLinking(ExpressionList *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	for (int i = 0; i < node->lst.size(); i++){
		treeBypassVarLinking(node->lst[i]);
	}
}

void MainClassTable::treeBypassVarLinking(Expression *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

	switch(node->type)
	{
		case EXPRESSION_TYPE::INT:
            // No need to do anything
		    break;
		case EXPRESSION_TYPE::DOUBLE:
            // No need to do anything
		    break;
		case EXPRESSION_TYPE::STRING:
            // No need to do anything
		    break;
		case EXPRESSION_TYPE::BOOL:
            // No need to do anything
		    break;
		case EXPRESSION_TYPE::NIL:
            // No need to do anything
		    break;
		case EXPRESSION_TYPE::IDENTIFIER:
            // Generate var
            if (m_defineLocalVars)
                node->varContext = m_currentContext->confirmLocalVar(node->identifier);
            else
                node->varContext = m_currentContext->confirmGlobalVar(node->identifier);
            // TODO: Code gen
            // Put variable on top of stack
		    break;
		case EXPRESSION_TYPE::BIN_PLUS:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_MINUS:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_MUL:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_DIV:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_CONCAT:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::CELL_BY_EXPR:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_REM_DIV:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_EXPON:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_AND:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_OR:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_NOT:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::LOG_AND:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::LOG_OR:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::LOG_NOT:
            treeBypassVarLinking(node->left);
		    break;
		case EXPRESSION_TYPE::UNAR_UMINUS:
            treeBypassVarLinking(node->left);
		    break;
		case EXPRESSION_TYPE::UNAR_BITWISE_NOT:
            treeBypassVarLinking(node->left);
		    break;
		case EXPRESSION_TYPE::UNAR_LEN:
            treeBypassVarLinking(node->left);
		    break;
		case EXPRESSION_TYPE::REL_EQUALS:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::REL_NOT_EQUALS:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::REL_GREATER:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::REL_GREATER_EQUALS:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::REL_LESS:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::REL_LESS_EQUALS:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_FLOOR_DIVISION:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BITWISE_LEFT_SHIFT:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BITWISE_RIGHT_SHIFT:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::KEY_VALUE_ASSOC:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::TABLE_CONSTRUCT:
            treeBypassVarLinking(node->lst);
		    break;
		case EXPRESSION_TYPE::METHOD_NAME:
            treeBypassVarLinking(node->left);
		    break;
		case EXPRESSION_TYPE::FUNCTION_CALL:
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->lst);
		    break;
		case EXPRESSION_TYPE::UNNAMED_FUNCTION_DEFINITION:
        {
            m_currentContext = m_currentContext->createChildContext();
            auto newFunctionClass = new FunctionClassTable(m_currentContext, node->params, node->code);
            m_functionClasses.push_back(newFunctionClass);
            m_currentContext->className = newFunctionClass->generateClassName();
            node->varContext = m_currentContext;
            treeBypassVarLinking(node->params);
            treeBypassVarLinking(node->code);
            m_currentContext = m_currentContext->m_parentContext;
        }
		    break;
		case EXPRESSION_TYPE::VARARG_REF:
            // Vararg is ignored here
		    break;
	}
}