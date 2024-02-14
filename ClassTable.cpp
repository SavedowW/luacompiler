#include "ClassTable.h"
#include <algorithm>
#include "Instructions.h"

extern Program *prg;

int VarsContext::lastID = 0;

TableEntry::TableEntry(ENTRY_TYPE type_):
    m_type(type_)
{
}


Utf8Info::Utf8Info(const std::string &s_):
    TableEntry(TableEntry::ENTRY_TYPE::UTF8),
    m_str(s_.c_str())
{
}

Utf8Info::Utf8Info(const DoublePtrString &s_):
    TableEntry(TableEntry::ENTRY_TYPE::UTF8),
    m_str(s_)
{
}

IntegerInfo::IntegerInfo(int num_):
    TableEntry(TableEntry::ENTRY_TYPE::INTEGER),
    m_num(num_)
{
}

FloatInfo::FloatInfo(float num_):
    TableEntry(TableEntry::ENTRY_TYPE::FLOAT),
    m_num(num_)
{
}

StringInfo::StringInfo(size_t stringIndex_):
    TableEntry(TableEntry::ENTRY_TYPE::STRING),
    m_stringIndex(stringIndex_)
{
}


ClassInfo::ClassInfo(size_t nameIndex_):
    TableEntry(TableEntry::ENTRY_TYPE::CLASS),
    m_nameIndex(nameIndex_)
{
}

NameAndTypeInfo::NameAndTypeInfo(size_t nameIndex_, size_t descriptorIndex_):
    TableEntry(TableEntry::ENTRY_TYPE::NAMEANDTYPE),
    m_nameIndex(nameIndex_),
    m_descriptorIndex(descriptorIndex_)
{
}

FieldRefInfo::FieldRefInfo(size_t classIndex_, size_t nameAndTypeIndex_):
    TableEntry(TableEntry::ENTRY_TYPE::FIELDREF),
    m_classIndex(classIndex_),
    m_nameAndTypeIndex(nameAndTypeIndex_)
{
}

MethodRefInfo::MethodRefInfo(size_t classIndex_, size_t nameAndTypeIndex_):
    TableEntry(TableEntry::ENTRY_TYPE::METHODREF),
    m_classIndex(classIndex_),
    m_nameAndTypeIndex(nameAndTypeIndex_)
{
}


Utf8Info::~Utf8Info()
{
    delete[] m_str.begin;
}

ClassTable::ClassTable()
{
    std::cout << "Initialized class table\n";
    if (!prg)
    {
        std::cout << "Cannot compile program, tree is invalid\n";
    }
}

