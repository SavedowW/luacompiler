#ifndef CLASS_TABLE_H_
#define CLASS_TABLE_H_
#include "Tree.h"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <memory>

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
    DoublePtrString m_str;
    virtual ~Utf8Info();
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

class MethodInfo
{
public:
    MethodInfo() = default;
    uint32_t m_accessFlags;
    size_t m_nameIndex;
    size_t m_descIndex;
    uint32_t m_attribCount = 1;
    uint32_t m_codeAttrNameIndex;
    uint32_t m_codeAttrLength;
    uint32_t m_maxStack = 2;
    uint32_t m_maxLocals = 1;
    uint32_t m_codeLength = 1;
    std::vector<char> m_byteCode;
    void addBytes(uint64_t bytes_, size_t countBytes_);
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
    void generateClassTable(const std::string &classname_);
    void generateClassFile();


private:
    std::string m_classname;
    std::ofstream m_output;
    size_t m_mainClassID = 1;
    size_t m_javaLangObjectID = 1;
    size_t m_javaLangSystemID = 1;
    size_t m_javaIOPrintStreamID = 1;
    size_t m_outFieldID = 1;
    size_t m_helloWorldStr = 1;
    size_t m_printlnStrID = 1;
    size_t m_mainNameID = 1;
    size_t m_mainTypeID = 1;
    size_t m_codeAttrNameID = 1;
    size_t m_fldref = 1;
    size_t m_mtd2ref = 1;
    size_t m_placeholderStr = 1;

    size_t addOrConfirmUtf8ToTable(const std::string &s_);
    size_t addOrConfirmClassToTable(const std::string &s_);
    size_t addOrConfirmStringToTable(const std::string &s_);
    size_t addOrConfirmNameAndTypeToTable(const std::string &name_, const std::string &descriptor_);
    size_t addOrConfirmFieldRefToTable(const std::string &name_, const std::string &descriptor_, const std::string &class_);
    size_t addOrConfirmMethodRefToTable(const std::string &name_, const std::string &descriptor_, const std::string &class_);
    void writeBytes(uint64_t bytes_, size_t countBytes_);
    void writeBytes(const DoublePtrString &str_);
    std::vector<TableEntry*> m_constantPool; // TODO: use unique ptr
    std::vector<MethodInfo*> m_methodPool; // TODO: use unique ptr
    std::vector<FieldInfo*> m_fieldPool; // TODO: use unique ptr
};

#endif