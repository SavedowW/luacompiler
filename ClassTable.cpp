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

void ClassTable::grabParams()
{
}

void ClassTable::generateUniversalTable()
{
    m_thisClassID = addOrConfirmClassToTable(m_classname);
    m_superClassID = addOrConfirmClassToTable("java/lang/Object");
    m_codeAttrNameID = addOrConfirmUtf8ToTable("Code");

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
    
    m_varlistClass = addOrConfirmClassToTable("VarList");
    m_varlistInit = addOrConfirmMethodRefToTable("<init>", "()V", "VarList");
    m_varlistAdd = addOrConfirmMethodRefToTable("add", "(LDynamicType;)V", "VarList");
    m_varlistGet = addOrConfirmMethodRefToTable("get", "(I)LDynamicType;", "VarList");
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
    while (std::find(context->m_variables.begin(), context->m_variables.end(), identifier_) == context->m_variables.end() && context->m_parentContext)
    {
        context = context->m_parentContext;
    }

    if (context->className != className)
    {
        bool alreadyHasDependency = false;
        auto functionContext = getOriginalFunctionContext();
        for (const auto &el : functionContext->m_dependencies)
        {
            if (el.m_context == context && el.varName == identifier_)
            {
                alreadyHasDependency = true;
                break;
            }
        }
        if (!alreadyHasDependency)
            functionContext->m_dependencies.push_back({context, identifier_});
    }
    else
    {
        context->confirmLocalVar(identifier_);
    }

    return context;
}

VarsContext *VarsContext::confirmParameter(const std::string &identifier_, int paramID_)
{
    if (std::find(m_variables.begin(), m_variables.end(), identifier_) == m_variables.end())
    {
        m_variables.push_back(identifier_);
        m_parameters.push_back({identifier_, paramID_});
    }

    return this;
}

VarsContext *VarsContext::createChildContext()
{
    auto *newcontext = new VarsContext(this);
    m_contexts.push_back(newcontext);
    newcontext->grabNearestClassName();
    return newcontext;
}

void VarsContext::grabNearestClassName()
{
    auto *context = m_parentContext;
    while (context->className == "" && context->m_parentContext)
    {
        context = context->m_parentContext;
    }

    className = context->className;
}

VarsContext *VarsContext::getOriginalFunctionContext()
{
    auto *context = this;
    while (context->m_parentContext && context->className == context->m_parentContext->className)
    {
        context = context->m_parentContext;
    }

    return context;
}

void ClassTable::generateFunctionClassVariables(VarsContext *currentContext)
{
    if (currentContext->m_parameters.size() > 0)
    {
        std::cout << "Parameters:\n";
        for (auto &el : currentContext->m_parameters)
        {
            auto varname = std::string("CONTEXT_") + std::to_string(currentContext->contextID) + "_" + el.paramName;
            auto fldName = addOrConfirmUtf8ToTable(varname);
            auto fldDesc = addOrConfirmUtf8ToTable("LDynamicType;");
            auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", currentContext->className);
            auto *paramfld = new FieldInfo();
            paramfld->m_accessFlags = 0x0009;
            paramfld->m_nameIndex = fldName;
            paramfld->m_descIndex = fldDesc;
            m_fieldPool.push_back(paramfld);
            std::cout << m_classname << "::" << varname << " (" << el.paramID << ")\n";
        }
    }

    if (currentContext->m_dependencies.size() > 0)
    {
        std::cout << "Dependencies:\n";
        for (auto &el : currentContext->m_dependencies)
        {
            auto varname = std::string("DEP_CONTEXT_") + std::to_string(currentContext->contextID) + "_" + el.varName;
            auto sourceVarName = std::string("CONTEXT_") + std::to_string(el.m_context->contextID) + "_" + el.varName;
            auto fldName = addOrConfirmUtf8ToTable(varname);
            auto fldDesc = addOrConfirmUtf8ToTable("LDynamicType;");
            auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", currentContext->className);
            auto sourceVarNameFieldRef = addOrConfirmFieldRefToTable(sourceVarName, "LDynamicType;", el.m_context->className);
            auto *depfld = new FieldInfo();
            depfld->m_accessFlags = 0x0001;
            depfld->m_nameIndex = fldName;
            depfld->m_descIndex = fldDesc;
            m_fieldPool.push_back(depfld);

            m_constructor->addBytes(0x2a, 1); // aload_0
            m_constructor->addBytes(0xb2, 1); // getstatic
            m_constructor->addBytes(sourceVarNameFieldRef, 2); // field reference
            m_constructor->addBytes(0xb5, 1); // putfield
            m_constructor->addBytes(fldRef, 2); // putfield

            std::cout << el.m_context->className << "::" << "CONTEXT_" << el.m_context->contextID << "_" << el.varName << std::endl;
        }
    }

    if (currentContext->m_variables.size() > 0)
    {
        std::cout << "Variables:\n";
        for (auto &el : currentContext->m_variables)
        {
            auto varname = std::string("CONTEXT_") + std::to_string(currentContext->contextID) + "_" + el;
            auto fldName = addOrConfirmUtf8ToTable(varname);
            auto fldDesc = addOrConfirmUtf8ToTable("LDynamicType;");
            auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", currentContext->className);
            auto *varfld = new FieldInfo();
            varfld->m_accessFlags = 0x0009;
            varfld->m_nameIndex = fldName;
            varfld->m_descIndex = fldDesc;
            m_fieldPool.push_back(varfld);
            initVar(varname, currentContext, fldRef);
            std::cout << m_classname << "::" << varname << std::endl;
        }
    }

    for (auto *child : currentContext->m_contexts)
    {
        if (child->className == currentContext->className)
            generateFunctionClassVariables(child);
    }
}


FieldData ClassTable::createFunctionField(MethodInfo *method, const std::string &functionName, const std::string &functionOwnerClassName, const std::string &className)
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

void ClassTable::createDynamicType(MethodInfo *method, int num_)
{
    // Create DT
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

void ClassTable::createDynamicType(MethodInfo *method)
{
    // Create DT
    method->addBytes(0xbb, 1); // new
    method->addBytes(m_dtClass, 2); // DynamicType
    method->addBytes(0x59, 1); // dup

    method->addBytes(0xb7, 1); // invokespecial
    method->addBytes(m_dtInitIdNIL, 2); // <init>
}

