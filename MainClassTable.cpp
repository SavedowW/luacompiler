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

    m_thisClassID = addOrConfirmClassToTable(classname_);
    m_superClassID = addOrConfirmClassToTable("java/lang/Object");
    auto mainNameID = addOrConfirmUtf8ToTable("main");
    auto mainTypeID = addOrConfirmUtf8ToTable("([Ljava/lang/String;)V");
    auto codeAttrNameID = addOrConfirmUtf8ToTable("Code");

    m_dtClass = addOrConfirmClassToTable("DynamicType");
    m_dtInitIdI = addOrConfirmMethodRefToTable("<init>", "(I)V", "DynamicType");
    m_dtInitIdD = addOrConfirmMethodRefToTable("<init>", "(D)V", "DynamicType");
    m_dtInitIdS = addOrConfirmMethodRefToTable("<init>", "(Ljava/lang/String;)V", "DynamicType");
    m_dtInitIdF = addOrConfirmMethodRefToTable("<init>", "(LFunctionClass;)V", "DynamicType");
    m_dtInitIdNIL = addOrConfirmMethodRefToTable("<init>", "()V", "DynamicType");
    m_dtFieldIdI = addOrConfirmFieldRefToTable("iValue", "I", "DynamicType");
    m_dtFieldIdD = addOrConfirmFieldRefToTable("dValue", "D", "DynamicType");
    m_dtFieldIdS = addOrConfirmFieldRefToTable("sValue", "Ljava/lang/String;", "DynamicType");
    m_dtFieldIdF = addOrConfirmFieldRefToTable("fValue", "LFunctionClass;", "DynamicType");

    m_dtCallRef = addOrConfirmMethodRefToTable("__call", "(LDynamicType;LVarList;)LVarList;", "DynamicType");
    
    auto varlistClass = addOrConfirmClassToTable("VarList");
    auto varlistInit = addOrConfirmMethodRefToTable("<init>", "()V", "VarList");
    auto varlistAdd = addOrConfirmMethodRefToTable("add", "(LDynamicType;)V", "VarList");
    auto varlistGet = addOrConfirmMethodRefToTable("get", "(I)LDynamicType;", "VarList");

    // =========== MAIN =================
    auto *mainmethod = new MethodInfo();
    mainmethod->m_accessFlags = 0x0009;
    mainmethod->m_nameIndex = mainNameID;
    mainmethod->m_descIndex = mainTypeID;
    mainmethod->m_attribCount = 1;
    mainmethod->m_codeAttrNameIndex = codeAttrNameID;
    mainmethod->m_maxStack = 10;
    mainmethod->m_maxLocals = 1;

    // === Initialize DynamicType with print function and put into field
    auto printdata = createFunctionField(mainmethod, "print", "print", classname_);
    
    // === Call print function

    // Put DynamicType from static into stack
    mainmethod->addBytes(0xb2, 1); // getstatic
    mainmethod->addBytes(printdata.m_fieldRefID, 2); // print field
    
    // Create and initialize VarList
    mainmethod->addBytes(0xbb, 1); // new
    mainmethod->addBytes(varlistClass, 2); // VarList
    mainmethod->addBytes(0x59, 1); // dup
    mainmethod->addBytes(0xb7, 1); // invokespecial
    mainmethod->addBytes(varlistInit, 2); // <init>

    //Duplicate varlist for add
    mainmethod->addBytes(0x59, 1); // dup

    // Create DT for VarList
    createDynamicType(mainmethod, 505);

    // Call VarList add
    mainmethod->addBytes(0xb6, 1); // invokevirtual
    mainmethod->addBytes(varlistAdd, 2); // add

    //Duplicate varlist for add
    mainmethod->addBytes(0x59, 1); // dup

    // Create DT for VarList
    createDynamicType(mainmethod, -107);

    // Call VarList add
    mainmethod->addBytes(0xb6, 1); // invokevirtual
    mainmethod->addBytes(varlistAdd, 2); // add

    //Duplicate varlist for add
    mainmethod->addBytes(0x59, 1); // dup

    // Create DT for VarList
    createDynamicType(mainmethod, 2);

    // Call VarList add
    mainmethod->addBytes(0xb6, 1); // invokevirtual
    mainmethod->addBytes(varlistAdd, 2); // add

    // Call print
    mainmethod->addBytes(0xb8, 1); // invokestatic
    mainmethod->addBytes(m_dtCallRef, 2); // __call

    // return
    mainmethod->addBytes(0xb1, 1);

    mainmethod->m_codeLength = mainmethod->m_byteCode.size();
    mainmethod->m_codeAttrLength = mainmethod->m_codeLength + 12;

    m_methodPool.push_back(mainmethod);

    std::cout << "VAR DATA: " << m_classname << std::endl;
    generateFunctionClassVariables(m_ownContext);
    for (auto &el : m_functionClasses)
    {
        el->generateClassTable(el->generateClassName());
    }
}

