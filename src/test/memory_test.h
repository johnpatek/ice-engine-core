#include "common.h"

class memory_test : public ::testing::Test
{
protected:
    virtual void SetUp(void) override;
    virtual void TearDown(void) override;
    std::shared_ptr<ice::core::memory_pool> _memory_pool;
    std::shared_ptr<ice::core::pool_allocator<ice::core::byte_type>> _pool_allocator;
};