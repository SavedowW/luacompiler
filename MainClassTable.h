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
    FieldData createFunctionField(MethodInfo *method, const std::string &functionName, const std::string &functionOwnerClassName, const std::string &className);
    void createDynamicType(MethodInfo *method, int num_);

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