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

    m_thisClassID = addOrConfirmClassToTable(classname_);
    m_superClassID = addOrConfirmClassToTable("FunctionClass");

    auto strid = addOrConfirmStringToTable("filler text");
    auto outFieldID = addOrConfirmFieldRefToTable("out", "Ljava/io/PrintStream;", "java/lang/System");
    auto printid = addOrConfirmMethodRefToTable("println", "(Ljava/lang/String;)V", "java/io/PrintStream");

    auto functionUTF8 = addOrConfirmUtf8ToTable("function");
    auto functionType = addOrConfirmUtf8ToTable("()V");
    auto codeAttrNameID = addOrConfirmUtf8ToTable("Code");

    auto initUTF8 = addOrConfirmUtf8ToTable("<init>");
    auto superInit = addOrConfirmMethodRefToTable("<init>", "()V", "FunctionClass");

    //auto dtclass = addOrConfirmClassToTable("DynamicType");
    //auto dtinitid = addOrConfirmMethodRefToTable("<init>", "(I)V", "DynamicType");
    //auto dtfieldid = addOrConfirmFieldRefToTable("iValue", "I", "DynamicType");

    // =========== INIT =================
    auto *mainmethod = new MethodInfo();
    mainmethod->m_accessFlags = 0x0001;
    mainmethod->m_nameIndex = initUTF8;
    mainmethod->m_descIndex = functionType;
    mainmethod->m_attribCount = 1;
    mainmethod->m_codeAttrNameIndex = codeAttrNameID;
    mainmethod->m_maxStack = 1;
    mainmethod->m_maxLocals = 1;

    mainmethod->addBytes(0x2a, 1); // aload_0
    mainmethod->addBytes(0xb7, 1); // invokespecial
    mainmethod->addBytes(superInit, 2); // super consutrctor
    mainmethod->addBytes(0xb1, 1);
    mainmethod->m_codeLength = mainmethod->m_byteCode.size();
    mainmethod->m_codeAttrLength = mainmethod->m_codeLength + 12;

    m_methodPool.push_back(mainmethod);

    // =========== MAIN =================
    mainmethod = new MethodInfo();
    mainmethod->m_accessFlags = 0x0001;
    mainmethod->m_nameIndex = functionUTF8;
    mainmethod->m_descIndex = functionType;
    mainmethod->m_attribCount = 1;
    mainmethod->m_codeAttrNameIndex = codeAttrNameID;
    mainmethod->m_maxStack = 3;
    mainmethod->m_maxLocals = 1;

    mainmethod->addBytes(0xb2, 1); // getstatic
    mainmethod->addBytes(outFieldID, 2); // System.out field
    mainmethod->addBytes(0x12, 1); // ldc
    mainmethod->addBytes(strid, 1); // string
    mainmethod->addBytes(0xb6, 1); // Invoke virtual
    mainmethod->addBytes(printid, 2); // println(string)
    mainmethod->addBytes(0xb1, 1);
    mainmethod->m_codeLength = mainmethod->m_byteCode.size();
    mainmethod->m_codeAttrLength = mainmethod->m_codeLength + 12;

    m_methodPool.push_back(mainmethod);

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
