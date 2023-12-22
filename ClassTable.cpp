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

ClassInfo::ClassInfo(size_t nameIndex_):
    TableEntry(TableEntry::ENTRY_TYPE::CLASS),
    m_nameIndex(nameIndex_)
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
    writeBytes(0x00000034, 4); // Java version
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
        }
    }
    
    writeBytes(0x0021, 2); // Access flags
    writeBytes(m_mainClassID, 2); // This class
    writeBytes(m_javaLangObjectID, 2); // Super class
    writeBytes(0x0000, 2); // Interface count
    writeBytes(0x0000, 2); // Fields count
    writeBytes(0x0000, 2); // Methods count
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
                return i;
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
            if (utfstr->m_nameIndex == classNameEntry - 1)
            {
                std::cout << "Class \"" << s_ << "\" already exists\n";
                return i;
            }
        }
    }

    std::cout << "Added class \"" << s_ << "\" to the constant pool\n";
    m_constantPool.push_back(new ClassInfo(classNameEntry));
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