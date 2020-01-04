#include "core/types.h"
#include <vector>
#include <mutex>
#include <cstring>

namespace ice
{
namespace core
{

class memory_chunk
{
private:
    size_type _size;
    bool _free;
public:
    memory_chunk() = default;
    memory_chunk(const size_type size);

    size_type get_size() const;
    void set_size(const size_type size);

    bool is_free() const;
    void set_free(const bool free);

    void read(const void* src);
    void write(void* dst);

    memory_chunk *prev;
    memory_chunk *next;
};

const int CHUNK_OVERHEAD = sizeof(memory_chunk);

#define BLOCK_OVERHEAD sizeof(memory_block)

class memory_pool
{
public:
    memory_pool(const size_type size);

    memory_pool(const memory_pool& other) = delete;
    
    memory_pool(memory_pool&& other) = default;

    byte_type* allocate_block(
        const size_type size);

    void deallocate_block(
        byte_type* block);

    byte_type* reallocate_block(
        byte_type* block, 
        const size_type size);

    size_type get_free_size() const;
    
private:
    mutable std::mutex _mutex;
    std::unique_ptr<ice::core::byte_type[]> _data;
    size_type _free_size;
};

template <class T>
class pool_allocator
{
private:
std::shared_ptr<memory_pool> _pool;
public:
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef T         value_type;

    pool_allocator(std::shared_ptr<memory_pool>&& pool) : _pool(pool)
    {
        
    }

    pool_allocator(const std::shared_ptr<memory_pool>& pool) : _pool(pool)
    {

    }

    pool_allocator(
        const pool_allocator& other) = default;



    pointer allocate(
        size_type n, 
        const void * p = 0) 
    {
        return _pool->allocate_block((size_type)n);
    }

    void deallocate(
        void* p, 
        size_type n = 0) 
    {
        if (p != NULL) 
        {
            _pool->deallocate_block((byte_type*)p);
            p = NULL;
        }
    }

    pointer address(
        reference x) const 
    { 
        return &x; 
    }
    
    const_pointer address(
        const_reference x) const 
    { 
        return &x; 
    }

    pool_allocator<T>& operator=(
        const pool_allocator&) 
    { 
        return *this; 
    }

    void construct(
        pointer p, 
        const T& val) 
    {
        new ((T*) p) T(val); 
    }

    void destroy(
        pointer p) 
    { 
        p->~T();
        deallocate(p); 
    }

    size_type max_size() const 
    { 
        return size_t(-1); 
    }

    template <class U>
    struct rebind 
    {
        typedef pool_allocator<U> other; 
    };

    template <class U>
    pool_allocator(
        const pool_allocator<U>&) 
    {

    }

    template <class U>
    pool_allocator& operator=(
        const pool_allocator<U>&) 
    { 
        return *this; 
    }
};

typedef std::vector<byte_type,std::allocator<byte_type>> heap_buffer;
typedef std::vector<byte_type,pool_allocator<byte_type>> pool_buffer;


}
}
