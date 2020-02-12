#include <gtest/gtest.h>
#include <core/http.h>
#include <core/memory.h>
#include <core/multithreading.h>
#include <core/types.h>
#include <random>
#include <chrono>
#include <type_traits>

static std::mt19937 random_engine(
    std::chrono::system_clock::now().time_since_epoch().count());

static void random_data(ice::core::byte_type* data, const ice::core::size_type size)
{
    for(int offset = 0; offset < size; offset++)
    {
        *(data + offset) = (ice::core::byte_type)random_engine();
    }
}

static int random_int(const int min, const int max)
{
    std::uniform_int_distribution<int> distribution(min,max);
    return distribution(random_engine);
}

