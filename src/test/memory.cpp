#include "test.h"
#include "core/filesystem.h"
#include "core/memory.h"
#include <cstring>

ICE_TEST_CASE(pool_test)
{
    const char * test_string = "Test string";
    
    std::shared_ptr<ice::core::memory_pool> pool = std::make_shared<ice::core::memory_pool>(4096);
    
    ice::core::pool_allocator<ice::core::byte_type> allocator(pool);

    ice::core::pool_buffer buffer(allocator);
    
    buffer.resize(
        strlen(
            test_string));
    
    memcpy(
        buffer.data(),
        test_string,
        buffer.size());
    
    ICE_TEST_ASSERT(0 == memcmp(
        buffer.data(),
        test_string,
        buffer.size()));
}

ICE_UNIT_TEST
{
    pool_test();
}