#include "MainClassTable.h"

extern Program *prg;

MainClassTable::MainClassTable() :
    ClassTable()
{

}

void MainClassTable::generateClassTable(const std::string &classname_)
{
    m_classname = classname_;

    treeBypass(prg);

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