#ifndef MAIN_CLASS_TABLE_H_
#define MAIN_CLASS_TABLE_H_
#include "ClassTable.h"
#include "FunctionClassTable.h"

class MainClassTable : public ClassTable
{
public:
    MainClassTable();
    void generateClassTable(const std::string &classname_) override;


protected:
    size_t m_javaLangSystemID = 1;
    size_t m_javaIOPrintStreamID = 1;
    size_t m_outFieldID = 1;
    size_t m_helloWorldStr = 1;
    size_t m_printlnStrID = 1;
    size_t m_mainNameID = 1;
    size_t m_mainTypeID = 1;
    size_t m_codeAttrNameID = 1;
    size_t m_fldref = 1;
    size_t m_placeholderStr = 1;
};

#endif