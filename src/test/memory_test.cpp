#include "memory_test.h"

const static ice::core::size_type TEST_BUFFER_SIZE = 1024 * 1024;

void memory_test::SetUp()
{
    // make a pool
    _memory_pool = std::make_shared<ice::core::memory_pool>(
        3 * TEST_BUFFER_SIZE);
    _pool_allocator = std::make_shared<ice::core::pool_allocator<ice::core::byte_type>>(
        _memory_pool);
}

void memory_test::TearDown()
{

}

// Fill buffer_1 with random data, copy to buffer_2, and compare.
template<class buffer_type> static void buffer_test(
    buffer_type & buffer_1,
    buffer_type & buffer_2)
{
    random_data(
        (ice::core::byte_type*)buffer_1.data(),
        buffer_1.size());
    
    memcpy(
        (ice::core::byte_type*)buffer_2.data(),
        (ice::core::byte_type*)buffer_1.data(),
        buffer_2.size());
    
    EXPECT_EQ(
        memcmp(
            (ice::core::byte_type*)buffer_2.data(),
            (ice::core::byte_type*)buffer_1.data(),
            buffer_2.size()),
            0);
}

TEST_F(memory_test,stack_test)
{
    ice::core::stack_buffer<TEST_BUFFER_SIZE> stack_buffer_1;
    ice::core::stack_buffer<TEST_BUFFER_SIZE> stack_buffer_2;
    buffer_test(stack_buffer_1,stack_buffer_2);
}

TEST_F(memory_test,heap_test)
{
    ice::core::heap_buffer heap_buffer_1;
    ice::core::heap_buffer heap_buffer_2;
    heap_buffer_1.resize(TEST_BUFFER_SIZE);
    heap_buffer_2.resize(TEST_BUFFER_SIZE);
    buffer_test(heap_buffer_1,heap_buffer_2);
}

TEST_F(memory_test,pool_test)
{
    ice::core::pool_buffer pool_buffer_1(*_pool_allocator);
    ice::core::pool_buffer pool_buffer_2(*_pool_allocator);
    pool_buffer_1.resize(TEST_BUFFER_SIZE);
    pool_buffer_2.resize(TEST_BUFFER_SIZE);
    buffer_test(pool_buffer_1,pool_buffer_2);
}