FieldData MainClassTable::createFunctionField(MethodInfo *method, const std::string &functionName, const std::string &functionOwnerClassName, const std::string &className)
{
    auto fldName = addOrConfirmUtf8ToTable(functionName);
    auto fldDesc = addOrConfirmUtf8ToTable("LDynamicType;");
    auto fldRef = addOrConfirmFieldRefToTable(functionName, "LDynamicType;", className);

    auto functionClass = addOrConfirmClassToTable(functionOwnerClassName);
    //auto functionCall = addOrConfirmMethodRefToTable("function", "(LVarList;)LVarList;", "print");
    auto fuhctionInit = addOrConfirmMethodRefToTable("<init>", "()V", functionName);

    // Create DynamicType
    method->addBytes(0xbb, 1); // new
    method->addBytes(m_dtClass, 2); // DynamicType
    method->addBytes(0x59, 1); // dup
    
    // Create and initialize function class
    method->addBytes(0xbb, 1); // new
    method->addBytes(functionClass, 2); // FunctionClass
    method->addBytes(0x59, 1); // dup
    method->addBytes(0xb7, 1); // invokespecial
    method->addBytes(fuhctionInit, 2); // <init>

    // Initialize DynamicType
    method->addBytes(0xb7, 1); // invokespecial
    method->addBytes(m_dtInitIdF, 2); // <init>

    // Put function into static field
    method->addBytes(0xb3, 1); // putstatic
    method->addBytes(fldRef, 2); // print field


    auto *mainfield = new FieldInfo();
    mainfield->m_accessFlags = 0x0009;
    mainfield->m_nameIndex = fldName;
    mainfield->m_descIndex = fldDesc;
    m_fieldPool.push_back(mainfield);

    return {fldName, fldDesc, fldRef};

}

void MainClassTable::createDynamicType(MethodInfo *method, int num_)
{
        // Create DT for VarList
    method->addBytes(0xbb, 1); // new
    method->addBytes(m_dtClass, 2); // DynamicType
    method->addBytes(0x59, 1); // dup

    switch (num_)
    {
        case (-1):
            method->addBytes(0x2, 1); // dup
            break;

        case (0):
            method->addBytes(0x3, 1); // dup
            break;

        case (1):
            method->addBytes(0x4, 1); // dup
            break;
        
        case (2):
            method->addBytes(0x5, 1); // dup
            break;

        case (3):
            method->addBytes(0x6, 1); // dup
            break;

        case (4):
            method->addBytes(0x7, 1); // dup
            break;

        case (5):
            method->addBytes(0x8, 1); // dup
            break;

        default:
        {
            auto numberId = addOrConfirmIntegerToTable(num_);
            method->addBytes(0x13, 1); // ldc_w
            method->addBytes(numberId, 2); // field with number
            break;
        }
    }


    method->addBytes(0xb7, 1); // invokespecial
    method->addBytes(m_dtInitIdI, 2); // <init>
}

//функции обхода деревьев
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

	treeBypassVarLinking(node->params);
	treeBypassVarLinking(node->data);

    m_currentContext = m_currentContext->createChildContext();
	treeBypassVarLinking(node->code);
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
		case STATEMENT_TYPE::RETURN:
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
            // TODO: code gen
            // Put DT value on stack
		    break;
		case EXPRESSION_TYPE::DOUBLE:
            // TODO: code gen
            // Put DT value on stack
		    break;
		case EXPRESSION_TYPE::STRING:
            // TODO: code gen
            // Put DT value on stack
		    break;
		case EXPRESSION_TYPE::BOOL:
            // TODO: code gen
            // Put DT value on stack
		    break;
		case EXPRESSION_TYPE::NIL:
            // TODO: code gen
            // Put DT value on stack
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
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_MINUS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_MUL:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_DIV:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_CONCAT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::CELL_BY_EXPR:
            // TODO: code gen
            // call func, leaves cell DT on stack
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_REM_DIV:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_EXPON:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_AND:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_OR:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_NOT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::LOG_AND:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::LOG_OR:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::LOG_NOT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
		    break;
		case EXPRESSION_TYPE::UNAR_UMINUS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
		    break;
		case EXPRESSION_TYPE::UNAR_BITWISE_NOT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
		    break;
		case EXPRESSION_TYPE::UNAR_LEN:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
		    break;
		case EXPRESSION_TYPE::REL_EQUALS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::REL_NOT_EQUALS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::REL_GREATER:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::REL_GREATER_EQUALS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::REL_LESS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::REL_LESS_EQUALS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_FLOOR_DIVISION:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BITWISE_LEFT_SHIFT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::BITWISE_RIGHT_SHIFT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::KEY_VALUE_ASSOC:
            // TODO: code gen
            // expects table on top of stack
            // performs assignment of cell
            // keeps table on top
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->right);
		    break;
		case EXPRESSION_TYPE::TABLE_CONSTRUCT:
            // TODO: code gen
            // creates table on top of stack
            // recursively assigns values to cells
            treeBypassVarLinking(node->lst);
		    break;
		case EXPRESSION_TYPE::METHOD_NAME:
            // TODO: code gen
            // expr in left should leave table on top of stack
            // identifier is cell name
            treeBypassVarLinking(node->left);
		    break;
		case EXPRESSION_TYPE::FUNCTION_CALL:
            // TODO: code gen
            // generates VarList, calls function, leaves VarList of returned values
            // expr in left puts function DynamicType on top of the stack
            treeBypassVarLinking(node->left);
            treeBypassVarLinking(node->lst);
		    break;
		case EXPRESSION_TYPE::UNNAMED_FUNCTION_DEFINITION:
        {
            // TODO: code gen
            // generates DynamicType with function on top of stack
            // Probably should be delegated to the FunctionClassTable from this point
            m_currentContext = m_currentContext->createChildContext();
            auto newFunctionClass = new FunctionClassTable(m_currentContext);
            m_functionClasses.push_back(newFunctionClass);
            m_currentContext->className = newFunctionClass->generateClassName();
            treeBypassVarLinking(node->params);
            treeBypassVarLinking(node->code);
            m_currentContext = m_currentContext->m_parentContext;
        }
		    break;
		case EXPRESSION_TYPE::VARARG_REF:
            throw std::string("VARARG_REF shouldn't exist");
		    break;
	}
}