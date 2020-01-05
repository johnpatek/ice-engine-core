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
            new ice::core::byte_type[size]);
        _free_size = size - CHUNK_OVERHEAD;
        memory_chunk first(_free_size);
        first.write((void*)_data.get());
    }
   
    byte_type * memory_pool::allocate_block(const size_type size)
    {
        byte_type * result(NULL);
        byte_type * data;
        size_type required_size(size + CHUNK_OVERHEAD);
        size_type remaining_size(0);
        memory_chunk * following_block;

        memory_chunk * search_block(
            (memory_chunk*)_data.get());
        
        while(
            (search_block != NULL) 
            && ((search_block->get_size() < required_size) 
                || !search_block->is_free()))
        {
            search_block = search_block->next;
        }

        if(search_block != NULL)
        {
            data = (byte_type*)search_block;
            remaining_size = search_block->get_size() - required_size;
            _free_size -= required_size;
            search_block->set_free(false);
            result = data + CHUNK_OVERHEAD;
        }


        if(remaining_size > CHUNK_OVERHEAD)
        {
            memory_chunk free_block(remaining_size);
            memory_chunk* free_data = (memory_chunk*)(data + required_size);
            free_block.next = search_block->next;
            free_block.prev = search_block;
            free_block.write(free_data);
            if(free_block.next)
            {
                free_block.next->prev = free_data;
            }
            search_block->next = free_data;
            search_block->set_size(size);
        }

        return result;
    }

    void memory_pool::deallocate_block(
        byte_type* data)
    {
        memory_chunk* block;
        bool join_prev(false);
        bool join_next(false);
        block = (
            memory_chunk*)(data - CHUNK_OVERHEAD);
        if((data != NULL) && !(block->is_free()))
        {
            block->set_free(true);
            _free_size += block->get_size() + CHUNK_OVERHEAD;
        }

        if(block->prev)
        {
            join_prev = block->prev->is_free();
        }

        if(block->next)
        {    
            join_next = block->next->is_free();
        }

        if(join_next)
        {
            block->set_size(
                block->get_size() 
                + block->next->get_size() 
                + CHUNK_OVERHEAD);
            block->next = block->next->next;
            if(block->next)
            {
                block->next->prev = block;
            }
        }

        if(join_prev)
        {
            block->prev->set_size(
                block->prev->get_size() 
                + block->get_size() 
                + CHUNK_OVERHEAD);
            if(block->next)
            {
                block->next->prev = block;
            }
            if(block->prev)
            {
                block->prev->next = block->next;
            }
        }
    }

    byte_type* memory_pool::reallocate_block(
        byte_type* data, 
        const size_type size)
    {
        return NULL;
    }

    size_type memory_pool::get_free_size() const
    {
        size_type result(0);
        auto data = (memory_chunk*)_data.get();
        do
        {
            if(data->is_free())
            {
                result += data->get_size();
            }
            data = data->next;
        }while(data);
        return result;
    }
}
}