#ifndef CLASS_TABLE_H_
#define CLASS_TABLE_H_
#include "Tree.h"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <memory>

struct VarDependency;
struct ParameterDependency;

struct FieldData
{
    size_t m_fieldNameID;
    size_t m_fieldDescID;
    size_t m_fieldRefID;
};

struct VarsContext
{
    VarsContext(VarsContext *parentContext_);

    std::vector<std::string> m_variables;
    std::vector<VarsContext*> m_contexts;
    std::vector<VarDependency> m_dependencies;
    std::vector<ParameterDependency> m_parameters;
    VarsContext *m_parentContext = nullptr;

    VarsContext *confirmLocalVar(const std::string &identifier_);
    VarsContext *confirmGlobalVar(const std::string &identifier_);
    VarsContext *confirmParameter(const std::string &identifier_, int paramID_);
    VarsContext *createChildContext();
    void grabNearestClassName();
    VarsContext *getOriginalFunctionContext();
    
    static int lastID;
    int contextID = 0;
    std::string className;
};

struct VarDependency {
    VarsContext *m_context;
    std::string varName;
};

struct ParameterDependency {
    std::string paramName;
    int paramID;
    int paramFldRef;
};

class TableEntry
{
public:
    enum class ENTRY_TYPE {
        UTF8 = 1,
        INTEGER = 3,
        FLOAT = 4,
        LONG = 5,
        DOUBLE = 6,
        CLASS = 7,
        STRING = 8,
        FIELDREF = 9,
        METHODREF = 10,
        INTERFACEMETHODREF = 11,
        NAMEANDTYPE = 12,
        METHODHANDLE = 15,
        METHODTYPE = 16,
        INVOKEDYNAMIC = 18,
    } m_type;

    TableEntry(ENTRY_TYPE type_);
    virtual ~TableEntry() = default;
};

class Utf8Info : public TableEntry
{
public:
    Utf8Info(const std::string &s_);
    Utf8Info(const DoublePtrString &s_);
    DoublePtrString m_str;
    virtual ~Utf8Info();
};

class IntegerInfo : public TableEntry
{
public:
    IntegerInfo(int num_);
    int m_num;
    virtual ~IntegerInfo() = default;
};

class FloatInfo : public TableEntry
{
public:
    FloatInfo(float num_);
    float m_num;
    virtual ~FloatInfo() = default;
};

class StringInfo : public TableEntry
{
public:
    StringInfo(size_t stringIndex_);
    size_t m_stringIndex;
    virtual ~StringInfo() = default;
};

class ClassInfo : public TableEntry
{
public:
    ClassInfo(size_t nameIndex_);
    size_t m_nameIndex;
    virtual ~ClassInfo() = default;
};

class FieldRefInfo : public TableEntry
{
public:
    FieldRefInfo(size_t classIndex_, size_t nameAndTypeIndex_);
    size_t m_classIndex;
    size_t m_nameAndTypeIndex;
    virtual ~FieldRefInfo() = default;
};

class MethodRefInfo : public TableEntry
{
public:
    MethodRefInfo(size_t classIndex_, size_t nameAndTypeIndex_);
    size_t m_classIndex;
    size_t m_nameAndTypeIndex;
    virtual ~MethodRefInfo() = default;
};

struct CodeRecorder
{
    CodeRecorder() = default;
    std::vector<char> m_byteCode;
    std::vector<int> m_unsolvedBreakLocations;
    void markBreak();
    void solveBreaks();
    void addBytes(uint64_t bytes_, size_t countBytes_);
    void addBytes(int16_t bytes_);
    void setBytesAt(int16_t bytes_, int location_);
    void append(CodeRecorder *moreCode_);
    virtual ~CodeRecorder() = default;
};

class MethodInfo : public CodeRecorder
{
public:
    MethodInfo() = default;
    uint32_t m_accessFlags;
    size_t m_nameIndex;
    size_t m_descIndex; // descriptor (type)
    uint32_t m_attribCount = 1;
    uint32_t m_codeAttrNameIndex;
    uint32_t m_codeAttrLength;
    uint32_t m_maxStack = 2;
    uint32_t m_maxLocals = 1;
    uint32_t m_codeLength = 1;
    virtual ~MethodInfo() = default;
};

class FieldInfo
{
public:
    uint32_t m_accessFlags;
    size_t m_nameIndex;
    size_t m_descIndex;
};

class NameAndTypeInfo : public TableEntry
{
public:
    NameAndTypeInfo(size_t nameIndex_, size_t descriptorIndex_);
    size_t m_nameIndex;
    size_t m_descriptorIndex;
    virtual ~NameAndTypeInfo() = default;
};


class ClassTable
{
public:
    ClassTable();
    virtual void generateClassTable(const std::string &classname_) = 0;
    virtual void generateClassFile();
    virtual void generateCode() = 0;


    std::string m_classname;    
protected:
    std::ofstream m_output;
    size_t addOrConfirmUtf8ToTable(const std::string &s_);
    size_t addOrConfirmUtf8ToTable(const DoublePtrString &s_);
    size_t addOrConfirmIntegerToTable(int num_);
    size_t addOrConfirmFloatToTable(float num_);
    size_t addOrConfirmClassToTable(const std::string &s_);
    size_t addOrConfirmStringToTable(const std::string &s_);
    size_t addOrConfirmStringToTable(const DoublePtrString &s_);
    size_t addOrConfirmNameAndTypeToTable(const std::string &name_, const std::string &descriptor_);
    size_t addOrConfirmFieldRefToTable(const std::string &name_, const std::string &descriptor_, const std::string &class_);
    size_t addOrConfirmMethodRefToTable(const std::string &name_, const std::string &descriptor_, const std::string &class_);
    void writeBytes(uint64_t bytes_, size_t countBytes_);
    void writeBytes(const DoublePtrString &str_);
    void writeInt(int32_t bytes_);
    void writeFloat(float bytes_);
    virtual void grabParams();
    void generateUniversalTable();

