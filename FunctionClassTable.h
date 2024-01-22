#ifndef FUNCTION_CLASS_TABLE_H_
#define FUNCTION_CLASS_TABLE_H_
#include "ClassTable.h"

class FunctionClassTable : public ClassTable
{
public:
    FunctionClassTable();
    void generateClassTable(const std::string &classname_) override;


protected:
};

#endif