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
        _data = std::unique_ptr<ice::core::byte_type[]>(
            new ice::core::byte_type[size + CHUNK_OVERHEAD]);
        memory_chunk first(size);
        first.write((void*)_data.get());
    }
   
    byte_type * memory_pool::allocate_block(const size_type size)
    {
        byte_type * result(NULL);
        
        size_type required(size + CHUNK_OVERHEAD);
        
        memory_chunk * search_block(
            (memory_chunk*)_data.get());
        
        while(
            (search_block != NULL) 
            && ((search_block->get_size() < required) 
                || !search_block->is_free()))
        {
            search_block = search_block->next;
        }

        if(search_block != NULL)
        {
            byte_type * data = (byte_type*)search_block;
            
        }

        return result;
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