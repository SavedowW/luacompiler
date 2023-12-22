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

class ClassInfo : public TableEntry
{
public:
    ClassInfo(size_t nameIndex_);
    size_t m_nameIndex;
    virtual ~ClassInfo() = default;
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

    size_t addOrConfirmUtf8ToTable(const std::string &s_);
    size_t addOrConfirmClassToTable(const std::string &s_);
    void writeBytes(uint64_t bytes_, size_t countBytes_);
    void writeBytes(const DoublePtrString &str_);
    std::vector<TableEntry*> m_constantPool; // TODO: use unique ptr
};

#endif