#include "ClassTable.h"

extern Program *prg;

TableEntry::TableEntry(ENTRY_TYPE type_):
    m_type(type_)
{
}


Utf8Info::Utf8Info(const std::string &s_):
    TableEntry(TableEntry::ENTRY_TYPE::UTF8),
    m_str(s_.c_str())
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

void ClassTable::generateClassTable(const std::string &classname_)
{
    m_classname = classname_;

    m_mainClassID = addOrConfirmClassToTable(classname_);
    m_javaLangObjectID = addOrConfirmClassToTable("java/lang/Object");
    m_javaLangSystemID = addOrConfirmClassToTable("java/lang/System");
    m_javaIOPrintStreamID = addOrConfirmClassToTable("java/io/PrintStream");
    m_helloWorldStr = addOrConfirmStringToTable("Hello, world!");
    m_placeholderStr = addOrConfirmStringToTable("Placeholder func");
    m_outFieldID = addOrConfirmFieldRefToTable("out", "Ljava/io/PrintStream;", "java/lang/System");
    m_printlnStrID = addOrConfirmMethodRefToTable("println", "(I)V", "java/io/PrintStream");
    m_mainNameID = addOrConfirmUtf8ToTable("main");
    m_mainTypeID = addOrConfirmUtf8ToTable("([Ljava/lang/String;)V");
    m_codeAttrNameID = addOrConfirmUtf8ToTable("Code");
    m_codeAttrNameID = addOrConfirmUtf8ToTable("Code");

    auto fldName = addOrConfirmUtf8ToTable("nobj");
    auto fldDesc = addOrConfirmUtf8ToTable("LDynamicType;");
    m_fldref = addOrConfirmFieldRefToTable("nobj", "LDynamicType;", classname_);

    auto dtclass = addOrConfirmClassToTable("DynamicType");
    auto dtinitid = addOrConfirmMethodRefToTable("<init>", "(I)V", "DynamicType");
    auto dtfieldid = addOrConfirmFieldRefToTable("iValue", "I", "DynamicType");

    // =========== MAIN =================
    auto *mainmethod = new MethodInfo();
    mainmethod->m_accessFlags = 0x0009;
    mainmethod->m_nameIndex = m_mainNameID;
    mainmethod->m_descIndex = m_mainTypeID;
    mainmethod->m_attribCount = 1;
    mainmethod->m_codeAttrNameIndex = m_codeAttrNameID;
    mainmethod->m_maxStack = 3;
    mainmethod->m_maxLocals = 1;

    mainmethod->addBytes(0xbb, 1); // new
    mainmethod->addBytes(dtclass, 2); // new
    mainmethod->addBytes(0x59, 1); // dup
    mainmethod->addBytes(0x2, 1); // iconst_m1
    mainmethod->addBytes(0xb7, 1); // invokespecial
    mainmethod->addBytes(dtinitid, 2); // <init>
    mainmethod->addBytes(0xb3, 1); // putstatic
    mainmethod->addBytes(m_fldref, 2); // nobj field
    mainmethod->addBytes(0xb2, 1); // getstatic
    mainmethod->addBytes(m_outFieldID, 2); // System.out field
    mainmethod->addBytes(0xb2, 1); // getstatic
    mainmethod->addBytes(m_fldref, 2); // nobj field
    mainmethod->addBytes(0xb4, 1); // getfield
    mainmethod->addBytes(dtfieldid, 2); // iValue field
    mainmethod->addBytes(0xb6, 1); // Invoke virtual
    mainmethod->addBytes(m_printlnStrID, 2); // println(string)
    mainmethod->addBytes(0xb1, 1);
    mainmethod->m_codeLength = mainmethod->m_byteCode.size();
    mainmethod->m_codeAttrLength = mainmethod->m_codeLength + 12;

    m_methodPool.push_back(mainmethod);

    auto *mainfield = new FieldInfo();
    mainfield->m_accessFlags = 0x0009;
    mainfield->m_nameIndex = fldName;
    mainfield->m_descIndex = fldDesc;
    m_fieldPool.push_back(mainfield);
}

void ClassTable::generateClassFile()
{
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
                writeBytes(properptr->m_str.end - properptr->m_str.begin - 1, 2);
                writeBytes(properptr->m_str);
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
    writeBytes(m_mainClassID, 2); // This class
    writeBytes(m_javaLangObjectID, 2); // Super class
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
    auto begin = end - countBytes_;
    for (auto i = countBytes_ - 1; i >= 0 && i < countBytes_; --i)
    {
        m_output.write(arr + i, 1);
    }
}

void ClassTable::writeBytes(const DoublePtrString &str_)
{
    m_output.write(str_.begin, str_.end - str_.begin - 1);
}

void MethodInfo::addBytes(uint64_t bytes_, size_t countBytes_)
{
    char *arr = (char*)&bytes_;  // TODO: to reinterpret
    auto end = sizeof(bytes_);
    auto begin = end - countBytes_;
    for (auto i = countBytes_ - 1; i >= 0 && i < countBytes_; --i)
    {
        m_byteCode.push_back(arr[i]);
    }
}
