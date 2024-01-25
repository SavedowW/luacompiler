#include "MainClassTable.h"

MainClassTable::MainClassTable() :
    ClassTable()
{

}

void MainClassTable::generateClassTable(const std::string &classname_)
{
    m_classname = classname_;

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
void MainClassTable::treeBypass(Program *node)
{
	if (node==NULL)	return;
	std::cout << node->stmts << std::endl;
	treeBypass(node->stmts);
}

void MainClassTable::treeBypass(StatementGotoCall *node)
{
	if (node==NULL)	return;
	std::cout << node->identifier << std::endl;
}
void MainClassTable::treeBypass(StatementGotoLabel *node)
{
	if (node==NULL)	return;
	std::cout << node->identifier << std::endl;
}
void MainClassTable::treeBypass(StatementForeachLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	treeBypass(node->params);
	treeBypass(node->data);
	treeBypass(node->code);
}
void MainClassTable::treeBypass(StatementForLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	std::cout << node->identifier << std::endl;
	treeBypass(node->begin);
	treeBypass(node->end);
	treeBypass(node->step);
	treeBypass(node->code);
}

void MainClassTable::treeBypass(ParamList *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	for (int i = 0; i < node->lst.size(); i++){
		std::cout << node->lst[i] << std::endl;
	}
}

void MainClassTable::treeBypass(StatementRepeatLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	treeBypass(node->condition);
	treeBypass(node->trueCode);
}

void MainClassTable::treeBypass(StatementWhileLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	treeBypass(node->condition);
	treeBypass(node->trueCode);
}

void MainClassTable::treeBypass(StatementIfElse *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	for (int i = 0; i < node->elseifs.size(); i++){
		std::cout << node->elseifs[i] << std::endl;
	}
	treeBypass(node->condition);
	treeBypass(node->trueCode);
	treeBypass(node->falseCode);
}

void MainClassTable::treeBypass(StatementFunctionCall *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	treeBypass(node->functionName);
	treeBypass(node->lst);
}

void MainClassTable::treeBypass(StatementReturn *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	treeBypass(node->lst);
}

void MainClassTable::treeBypass(StatementMultipleAssign *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	treeBypass(node->left);
	treeBypass(node->right);
}

void MainClassTable::treeBypass(StatementAssign *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	treeBypass(node->left);
	treeBypass(node->right);
}

void MainClassTable::treeBypass(StatementList *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	for (int i = 0; i < node->lst.size(); i++){
		std::cout << node->lst[i] << std::endl;
	}
}

void MainClassTable::treeBypass(Statement *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

	switch(node->type)
	{
		case STATEMENT_TYPE::ASSIGN:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::MULTIPLE_ASSIGN:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::STMT_LIST:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::RETURN:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::FUNCTION_CALL:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::BREAK:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::IF_ELSE:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::WHILE_LOOP:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::REPEAT_LOOP:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::FOR_NUMERIC:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::FOR_EACH:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::GOTO_LABEL:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::GOTO_CALL:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypass(realnode);
		}
		break;
	}
}

void MainClassTable::treeBypass(ExpressionList *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	for (int i = 0; i < node->lst.size(); i++){
		std::cout << node->lst[i] << std::endl;
	}
}

void MainClassTable::treeBypass(Expression *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	std::cout << node->identifier << std::endl;
	treeBypass(node->left);
	treeBypass(node->right);
	treeBypass(node->lst);
	treeBypass(node->code);
	treeBypass(node->params);

	switch(node->type)
	{
		case EXPRESSION_TYPE::INT:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::DOUBLE:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::STRING:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BOOL:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::NIL:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::IDENTIFIER:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BIN_PLUS:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BIN_MINUS:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BIN_MUL:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BIN_DIV:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BIN_CONCAT:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::CELL_BY_EXPR:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BIN_REM_DIV:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BIN_EXPON:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BIN_AND:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BIN_OR:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BIN_NOT:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::LOG_AND:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::LOG_OR:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::LOG_NOT:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::UNAR_UMINUS:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::UNAR_BITWISE_NOT:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::UNAR_LEN:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::REL_EQUALS:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::REL_NOT_EQUALS:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::REL_GREATER:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::REL_GREATER_EQUALS:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::REL_LESS:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::REL_LESS_EQUALS:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BIN_FLOOR_DIVISION:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BITWISE_LEFT_SHIFT:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::BITWISE_RIGHT_SHIFT:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::KEY_VALUE_ASSOC:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::TABLE_CONSTRUCT:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::METHOD_NAME:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::FUNCTION_CALL:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::UNNAMED_FUNCTION_DEFINITION:
            treeBypass(node);
		    break;
		case EXPRESSION_TYPE::VARARG_REF:
            treeBypass(node);
		    break;
	}
}

