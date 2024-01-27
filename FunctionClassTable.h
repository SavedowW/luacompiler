#ifndef FUNCTION_CLASS_TABLE_H_
#define FUNCTION_CLASS_TABLE_H_
#include "ClassTable.h"

class FunctionClassTable : public ClassTable
{
public:
    FunctionClassTable(VarsContext *ownContext_, ParamList *params_, StatementList *code_);
    void generateClassTable(const std::string &classname_) override;
    int getClassID() const;
    std::string generateClassName() const;
    void generateCode() override;


protected:
    void initVar(const std::string &identifier_, VarsContext *context_, int fieldref) override;
    void grabParams() override;

    static int lastID;
    int m_functionClassID = 0;

    ParamList *m_params;
    StatementList *m_code;
};

#endif