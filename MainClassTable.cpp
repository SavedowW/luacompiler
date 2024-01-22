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
    m_javaLangSystemID = addOrConfirmClassToTable("java/lang/System");
    m_javaIOPrintStreamID = addOrConfirmClassToTable("java/io/PrintStream");
    m_helloWorldStr = addOrConfirmStringToTable("Hello, world!");
    m_placeholderStr = addOrConfirmStringToTable("Placeholder func");
    m_outFieldID = addOrConfirmFieldRefToTable("out", "Ljava/io/PrintStream;", "java/lang/System");
    m_printlnStrID = addOrConfirmMethodRefToTable("println", "(I)V", "java/io/PrintStream");
    m_mainNameID = addOrConfirmUtf8ToTable("main");
    m_mainTypeID = addOrConfirmUtf8ToTable("([Ljava/lang/String;)V");
    m_codeAttrNameID = addOrConfirmUtf8ToTable("Code");

    auto fldName = addOrConfirmUtf8ToTable("nobj");
    auto fldDesc = addOrConfirmUtf8ToTable("LDynamicType;");
    m_fldref = addOrConfirmFieldRefToTable("nobj", "LDynamicType;", classname_);

    auto dtclass = addOrConfirmClassToTable("DynamicType");
    auto dtinitid = addOrConfirmMethodRefToTable("<init>", "(I)V", "DynamicType");
    auto dtfieldid = addOrConfirmFieldRefToTable("iValue", "I", "DynamicType");

    auto func1class = addOrConfirmClassToTable("FunctionClass1");
    auto func1call = addOrConfirmMethodRefToTable("function", "()V", "FunctionClass1");
    auto func1init = addOrConfirmMethodRefToTable("<init>", "()V", "FunctionClass1");

    // =========== MAIN =================
    auto *mainmethod = new MethodInfo();
    mainmethod->m_accessFlags = 0x0009;
    mainmethod->m_nameIndex = m_mainNameID;
    mainmethod->m_descIndex = m_mainTypeID;
    mainmethod->m_attribCount = 1;
    mainmethod->m_codeAttrNameIndex = m_codeAttrNameID;
    mainmethod->m_maxStack = 3;
    mainmethod->m_maxLocals = 1;

    /* Initialize DynamicType and put into field
    mainmethod->addBytes(0xbb, 1); // new
    mainmethod->addBytes(dtclass, 2); // DynamicType
    mainmethod->addBytes(0x59, 1); // dup
    mainmethod->addBytes(0x2, 1); // iconst_m1
    mainmethod->addBytes(0xb7, 1); // invokespecial
    mainmethod->addBytes(dtinitid, 2); // <init>
    mainmethod->addBytes(0xb3, 1); // putstatic
    mainmethod->addBytes(m_fldref, 2); // nobj field
    */
    mainmethod->addBytes(0xbb, 1); // new
    mainmethod->addBytes(func1class, 2); // FunctionClass1
    mainmethod->addBytes(0x59, 1); // dup
    mainmethod->addBytes(0xb7, 1); // invokespecial
    mainmethod->addBytes(func1init, 2); // <init>
    mainmethod->addBytes(0xb6, 1); // Invoke virtual
    mainmethod->addBytes(func1call, 2); // FunctionClass1 function
    mainmethod->addBytes(0xb1, 1);
    mainmethod->m_codeLength = mainmethod->m_byteCode.size();
    mainmethod->m_codeAttrLength = mainmethod->m_codeLength + 12;

    m_methodPool.push_back(mainmethod);

    auto *mainfield = new FieldInfo();
    mainfield->m_accessFlags = 0x0009;
    mainfield->m_nameIndex = fldName;
    mainfield->m_descIndex = fldDesc;
    m_fieldPool.push_back(mainfield);

    FunctionClassTable funcclass;
    funcclass.generateClassTable("FunctionClass1");
    funcclass.generateClassFile();
}