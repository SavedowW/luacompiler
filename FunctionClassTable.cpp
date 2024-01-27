#include "FunctionClassTable.h"

int FunctionClassTable::lastID = 1;

FunctionClassTable::FunctionClassTable(VarsContext *ownContext_) :
    ClassTable()
{
    m_ownContext = ownContext_;
    m_functionClassID = lastID++;
}

void FunctionClassTable::generateClassTable(const std::string &classname_)
{
    m_classname = classname_;

    generateUniversalTable();

    m_thisClassID = addOrConfirmClassToTable(classname_);
    m_superClassID = addOrConfirmClassToTable("FunctionClass");

    auto functionUTF8 = addOrConfirmUtf8ToTable("function");
    auto functionType = addOrConfirmUtf8ToTable("()V");

    auto initUTF8 = addOrConfirmUtf8ToTable("<init>");
    auto superInit = addOrConfirmMethodRefToTable("<init>", "()V", "FunctionClass");

    // =========== INIT =================
    m_constructor = new MethodInfo();
    m_constructor->m_accessFlags = 0x0001;
    m_constructor->m_nameIndex = initUTF8;
    m_constructor->m_descIndex = functionType;
    m_constructor->m_attribCount = 1;
    m_constructor->m_codeAttrNameIndex = m_codeAttrNameID;
    m_constructor->m_maxStack = 10;
    m_constructor->m_maxLocals = 1;

    m_constructor->addBytes(0x2a, 1); // aload_0
    m_constructor->addBytes(0xb7, 1); // invokespecial
    m_constructor->addBytes(superInit, 2); // super consutrctor

    m_methodPool.push_back(m_constructor);

    // =========== MAIN =================
    m_function = new MethodInfo();
    m_function->m_accessFlags = 0x0001;
    m_function->m_nameIndex = functionUTF8;
    m_function->m_descIndex = functionType;
    m_function->m_attribCount = 1;
    m_function->m_codeAttrNameIndex = m_codeAttrNameID;
    m_function->m_maxStack = 3;
    m_function->m_maxLocals = 1;

    m_methodPool.push_back(m_function);

    std::cout << "VAR DATA: " << m_classname << std::endl;
    generateFunctionClassVariables(m_ownContext);



    m_constructor->addBytes(0xb1, 1);
    m_constructor->m_codeLength = m_constructor->m_byteCode.size();
    m_constructor->m_codeAttrLength = m_constructor->m_codeLength + 12;
    
    m_function->addBytes(0xb1, 1);

    m_function->m_codeLength = m_function->m_byteCode.size();
    m_function->m_codeAttrLength = m_function->m_codeLength + 12;
}

int FunctionClassTable::getClassID() const
{
    return m_functionClassID;
}

std::string FunctionClassTable::generateClassName() const
{
    return std::string("FunctionClass") + std::to_string(m_functionClassID);
}

void FunctionClassTable::initVar(const std::string &identifier_, VarsContext *context_, int fieldref)
{
    createDynamicType(m_function);
    m_function->addBytes(0xb3, 1); // putstatic
    m_function->addBytes(fieldref, 2); // putstatic
}

void FunctionClassTable::grabParams()
{
    for (auto &param : m_ownContext->m_parameters)
    {
        m_function->addBytes(0x2b, 1); // aload_1
    }
}
