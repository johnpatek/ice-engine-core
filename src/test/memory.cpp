#include "test.h"
#include "core/filesystem.h"
#include "core/memory.h"
#include <cstring>
#include <iostream>

ICE_TEST_CASE(pool_test)
{
    const int POOL_SIZE_1 = 4096;
    const int BLOCK_SIZE_1 = 256;
    const int BLOCK_SIZE_2 = 512;
    const char * TEST_STRING_1 = "Test string 1";

    ice::core::memory_pool test_pool_1(POOL_SIZE_1);

    // 4064 = 4096 - 32
    auto expected_free_size_1 = POOL_SIZE_1 - ice::core::CHUNK_OVERHEAD;
    
    auto actual_free_size_1 = test_pool_1.get_free_size();
    
    ICE_TEST_ASSERT(expected_free_size_1 == actual_free_size_1)

    // 3776 = 4064 - 256 - 32
    auto expected_free_size_2 = expected_free_size_1 - BLOCK_SIZE_1 - ice::core::CHUNK_OVERHEAD;
    
    auto test_block_1 = test_pool_1.allocate_block(BLOCK_SIZE_1);

    auto actual_free_size_2 = test_pool_1.get_free_size();

    ICE_TEST_ASSERT(expected_free_size_2 == actual_free_size_2)

    // 3232 = 3776 - 512 - 32
    auto expected_free_size_3 = expected_free_size_2 - BLOCK_SIZE_2 - ice::core::CHUNK_OVERHEAD;
    
    auto test_block_2 = test_pool_1.allocate_block(BLOCK_SIZE_2);

    auto actual_free_size_3 = test_pool_1.get_free_size();

    ICE_TEST_ASSERT(expected_free_size_3 == actual_free_size_3)

    memcpy(test_block_1,TEST_STRING_1,strlen(TEST_STRING_1));

    memcpy(test_block_2,TEST_STRING_1,strlen(TEST_STRING_1));

    ICE_TEST_ASSERT(0 == memcmp(
        test_block_1,
        test_block_2,
        strlen(
            TEST_STRING_1)))

    // 3776 = 3232 + 512 + 32
    auto expected_free_size_4 = expected_free_size_3 + BLOCK_SIZE_2 + ice::core::CHUNK_OVERHEAD;

    test_pool_1.deallocate_block(test_block_2);

    auto actual_free_size_4 = test_pool_1.get_free_size();

    ICE_TEST_ASSERT(expected_free_size_4 == actual_free_size_4)

    // 4064 = 3776 + 256 + 32
    auto expected_free_size_5 = expected_free_size_4 + BLOCK_SIZE_1 + ice::core::CHUNK_OVERHEAD;

    test_pool_1.deallocate_block(test_block_1);

    auto actual_free_size_5 = test_pool_1.get_free_size();

    ICE_TEST_ASSERT(expected_free_size_5 == actual_free_size_5)
}

ICE_TEST_CASE(allocator_test)
{
    const int POOL_SIZE_1 = 512;
    const int POOL_SIZE_2 = 1024;
    const int BLOCK_SIZE_1 = 100;
    const int BLOCK_SIZE_2 = 100;
    const char * TEST_STRING_1 = "Test string 1";

    std::shared_ptr<ice::core::memory_pool> test_pool_1 = 
        std::make_shared<ice::core::memory_pool>(POOL_SIZE_1);
    ice::core::pool_allocator<ice::core::byte_type> test_allocator_1(
        test_pool_1);

    auto test_block_1 = test_allocator_1.allocate(BLOCK_SIZE_1);

    auto test_block_2 = test_allocator_1.allocate(BLOCK_SIZE_2);

    memcpy(test_block_1,TEST_STRING_1,strlen(TEST_STRING_1));

    memcpy(test_block_2,TEST_STRING_1,strlen(TEST_STRING_1));

    ICE_TEST_ASSERT(0 == memcmp(
        test_block_1,
        test_block_2,
        strlen(
            TEST_STRING_1)))
    
    std::shared_ptr<ice::core::memory_pool> test_pool_2 = 
        std::make_shared<ice::core::memory_pool>(POOL_SIZE_2);

    ice::core::pool_allocator<ice::core::byte_type> test_allocator_2(
        test_pool_2);
    
    std::vector<ice::core::byte_type,ice::core::pool_allocator<ice::core::byte_type>> test_vector_1(
        test_allocator_2);

    test_vector_1.resize(50);
}

ICE_TEST_CASE(buffer_test)
{
    const int POOL_SIZE_1 = 512;
    const char * TEST_STRING_1 = "Test string 1";

    std::shared_ptr<ice::core::memory_pool> test_pool_1 = 
        std::make_shared<ice::core::memory_pool>(POOL_SIZE_1);
    
    ice::core::pool_allocator<ice::core::byte_type> test_allocator_1(
        test_pool_1);

    ice::core::heap_buffer test_heap_buffer_1;
    
    ice::core::pool_buffer test_pool_buffer_1(test_allocator_1); 

    test_heap_buffer_1.resize(strlen(TEST_STRING_1));
    
    test_pool_buffer_1.resize(strlen(TEST_STRING_1));

    memcpy(
        test_heap_buffer_1.data(),
        TEST_STRING_1,
        test_heap_buffer_1.size());
    
    memcpy(
        test_pool_buffer_1.data(),
        TEST_STRING_1,
        test_pool_buffer_1.size());

    ICE_TEST_ASSERT(
        0 == memcmp(
            test_heap_buffer_1.data(),
            test_pool_buffer_1.data(),
            strlen(TEST_STRING_1)))
}

ICE_UNIT_TEST
{
    ICE_LOG_TEST_CASE(pool_test,std::cerr)
    ICE_LOG_TEST_CASE(allocator_test,std::cerr)
    ICE_LOG_TEST_CASE(buffer_test,std::cerr)
}