    bool doesFieldExist(int nameId_);
    
    virtual void initVar(const std::string &identifier_, VarsContext *context_, int fieldref) = 0;

    void generateFunctionClassVariables(VarsContext *currentContext);

    size_t m_thisClassID = 0;
    size_t m_superClassID = 0;

    std::vector<TableEntry*> m_constantPool; // TODO: use unique ptr
    std::vector<MethodInfo*> m_methodPool; // TODO: use unique ptr
    std::vector<FieldInfo*> m_fieldPool; // TODO: use unique ptr


    // Code gen
    size_t m_dtClass = 1;
    size_t m_dtInitIdI = 1;
    size_t m_dtInitIdFl = 1;
    size_t m_dtInitIdS = 1;
    size_t m_dtInitIdB = 1;
    size_t m_dtInitIdF = 1;
    size_t m_dtInitIdNIL = 1;
    size_t m_dtCreateTable = 1;
    size_t m_dtFieldIdI = 1;
    size_t m_dtFieldIdD = 1;
    size_t m_dtFieldIdS = 1;
    size_t m_dtFieldIdF = 1;
    size_t m_dtCallRef = 1;
    size_t m_dtCallMethodRef = 1;
    size_t m_dtRevset = 1;
    size_t m_dtSetF = 1;
    size_t m_dtSetDT = 1;
    size_t m_dt__add = 1;
    size_t m_dt__sub = 1;
    size_t m_dt__mul = 1;
    size_t m_dt__div = 1;
    size_t m_dt__mod = 1;
    size_t m_dt__pow = 1;
    size_t m_dt__unm = 1;
    size_t m_dt__idiv = 1;
    size_t m_dt__band = 1;
    size_t m_dt__bor = 1;
    size_t m_dt__bxor = 1;
    size_t m_dt__bnot = 1;
    size_t m_dt__shl = 1;
    size_t m_dt__shr = 1;
    size_t m_dt__concat = 1;
    size_t m_dt__len = 1;
    size_t m_dt__and = 1;
    size_t m_dt__or = 1;
    size_t m_dt__not = 1;
    size_t m_dt__lt = 1;
    size_t m_dt__le = 1;
    size_t m_dt__eq = 1;
    size_t m_dt__neq = 1;
    size_t m_dt__gt = 1;
    size_t m_dt__ge = 1;
    size_t m_dt__index = 1;
    size_t m_dt__newindex = 1;
    size_t m_dt__autoindex = 1;
    size_t m_dt_toBool = 1;
    size_t m_varlistClass = 1;
    size_t m_varlistInit = 1;
    size_t m_varlistAdd = 1;
    size_t m_varlistAddRef = 1;
    size_t m_varlistGet = 1;
    size_t m_varlistAssign = 1;
    size_t m_varlistAppend = 1;
    size_t m_varlistSetVarargPoint = 1;
    size_t m_varlistGetVararg = 1;
    size_t m_codeAttrNameID = 1;
    size_t m_forLoopIter = 1;
    size_t m_forLoopCond = 1;
    size_t m_foreachHead = 1;
    size_t m_foreachCondition = 1;

    VarsContext *m_ownContext = nullptr;
    MethodInfo *m_constructor = nullptr;
    MethodInfo *m_function = nullptr;

    bool m_assignContext = false;
    CodeRecorder *m_currentCodeRecorder = nullptr;
    int cycleCount = 0;

    FieldData createFunctionField(CodeRecorder *method, const std::string &functionName, const std::string &functionOwnerClassName, const std::string &className);
    FieldData createTableField(CodeRecorder *method, const std::string &tableName, const std::string &className);
    void createFunctionInTableField(CodeRecorder *method, const std::string &functionField, const std::string &functionOwnerClassName, int tableRef, const std::string &className);
    int createIntOnStack(CodeRecorder *method, int num_);
    void createFloatOnStack(CodeRecorder *method, float num_);
    void createStringOnStack(CodeRecorder *method, const DoublePtrString &s_);
    void createVarList(CodeRecorder *method);
    void createDynamicType(CodeRecorder *method, int num_);
    void createDynamicType(CodeRecorder *method, float num_);
    void createDynamicType(CodeRecorder *method, const DoublePtrString &s_);
    void createDynamicType(CodeRecorder *method, bool val_);
    void createDynamicTypeTable(CodeRecorder *method);
    void createDynamicType(CodeRecorder *method);

    void treeBypassCodeGen(Program *);
    void treeBypassCodeGen(StatementList *);
    void treeBypassCodeGen(Statement *);
    void treeBypassCodeGen(StatementAssign *);
    void treeBypassCodeGen(StatementMultipleAssign *);
    void treeBypassCodeGen(StatementFunctionCall *);
    void treeBypassCodeGen(ExpressionList *);
    void treeBypassCodeGen_CreateReferences(ExpressionList *);
    void treeBypassCodeGen(ParamList *);
    void treeBypassCodeGen(Expression *);
    void treeBypassCodeGen(StatementGotoCall *);
    void treeBypassCodeGen(StatementGotoLabel *); 
    void treeBypassCodeGen(StatementForeachLoop *);
    void treeBypassCodeGen(StatementForLoop *);
    void treeBypassCodeGen(StatementRepeatLoop *);
    void treeBypassCodeGen(StatementWhileLoop *);
    void treeBypassCodeGen(StatementIfElse *);
    void treeBypassCodeGen(StatementReturn *);

    virtual void handleVararg();
};

#endif