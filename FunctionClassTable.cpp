#include "FunctionClassTable.h"
#include "Instructions.h"
int FunctionClassTable::lastID = 1;

FunctionClassTable::FunctionClassTable(VarsContext *ownContext_, ParamList *params_, StatementList *code_) :
    ClassTable(),
    m_params(params_),
    m_code(code_)
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

    auto functionUTF8 = addOrConfirmUtf8ToTable(std::string("function"));
    auto functionType = addOrConfirmUtf8ToTable(std::string("(LVarList;)LVarList;"));

    auto initUTF8 = addOrConfirmUtf8ToTable(std::string("<init>"));
    auto initType = addOrConfirmUtf8ToTable(std::string("()V"));
    auto superInit = addOrConfirmMethodRefToTable("<init>", "()V", "FunctionClass");

    // =========== INIT =================
    m_constructor = new MethodInfo();
    m_constructor->m_accessFlags = 0x0001;
    m_constructor->m_nameIndex = initUTF8;
    m_constructor->m_descIndex = initType;
    m_constructor->m_attribCount = 1;
    m_constructor->m_codeAttrNameIndex = m_codeAttrNameID;
    m_constructor->m_maxStack = 512;
    m_constructor->m_maxLocals = 1;

    m_constructor->addBytes(0x2a, 1); // aload_0
    m_constructor->addBytes(0xb7, 1); // invokespecial
    m_constructor->addBytes(superInit, 2); // super consutrctor

    m_methodPool.push_back(m_constructor);

    // =========== MAIN =================
    m_function = new MethodInfo();
    m_currentCodeRecorder = m_function;
    m_function->m_accessFlags = 0x0001;
    m_function->m_nameIndex = functionUTF8;
    m_function->m_descIndex = functionType;
    m_function->m_attribCount = 1;
    m_function->m_codeAttrNameIndex = m_codeAttrNameID;
    m_function->m_maxStack = 512;
    m_function->m_maxLocals = 2;

    m_methodPool.push_back(m_function);

    std::cout << "VAR DATA: " << m_classname << std::endl;
    generateFunctionClassVariables(m_ownContext);
}

int FunctionClassTable::getClassID() const
{
    return m_functionClassID;
}

std::string FunctionClassTable::generateClassName() const
{
    return std::string("FunctionClass") + std::to_string(m_functionClassID);
}

void FunctionClassTable::generateCode()
{
    m_constructor->addBytes(0xb1, 1);
    m_constructor->m_codeLength = m_constructor->m_byteCode.size();
    m_constructor->m_codeAttrLength = m_constructor->m_codeLength + 12;

    grabParams();

    treeBypassCodeGen(m_code);
    createVarList(m_function);
    m_function->addBytes(ARETURN, 1);

    m_function->m_codeLength = m_function->m_byteCode.size();
    m_function->m_codeAttrLength = m_function->m_codeLength + 12;
}

void FunctionClassTable::initVar(const std::string &identifier_, VarsContext *context_, int fieldref)
{
    createDynamicType(m_function);
    m_function->addBytes(PUTSTATIC, 1); // putstatic
    m_function->addBytes(fieldref, 2); // putstatic
}

void FunctionClassTable::grabParams()
{
    for (auto &param : m_ownContext->m_parameters)
    {
        m_function->addBytes(ALOAD_1, 1); // aload_1
        createIntOnStack(m_function, param.paramID);
        m_function->addBytes(INVOKEVIRTUAL, 1); // invokevirtual
        m_function->addBytes(m_varlistGet, 2); // VarList::get
        m_function->addBytes(PUTSTATIC, 1); // putstatic
        m_function->addBytes(param.paramFldRef, 2); // putstatic

    }

    if (m_params->hasVararg)
    {
        m_function->addBytes(ALOAD_1, 1); // aload_1
        createIntOnStack(m_function, m_params->lst.size());
        m_function->addBytes(INVOKEVIRTUAL, 1); // invokevirtual
        m_function->addBytes(m_varlistSetVarargPoint, 2); // VarList::get
    }
}

void FunctionClassTable::handleVararg()
{
    if (!m_params->hasVararg)
        throw std::string("Vararg used in non-vararg function");

    m_function->addBytes(ALOAD_1, 1); // aload_1
    m_function->addBytes(INVOKEVIRTUAL, 1); // invokevirtual
    m_function->addBytes(m_varlistGetVararg, 2); // VarList::get
}
