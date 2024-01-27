#ifndef MAIN_CLASS_TABLE_H_
#define MAIN_CLASS_TABLE_H_
#include "ClassTable.h"
#include "FunctionClassTable.h"

class MainClassTable : public ClassTable
{
public:
    MainClassTable();
    void generateClassTable(const std::string &classname_) override;
    void generateClassFiles();


protected:
    std::vector<FunctionClassTable*> m_functionClasses;
    VarsContext *m_currentContext = nullptr;
    bool m_defineLocalVars = false;
    void initVar(const std::string &identifier_, VarsContext *context_, int fieldref) override;

    void treeBypassVarLinking(Program *);
    void treeBypassVarLinking(StatementList *);
    void treeBypassVarLinking(Statement *);
    void treeBypassVarLinking(ExpressionList *);
    void treeBypassVarLinking(ParamList *);
    void treeBypassVarLinking(Expression *);
    void treeBypassVarLinking(StatementGotoCall *);
    void treeBypassVarLinking(StatementGotoLabel *); 
    void treeBypassVarLinking(StatementForeachLoop *);
    void treeBypassVarLinking(StatementForLoop *);
    void treeBypassVarLinking(StatementRepeatLoop *);
    void treeBypassVarLinking(StatementWhileLoop *);
    void treeBypassVarLinking(StatementIfElse *);
    void treeBypassVarLinking(StatementFunctionCall *);
    void treeBypassVarLinking(StatementReturn *);
    void treeBypassVarLinking(StatementMultipleAssign *);
    void treeBypassVarLinking(StatementAssign *);
};

#endif