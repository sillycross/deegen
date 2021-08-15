#pragma once

#include "common.h"
#include "thirdparty_moodycamel_concurrentqueue_impl.h"

namespace deegen
{

// https://github.com/cameron314/concurrentqueue
//
template<typename T>
using ConcurrentQueue = ::moodycamel::ConcurrentQueue<T>;

}   // namespace deegen
