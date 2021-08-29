#include "deegen/bytecode_metadata.h"

#include "gtest/gtest.h"

namespace TestBytecodeMetadata
{

struct MyClass1
{
    BEGIN_FIELD_LIST();
        FIELD(x, int);
        FIELD(y, double);
    END_FIELD_LIST();
};

TEST(BytecodeMetadata, Sanity1)
{
    using md = deegen::BytecodeMetadataAccessor<MyClass1>;
    ReleaseAssert(md::numFields == 2);
    ReleaseAssert(md::mdArray.size() == 2);
    ReleaseAssert(md::mdArray[0].m_fieldKind == deegen::BytecodeFieldKind::SimpleField);
    ReleaseAssert(std::string(md::mdArray[0].m_fieldName) == "x");
    ReleaseAssert(md::mdArray[0].m_typeName == "int");

    ReleaseAssert(md::mdArray[1].m_fieldKind == deegen::BytecodeFieldKind::SimpleField);
    ReleaseAssert(std::string(md::mdArray[1].m_fieldName) == "y");
    ReleaseAssert(md::mdArray[1].m_typeName == "double");
}

struct MyClass2
{
    BEGIN_FIELD_LIST();
        FIELD(a, deegen::BytecodeRef);
        FIELD(b, deegen::BytecodeRef);
        FIELD(c, deegen::BytecodeDest);
    END_FIELD_LIST();
};

TEST(BytecodeMetadata, Sanity2)
{
    using md = deegen::BytecodeMetadataAccessor<MyClass2>;
    ReleaseAssert(md::numFields == 3);
    ReleaseAssert(md::mdArray.size() == 3);
    ReleaseAssert(md::mdArray[0].m_fieldKind == deegen::BytecodeFieldKind::SimpleField);
    ReleaseAssert(std::string(md::mdArray[0].m_fieldName) == "a");
    ReleaseAssert(md::mdArray[0].m_typeName == "deegen::BytecodeRef");

    ReleaseAssert(md::mdArray[1].m_fieldKind == deegen::BytecodeFieldKind::SimpleField);
    ReleaseAssert(std::string(md::mdArray[1].m_fieldName) == "b");
    ReleaseAssert(md::mdArray[1].m_typeName == "deegen::BytecodeRef");

    ReleaseAssert(md::mdArray[2].m_fieldKind == deegen::BytecodeFieldKind::SimpleField);
    ReleaseAssert(std::string(md::mdArray[2].m_fieldName) == "c");
    ReleaseAssert(md::mdArray[2].m_typeName == "deegen::BytecodeDest");
}

struct MyClass3
{
    BEGIN_FIELD_LIST();
        FIELD_ARR_FIXLEN(a, 3, deegen::BytecodeRef);
        FIELD_ARR_VARLEN(b, n, deegen::BytecodeDest);
        FIELD_ARR_FIXLEN(c, 2, deegen::BytecodeDest);
    END_FIELD_LIST();
};

TEST(BytecodeMetadata, Sanity3)
{
    using md = deegen::BytecodeMetadataAccessor<MyClass3>;
    ReleaseAssert(md::numFields == 4);
    ReleaseAssert(md::mdArray.size() == 4);
    ReleaseAssert(md::mdArray[0].m_fieldKind == deegen::BytecodeFieldKind::FixLenArray);
    ReleaseAssert(std::string(md::mdArray[0].m_fieldName) == "a");
    ReleaseAssert(md::mdArray[0].m_arrayFixedLength == 3);
    ReleaseAssert(md::mdArray[0].m_typeName == "deegen::BytecodeRef");

    ReleaseAssert(md::mdArray[1].m_fieldKind == deegen::BytecodeFieldKind::VarLenArray);
    ReleaseAssert(std::string(md::mdArray[1].m_fieldName) == "b");
    ReleaseAssert(std::string(md::mdArray[1].m_arrayVarLengthFieldName) == "n");
    ReleaseAssert(md::mdArray[1].m_typeName == "deegen::BytecodeDest");

    ReleaseAssert(md::mdArray[2].m_fieldKind == deegen::BytecodeFieldKind::SimpleField);
    ReleaseAssert(std::string(md::mdArray[2].m_fieldName) == "n");
    ReleaseAssert(md::mdArray[2].m_typeName == "unsigned int");

    ReleaseAssert(md::mdArray[3].m_fieldKind == deegen::BytecodeFieldKind::FixLenArray);
    ReleaseAssert(std::string(md::mdArray[3].m_fieldName) == "c");
    ReleaseAssert(md::mdArray[3].m_arrayFixedLength == 2);
    ReleaseAssert(md::mdArray[3].m_typeName == "deegen::BytecodeDest");
}

}   // namespace TestBytecodeMetadata
