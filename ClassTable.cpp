#include "ClassTable.h"
#include <algorithm>

extern Program *prg;

int VarsContext::lastID = 0;

TableEntry::TableEntry(ENTRY_TYPE type_):
    m_type(type_)
{
}


Utf8Info::Utf8Info(const std::string &s_):
    TableEntry(TableEntry::ENTRY_TYPE::UTF8),
    m_str(s_.c_str())
{
}

IntegerInfo::IntegerInfo(int num_):
    TableEntry(TableEntry::ENTRY_TYPE::INTEGER),
    m_num(num_)
{
}

StringInfo::StringInfo(size_t stringIndex_):
    TableEntry(TableEntry::ENTRY_TYPE::STRING),
    m_stringIndex(stringIndex_)
{
}


ClassInfo::ClassInfo(size_t nameIndex_):
    TableEntry(TableEntry::ENTRY_TYPE::CLASS),
    m_nameIndex(nameIndex_)
{
}

NameAndTypeInfo::NameAndTypeInfo(size_t nameIndex_, size_t descriptorIndex_):
    TableEntry(TableEntry::ENTRY_TYPE::NAMEANDTYPE),
    m_nameIndex(nameIndex_),
    m_descriptorIndex(descriptorIndex_)
{
}

FieldRefInfo::FieldRefInfo(size_t classIndex_, size_t nameAndTypeIndex_):
    TableEntry(TableEntry::ENTRY_TYPE::FIELDREF),
    m_classIndex(classIndex_),
    m_nameAndTypeIndex(nameAndTypeIndex_)
{
}

MethodRefInfo::MethodRefInfo(size_t classIndex_, size_t nameAndTypeIndex_):
    TableEntry(TableEntry::ENTRY_TYPE::METHODREF),
    m_classIndex(classIndex_),
    m_nameAndTypeIndex(nameAndTypeIndex_)
{
}


Utf8Info::~Utf8Info()
{
    delete[] m_str.begin;
}

ClassTable::ClassTable()
{
    std::cout << "Initialized class table\n";
    if (!prg)
    {
        std::cout << "Cannot compile program, tree is invalid\n";
    }
}