size_t ClassTable::addOrConfirmUtf8ToTable(const std::string &s_)
{
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::UTF8)
        {
            auto *utfstr = dynamic_cast<Utf8Info*>(el);
            if (utfstr->m_str == s_)
            {
                std::cout << "String \"" << s_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added utf-8 string \"" << s_ << "\" to the constant pool\n";
    m_constantPool.push_back(new Utf8Info(s_));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmUtf8ToTable(const DoublePtrString &s_)
{
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::UTF8)
        {
            auto *utfstr = dynamic_cast<Utf8Info*>(el);
            if (utfstr->m_str == s_)
            {
                std::cout << "String \"" << s_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added utf-8 string \"" << s_ << "\" to the constant pool\n";
    m_constantPool.push_back(new Utf8Info(s_));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmIntegerToTable(int num_)
{
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::INTEGER)
        {
            auto *intfield = dynamic_cast<IntegerInfo*>(el);
            if (intfield->m_num == num_)
            {
                std::cout << "Integer \"" << num_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added integer \"" << num_ << "\" to the constant pool\n";
    m_constantPool.push_back(new IntegerInfo(num_));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmFloatToTable(float num_)
{
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::FLOAT)
        {
            auto *doublefield = dynamic_cast<FloatInfo*>(el);
            if (doublefield->m_num == num_)
            {
                std::cout << "Double \"" << num_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added double \"" << num_ << "\" to the constant pool\n";
    m_constantPool.push_back(new FloatInfo(num_));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmClassToTable(const std::string &s_)
{
    auto classNameEntry = addOrConfirmUtf8ToTable(s_);
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::CLASS)
        {
            auto *utfstr = dynamic_cast<ClassInfo*>(el);
            if (utfstr->m_nameIndex == classNameEntry)
            {
                std::cout << "Class \"" << s_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added class \"" << s_ << "\" to the constant pool\n";
    m_constantPool.push_back(new ClassInfo(classNameEntry));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmStringToTable(const std::string &s_)
{
    auto utfstrid = addOrConfirmUtf8ToTable(s_);
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::STRING)
        {
            auto *utfstr = dynamic_cast<StringInfo*>(el);
            if (utfstr->m_stringIndex == utfstrid)
            {
                std::cout << "String \"" << s_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added string \"" << s_ << "\" to the constant pool\n";
    m_constantPool.push_back(new StringInfo(utfstrid));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmStringToTable(const DoublePtrString &s_)
{
    auto utfstrid = addOrConfirmUtf8ToTable(s_);
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::STRING)
        {
            auto *utfstr = dynamic_cast<StringInfo*>(el);
            if (utfstr->m_stringIndex == utfstrid)
            {
                std::cout << "String \"" << s_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added string \"" << s_ << "\" to the constant pool\n";
    m_constantPool.push_back(new StringInfo(utfstrid));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmNameAndTypeToTable(const std::string &name_, const std::string &descriptor_)
{
    auto nameid = addOrConfirmUtf8ToTable(name_);
    auto descid = addOrConfirmUtf8ToTable(descriptor_);
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::NAMEANDTYPE)
        {
            auto *utfstr = dynamic_cast<NameAndTypeInfo*>(el);
            if (utfstr->m_nameIndex == nameid && utfstr->m_descriptorIndex == descid)
            {
                std::cout << "N&T \"" << name_ << " : " << descriptor_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added N&T \"" << name_ << " : " << descriptor_ << "\" to the constant pool\n";
    m_constantPool.push_back(new NameAndTypeInfo(nameid, descid));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmFieldRefToTable(const std::string &name_, const std::string &descriptor_, const std::string &class_)
{
    auto classid = addOrConfirmClassToTable(class_);
    auto ntid = addOrConfirmNameAndTypeToTable(name_, descriptor_);
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::FIELDREF)
        {
            auto *ntentry = dynamic_cast<FieldRefInfo*>(el);
            if (ntentry->m_classIndex == classid && ntentry->m_nameAndTypeIndex == ntid)
            {
                std::cout << "FieldRef \"" << class_ << " . " << name_ << " : " << descriptor_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added FieldRef \"" << class_ << " . " << name_ << " : " << descriptor_ << "\" to the constant pool\n";
    m_constantPool.push_back(new FieldRefInfo(classid, ntid));
    return m_constantPool.size();
}

size_t ClassTable::addOrConfirmMethodRefToTable(const std::string &name_, const std::string &descriptor_, const std::string &class_)
{
    auto classid = addOrConfirmClassToTable(class_);
    auto ntid = addOrConfirmNameAndTypeToTable(name_, descriptor_);
    for (int i = 0; i < m_constantPool.size(); ++i)
    {
        auto *el = m_constantPool[i];
        if (el->m_type == TableEntry::ENTRY_TYPE::METHODREF)
        {
            auto *ntentry = dynamic_cast<MethodRefInfo*>(el);
            if (ntentry->m_classIndex == classid && ntentry->m_nameAndTypeIndex == ntid)
            {
                std::cout << "MethodRef \"" << class_ << " . " << name_ << " : " << descriptor_ << "\" already exists\n";
                return i + 1;
            }
        }
    }

    std::cout << "Added MethodRef \"" << class_ << " . " << name_ << " : " << descriptor_ << "\" to the constant pool\n";
    m_constantPool.push_back(new MethodRefInfo(classid, ntid));
    return m_constantPool.size();
}

void ClassTable::writeBytes(uint64_t bytes_, size_t countBytes_)
{
    char *arr = (char*)&bytes_;  // TODO: to reinterpret
    auto end = sizeof(bytes_);
    for (auto i = countBytes_ - 1; i >= 0 && i < countBytes_; --i)
    {
        m_output.write(arr + i, 1);
    }
}

void ClassTable::writeBytes(const DoublePtrString &str_)
{
    for (const char *ptr = str_.begin; ptr != str_.end - 1; ++ptr)
    {
        if (*ptr != '\0')
            m_output.write(ptr, 1);
        else
        {
            /*char ch = 0xc0;
            m_output.write(&ch, 1);
            ch = 0x80;
            m_output.write(&ch, 1);*/

            writeBytes(0xc0, 1);
            writeBytes(0x80, 1);
        }
    }
}

void ClassTable::writeInt(int32_t bytes_)
{
    char *arr = (char*)&bytes_;  // TODO: to reinterpret
    auto end = sizeof(bytes_);
    for (auto i = 3; i >= 0 && i < 4; --i)
    {
        m_output.write(arr + i, 1);
    }
}

void ClassTable::writeFloat(float bytes_)
{
    char *arr = (char*)&bytes_; 
    auto end = sizeof(bytes_);
    for (auto i = 3; i >= 0 && i < 4; --i)
    {
        m_output.write(arr + i, 1);
    }
}

void ClassTable::grabParams()
{
}

void ClassTable::generateUniversalTable()
{
    m_thisClassID = addOrConfirmClassToTable(m_classname);
    m_superClassID = addOrConfirmClassToTable("java/lang/Object");
    m_codeAttrNameID = addOrConfirmUtf8ToTable(std::string("Code"));

    m_dtClass = addOrConfirmClassToTable("DynamicType");
    m_dtInitIdI = addOrConfirmMethodRefToTable("<init>", "(I)V", "DynamicType");
    m_dtInitIdFl = addOrConfirmMethodRefToTable("<init>", "(F)V", "DynamicType");
    m_dtInitIdS = addOrConfirmMethodRefToTable("<init>", "(Ljava/lang/String;)V", "DynamicType");
    m_dtInitIdB = addOrConfirmMethodRefToTable("<init>", "(Z)V", "DynamicType");
    m_dtInitIdF = addOrConfirmMethodRefToTable("<init>", "(LFunctionClass;)V", "DynamicType");
    m_dtInitIdNIL = addOrConfirmMethodRefToTable("<init>", "()V", "DynamicType");
    m_dtCreateTable = addOrConfirmMethodRefToTable("createTable", "()LDynamicType;", "DynamicType");
    m_dtFieldIdI = addOrConfirmFieldRefToTable("iValue", "I", "DynamicType");
    m_dtFieldIdD = addOrConfirmFieldRefToTable("dValue", "D", "DynamicType");
    m_dtFieldIdS = addOrConfirmFieldRefToTable("sValue", "Ljava/lang/String;", "DynamicType");
    m_dtFieldIdF = addOrConfirmFieldRefToTable("fValue", "LFunctionClass;", "DynamicType");

    m_dtSetDT = addOrConfirmMethodRefToTable("set", "(LDynamicType;)V", "DynamicType");
    m_dtSetF = addOrConfirmMethodRefToTable("set", "(LFunctionClass;)V", "DynamicType");

    m_dt__add = addOrConfirmMethodRefToTable("__add", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__sub = addOrConfirmMethodRefToTable("__sub", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__mul = addOrConfirmMethodRefToTable("__mul", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__div = addOrConfirmMethodRefToTable("__div", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__mod = addOrConfirmMethodRefToTable("__mod", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__pow = addOrConfirmMethodRefToTable("__pow", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__unm = addOrConfirmMethodRefToTable("__unm", "(LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__lt = addOrConfirmMethodRefToTable("__lt", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__le = addOrConfirmMethodRefToTable("__le", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__eq = addOrConfirmMethodRefToTable("__eq", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__neq = addOrConfirmMethodRefToTable("__neq", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__gt = addOrConfirmMethodRefToTable("__gt", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__ge = addOrConfirmMethodRefToTable("__ge", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__index = addOrConfirmMethodRefToTable("__index", "(LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__newindex = addOrConfirmMethodRefToTable("__newindex", "(LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__autoindex = addOrConfirmMethodRefToTable("__autoindex", "()LDynamicType;", "DynamicType");
    m_dt__idiv = addOrConfirmMethodRefToTable("__idiv", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__band = addOrConfirmMethodRefToTable("__band", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__bor = addOrConfirmMethodRefToTable("__bor", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__bxor = addOrConfirmMethodRefToTable("__bxor", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__bnot = addOrConfirmMethodRefToTable("__bnot", "(LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__shl = addOrConfirmMethodRefToTable("__shl", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__shr = addOrConfirmMethodRefToTable("__shr", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__concat = addOrConfirmMethodRefToTable("__concat", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__and = addOrConfirmMethodRefToTable("__and", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__or = addOrConfirmMethodRefToTable("__or", "(LDynamicType;LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__len = addOrConfirmMethodRefToTable("__len", "(LDynamicType;)LDynamicType;", "DynamicType");
    m_dt__not = addOrConfirmMethodRefToTable("__not", "(LDynamicType;)LDynamicType;", "DynamicType");

    m_dt_toBool = addOrConfirmMethodRefToTable("tobool", "(LDynamicType;)Z", "DynamicType");

    m_dtCallRef = addOrConfirmMethodRefToTable("__call", "(LDynamicType;LVarList;)LVarList;", "DynamicType");
    m_dtCallMethodRef = addOrConfirmMethodRefToTable("__callmethod", "(LDynamicType;LDynamicType;LVarList;)LVarList;", "DynamicType");
    m_dtRevset = addOrConfirmMethodRefToTable("revset", "(LDynamicType;LDynamicType;)V", "DynamicType");
    
    m_varlistClass = addOrConfirmClassToTable("VarList");
    m_varlistInit = addOrConfirmMethodRefToTable("<init>", "()V", "VarList");
    m_varlistAdd = addOrConfirmMethodRefToTable("add", "(LDynamicType;)V", "VarList");
    m_varlistAddRef = addOrConfirmMethodRefToTable("addRef", "(LDynamicType;)V", "VarList");
    m_varlistGet = addOrConfirmMethodRefToTable("get", "(I)LDynamicType;", "VarList");
    m_varlistAssign = addOrConfirmMethodRefToTable("multipleAssign", "(LVarList;LVarList;)V", "VarList");
    m_varlistAppend = addOrConfirmMethodRefToTable("append", "(LVarList;)V", "VarList");
    m_varlistSetVarargPoint = addOrConfirmMethodRefToTable("setVarargPoint", "(I)V", "VarList");
    m_varlistGetVararg = addOrConfirmMethodRefToTable("getVararg", "()LVarList;", "VarList");

    m_forLoopIter = addOrConfirmMethodRefToTable("forTurn", "(LVarList;)LVarList;", "VarList");
    m_forLoopCond = addOrConfirmMethodRefToTable("forCondition", "(LVarList;)Z", "VarList");
    m_foreachHead = addOrConfirmMethodRefToTable("foreachHead", "(LVarList;)LVarList;", "VarList");
    m_foreachCondition = addOrConfirmMethodRefToTable("foreachCondition", "(LVarList;)Z", "VarList");
}

bool ClassTable::doesFieldExist(int nameId_)
{
    for (auto &el : m_fieldPool)
    {
        if (el->m_nameIndex == nameId_)
        {
            return true;
            break;
        }
    }

    return false;
}

void CodeRecorder::markBreak()
{
    m_unsolvedBreakLocations.push_back(m_byteCode.size() - 3);
}

void CodeRecorder::solveBreaks()
{
    for (auto &el : m_unsolvedBreakLocations)
    {
        m_byteCode[el] = (char)GOTO;
        setBytesAt(m_byteCode.size() - el, el+1);
    }

    m_unsolvedBreakLocations.clear();
}

void CodeRecorder::addBytes(uint64_t bytes_, size_t countBytes_)
{
    char *arr = (char*)&bytes_;  // TODO: to reinterpret
    auto end = sizeof(bytes_);
    auto begin = end - countBytes_;
    for (auto i = countBytes_ - 1; i >= 0 && i < countBytes_; --i)
    {
        m_byteCode.push_back(arr[i]);
    }
}

void CodeRecorder::addBytes(int16_t bytes_)
{
    char *arr = (char*)&bytes_;  // TODO: to reinterpret
    auto end = sizeof(bytes_);
    for (auto i = 1; i >= 0 && i < 2; --i)
    {
        m_byteCode.push_back(arr[i]);
    }
}

void CodeRecorder::setBytesAt(int16_t bytes_, int location_)
{
    char *arr = (char*)&bytes_;  // TODO: to reinterpret
    auto end = sizeof(bytes_);
    for (auto i = 1; i >= 0 && i < 2; --i)
    {
        m_byteCode[location_ + (1 - i)] = arr[i];
    }
}

void CodeRecorder::append(CodeRecorder *moreCode_)
{
    auto bcsize = m_byteCode.size();
    m_byteCode.insert(m_byteCode.end(), moreCode_->m_byteCode.begin(), moreCode_->m_byteCode.end());
    for (auto el : moreCode_->m_unsolvedBreakLocations)
    {
        el += bcsize;
        m_unsolvedBreakLocations.push_back(el);
    }
}

void ClassTable::generateClassFile()
{
    system((std::string("del ") + m_classname + ".class").c_str());
    m_output.open(m_classname + ".class", std::ios::binary | std::ios::trunc);
    if (!m_output.is_open())
    {
        std::cout << "Failed to create class file\n";
        return;
    }

    writeBytes(0xcafebabe, 4); // Magic number
    writeBytes(0x0000003f, 4); // Java version
    writeBytes(m_constantPool.size() + 1, 2); // Constant pool size

    for (int i = 0; i < m_constantPool.size(); ++i)
    {
            writeBytes((int)m_constantPool[i]->m_type, 1); // TODO: to reinterpret
        switch (m_constantPool[i]->m_type)
        {
            case (TableEntry::ENTRY_TYPE::UTF8):
            {
                auto *properptr = dynamic_cast<Utf8Info*>(m_constantPool[i]);
                writeBytes(properptr->m_str.getByteLen(), 2);
                writeBytes(properptr->m_str);
                break;
            }

            case (TableEntry::ENTRY_TYPE::INTEGER):
            {
                auto *properptr = dynamic_cast<IntegerInfo*>(m_constantPool[i]);
                writeInt(properptr->m_num);
                break;
            }

            case (TableEntry::ENTRY_TYPE::FLOAT):
            {
                auto *properptr = dynamic_cast<FloatInfo*>(m_constantPool[i]);
                writeFloat(properptr->m_num);
                //writeInt(properptr->m_num);
                break;
            }

            case (TableEntry::ENTRY_TYPE::CLASS):
            {
                auto *properptr = dynamic_cast<ClassInfo*>(m_constantPool[i]);
                writeBytes(properptr->m_nameIndex, 2);
                break;
            }

            case (TableEntry::ENTRY_TYPE::STRING):
            {
                auto *properptr = dynamic_cast<StringInfo*>(m_constantPool[i]);
                writeBytes(properptr->m_stringIndex, 2);
                break;
            }

            case (TableEntry::ENTRY_TYPE::NAMEANDTYPE):
            {
                auto *properptr = dynamic_cast<NameAndTypeInfo*>(m_constantPool[i]);
                writeBytes(properptr->m_nameIndex, 2);
                writeBytes(properptr->m_descriptorIndex, 2);
                break;
            }

            case (TableEntry::ENTRY_TYPE::FIELDREF):
            {
                auto *properptr = dynamic_cast<FieldRefInfo*>(m_constantPool[i]);
                writeBytes(properptr->m_classIndex, 2);
                writeBytes(properptr->m_nameAndTypeIndex, 2);
                break;
            }

            case (TableEntry::ENTRY_TYPE::METHODREF):
            {
                auto *properptr = dynamic_cast<MethodRefInfo*>(m_constantPool[i]);
                writeBytes(properptr->m_classIndex, 2);
                writeBytes(properptr->m_nameAndTypeIndex, 2);
                break;
            }
        }
    }
    
    writeBytes(0x0021, 2); // Access flags
    writeBytes(m_thisClassID, 2); // This class
    writeBytes(m_superClassID, 2); // Super class
    writeBytes(0x0000, 2); // Interface count

    writeBytes(m_fieldPool.size(), 2); // Fields count

    // Fields
    for (int i = 0; i < m_fieldPool.size(); ++i)
    {
        writeBytes(m_fieldPool[i]->m_accessFlags, 2);
        writeBytes(m_fieldPool[i]->m_nameIndex, 2);
        writeBytes(m_fieldPool[i]->m_descIndex, 2);
        writeBytes(0, 2); // Attributes
    }

    writeBytes(m_methodPool.size(), 2); // Methods count

    // Methods
    for (int i = 0; i < m_methodPool.size(); ++i)
    {
        writeBytes(m_methodPool[i]->m_accessFlags, 2); // Access flags
        writeBytes(m_methodPool[i]->m_nameIndex, 2); // Method name flags
        writeBytes(m_methodPool[i]->m_descIndex, 2); // Method descriptor flags
        writeBytes(m_methodPool[i]->m_attribCount, 2); // Count of attributes
        writeBytes(m_methodPool[i]->m_codeAttrNameIndex, 2); // Attribute name index
        writeBytes(m_methodPool[i]->m_codeAttrLength, 4); // Attribute length
        writeBytes(m_methodPool[i]->m_maxStack, 2); // Max stack for code
        writeBytes(m_methodPool[i]->m_maxLocals, 2); // Max locals (including args)
        writeBytes(m_methodPool[i]->m_codeLength, 4); // Length of code
        for (auto &el : m_methodPool[i]->m_byteCode)
        {
            writeBytes(el, 1); // Actual code
        }
        writeBytes(0x0, 2); // Exception table length
        writeBytes(0x0, 2); // Attribute count (for code)
    }

    writeBytes(0x0000, 2); // Attributes count

    m_output.close();
}

VarsContext::VarsContext(VarsContext *parentContext_) :
    m_parentContext(parentContext_)
{
    contextID = lastID++;
    std::cout << "Created context " << contextID << std::endl;
}

VarsContext *VarsContext::confirmLocalVar(const std::string &identifier_)
{
    if (std::find(m_variables.begin(), m_variables.end(), identifier_) == m_variables.end())
        m_variables.push_back(identifier_);

    return this;
}

VarsContext *VarsContext::confirmGlobalVar(const std::string &identifier_)
{
    auto *context = this;
    while (std::find(context->m_variables.begin(), context->m_variables.end(), identifier_) == context->m_variables.end() && context->m_parentContext)
    {
        context = context->m_parentContext;
    }

    if (context->className != className)
    {
        bool alreadyHasDependency = false;
        auto functionContext = getOriginalFunctionContext();
        for (const auto &el : functionContext->m_dependencies)
        {
            if (el.m_context == context && el.varName == identifier_)
            {
                alreadyHasDependency = true;
                break;
            }
        }
        if (!alreadyHasDependency)
            functionContext->m_dependencies.push_back({context, identifier_});
    }

        context->confirmLocalVar(identifier_);

    return context;
}

VarsContext *VarsContext::confirmParameter(const std::string &identifier_, int paramID_)
{
    if (std::find(m_variables.begin(), m_variables.end(), identifier_) == m_variables.end())
    {
        m_variables.push_back(identifier_);
        m_parameters.push_back({identifier_, paramID_, -1});
    }

    return this;
}

VarsContext *VarsContext::createChildContext()
{
    auto *newcontext = new VarsContext(this);
    m_contexts.push_back(newcontext);
    newcontext->grabNearestClassName();
    return newcontext;
}

void VarsContext::grabNearestClassName()
{
    auto *context = m_parentContext;
    while (context->className == "" && context->m_parentContext)
    {
        context = context->m_parentContext;
    }

    className = context->className;
}

VarsContext *VarsContext::getOriginalFunctionContext()
{
    auto *context = this;
    while (context->m_parentContext && context->className == context->m_parentContext->className)
    {
        context = context->m_parentContext;
    }

    return context;
}

void ClassTable::generateFunctionClassVariables(VarsContext *currentContext)
{
    if (currentContext->m_parameters.size() > 0)
    {
        std::cout << "Parameters:\n";
        for (auto &el : currentContext->m_parameters)
        {
            auto varname = std::string("CONTEXT_") + std::to_string(currentContext->contextID) + "_" + el.paramName;
            auto fldName = addOrConfirmUtf8ToTable(varname);
            auto fldDesc = addOrConfirmUtf8ToTable(std::string("LDynamicType;"));
            auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", currentContext->className);
            auto *paramfld = new FieldInfo();
            paramfld->m_accessFlags = 0x0009;
            paramfld->m_nameIndex = fldName;
            paramfld->m_descIndex = fldDesc;
            m_fieldPool.push_back(paramfld);
            el.paramFldRef = fldRef;
            std::cout << m_classname << "::" << varname << " (" << el.paramID << ")\n";
        }
    }

    if (currentContext->m_dependencies.size() > 0)
    {
        std::cout << "Dependencies:\n";
        for (auto &el : currentContext->m_dependencies)
        {
            auto varname = std::string("DEP_CONTEXT_") + std::to_string(currentContext->contextID) + "_" + el.varName;
            auto sourceVarName = std::string("CONTEXT_") + std::to_string(el.m_context->contextID) + "_" + el.varName;
            auto fldName = addOrConfirmUtf8ToTable(varname);
            auto fldDesc = addOrConfirmUtf8ToTable(std::string("LDynamicType;"));
            auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", currentContext->className);
            auto sourceVarNameFieldRef = addOrConfirmFieldRefToTable(sourceVarName, "LDynamicType;", el.m_context->className);
            auto *depfld = new FieldInfo();
            depfld->m_accessFlags = 0x0001;
            depfld->m_nameIndex = fldName;
            depfld->m_descIndex = fldDesc;
            m_fieldPool.push_back(depfld);

            m_constructor->addBytes(0x2a, 1); // aload_0
            m_constructor->addBytes(0xb2, 1); // getstatic
            m_constructor->addBytes(sourceVarNameFieldRef, 2); // field reference
            m_constructor->addBytes(0xb5, 1); // putfield
            m_constructor->addBytes(fldRef, 2); // putfield

            std::cout << el.m_context->className << "::" << "CONTEXT_" << el.m_context->contextID << "_" << el.varName << std::endl;
        }
    }

    if (currentContext->m_variables.size() > 0)
    {
        std::cout << "Variables:\n";
        for (auto &el : currentContext->m_variables)
        {
            auto varname = std::string("CONTEXT_") + std::to_string(currentContext->contextID) + "_" + el;
            auto fldName = addOrConfirmUtf8ToTable(varname);
            auto fldDesc = addOrConfirmUtf8ToTable(std::string("LDynamicType;"));
            auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", currentContext->className);
            if (!doesFieldExist(fldName))
            {
                auto *varfld = new FieldInfo();
                varfld->m_accessFlags = 0x0009;
                varfld->m_nameIndex = fldName;
                varfld->m_descIndex = fldDesc;
                m_fieldPool.push_back(varfld);
                initVar(varname, currentContext, fldRef);
            }
            std::cout << m_classname << "::" << varname << std::endl;
        }
    }

    for (auto *child : currentContext->m_contexts)
    {
        if (child->className == currentContext->className)
            generateFunctionClassVariables(child);
    }
}


FieldData ClassTable::createFunctionField(CodeRecorder *method, const std::string &functionName, const std::string &functionOwnerClassName, const std::string &className)
{
    auto fldName = addOrConfirmUtf8ToTable(functionName);
    auto fldDesc = addOrConfirmUtf8ToTable(std::string("LDynamicType;"));
    auto fldRef = addOrConfirmFieldRefToTable(functionName, "LDynamicType;", className);

    auto functionClass = addOrConfirmClassToTable(functionOwnerClassName);
    auto fuhctionInit = addOrConfirmMethodRefToTable("<init>", "()V", functionOwnerClassName);

    // Create DynamicType
    method->addBytes(0xbb, 1); // new
    method->addBytes(m_dtClass, 2); // DynamicType
    method->addBytes(0x59, 1); // dup
    
    // Create and initialize function class
    method->addBytes(0xbb, 1); // new
    method->addBytes(functionClass, 2); // FunctionClass
    method->addBytes(0x59, 1); // dup
    method->addBytes(0xb7, 1); // invokespecial
    method->addBytes(fuhctionInit, 2); // <init>

    // Initialize DynamicType
    method->addBytes(0xb7, 1); // invokespecial
    method->addBytes(m_dtInitIdF, 2); // <init>

    // Put function into static field
    method->addBytes(0xb3, 1); // putstatic
    method->addBytes(fldRef, 2); // print field

    if (!doesFieldExist(fldName))
    {
        auto *mainfield = new FieldInfo();
        mainfield->m_accessFlags = 0x0009;
        mainfield->m_nameIndex = fldName;
        mainfield->m_descIndex = fldDesc;
        m_fieldPool.push_back(mainfield);
    }

    return {fldName, fldDesc, fldRef};

}

FieldData ClassTable::createTableField(CodeRecorder *method, const std::string &tableName, const std::string &className)
{
    auto fldName = addOrConfirmUtf8ToTable(tableName);
    auto fldDesc = addOrConfirmUtf8ToTable(std::string("LDynamicType;"));
    auto fldRef = addOrConfirmFieldRefToTable(tableName, "LDynamicType;", className);

    // Create DynamicType
    method->addBytes(INVOKESTATIC, 1);
    method->addBytes(m_dtCreateTable, 2); // new

    // Put function into static field
    method->addBytes(PUTSTATIC, 1); // putstatic
    method->addBytes(fldRef, 2); // print field

    if (!doesFieldExist(fldName))
    {
        auto *mainfield = new FieldInfo();
        mainfield->m_accessFlags = 0x0009;
        mainfield->m_nameIndex = fldName;
        mainfield->m_descIndex = fldDesc;
        m_fieldPool.push_back(mainfield);
    }

    return {fldName, fldDesc, fldRef};
}

void ClassTable::createFunctionInTableField(CodeRecorder *method, const std::string &functionField, const std::string &functionOwnerClassName, int tableRef, const std::string &className)
{
    //auto fldName = addOrConfirmUtf8ToTable(functionName);
    //auto fldDesc = addOrConfirmUtf8ToTable(std::string("LDynamicType;"));
    //auto fldRef = addOrConfirmFieldRefToTable(functionName, "LDynamicType;", className);

    auto functionClass = addOrConfirmClassToTable(functionOwnerClassName);
    auto fuhctionInit = addOrConfirmMethodRefToTable("<init>", "()V", functionOwnerClassName);

    // Get DT from table
    method->addBytes(GETSTATIC, 1);
    method->addBytes(tableRef, 2);
    auto *funcstr = new DoublePtrString(functionField.c_str());
    createDynamicType(method, *funcstr);
    //createStringOnStack(method, *funcstr);
    method->addBytes(INVOKEVIRTUAL, 1);
    method->addBytes(m_dt__newindex, 2);

    // Initialize DynamicType
    method->addBytes(NEW, 1);
    method->addBytes(functionClass, 2);
    method->addBytes(DUP, 1);
    method->addBytes(INVOKESPECIAL, 1); // invokespecial
    method->addBytes(fuhctionInit, 2); // <init>

    // Put function into static field
    method->addBytes(INVOKEVIRTUAL, 1); // putstatic
    method->addBytes(m_dtSetF, 2); // print field
}

int ClassTable::createIntOnStack(CodeRecorder *method_, int num_)
{
    switch (num_)
    {
        case (-1):
            method_->addBytes(ICONST_M1, 1); // dup
            return 1;
            break;

        case (0):
            method_->addBytes(ICONST_0, 1); // dup
            return 1;
            break;

        case (1):
            method_->addBytes(ICONST_1, 1); // dup
            return 1;
            break;
        
        case (2):
            method_->addBytes(ICONST_2, 1); // dup
            return 1;
            break;

        case (3):
            method_->addBytes(ICONST_3, 1); // dup
            return 1;
            break;

        case (4):
            method_->addBytes(ICONST_4, 1); // dup
            return 1;
            break;

        case (5):
            method_->addBytes(ICONST_5, 1); // dup
            return 1;
            break;

        default:
        {
            auto numberId = addOrConfirmIntegerToTable(num_);
            method_->addBytes(LDC_W, 1); // ldc_w
            method_->addBytes(numberId, 2); // field with number
            return 3;
            break;
        }
    }
}

void ClassTable::createFloatOnStack(CodeRecorder *method_, float num_)
{
    auto numberId = addOrConfirmFloatToTable(num_);
    method_->addBytes(LDC_W, 1); // ldc_w
    method_->addBytes(numberId, 2); // field with number
}

void ClassTable::createStringOnStack(CodeRecorder *method_, const DoublePtrString &s_)
{
    auto numberId = addOrConfirmStringToTable(s_);
    method_->addBytes(LDC_W, 1); // ldc_w
    method_->addBytes(numberId, 2); // field with number
}

void ClassTable::createVarList(CodeRecorder *method)
{
    method->addBytes(NEW, 1);
    method->addBytes(m_varlistClass, 2);
    method->addBytes(DUP, 1); // dup

    method->addBytes(INVOKESPECIAL, 1); // invokespecial
    method->addBytes(m_varlistInit, 2); // <init>
}

void ClassTable::createDynamicType(CodeRecorder *method_, int num_)
{
    // Create DT
    method_->addBytes(NEW, 1); // new
    method_->addBytes(m_dtClass, 2); // DynamicType
    method_->addBytes(DUP, 1); // dup

    createIntOnStack(method_, num_);

    method_->addBytes(INVOKESPECIAL, 1); // invokespecial
    method_->addBytes(m_dtInitIdI, 2); // <init>
}

void ClassTable::createDynamicType(CodeRecorder *method_, float num_)
{
    // Create DT
    method_->addBytes(NEW, 1); // new
    method_->addBytes(m_dtClass, 2); // DynamicType
    method_->addBytes(DUP, 1); // dup

    createFloatOnStack(method_, num_);

    method_->addBytes(INVOKESPECIAL, 1); // invokespecial
    method_->addBytes(m_dtInitIdFl, 2); // <init>
}

void ClassTable::createDynamicType(CodeRecorder *method_, const DoublePtrString &s_)
{
    std::cout << s_ << std::endl;
    // Create DT
    method_->addBytes(NEW, 1); // new
    method_->addBytes(m_dtClass, 2); // DynamicType
    method_->addBytes(DUP, 1); // dup

    createStringOnStack(method_, s_);

    method_->addBytes(INVOKESPECIAL, 1); // invokespecial
    method_->addBytes(m_dtInitIdS, 2); // <init>
}

void ClassTable::createDynamicType(CodeRecorder *method_, bool val_)
{
    // Create DT
    method_->addBytes(NEW, 1); // new
    method_->addBytes(m_dtClass, 2); // DynamicType
    method_->addBytes(DUP, 1); // dup

    createIntOnStack(method_, (val_ ? 1 : 0));

    method_->addBytes(INVOKESPECIAL, 1); // invokespecial
    method_->addBytes(m_dtInitIdB, 2); // <init>
}

void ClassTable::createDynamicTypeTable(CodeRecorder *method)
{
    // Create DT
    method->addBytes(INVOKESTATIC, 1); // invokespecial
    method->addBytes(m_dtCreateTable, 2); // <init>
}

void ClassTable::createDynamicType(CodeRecorder *method)
{
    // Create DT
    method->addBytes(NEW, 1); // new
    method->addBytes(m_dtClass, 2); // DynamicType
    method->addBytes(DUP, 1); // dup

    method->addBytes(INVOKESPECIAL, 1); // invokespecial
    method->addBytes(m_dtInitIdNIL, 2); // <init>
}

void ClassTable::treeBypassCodeGen(Program *node)
{
	if (node==NULL)	return;
	treeBypassCodeGen(node->stmts);
}

void ClassTable::treeBypassCodeGen(StatementList *node)
{
	if (node==NULL)	return;
	for (int i = 0; i < node->lst.size(); i++)
    {
		treeBypassCodeGen(node->lst[i]);
	}
}

void ClassTable::treeBypassCodeGen(Statement *node)
{
	if (node==NULL)	return;

	switch(node->type)
	{
		case STATEMENT_TYPE::ASSIGN:
		{
			auto *realnode = dynamic_cast<StatementAssign*>(node);
			treeBypassCodeGen(realnode);
		}
		break;
		case STATEMENT_TYPE::MULTIPLE_ASSIGN:
		{
			auto *realnode = dynamic_cast<StatementMultipleAssign*>(node);
			treeBypassCodeGen(realnode);
		}
		break;
		case STATEMENT_TYPE::STMT_LIST:
		{
			throw std::string("Statement list should not be kept as a statement");
		}
		break;
		case STATEMENT_TYPE::STRETURN:
		{
			auto *realnode = dynamic_cast<StatementReturn*>(node);
			treeBypassCodeGen(realnode);
		}
		break;
		case STATEMENT_TYPE::FUNCTION_CALL:
		{
			auto *realnode = dynamic_cast<StatementFunctionCall*>(node);
			treeBypassCodeGen(realnode);
		}
		break;
		case STATEMENT_TYPE::BREAK:
		{
			m_currentCodeRecorder->addBytes(NOP, 3);
            m_currentCodeRecorder->markBreak();
		}
		break;
		case STATEMENT_TYPE::IF_ELSE:
		{
			auto *realnode = dynamic_cast<StatementIfElse*>(node);
			treeBypassCodeGen(realnode);
		}
		break;
		case STATEMENT_TYPE::WHILE_LOOP:
		{
			auto *realnode = dynamic_cast<StatementWhileLoop*>(node);
			treeBypassCodeGen(realnode);
		}
		break;
		case STATEMENT_TYPE::REPEAT_LOOP:
		{
			auto *realnode = dynamic_cast<StatementRepeatLoop*>(node);
			treeBypassCodeGen(realnode);
		}
		break;
		case STATEMENT_TYPE::FOR_NUMERIC:
		{
			auto *realnode = dynamic_cast<StatementForLoop*>(node);
			treeBypassCodeGen(realnode);
		}
		break;
		case STATEMENT_TYPE::FOR_EACH:
		{
			auto *realnode = dynamic_cast<StatementForeachLoop*>(node);
			treeBypassCodeGen(realnode);
		}
		break;
		case STATEMENT_TYPE::GOTO_LABEL:
		{
			auto *realnode = dynamic_cast<StatementGotoLabel*>(node);
			treeBypassCodeGen(realnode);
		}
		break;
		case STATEMENT_TYPE::GOTO_CALL:
		{
			auto *realnode = dynamic_cast<StatementGotoCall*>(node);
			treeBypassCodeGen(realnode);
		}
		break;
	}
}

void ClassTable::treeBypassCodeGen(StatementMultipleAssign *node)
{
	if (node==NULL)	return;	

    if (node->isLocal)
    {
        for (auto &leftel : node->left->lst)
        {
            if (leftel->type != EXPRESSION_TYPE::IDENTIFIER)
                throw std::string("All left-side operators of local assignment should be identifiers");
            auto varname = std::string("CONTEXT_") + std::to_string(leftel->varContext->contextID) + "_" + leftel->identifier;
            auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", leftel->varContext->className);
            createDynamicType(m_currentCodeRecorder);
            m_currentCodeRecorder->addBytes(PUTSTATIC, 1); // putstatic
            m_currentCodeRecorder->addBytes(fldRef, 2); // putstatic
        }
    }

    m_assignContext = true;

    treeBypassCodeGen_CreateReferences(node->left); // Should create VarList with references

    m_assignContext = false;

    treeBypassCodeGen(node->right); // Should create VarList

    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    m_currentCodeRecorder->addBytes(m_varlistAssign, 2);

}

void ClassTable::treeBypassCodeGen(StatementAssign *node)
{
	if (node==NULL)	return;	

    if (node->isLocal)
    {
        if (node->left->type != EXPRESSION_TYPE::IDENTIFIER)
            throw std::string("All left-side operators of local assignment should be identifiers");
        auto varname = std::string("CONTEXT_") + std::to_string(node->left->varContext->contextID) + "_" + node->left->identifier;
        auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", node->left->varContext->className);
        createDynamicType(m_currentCodeRecorder);
        m_currentCodeRecorder->addBytes(PUTSTATIC, 1); // putstatic
        m_currentCodeRecorder->addBytes(fldRef, 2); // putstatic
    }

    m_assignContext = true;

    createVarList(m_currentCodeRecorder);
    m_currentCodeRecorder->addBytes(DUP, 1);
	treeBypassCodeGen(node->left); // Should create DynamicType on stack that most likely references existing and accessable variable
    m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
    m_currentCodeRecorder->addBytes(m_varlistAddRef, 2);

    m_assignContext = false;

    treeBypassCodeGen(node->right); // Should create VarList

    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    m_currentCodeRecorder->addBytes(m_varlistAssign, 2);
}

void ClassTable::treeBypassCodeGen(StatementFunctionCall *node)
{
    if (node==NULL)	return;
	treeBypassCodeGen(node->functionName); // Creates DynamicType with function on stack
	treeBypassCodeGen(node->lst); // Create VarList on stack
    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    if (node->functionName->type != EXPRESSION_TYPE::METHOD_NAME)
        m_currentCodeRecorder->addBytes(m_dtCallRef, 2); // Leaves VarList on stack
    else
        m_currentCodeRecorder->addBytes(m_dtCallMethodRef, 2); // Leaves VarList on stack
    m_currentCodeRecorder->addBytes(POP, 1);
}

void ClassTable::treeBypassCodeGen(ExpressionList *node)
{
	if (node==NULL)	return;

    createVarList(m_currentCodeRecorder);

	for (int i = 0; i < node->lst.size(); i++)
    {
        m_currentCodeRecorder->addBytes(DUP, 1);
		treeBypassCodeGen(node->lst[i]); // Creates DynamicType unless function call or vararg
        if (node->lst[i]->type == EXPRESSION_TYPE::FUNCTION_CALL || node->lst[i]->type == EXPRESSION_TYPE::VARARG_REF)
        {
            // If function call is the last element, append it entirely
            if (i == node->lst.size() - 1)
            {
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistAppend, 2);
            }
            else
            {
                createIntOnStack(m_currentCodeRecorder, 0);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistGet, 2);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistAdd, 2);
            }
        }
        else
        {
            m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
            m_currentCodeRecorder->addBytes(m_varlistAdd, 2);
        }
	}
}

void ClassTable::treeBypassCodeGen_CreateReferences(ExpressionList *node)
{
	if (node==NULL)	return;

    createVarList(m_currentCodeRecorder);

	for (int i = 0; i < node->lst.size(); i++)
    {
        m_currentCodeRecorder->addBytes(DUP, 1);
		treeBypassCodeGen(node->lst[i]); // Creates DynamicType unless function call or vararg
        if (node->lst[i]->type == EXPRESSION_TYPE::FUNCTION_CALL  || node->lst[i]->type == EXPRESSION_TYPE::VARARG_REF)
        {
            throw std::string("Function calls or varargs can't be on the left side of assignment");
        }
        else
        {
            m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
            m_currentCodeRecorder->addBytes(m_varlistAddRef, 2);
        }
	}
}

void ClassTable::treeBypassCodeGen(ParamList *node)
{
	std::cout << "here\n";
}

void ClassTable::treeBypassCodeGen(Expression *node)
{
	if (node==NULL)	return;
	std::cout << node->name << std::endl;

    int operRef = -1;

	switch(node->type)
	{
		case EXPRESSION_TYPE::INT:
            createDynamicType(m_currentCodeRecorder, node->iValue);
		    break;
		case EXPRESSION_TYPE::DOUBLE:
            createDynamicType(m_currentCodeRecorder, node->fValue);
		    break;
		case EXPRESSION_TYPE::STRING:
            createDynamicType(m_currentCodeRecorder, node->sValue);
		    break;
		case EXPRESSION_TYPE::BOOL:
            createDynamicType(m_currentCodeRecorder, node->bValue);
		    break;
		case EXPRESSION_TYPE::NIL:
            createDynamicType(m_currentCodeRecorder);
		    break;
		case EXPRESSION_TYPE::IDENTIFIER:
            {
                // Puts a reference to the variable
                if (node->varContext->className == m_ownContext->className)
                {
                    // If its a local variable
                    auto varname = std::string("CONTEXT_") + std::to_string(node->varContext->contextID) + "_" + node->identifier;
                    auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", node->varContext->className);
                    m_currentCodeRecorder->addBytes(GETSTATIC, 1);
                    m_currentCodeRecorder->addBytes(fldRef, 2);
                }
                else
                {
                    // If its a dependency from a higher-level scope
                    auto varname = std::string("DEP_CONTEXT_") + std::to_string(m_ownContext->contextID) + "_" + node->identifier;
                    auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", m_ownContext->className);
                    m_currentCodeRecorder->addBytes(ALOAD_0, 1);
                    m_currentCodeRecorder->addBytes(GETFIELD, 1);
                    m_currentCodeRecorder->addBytes(fldRef, 2);
                }
            }
		    break;
		case EXPRESSION_TYPE::BIN_PLUS:
            operRef = m_dt__add;
		    [[fallthrough]]
		case EXPRESSION_TYPE::BIN_MINUS:
        if (operRef == -1)
            operRef = m_dt__sub;
		    [[fallthrough]]
		case EXPRESSION_TYPE::BIN_MUL:
        if (operRef == -1)
            operRef = m_dt__mul;
		    [[fallthrough]]
		case EXPRESSION_TYPE::BIN_DIV:
        if (operRef == -1)
            operRef = m_dt__div;
		    [[fallthrough]]
        case EXPRESSION_TYPE::BIN_REM_DIV:
        if (operRef == -1)
            operRef = m_dt__mod;
		    [[fallthrough]]
        case EXPRESSION_TYPE::REL_EQUALS:
        if (operRef == -1)
            operRef = m_dt__eq;
		    [[fallthrough]]
    	case EXPRESSION_TYPE::REL_LESS:
        if (operRef == -1)
            operRef = m_dt__lt;
		    [[fallthrough]]
        case EXPRESSION_TYPE::REL_LESS_EQUALS:
        if (operRef == -1)
            operRef = m_dt__le;
		    [[fallthrough]]
        case EXPRESSION_TYPE::BIN_CONCAT:
            if (operRef == -1)
            operRef = m_dt__concat;
		    [[fallthrough]]
        case EXPRESSION_TYPE::BIN_AND:
            if (operRef == -1)
            operRef = m_dt__band;
		    [[fallthrough]]
		case EXPRESSION_TYPE::BIN_OR:
            if (operRef == -1)
            operRef = m_dt__bor;
		    [[fallthrough]]
		case EXPRESSION_TYPE::BIN_FLOOR_DIVISION:
            if (operRef == -1)
            operRef = m_dt__idiv;
		    [[fallthrough]]
		case EXPRESSION_TYPE::BITWISE_LEFT_SHIFT:
            if (operRef == -1)
            operRef = m_dt__shl;
		    [[fallthrough]]
		case EXPRESSION_TYPE::BITWISE_RIGHT_SHIFT:
            if (operRef == -1)
            operRef = m_dt__shr;
		    [[fallthrough]]
        case EXPRESSION_TYPE::BIN_NOT:
            if (operRef == -1)
            operRef = m_dt__bxor;
		    [[fallthrough]]
        case EXPRESSION_TYPE::REL_NOT_EQUALS:
            if (operRef == -1)
            operRef = m_dt__neq;
		    [[fallthrough]]
		case EXPRESSION_TYPE::REL_GREATER:
            if (operRef == -1)
            operRef = m_dt__gt;
		    [[fallthrough]]
		case EXPRESSION_TYPE::REL_GREATER_EQUALS:
            if (operRef == -1)
            operRef = m_dt__ge;
		    [[fallthrough]]
        case EXPRESSION_TYPE::BIN_EXPON:
        if (operRef == -1)
            operRef = m_dt__pow;
            treeBypassCodeGen(node->left);
            if (node->left->type == EXPRESSION_TYPE::FUNCTION_CALL || node->left->type == EXPRESSION_TYPE::VARARG_REF)
            {
                createIntOnStack(m_currentCodeRecorder, 0);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistGet, 2);
            }
            treeBypassCodeGen(node->right);
            if (node->right->type == EXPRESSION_TYPE::FUNCTION_CALL || node->left->type == EXPRESSION_TYPE::VARARG_REF)
            {
                createIntOnStack(m_currentCodeRecorder, 0);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistGet, 2);
            }

            m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
            m_currentCodeRecorder->addBytes(operRef, 2);
		    break;
        case EXPRESSION_TYPE::LOG_AND:
        {
            CodeRecorder *rightOper = new CodeRecorder();
            auto *temp = m_currentCodeRecorder;

            //m_currentCodeRecorder->addBytes(NEW, 1);
            //m_currentCodeRecorder->addBytes(m_dtClass, 2);
            //m_currentCodeRecorder->addBytes(DUP, 1);

            treeBypassCodeGen(node->left);
            if (node->left->type == EXPRESSION_TYPE::FUNCTION_CALL || node->left->type == EXPRESSION_TYPE::VARARG_REF)
            {
                createIntOnStack(m_currentCodeRecorder, 0);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistGet, 2);
            }

            m_currentCodeRecorder = rightOper;
            treeBypassCodeGen(node->right);
            if (node->right->type == EXPRESSION_TYPE::FUNCTION_CALL || node->right->type == EXPRESSION_TYPE::VARARG_REF)
            {
                createIntOnStack(m_currentCodeRecorder, 0);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistGet, 2);
            }

            m_currentCodeRecorder = temp;

            m_currentCodeRecorder->addBytes(DUP, 1);
            m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
            m_currentCodeRecorder->addBytes(m_dt_toBool, 2);
            m_currentCodeRecorder->addBytes(IFEQ, 1);
            m_currentCodeRecorder->addBytes((int16_t)(rightOper->m_byteCode.size() + 4));
            m_currentCodeRecorder->addBytes(POP, 1);
            m_currentCodeRecorder->append(rightOper);

        }
            break;
		case EXPRESSION_TYPE::LOG_OR:
		{
            CodeRecorder *rightOper = new CodeRecorder();
            auto *temp = m_currentCodeRecorder;

            //m_currentCodeRecorder->addBytes(NEW, 1);
            //m_currentCodeRecorder->addBytes(m_dtClass, 2);
            //m_currentCodeRecorder->addBytes(DUP, 1);

            treeBypassCodeGen(node->left);
            if (node->left->type == EXPRESSION_TYPE::FUNCTION_CALL || node->left->type == EXPRESSION_TYPE::VARARG_REF)
            {
                createIntOnStack(m_currentCodeRecorder, 0);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistGet, 2);
            }

            m_currentCodeRecorder = rightOper;
            treeBypassCodeGen(node->right);
            if (node->right->type == EXPRESSION_TYPE::FUNCTION_CALL || node->right->type == EXPRESSION_TYPE::VARARG_REF)
            {
                createIntOnStack(m_currentCodeRecorder, 0);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistGet, 2);
            }

            m_currentCodeRecorder = temp;

            m_currentCodeRecorder->addBytes(DUP, 1);
            m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
            m_currentCodeRecorder->addBytes(m_dt_toBool, 2);
            m_currentCodeRecorder->addBytes(IFNE, 1);
            m_currentCodeRecorder->addBytes((int16_t)(rightOper->m_byteCode.size() + 4));
            m_currentCodeRecorder->addBytes(POP, 1);
            m_currentCodeRecorder->append(rightOper);

        }
            break;
        case EXPRESSION_TYPE::LOG_NOT:
            if (operRef == -1)
            operRef = m_dt__not;
		    [[fallthrough]]
		case EXPRESSION_TYPE::UNAR_LEN:
            if (operRef == -1)
            operRef = m_dt__len;
		    [[fallthrough]]
        case EXPRESSION_TYPE::UNAR_BITWISE_NOT:
            if (operRef == -1)
            operRef = m_dt__bnot;
		    [[fallthrough]]
        case EXPRESSION_TYPE::UNAR_UMINUS:
            if (operRef == -1)
            operRef = m_dt__unm;
            treeBypassCodeGen(node->left);
            if (node->left->type == EXPRESSION_TYPE::FUNCTION_CALL || node->left->type == EXPRESSION_TYPE::VARARG_REF)
            {
                createIntOnStack(m_currentCodeRecorder, 0);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistGet, 2);
            }
            m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
            m_currentCodeRecorder->addBytes(operRef, 2);
		    break;
        case EXPRESSION_TYPE::CELL_BY_EXPR:
            treeBypassCodeGen(node->left);
            if (node->left->type == EXPRESSION_TYPE::FUNCTION_CALL || node->left->type == EXPRESSION_TYPE::VARARG_REF)
            {
                createIntOnStack(m_currentCodeRecorder, 0);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistGet, 2);
            }

            treeBypassCodeGen(node->right);
            if (node->right->type == EXPRESSION_TYPE::FUNCTION_CALL || node->right->type == EXPRESSION_TYPE::VARARG_REF)
            {
                createIntOnStack(m_currentCodeRecorder, 0);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistGet, 2);
            }

            m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
            if (m_assignContext)
                m_currentCodeRecorder->addBytes(m_dt__newindex, 2);
            else
                m_currentCodeRecorder->addBytes(m_dt__index, 2);


		    break;
		case EXPRESSION_TYPE::KEY_VALUE_ASSOC:
            m_assignContext = true;
            m_currentCodeRecorder->addBytes(DUP, 1);
            if (node->left)
            {
                treeBypassCodeGen(node->left);
                if (node->left->type == EXPRESSION_TYPE::FUNCTION_CALL || node->left->type == EXPRESSION_TYPE::VARARG_REF)
                {
                    createIntOnStack(m_currentCodeRecorder, 0);
                    m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                    m_currentCodeRecorder->addBytes(m_varlistGet, 2);
                }

                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                if (m_assignContext)
                    m_currentCodeRecorder->addBytes(m_dt__newindex, 2);
                else
                    m_currentCodeRecorder->addBytes(m_dt__index, 2);
            }
            else
            {
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_dt__autoindex, 2);
            }
            m_assignContext = false;

            // get right side
            treeBypassCodeGen(node->right);
            if (node->right->type == EXPRESSION_TYPE::FUNCTION_CALL || node->right->type == EXPRESSION_TYPE::VARARG_REF)
            {
                createIntOnStack(m_currentCodeRecorder, 0);
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                m_currentCodeRecorder->addBytes(m_varlistGet, 2);
            }

            // call set
            m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
            m_currentCodeRecorder->addBytes(m_dtSetDT, 2);
		    break;
		case EXPRESSION_TYPE::TABLE_CONSTRUCT:
            createDynamicTypeTable(m_currentCodeRecorder);
            if (node->lst)
            {
                for (auto *asgn : node->lst->lst)
                {
                    if (asgn->type != EXPRESSION_TYPE::KEY_VALUE_ASSOC)
                        throw std::string("Only KEY_VALUE_ASSOC can be in table constructor");

                    treeBypassCodeGen(asgn);
                }
            }
		    break;
		case EXPRESSION_TYPE::METHOD_NAME:
            {
                treeBypassCodeGen(node->left);
                if (node->left->type == EXPRESSION_TYPE::FUNCTION_CALL || node->left->type == EXPRESSION_TYPE::VARARG_REF)
                {
                    createIntOnStack(m_currentCodeRecorder, 0);
                    m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                    m_currentCodeRecorder->addBytes(m_varlistGet, 2);
                }
    
                m_currentCodeRecorder->addBytes(DUP, 1);
    
                auto *newst = new DoublePtrString(node->identifier.c_str());
                createDynamicType(m_currentCodeRecorder, *newst);
    
                m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
                if (m_assignContext)
                    m_currentCodeRecorder->addBytes(m_dt__newindex, 2);
                else
                    m_currentCodeRecorder->addBytes(m_dt__index, 2);
            }
		    break;
		case EXPRESSION_TYPE::FUNCTION_CALL:
            if (node==NULL)	return;
	        treeBypassCodeGen(node->left); // Creates DynamicType with function on stack and probably leaves caller in the beginning
	        treeBypassCodeGen(node->lst); // Create VarList on stack
            m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
            if (node->left->type != EXPRESSION_TYPE::METHOD_NAME)
                m_currentCodeRecorder->addBytes(m_dtCallRef, 2); // Leaves VarList on stack
            else
                m_currentCodeRecorder->addBytes(m_dtCallMethodRef, 2); // Leaves VarList on stack
		    break;
		case EXPRESSION_TYPE::UNNAMED_FUNCTION_DEFINITION:
        {
            auto funcClassID = addOrConfirmClassToTable(node->varContext->className);
            auto funcClassInitID = addOrConfirmMethodRefToTable("<init>", "()V", node->varContext->className);

            // Create DynamicType
            m_currentCodeRecorder->addBytes(NEW, 1);
            m_currentCodeRecorder->addBytes(m_dtClass, 2);
            m_currentCodeRecorder->addBytes(DUP, 1);

            // Create FunctionClass
            m_currentCodeRecorder->addBytes(NEW, 1);
            m_currentCodeRecorder->addBytes(funcClassID, 2);
            m_currentCodeRecorder->addBytes(DUP, 1);
            m_currentCodeRecorder->addBytes(INVOKESPECIAL, 1);
            m_currentCodeRecorder->addBytes(funcClassInitID, 2);

            // Initialize DynamicType
            m_currentCodeRecorder->addBytes(INVOKESPECIAL, 1);
            m_currentCodeRecorder->addBytes(m_dtInitIdF, 2);
        }
		    break;
		case EXPRESSION_TYPE::VARARG_REF:
            handleVararg();
		    break;
	}
}

void ClassTable::treeBypassCodeGen(StatementGotoCall *node)
{
    // TODO: No idea what to do with it
	if (node==NULL)	return;
}

void ClassTable::treeBypassCodeGen(StatementGotoLabel *node)
{
    // TODO: No idea what to do with it
	if (node==NULL)	return;
}

void ClassTable::treeBypassCodeGen(StatementForeachLoop *node)
{
	if (node==NULL)	return;


    std::vector<std::string> varnames;
    std::vector<int> varrefs;
    int varcount = 0;
    for (auto &el : node->params->lst)
    {
        varcount++;
        auto varname = std::string("CONTEXT_") + std::to_string(node->iterContext->contextID) + "_" + el;
        auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", node->iterContext->className);
        varnames.push_back(varname);
        varrefs.push_back(fldRef);
    }

    auto *codeblock = new CodeRecorder();
    auto *temp = m_currentCodeRecorder;

    m_currentCodeRecorder = codeblock;
    treeBypassCodeGen(node->code);

    m_currentCodeRecorder = temp;
    treeBypassCodeGen(node->data);

    // Iter starts here

    m_currentCodeRecorder->addBytes(DUP, 1);
    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    m_currentCodeRecorder->addBytes(m_foreachHead, 2);

    // Grab all vars
    int totalspecificsize = 0;
    for (int i = 0; i < varcount; ++i) // varcount * 
    {
        m_currentCodeRecorder->addBytes(DUP, 1);
        totalspecificsize += createIntOnStack(m_currentCodeRecorder, i);
        m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
        m_currentCodeRecorder->addBytes(m_varlistGet, 2);
        m_currentCodeRecorder->addBytes(GETSTATIC, 1);
        m_currentCodeRecorder->addBytes(varrefs[i], 2);
        m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
        m_currentCodeRecorder->addBytes(m_dtRevset, 2);
        totalspecificsize += 10;
    }

    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    m_currentCodeRecorder->addBytes(m_foreachCondition, 2);
    m_currentCodeRecorder->addBytes(IFEQ, 1);
    m_currentCodeRecorder->addBytes((int16_t)(codeblock->m_byteCode.size() + 6));

    m_currentCodeRecorder->append(codeblock);

    m_currentCodeRecorder->addBytes(GOTO, 1);
    m_currentCodeRecorder->addBytes((int16_t)(-codeblock->m_byteCode.size() - 10 - totalspecificsize));

}

void ClassTable::treeBypassCodeGen(StatementForLoop *node)
{
	if (node==NULL)	return;

	auto *codeblock = new CodeRecorder();
    auto *beginIter = new CodeRecorder();
    auto *endIter = new CodeRecorder();
    auto *stepIter = new CodeRecorder();

    // Iterator var data
    auto varname = std::string("CONTEXT_") + std::to_string(node->iterContext->contextID) + "_" + node->identifier;
    auto fldRef = addOrConfirmFieldRefToTable(varname, "LDynamicType;", node->iterContext->className);

    auto *temp = m_currentCodeRecorder;

    m_currentCodeRecorder = codeblock;
    treeBypassCodeGen(node->code);

    m_currentCodeRecorder = beginIter;
    treeBypassCodeGen(node->begin);

    m_currentCodeRecorder = endIter;
    treeBypassCodeGen(node->end);

    m_currentCodeRecorder = stepIter;
    treeBypassCodeGen(node->step);

    m_currentCodeRecorder = temp;

    createVarList(m_currentCodeRecorder);
    m_currentCodeRecorder->addBytes(DUP, 1);
    m_currentCodeRecorder->append(beginIter);
    if (node->begin->type == EXPRESSION_TYPE::FUNCTION_CALL || node->begin->type == EXPRESSION_TYPE::VARARG_REF)
    {
        createIntOnStack(m_currentCodeRecorder, 0);
        m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
        m_currentCodeRecorder->addBytes(m_varlistGet, 2);
    }
    m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
    m_currentCodeRecorder->addBytes(m_varlistAdd, 2);

    m_currentCodeRecorder->addBytes(DUP, 1);
    m_currentCodeRecorder->append(endIter);
    if (node->end->type == EXPRESSION_TYPE::FUNCTION_CALL || node->end->type == EXPRESSION_TYPE::VARARG_REF)
    {
        createIntOnStack(m_currentCodeRecorder, 0);
        m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
        m_currentCodeRecorder->addBytes(m_varlistGet, 2);
    }
    m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
    m_currentCodeRecorder->addBytes(m_varlistAdd, 2);

    m_currentCodeRecorder->addBytes(DUP, 1);
    m_currentCodeRecorder->append(stepIter);
    if (node->step->type == EXPRESSION_TYPE::FUNCTION_CALL || node->step->type == EXPRESSION_TYPE::VARARG_REF)
    {
        createIntOnStack(m_currentCodeRecorder, 0);
        m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
        m_currentCodeRecorder->addBytes(m_varlistGet, 2);
    }
    m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
    m_currentCodeRecorder->addBytes(m_varlistAdd, 2);

    // Set iterator var
    m_currentCodeRecorder->addBytes(DUP, 1);
    createIntOnStack(m_currentCodeRecorder, 0);
    m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
    m_currentCodeRecorder->addBytes(m_varlistGet, 2);
    m_currentCodeRecorder->addBytes(GETSTATIC, 1);
    m_currentCodeRecorder->addBytes(fldRef, 2);
    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    m_currentCodeRecorder->addBytes(m_dtRevset, 2);

    // Iters start from here

    m_currentCodeRecorder->addBytes(DUP, 1);
    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    m_currentCodeRecorder->addBytes(m_forLoopCond, 2);
    m_currentCodeRecorder->addBytes(IFEQ, 1);
    m_currentCodeRecorder->addBytes((int16_t)(codeblock->m_byteCode.size() + 20));

    m_currentCodeRecorder = codeblock;

    // Start of last iter segment
    // Proceed iter
    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    m_currentCodeRecorder->addBytes(m_forLoopIter, 2);

    // Set iterator var
    m_currentCodeRecorder->addBytes(DUP, 1);
    createIntOnStack(m_currentCodeRecorder, 0);
    m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
    m_currentCodeRecorder->addBytes(m_varlistGet, 2);
    m_currentCodeRecorder->addBytes(GETSTATIC, 1);
    m_currentCodeRecorder->addBytes(fldRef, 2);
    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    m_currentCodeRecorder->addBytes(m_dtRevset, 2);

    // Return to condition
    m_currentCodeRecorder->addBytes(GOTO, 1);
    m_currentCodeRecorder->addBytes((int16_t)(-codeblock->m_byteCode.size() - 6));
    m_currentCodeRecorder->solveBreaks();

    m_currentCodeRecorder = temp;
    m_currentCodeRecorder->append(codeblock);
    m_currentCodeRecorder->addBytes(POP, 1);

}

void ClassTable::treeBypassCodeGen(StatementRepeatLoop *node)
{
	if (node==NULL)	return;


    auto *codeblock = new CodeRecorder();
    auto *condition = new CodeRecorder();

    auto *temp = m_currentCodeRecorder;

    m_currentCodeRecorder = condition;
    treeBypassCodeGen(node->condition);

    m_currentCodeRecorder = codeblock;
    treeBypassCodeGen(node->trueCode);

    m_currentCodeRecorder->append(condition);
    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    m_currentCodeRecorder->addBytes(m_dt_toBool, 2);
    m_currentCodeRecorder->addBytes(IFEQ, 1);
    m_currentCodeRecorder->addBytes((int16_t)(-m_currentCodeRecorder->m_byteCode.size() + 1));
    m_currentCodeRecorder->solveBreaks();

    m_currentCodeRecorder = temp;
    m_currentCodeRecorder->append(codeblock);


}

void ClassTable::treeBypassCodeGen(StatementWhileLoop *node)
{
	if (node==NULL)	return;

    auto *codeblock = new CodeRecorder();
    auto *condition = new CodeRecorder();

    auto *temp = m_currentCodeRecorder;

    m_currentCodeRecorder = condition;
    treeBypassCodeGen(node->condition);

    m_currentCodeRecorder = codeblock;
    treeBypassCodeGen(node->trueCode);
    
    m_currentCodeRecorder = temp;

    m_currentCodeRecorder->append(condition);
    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    m_currentCodeRecorder->addBytes(m_dt_toBool, 2);
    m_currentCodeRecorder->addBytes(IFEQ, 1);
    m_currentCodeRecorder->addBytes(codeblock->m_byteCode.size() + 6, 2);

    m_currentCodeRecorder = codeblock;

    //m_currentCodeRecorder->append(codeblock);
    m_currentCodeRecorder->addBytes(GOTO, 1);
    m_currentCodeRecorder->addBytes((int16_t)(-codeblock->m_byteCode.size() - condition->m_byteCode.size() - 5));
    m_currentCodeRecorder->solveBreaks();

    m_currentCodeRecorder = temp;
    m_currentCodeRecorder->append(codeblock);

    delete codeblock;
}

void ClassTable::treeBypassCodeGen(StatementIfElse *node)
{
	if (node==NULL)	return;

	auto *condition = new CodeRecorder();
    auto *trueCode = new CodeRecorder();
    auto *falseCode = new CodeRecorder();

    auto *temp = m_currentCodeRecorder;

    m_currentCodeRecorder = condition;
    treeBypassCodeGen(node->condition);
    if (node->condition->type == EXPRESSION_TYPE::FUNCTION_CALL || node->condition->type == EXPRESSION_TYPE::VARARG_REF)
    {
        createIntOnStack(m_currentCodeRecorder, 0);
        m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
        m_currentCodeRecorder->addBytes(m_varlistGet, 2);
    }

    m_currentCodeRecorder = trueCode;
    treeBypassCodeGen(node->trueCode);
    m_currentCodeRecorder = falseCode;
    treeBypassCodeGen(node->falseCode);

    std::vector<CodeRecorder*> conditions;
    std::vector<CodeRecorder*> codes;

    int totalElifSize = 0;

    for (auto *el : node->elseifs)
    {
        auto *elseifCond = new CodeRecorder();
        auto *elseifCode = new CodeRecorder();

        m_currentCodeRecorder = elseifCond;
        treeBypassCodeGen(el->condition);
        int additionalsize = 0;
        if (el->condition->type == EXPRESSION_TYPE::FUNCTION_CALL || el->condition->type == EXPRESSION_TYPE::VARARG_REF)
        {
            additionalsize = createIntOnStack(m_currentCodeRecorder, 0);
            m_currentCodeRecorder->addBytes(INVOKEVIRTUAL, 1);
            m_currentCodeRecorder->addBytes(m_varlistGet, 2);
        }

        m_currentCodeRecorder = elseifCode;
        treeBypassCodeGen(el->trueCode);

        totalElifSize += elseifCond->m_byteCode.size() + elseifCode->m_byteCode.size() + 9;

        conditions.push_back(elseifCond);
        codes.push_back(elseifCode);
    }

    int totalElifSizeLeft = totalElifSize;

    m_currentCodeRecorder = temp;

    m_currentCodeRecorder->append(condition);
    m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
    m_currentCodeRecorder->addBytes(m_dt_toBool, 2);
    m_currentCodeRecorder->addBytes(IFEQ, 1);
    m_currentCodeRecorder->addBytes(trueCode->m_byteCode.size() + 6, 2);

    m_currentCodeRecorder->append(trueCode);
    m_currentCodeRecorder->addBytes(GOTO, 1);
    m_currentCodeRecorder->addBytes(totalElifSize + falseCode->m_byteCode.size() + 3, 2);

    for (int i = 0; i < conditions.size(); ++i)
    {
        m_currentCodeRecorder->append(conditions[i]);
        m_currentCodeRecorder->addBytes(INVOKESTATIC, 1);
        m_currentCodeRecorder->addBytes(m_dt_toBool, 2);
        m_currentCodeRecorder->addBytes(IFEQ, 1);
        m_currentCodeRecorder->addBytes(codes[i]->m_byteCode.size() + 6, 2);
        m_currentCodeRecorder->append(codes[i]);
        m_currentCodeRecorder->addBytes(GOTO, 1);
        totalElifSizeLeft -= conditions[i]->m_byteCode.size();
        totalElifSizeLeft -= codes[i]->m_byteCode.size();
        totalElifSizeLeft -= 9;
        m_currentCodeRecorder->addBytes(totalElifSizeLeft + falseCode->m_byteCode.size() + 3, 2);
    }

    m_currentCodeRecorder->append(falseCode);
}

void ClassTable::treeBypassCodeGen(StatementReturn *node)
{
	if (node==NULL)	return;

    if (node->lst)
    {
	    treeBypassCodeGen(node->lst);
        m_currentCodeRecorder->addBytes(ARETURN, 1);
    }
    else
    {
        createVarList(m_currentCodeRecorder);
        m_currentCodeRecorder->addBytes(ARETURN, 1);
    }
}

void ClassTable::handleVararg()
{
    throw std::string("Vararg used in non-vararg function");
}
