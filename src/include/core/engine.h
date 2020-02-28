#ifndef ENGINE_H
#define ENGINE_H

#include <core/multithreading.h>
#include <core/memory.h>

namespace ice
{
namespace core
{
    class engine
    {
    private:
        std::shared_ptr<ice::core::thread_pool> _thread_pool;
    public:
        engine(const ice::core::size_type threads = std::thread::hardware_concurrency());
        
        engine(const engine& other) = default;
        
        engine(engine&& other) = default;

        ~engine() = default;
    };
}
}

#endif