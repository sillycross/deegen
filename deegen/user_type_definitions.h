#pragma once

#include "deegen_common_utils.h"

namespace deegen
{

#define FOR_EACH_TYPE_REPRESENTATION                            \
/*      Name        C++ Representation    Var-length Struct?  */\
/* ===========================================================*/\
F(      Int32,          int32_t,                false)          \
F(      Double,         double,                 false)          \
/* =========================================================== */

enum class TypeEnum
{
#define F(name, rep, is_va) name ,
    FOR_EACH_TYPE_REPRESENTATION
#undef F
    X_END_OF_ENUM
};
static_assert(static_cast<size_t>(TypeEnum::X_END_OF_ENUM) > 0,
              "At least one type must be provided");

template<TypeEnum te>
struct TypeImpl /*intentionally undefined*/;

#define F(name, rep, is_va)                                     \
    template<> struct TypeImpl<TypeEnum::name> {                \
        using type = rep;                                       \
    };
    FOR_EACH_TYPE_REPRESENTATION
#undef F

template<TypeEnum te>
using Type = typename TypeImpl<te>::type;

}   // namespace deegen
