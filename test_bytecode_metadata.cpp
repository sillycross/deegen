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

struct MyClass4
{
    using TE = deegen::TypeEnum;

    template<TE te1, TE te2>
    void f(deegen::Type<te1>* /*v1*/, deegen::Type<te2>* /*v2*/, int /*v3*/) const noexcept
    { }

    REGISTER_DISPATCHER(MyClass4, f, 2)

    void g(deegen::BoxedValue a, deegen::BoxedValue b)
    {
        dispatch_to_f(a, b, 1);
    }
};

struct MyClass4Checker
{
    template<size_t i1, size_t i2>
    static void checker(const std::vector<void*>& o)
    {
        constexpr size_t numTypes = static_cast<size_t>(deegen::TypeEnum::X_END_OF_ENUM);
        static_assert(i1 < numTypes && i2 < numTypes);
        ReleaseAssert(o[i1 * numTypes + i2] ==
                deegen::GetClassMethodPtr(
                    &MyClass4::f<
                        static_cast<deegen::TypeEnum>(i1),
                        static_cast<deegen::TypeEnum>(i2)>));
        if constexpr(i2 + 1 < numTypes)
        {
            checker<i1, i2 + 1>(o);
        }
        else if constexpr(i1 + 1 < numTypes)
        {
            checker<i1 + 1, 0>(o);
        }
    }
};

TEST(BytecodeMetadata, Sanity4)
{
    std::vector<void*> o;
    MyClass4::__gen_voidstar_list_f(&o);
    size_t numTypes = static_cast<size_t>(deegen::TypeEnum::X_END_OF_ENUM);
    ReleaseAssert(o.size() == numTypes * numTypes);
    MyClass4Checker::checker<0, 0>(o);
}

}   // namespace TestBytecodeMetadata
