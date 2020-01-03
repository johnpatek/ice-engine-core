#include "core/types.h"
#include "core/memory.h"
namespace ice { 
namespace core {

    memory_chunk::memory_chunk(const size_type size)
    {
        _size = size;
        _free = true;
        prev = NULL;
        next = NULL;
    }

    size_type memory_chunk::get_size() const
    {
        return _size;
    }
    
    void memory_chunk::set_size(const size_type size)
    {
        _size = size;
    }

    bool memory_chunk::is_free() const
    {
        return _free;
    }

    void memory_chunk::set_free(const bool free)
    {
        _free = free;
    }

    void memory_chunk::read(const void* src)
    {
        if(!memcpy(this,src,CHUNK_OVERHEAD))
        {
            // TODO: add error handling
        }
    }

    void memory_chunk::write(void* dst)
    {
        if(!memcpy(dst,this,CHUNK_OVERHEAD))
        {
            // TODO: add error handling
        }
    }

    memory_pool::memory_pool(const size_type size)
    {
        _first_block = std::unique_ptr<ice::core::byte_type[]>(
            new ice::core::byte_type[size + CHUNK_OVERHEAD]);
        
    }
   
    byte_type * memory_pool::allocate_block(const size_type size)
    {
        return NULL;
    }

    void memory_pool::deallocate_block(
        byte_type* block)
    {
        
    }

    byte_type* memory_pool::reallocate_block(
        byte_type* block, 
        const size_type size)
    {
        return NULL;
    }


}
}