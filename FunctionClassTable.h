#ifndef FUNCTION_CLASS_TABLE_H_
#define FUNCTION_CLASS_TABLE_H_
#include "ClassTable.h"

class FunctionClassTable : public ClassTable
{
public:
    FunctionClassTable(VarsContext *ownContext_);
    void generateClassTable(const std::string &classname_) override;
    int getClassID() const;
    std::string generateClassName() const;


protected:

    static int lastID;
    int m_functionClassID = 0;
};

#endif