size_t ClassTable::addOrConfirmUtf8ToTable(const std::string &s_)
{
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::UTF8)
        {
            auto *utfstr = dynamic_cast<Utf8Info*>(el);
            if (utfstr->m_str == s_)
            {
                std::cout << "String \"" << s_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added utf-8 string \"" << s_ << "\" to the constant pool\n";
    m_constantPool.push_back(new Utf8Info(s_));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmIntegerToTable(int num_)
{
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::INTEGER)
        {
            auto *intfield = dynamic_cast<IntegerInfo*>(el);
            if (intfield->m_num == num_)
            {
                std::cout << "Integer \"" << num_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added integer \"" << num_ << "\" to the constant pool\n";
    m_constantPool.push_back(new IntegerInfo(num_));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmClassToTable(const std::string &s_)
{
    auto classNameEntry = addOrConfirmUtf8ToTable(s_);
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::CLASS)
        {
            auto *utfstr = dynamic_cast<ClassInfo*>(el);
            if (utfstr->m_nameIndex == classNameEntry)
            {
                std::cout << "Class \"" << s_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added class \"" << s_ << "\" to the constant pool\n";
    m_constantPool.push_back(new ClassInfo(classNameEntry));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmStringToTable(const std::string &s_)
{
    auto utfstrid = addOrConfirmUtf8ToTable(s_);
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::STRING)
        {
            auto *utfstr = dynamic_cast<StringInfo*>(el);
            if (utfstr->m_stringIndex == utfstrid)
            {
                std::cout << "String \"" << s_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added string \"" << s_ << "\" to the constant pool\n";
    m_constantPool.push_back(new StringInfo(utfstrid));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmNameAndTypeToTable(const std::string &name_, const std::string &descriptor_)
{
    auto nameid = addOrConfirmUtf8ToTable(name_);
    auto descid = addOrConfirmUtf8ToTable(descriptor_);
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::NAMEANDTYPE)
        {
            auto *utfstr = dynamic_cast<NameAndTypeInfo*>(el);
            if (utfstr->m_nameIndex == nameid && utfstr->m_descriptorIndex == descid)
            {
                std::cout << "N&T \"" << name_ << " : " << descriptor_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added N&T \"" << name_ << " : " << descriptor_ << "\" to the constant pool\n";
    m_constantPool.push_back(new NameAndTypeInfo(nameid, descid));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmFieldRefToTable(const std::string &name_, const std::string &descriptor_, const std::string &class_)
{
    auto classid = addOrConfirmClassToTable(class_);
    auto ntid = addOrConfirmNameAndTypeToTable(name_, descriptor_);
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::FIELDREF)
        {
            auto *ntentry = dynamic_cast<FieldRefInfo*>(el);
            if (ntentry->m_classIndex == classid && ntentry->m_nameAndTypeIndex == ntid)
            {
                std::cout << "FieldRef \"" << class_ << " . " << name_ << " : " << descriptor_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added FieldRef \"" << class_ << " . " << name_ << " : " << descriptor_ << "\" to the constant pool\n";
    m_constantPool.push_back(new FieldRefInfo(classid, ntid));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmMethodRefToTable(const std::string &name_, const std::string &descriptor_, const std::string &class_)
{
    auto classid = addOrConfirmClassToTable(class_);
    auto ntid = addOrConfirmNameAndTypeToTable(name_, descriptor_);
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::METHODREF)
        {
            auto *ntentry = dynamic_cast<MethodRefInfo*>(el);
            if (ntentry->m_classIndex == classid && ntentry->m_nameAndTypeIndex == ntid)
            {
                std::cout << "MethodRef \"" << class_ << " . " << name_ << " : " << descriptor_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added MethodRef \"" << class_ << " . " << name_ << " : " << descriptor_ << "\" to the constant pool\n";
    m_constantPool.push_back(new MethodRefInfo(classid, ntid));
    return m_constantPool.size();
}

void ClassTable::writeBytes(uint64_t bytes_, size_t countBytes_)
{
    char *arr = (char*)&bytes_;  // TODO: to reinterpret
    auto end = sizeof(bytes_);
    for (auto i = countBytes_ - 1; i >= 0 && i < countBytes_; --i)
    {
        m_output.write(arr + i, 1);
    }
}

void ClassTable::writeBytes(const DoublePtrString &str_)
{
    m_output.write(str_.begin, str_.end - str_.begin - 1);
}

void ClassTable::writeInt(int32_t bytes_)
{
    char *arr = (char*)&bytes_;  // TODO: to reinterpret
    auto end = sizeof(bytes_);
    for (auto i = 3; i >= 0 && i < 4; --i)
    {
        m_output.write(arr + i, 1);
    }
}

void MethodInfo::addBytes(uint64_t bytes_, size_t countBytes_)
{
    char *arr = (char*)&bytes_;  // TODO: to reinterpret
    auto end = sizeof(bytes_);
    auto begin = end - countBytes_;
    for (auto i = countBytes_ - 1; i >= 0 && i < countBytes_; --i)
    {
        m_byteCode.push_back(arr[i]);
    }
}


void ClassTable::generateClassFile()
{
    m_output.open(m_classname + ".class", std::ios::binary | std::ios::trunc);
    if (!m_output.is_open())
    {
        std::cout << "Failed to create class file\n";
        return;
    }

    writeBytes(0xcafebabe, 4); // Magic number
    writeBytes(0x0000003f, 4); // Java version
    writeBytes(m_constantPool.size() + 1, 2); // Constant pool size

    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        writeBytes((int)m_constantPool[i]->m_type, 1); // TODO: to reinterpret
        switch (m_constantPool[i]->m_type)
        {
            case (TableEntry::ENTRY_TYPE::UTF8):
            {
                auto *properptr = dynamic_cast<Utf8Info*>(m_constantPool[i]);
                writeBytes(properptr->m_str.end - properptr->m_str.begin - 1, 2);
                writeBytes(properptr->m_str);
                break;
            }

            case (TableEntry::ENTRY_TYPE::INTEGER):
            {
                auto *properptr = dynamic_cast<IntegerInfo*>(m_constantPool[i]);
                writeInt(properptr->m_num);
                break;
            }

            case (TableEntry::ENTRY_TYPE::CLASS):
            {
                auto *properptr = dynamic_cast<ClassInfo*>(m_constantPool[i]);
                writeBytes(properptr->m_nameIndex, 2);
                break;
            }

            case (TableEntry::ENTRY_TYPE::STRING):
            {
                auto *properptr = dynamic_cast<StringInfo*>(m_constantPool[i]);
                writeBytes(properptr->m_stringIndex, 2);
                break;
            }

            case (TableEntry::ENTRY_TYPE::NAMEANDTYPE):
            {
                auto *properptr = dynamic_cast<NameAndTypeInfo*>(m_constantPool[i]);
                writeBytes(properptr->m_nameIndex, 2);
                writeBytes(properptr->m_descriptorIndex, 2);
                break;
            }

            case (TableEntry::ENTRY_TYPE::FIELDREF):
            {
                auto *properptr = dynamic_cast<FieldRefInfo*>(m_constantPool[i]);
                writeBytes(properptr->m_classIndex, 2);
                writeBytes(properptr->m_nameAndTypeIndex, 2);
                break;
            }

            case (TableEntry::ENTRY_TYPE::METHODREF):
            {
                auto *properptr = dynamic_cast<MethodRefInfo*>(m_constantPool[i]);
                writeBytes(properptr->m_classIndex, 2);
                writeBytes(properptr->m_nameAndTypeIndex, 2);
                break;
            }
        }
    }
    
    writeBytes(0x0021, 2); // Access flags
    writeBytes(m_thisClassID, 2); // This class
    writeBytes(m_superClassID, 2); // Super class
    writeBytes(0x0000, 2); // Interface count

    writeBytes(m_fieldPool.size(), 2); // Fields count

    // Fields
    for (int i = 0; i < m_fieldPool.size(); ++i)
    {
        writeBytes(m_fieldPool[i]->m_accessFlags, 2);
        writeBytes(m_fieldPool[i]->m_nameIndex, 2);
        writeBytes(m_fieldPool[i]->m_descIndex, 2);
        writeBytes(0, 2); // Attributes
    }

    writeBytes(m_methodPool.size(), 2); // Methods count

    // Methods
    for (int i = 0; i < m_methodPool.size(); ++i)
    {
        writeBytes(m_methodPool[i]->m_accessFlags, 2); // Access flags
        writeBytes(m_methodPool[i]->m_nameIndex, 2); // Method name flags
        writeBytes(m_methodPool[i]->m_descIndex, 2); // Method descriptor flags
        writeBytes(m_methodPool[i]->m_attribCount, 2); // Count of attributes
        writeBytes(m_methodPool[i]->m_codeAttrNameIndex, 2); // Attribute name index
        writeBytes(m_methodPool[i]->m_codeAttrLength, 4); // Attribute length
        writeBytes(m_methodPool[i]->m_maxStack, 2); // Max stack for code
        writeBytes(m_methodPool[i]->m_maxLocals, 2); // Max locals (including args)
        writeBytes(m_methodPool[i]->m_codeLength, 4); // Length of code
        for (auto &el : m_methodPool[i]->m_byteCode)
        {
            writeBytes(el, 1); // Actual code
        }
        writeBytes(0x0, 2); // Exception table length
        writeBytes(0x0, 2); // Attribute count (for code)
    }

    writeBytes(0x0000, 2); // Attributes count

    m_output.close();
}

//функции обхода деревьев
void ClassTable::treeBypass(Program *node)
{
	if (node==NULL)	return;
	std::cout << node->stmts << std::endl;
    m_ownContext = new VarsContext(nullptr);
    m_currentContext = m_ownContext;
	treeBypass(node->stmts);
}

void ClassTable::treeBypass(StatementGotoCall *node)
{
    // TODO: No idea what to do with it
	if (node==NULL)	return;
	std::cout << node->identifier << std::endl;
}
void ClassTable::treeBypass(StatementGotoLabel *node)
{
    // TODO: No idea what to do with it
	if (node==NULL)	return;
	std::cout << node->identifier << std::endl;
}
void ClassTable::treeBypass(StatementForeachLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

	treeBypass(node->params);
	treeBypass(node->data);

    m_currentContext = m_currentContext->createChildContext();
	treeBypass(node->code);
    m_currentContext = m_currentContext->m_parentContext;

}
void ClassTable::treeBypass(StatementForLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	std::cout << node->identifier << std::endl;
	treeBypass(node->begin);
	treeBypass(node->end);
	treeBypass(node->step);

    m_currentContext = m_currentContext->createChildContext();
	treeBypass(node->code);
    m_currentContext = m_currentContext->m_parentContext;
}

void ClassTable::treeBypass(ParamList *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	for (int i = 0; i < node->lst.size(); i++)
    {
		std::cout << "Param: " << node->lst[i] << std::endl;
        m_currentContext->confirmLocalVar(node->lst[i]);
	}
}

void ClassTable::treeBypass(StatementRepeatLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

    m_currentContext = m_currentContext->createChildContext();
	treeBypass(node->trueCode);
    m_currentContext = m_currentContext->m_parentContext;

	treeBypass(node->condition);
}

void ClassTable::treeBypass(StatementWhileLoop *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

	treeBypass(node->condition);

    m_currentContext = m_currentContext->createChildContext();
	treeBypass(node->trueCode);
    m_currentContext = m_currentContext->m_parentContext;
}

void ClassTable::treeBypass(StatementIfElse *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

	treeBypass(node->condition);

    m_currentContext = m_currentContext->createChildContext();
	treeBypass(node->trueCode);
    m_currentContext = m_currentContext->m_parentContext;

    for (int i = 0; i < node->elseifs.size(); i++)
    {
        m_currentContext = m_currentContext->createChildContext();
		treeBypass(node->elseifs[i]->trueCode);
        m_currentContext = m_currentContext->m_parentContext;
	}

    m_currentContext = m_currentContext->createChildContext();
	treeBypass(node->falseCode);
    m_currentContext = m_currentContext->m_parentContext;
}

void ClassTable::treeBypass(StatementFunctionCall *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	treeBypass(node->functionName);
	treeBypass(node->lst);
}

void ClassTable::treeBypass(StatementReturn *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	treeBypass(node->lst);
}

void ClassTable::treeBypass(StatementMultipleAssign *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

	// Since rights side should ignore newly defined local vars, its vars are calculated first
	treeBypass(node->right);

    if (node->isLocal)
        m_defineLocalVars = true;
	treeBypass(node->left);
    if (node->isLocal)
        m_defineLocalVars = false;
}

void ClassTable::treeBypass(StatementAssign *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

    // Since rights side should ignore newly defined local vars, its vars are calculated first
	treeBypass(node->right);

    if (node->isLocal)
        m_defineLocalVars = true;
	treeBypass(node->left);
    if (node->isLocal)
        m_defineLocalVars = false;
}

void ClassTable::treeBypass(StatementList *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
    node->ownContext = m_currentContext;
	for (int i = 0; i < node->lst.size(); i++)
    {
		treeBypass(node->lst[i]);
	}
}

void ClassTable::treeBypass(Statement *node)
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
			auto *realnode = dynamic_cast<StatementMultipleAssign*>(node);
			treeBypass(realnode);
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
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::FUNCTION_CALL:
		{
			auto *realnode = dynamic_cast<StatementFunctionCall*>(node);
			treeBypass(realnode);
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
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::WHILE_LOOP:
		{
			auto *realnode = dynamic_cast<StatementWhileLoop*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::REPEAT_LOOP:
		{
			auto *realnode = dynamic_cast<StatementRepeatLoop*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::FOR_NUMERIC:
		{
			auto *realnode = dynamic_cast<StatementForLoop*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::FOR_EACH:
		{
			auto *realnode = dynamic_cast<StatementForeachLoop*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::GOTO_LABEL:
		{
			auto *realnode = dynamic_cast<StatementGotoLabel*>(node);
			treeBypass(realnode);
		}
		break;
		case STATEMENT_TYPE::GOTO_CALL:
		{
			auto *realnode = dynamic_cast<StatementGotoCall*>(node);
			treeBypass(realnode);
		}
		break;
	}
}

void ClassTable::treeBypass(ExpressionList *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;
	for (int i = 0; i < node->lst.size(); i++){
		treeBypass(node->lst[i]);
	}
}

void ClassTable::treeBypass(Expression *node)
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
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_MINUS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_MUL:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_DIV:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_CONCAT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::CELL_BY_EXPR:
            // TODO: code gen
            // call func, leaves cell DT on stack
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_REM_DIV:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_EXPON:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_AND:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_OR:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_NOT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::LOG_AND:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::LOG_OR:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::LOG_NOT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
		    break;
		case EXPRESSION_TYPE::UNAR_UMINUS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
		    break;
		case EXPRESSION_TYPE::UNAR_BITWISE_NOT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
		    break;
		case EXPRESSION_TYPE::UNAR_LEN:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
		    break;
		case EXPRESSION_TYPE::REL_EQUALS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::REL_NOT_EQUALS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::REL_GREATER:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::REL_GREATER_EQUALS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::REL_LESS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::REL_LESS_EQUALS:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BIN_FLOOR_DIVISION:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BITWISE_LEFT_SHIFT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::BITWISE_RIGHT_SHIFT:
            // TODO: code gen
            // call func, leaves result on stack
            // if any side is a function call, extracts first value (same as all operators)
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::KEY_VALUE_ASSOC:
            // TODO: code gen
            // expects table on top of stack
            // performs assignment of cell
            // keeps table on top
            treeBypass(node->left);
            treeBypass(node->right);
		    break;
		case EXPRESSION_TYPE::TABLE_CONSTRUCT:
            // TODO: code gen
            // creates table on top of stack
            // recursively assigns values to cells
            treeBypass(node->lst);
		    break;
		case EXPRESSION_TYPE::METHOD_NAME:
            // TODO: code gen
            // expr in left should leave table on top of stack
            // identifier is cell name
            treeBypass(node->left);
		    break;
		case EXPRESSION_TYPE::FUNCTION_CALL:
            // TODO: code gen
            // generates VarList, calls function, leaves VarList of returned values
            // expr in left puts function DynamicType on top of the stack
            treeBypass(node->left);
            treeBypass(node->lst);
		    break;
		case EXPRESSION_TYPE::UNNAMED_FUNCTION_DEFINITION:
            // TODO: code gen
            // generates DynamicType with function on top of stack
            // Probably should be delegated to the FunctionClassTable from this point
            m_currentContext = m_currentContext->createChildContext();
            treeBypass(node->params);
            treeBypass(node->code);
            m_currentContext = m_currentContext->m_parentContext;
		    break;
		case EXPRESSION_TYPE::VARARG_REF:
            throw std::string("VARARG_REF shouldn't exist");
		    break;
	}
}

VarsContext::VarsContext(VarsContext *parentContext_) :
    m_parentContext(parentContext_)
{
    contextID = lastID++;
    std::cout << "Created context " << contextID << std::endl;
}

VarsContext *VarsContext::confirmLocalVar(const std::string &identifier_)
{
    if (std::find(m_variables.begin(), m_variables.end(), identifier_) == m_variables.end())
        m_variables.push_back(identifier_);

    return this;
}

VarsContext *VarsContext::confirmGlobalVar(const std::string &identifier_)
{
    auto *context = this;
    while (std::find(context->m_variables.begin(), context->m_variables.end(), identifier_) != context->m_variables.end() && context->m_parentContext)
        context = context->m_parentContext;

    return context->confirmLocalVar(identifier_);
}

VarsContext *VarsContext::createChildContext()
{
    auto *newcontext = new VarsContext(this);
    m_contexts.push_back(newcontext);
    return newcontext;
}
