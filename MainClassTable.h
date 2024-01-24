#ifndef MAIN_CLASS_TABLE_H_
#define MAIN_CLASS_TABLE_H_
#include "ClassTable.h"
#include "FunctionClassTable.h"

struct FieldData
{
    size_t m_fieldNameID;
    size_t m_fieldDescID;
    size_t m_fieldRefID;
};

class MainClassTable : public ClassTable
{
public:
    MainClassTable();
    void generateClassTable(const std::string &classname_) override;


protected:
    FieldData createFunctionField(MethodInfo *method, const std::string &functionName, const std::string &className);

    size_t m_javaLangSystemID = 1;
    size_t m_javaIOPrintStreamID = 1;
    size_t m_outFieldID = 1;
    size_t m_helloWorldStr = 1;
    size_t m_printlnStrID = 1;
    size_t m_mainNameID = 1;
    size_t m_mainTypeID = 1;
    size_t m_codeAttrNameID = 1;
    size_t m_placeholderStr = 1;

    size_t m_dtClass = 1;
    size_t m_dtInitIdI = 1;
    size_t m_dtInitIdD = 1;
    size_t m_dtInitIdS = 1;
    size_t m_dtInitIdF = 1;
    size_t m_dtInitIdNIL = 1;
    size_t m_dtFieldIdI = 1;
    size_t m_dtFieldIdD = 1;
    size_t m_dtFieldIdS = 1;
    size_t m_dtFieldIdF = 1;
    size_t m_dtCallRef = 1;
};

#endif